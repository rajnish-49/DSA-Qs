/*
================================================================================
PROBLEM EXPLANATION
================================================================================
What the problem is asking:
Given an array of integers and an integer 'k', partition the array into at most 
'k' contiguous, non-empty subarrays. The "score" is the sum of the averages of 
each subarray. We need to find the maximum possible score.

Definitions of important terms:
- Partition: Dividing the array continuously such that every element belongs to 
  exactly one subarray, and the order of elements is strictly maintained.
- Average: The arithmetic mean of a subarray (sum of its elements divided by its 
  length), represented as a floating-point number.

Constraints and their implications:
- 1 <= nums.length <= 100: The array is very small. This is a massive hint that 
  an O(N^3) or even O(N^2 * K) polynomial time algorithm is perfectly acceptable 
  (since 100^3 = 1,000,000 operations, well within the 10^8 per second limit).
- 1 <= nums[i] <= 10^4: Values are positive, so averages will be positive. Sums 
  fit comfortably within standard integer limits, avoiding overflow during 
  prefix sum calculations.
- 1 <= k <= nums.length: We can partition elements individually if k equals the 
  length of the array.

Edge cases:
- k = 1: The entire array must remain as a single block. The only valid answer 
  is the average of the whole array.
- k = nums.length: The maximum score is trivially achieved by making every 
  single element its own partition (since maximizing individual elements without 
  averaging them downwards yields the highest sum).

================================================================================
THINKING FROM FIRST PRINCIPLES
================================================================================
Start from scratch:
Imagine we are standing at the end of the array, trying to place the final 
divider to separate the 'k-th' partition from the rest of the array. If we place 
that divider at index 'j', the score contributed by this final piece is the 
average of elements from 'j' to the end. But what about the elements before 'j'? 
To maximize the total score, the elements from index 0 to 'j-1' must ALSO be 
partitioned optimally into 'k-1' (or fewer) groups. 

Build intuition step by step:
1. Greedily trying to isolate large numbers doesn't work because isolating a 
   large number might force many small numbers into a large block, dragging down 
   their collective average too much. 
2. Because every choice of a boundary affects the available elements for the 
   remaining partitions, decisions are globally intertwined. We must explore all 
   possible boundary placements.
3. Exploring all combinations naively leads to exponential time. However, the 
   problem exhibits "Optimal Substructure": the optimal way to partition 'i' 
   elements into 'p' groups depends entirely on the optimal way to partition 'j' 
   elements into 'p-1' groups.
4. It also exhibits "Overlapping Subproblems": we will repeatedly ask "what is 
   the best way to partition the first 5 elements into 2 groups" while 
   evaluating different bounds for the 3rd group. 

Discovering the Key Observation:
Whenever we have optimal substructure and overlapping subproblems over a sequence, 
Dynamic Programming is the mathematical tool of choice. The sequence order is fixed, 
so we only ever need to know "how many elements are we considering" and "how many 
partitions are we allowed to use".

Core concept deeply:
We will build a 2D table where we incrementally solve the problem. We first solve 
what happens if we are only allowed 1 partition for all possible array prefixes. 
Then, we use those answers to solve for 2 partitions. We use the answers for 2 
partitions to solve for 3, up to 'k'. 

Defining Invariants / States / Transitions:
- State: dp[i][p] = the maximum score achieved by partitioning the FIRST 'i' 
  elements of the array into EXACTLY 'p' contiguous subarrays.
- Invariant: dp[i][p] always holds the mathematically proven maximum score for 
  that specific prefix and partition count.
- Transition: 
  dp[i][p] = MAX_{over all valid j} ( dp[j][p-1] + average(nums[j ... i-1]) )
  Where 'j' is the start index of the final partition. 'j' must be at least 
  'p-1' (to leave enough elements for the previous 'p-1' partitions).

================================================================================
WHY THIS SPECIFIC ALGORITHM WORKS
================================================================================
Why this data structure is chosen:
- A 2D vector 'dp' is chosen because our state space depends on two discrete 
  integer variables: the number of elements (i) and the number of partitions (p).
- A 1D vector 'prefix' is chosen to cache the running sum of the array. This 
  allows us to query the sum of any subarray in O(1) time, preventing an inner 
  loop that would otherwise push the complexity to O(N^4).

Why correctness holds:
Because we iterate systematically through every possible boundary 'j' for the 
final partition, and we look up the guaranteed optimal sub-answer for the 
elements prior to 'j', we mathematically exhaust all valid configurations. By 
taking the maximum across all 'j', we ensure the globally optimal configuration 
is carried forward.

Why optimal complexity is achieved:
Precomputing prefix sums eliminates redundant math. Processing the DP table 
iteratively limits our work strictly to evaluating the transition function for 
each state. We visit O(N * K) states, and for each state, we do O(N) work to 
find the best 'j'. Thus, we perfectly bound the computation to O(N^2 * K).
*/

#include <vector>
#include <algorithm>

using namespace std; // Using standard namespace globally to avoid std:: prefix pollution

class Solution {
public:
    double largestSumOfAverages(vector<int>& nums, int k) {
        // 'n' holds the boundary limit for our problem scope
        int n = nums.size();
        
        // ---------------------------------------------------------------------
        // PREFIX SUM CALCULATION
        // ---------------------------------------------------------------------
        // prefix[i] will store the sum of the first 'i' elements.
        // It is sized n + 1 to handle the empty prefix (sum of 0 elements is 0).
        // This is a crucial optimization: without it, calculating the average of 
        // nums[j...i-1] would require an O(N) loop every time.
        vector<double> prefix(n + 1, 0.0);
        for (int i = 0; i < n; ++i) {
            // The sum up to index 'i' is the sum up to 'i-1' plus the current element.
            prefix[i + 1] = prefix[i] + nums[i];
        }
        
        // ---------------------------------------------------------------------
        // DYNAMIC PROGRAMMING TABLE SETUP
        // ---------------------------------------------------------------------
        // dp[i][p] represents the maximum score achievable using the first 'i' 
        // elements grouped into exactly 'p' partitions.
        // Dimensions: (n + 1) for lengths 0 to n. (k + 1) for partitions 0 to k.
        vector<vector<double>> dp(n + 1, vector<double>(k + 1, 0.0));
        
        // ---------------------------------------------------------------------
        // BASE CASE INITIALIZATION
        // ---------------------------------------------------------------------
        // If we are only allowed 1 partition (p = 1), there are no decisions to make.
        // The first 'i' elements MUST be grouped together into a single block.
        // The score is simply the mathematical average of these 'i' elements.
        for (int i = 1; i <= n; ++i) {
            dp[i][1] = prefix[i] / i; 
        }
        
        // ---------------------------------------------------------------------
        // DP TRANSITIONS (THE CORE ENGINE)
        // ---------------------------------------------------------------------
        // We incrementally build answers for 2 partitions, then 3, up to 'k'.
        // This outer loop ensures that when evaluating 'p', all subproblems 
        // for 'p-1' are already fully solved and immutable.
        for (int p = 2; p <= k; ++p) {
            
            // 'i' dictates the size of the array prefix we are currently solving.
            // We start 'i' at 'p' because you cannot create 'p' non-empty 
            // partitions from fewer than 'p' elements.
            for (int i = p; i <= n; ++i) {
                
                // 'j' acts as the splitting point (the start of our final partition).
                // We are deciding to cut the array so that the last group is nums[j...i-1].
                // We start 'j' at 'p - 1' because the left side (0 to j-1) must contain 
                // at least 'p - 1' elements to validly form the previous 'p - 1' partitions.
                for (int j = p - 1; j < i; ++j) {
                    
                    // The sum of the final group nums[j...i-1] is seamlessly fetched 
                    // in O(1) time using our prefix sum array.
                    // We divide by (i - j), which is the exact number of elements in this group.
                    double currentAverage = (prefix[i] - prefix[j]) / (i - j);
                    
                    // The core recurrence relation.
                    // We propose a total score: the optimal score of the left side 
                    // (first 'j' elements into 'p-1' partitions) PLUS the average of our right side.
                    // We keep the maximum score found across all valid splitting points 'j'.
                    dp[i][p] = max(dp[i][p], dp[j][p - 1] + currentAverage);
                }
            }
        }
        
        // ---------------------------------------------------------------------
        // RESULT
        // ---------------------------------------------------------------------
        // The question asks for at MOST k partitions. However, because all numbers 
        // are strictly positive, adding more partitions (splitting groups) mathematically 
        // never decreases the maximum potential sum of averages. Therefore, the exact 
        // 'k' partitions on exactly 'n' elements will hold the absolute maximum score.
        return dp[n][k];
    }
};


/* ============================================================
 * APPROACH 2 — MEMOIZATION (TOP-DOWN)
 * ============================================================
 *
 * STATE
 * -----
 * solve(i, p) = max sum of averages of first i elements in exactly p groups
 * i is a COUNT (not index): first i elements = nums[0..i-1]
 *
 * TRANSITION
 * ----------
 * Fix where the last group starts (index j, 0-indexed).
 * Last group = nums[j..i-1], average = (prefix[i] - prefix[j]) / (i - j)
 * Previous groups = solve(j, p-1)
 * Try all valid j from p-1 to i-1, take maximum.
 *
 * BASE CASE
 * ---------
 * p == 1: only 1 group, must take all first i elements.
 * Return prefix[i] / i = average of nums[0..i-1].
 *
 * MEMOIZATION CHECK
 * -----------------
 * memo[i][p] > 0.0 means already computed.
 * Valid because all scores are positive (nums[i] >= 1).
 * 0.0 is used as "not yet computed" sentinel.
 *
 * ============================================================
 */
 
class SolutionMemo {
    double solve(int i, int p, const vector<double>& prefix, vector<vector<double>>& memo) {
 
        // Return cached result if already computed.
        // memo[i][p] > 0.0 is safe sentinel because all nums[i] >= 1,
        // so all valid scores are strictly positive.
        if (memo[i][p] > 0.0)
            return memo[i][p];
 
        // Base case: only 1 group left, must take all first i elements.
        // Average of nums[0..i-1] = prefix[i] / i.
        if (p == 1)
            return memo[i][p] = prefix[i] / i;
 
        double maxScore = 0.0;
 
        // j = starting index of the last group (0-indexed).
        // j >= p-1 : first p-1 groups need at least p-1 elements before j
        // j < i    : last group needs at least 1 element (covers j..i-1)
        for (int j = p - 1; j < i; ++j) {
 
            // Average of last group nums[j..i-1].
            // prefix[i] - prefix[j] = sum(nums[j..i-1]) in O(1).
            // i - j = number of elements in last group.
            double currentAverage = (prefix[i] - prefix[j]) / (i - j);
 
            // solve(j, p-1) = best score for first j elements in p-1 groups.
            // Already computed or will be computed and cached.
            maxScore = max(maxScore, solve(j, p - 1, prefix, memo) + currentAverage);
        }
 
        return memo[i][p] = maxScore;
    }
 
public:
    double largestSumOfAverages(vector<int>& nums, int k) {
        int n = nums.size();
 
        // prefix[i] = sum of nums[0..i-1].
        // Allows O(1) range sum: sum(nums[j..i-1]) = prefix[i] - prefix[j].
        // Without this, each range sum costs O(n) → total O(n^3*k).
        vector<double> prefix(n + 1, 0.0);
        for (int i = 0; i < n; ++i)
            prefix[i + 1] = prefix[i] + nums[i];
 
        // memo[i][p] = best score for first i elements in p groups.
        // 0.0 = not yet computed (safe sentinel since all scores > 0).
        vector<vector<double>> memo(n + 1, vector<double>(k + 1, 0.0));
 
        // solve(n, k) = best score for all n elements in exactly k groups.
        return solve(n, k, prefix, memo);
    }
};
 

/*
================================================================================
DRY RUN
================================================================================
Input: nums = [9, 1, 2], k = 2

1. Prefix Sum Setup:
   prefix = [0.0, 9.0, 10.0, 12.0]
   (e.g., prefix[3] = sum(9,1,2) = 12.0)

2. Base Case (p = 1):
   dp[1][1] = prefix[1] / 1 = 9.0 / 1 = 9.0
   dp[2][1] = prefix[2] / 2 = 10.0 / 2 = 5.0
   dp[3][1] = prefix[3] / 3 = 12.0 / 3 = 4.0

3. DP Transitions (p = 2):
   - For i = 2 (evaluating first 2 elements: [9, 1])
     - Valid j's: j can only be 1.
       - j = 1: Split is [9] | [1].
         score = dp[1][1] + average(nums[1...1]) 
               = 9.0 + (10.0 - 9.0)/1 = 9.0 + 1.0 = 10.0
       -> dp[2][2] = 10.0

   - For i = 3 (evaluating first 3 elements: [9, 1, 2])
     - Valid j's: j can be 1 or 2.
       - Try j = 1: Split is [9] | [1, 2]
         score = dp[1][1] + average(nums[1...2])
               = 9.0 + (prefix[3] - prefix[1]) / 2
               = 9.0 + (12.0 - 9.0) / 2 = 9.0 + 1.5 = 10.5
               
       - Try j = 2: Split is [9, 1] | [2]
         score = dp[2][1] + average(nums[2...2])
               = 5.0 + (prefix[3] - prefix[2]) / 1
               = 5.0 + (12.0 - 10.0) / 1 = 5.0 + 2.0 = 7.0
               
       -> dp[3][2] = max(10.5, 7.0) = 10.5

4. Return Result:
   return dp[3][2] -> 10.5
   
================================================================================
COMPLEXITY ANALYSIS
================================================================================
Time Complexity: O(K * N^2)
- Calculating the prefix sum array takes O(N) time.
- The base case initialization takes O(N) time.
- The nested loops:
  - The outermost loop runs 'K' times.
  - The middle loop runs 'N' times.
  - The innermost loop runs up to 'N' times.
- Within the innermost loop, the operations (prefix sum lookup, subtraction, 
  division, and max comparison) all execute in strictly O(1) time.
- Therefore, the total time bound is dominated by the nested loops: O(K * N * N).

Space Complexity: O(N * K)
- The 'prefix' vector requires O(N) space.
- The 'dp' 2D table requires (N + 1) * (K + 1) space, which simplifies to O(N * K).
- Space could theoretically be optimized to O(N) by only keeping the 'p-1' row in 
  memory since 'p' only ever looks back at 'p-1', but retaining the 2D matrix 
  greatly improves code readability and debuggability without violating constraints.

Optimality:
Given that the problem dictates discovering the optimal grouping boundaries, and 
all boundary selections influence subsequent choices, DP is required. We evaluate 
all topological subproblems efficiently with memorized states, avoiding exponential 
recalculation. O(K * N^2) is perfectly optimal for the tight constraint of N <= 100.
================================================================================
*/