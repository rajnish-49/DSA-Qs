/*
================================================================================
PROBLEM EXPLANATION (MASTER CONCEPT: 1D PARTITION DP)
================================================================================
What the problem is asking:
Given an array of length N, divide it into a sequence of contiguous, non-empty 
subarrays such that the total "cost" or "score" of all subarrays is optimized 
(minimized or maximized). There is NO restriction on how many pieces you make.

Definitions of important terms:
- Prefix: The first 'i' elements of the array.
- Cost Function: A rule that determines the value of a specific slice of the array.
- Valid Slice: Some problems dictate that a slice is only allowed if it meets 
  a condition (e.g., the slice is a palindrome, or its sum is < limit).

Constraints and their implications:
- Standard constraint: N <= 1000 to 2000. 
- Implication: An O(N^2) algorithm is expected. Trying all cuts takes N steps, 
  and evaluating each cut takes N steps.

Edge cases:
- Array of length 0 or 1.
- No valid partitions are possible (must return -1 or INF).

================================================================================
THINKING FROM FIRST PRINCIPLES
================================================================================
Start from scratch:
Imagine an array [A, B, C, D]. You want to find the best way to slice the whole 
thing. Trying every combination from left to right is a mess. Instead, stand at 
the end of the array and look backwards. 

Build intuition step by step:
1. Every valid partitioning MUST have a "final piece". 
2. Let's say that final piece starts at index 'j' and goes to the end.
3. The total score would be: (Score of the best way to chop everything before 'j') 
   PLUS (Cost of the final piece from 'j' to the end).
4. Since we don't know where the optimal 'j' is, we simply test every single 
   spot where a final piece could logically start!

Explain how the key observation is discovered:
Because solving the whole array depends on solving a smaller prefix of the array 
(everything before 'j'), we have Overlapping Subproblems. We only need ONE 
variable to define our state: "How many elements from the front are we looking at?"

Clearly define invariants / states / transitions:
- State: dp[i] = the optimal score for slicing the first 'i' elements.
- Invariant: By the time the loop reaches 'i', dp[i] holds the absolute, proven 
  best answer for that prefix.
- Transition: 
  dp[i] = MIN_{over all valid j < i} ( dp[j] + cost_of_slice(j, i-1) )
  (We try making the final slice start at 'j'. We look up the best score for the 
  'j' elements before it, and add the cost of the new slice).

================================================================================
WHY THIS SPECIFIC ALGORITHM WORKS
================================================================================
Why this data structure is chosen:
A flat 1D vector 'dp' is perfect because our only state is the length of the prefix.

Why correctness holds:
It is mathematically exhaustive. To solve for length 'i', we check every possible 
final piece of length 1, length 2, length 3... up to length 'i'. Because we 
rely on previously finalized DP states for the remainder, we guarantee optimal 
substructure without doing redundant work.

Why optimal complexity is achieved:
We solve N states. To solve one state, we check up to N cut points ('j'). This 
yields O(N^2) transitions, which is the theoretical limit for finding arbitrary cuts.
*/

#include <vector>
#include <algorithm>
#include <string>

using namespace std; 

class PurePartitionDP {
public:
    // Helper to determine the cost of the subarray nums[j ... i-1].
    // NOTE: In a real interview, you often precompute values (like prefix sums, 
    // or a 2D boolean table of palindromes) so this function runs in O(1) time.
    int getSliceCost(int j, int i, const vector<int>& nums) {
        // Example: If a slice isn't allowed, return INF.
        // Otherwise, return the mathematical cost of this block.
        return 0; 
    }

    int solve(vector<int>& nums) {
        int n = nums.size();
        const int INF = 1e9; // Use a safe large number to avoid integer overflow
        
        // dp[i] represents the optimal cost to partition the first 'i' elements.
        // We initialize with INF because we are looking for a minimum cost.
        vector<int> dp(n + 1, INF);
        
        // Base case: The cost to partition 0 elements is 0. 
        // This is crucial! When j=0, the "final piece" is the ENTIRE prefix.
        // dp[0] allows us to say: 0 + cost(whole_prefix).
        dp[0] = 0;
        
        // ---------------------------------------------------------------------
        // THE PARTITION ENGINE
        // ---------------------------------------------------------------------
        // 'i' represents the length of the prefix we are currently solving.
        // We start from length 1 and build up to the full length 'n'.
        for (int i = 1; i <= n; ++i) {
            
            // 'j' represents the STARTING INDEX of the final slice.
            // If the final slice starts at 'j', it covers from 'j' up to 'i-1'.
            // 'j' can be anywhere from 0 (meaning no prior slices, just one big piece) 
            // up to i-1 (meaning the final slice is just a single element).
            for (int j = 0; j < i; ++j) {
                
                // 1. What is the cost of this proposed final slice?
                int finalSliceCost = getSliceCost(j, i, nums);
                
                // Optimization/Validity Check: If the slice isn't allowed, skip it.
                if (finalSliceCost == INF) continue;
                
                // 2. The Recurrence Relation.
                // Our proposed total score is the optimal score for the first 'j' 
                // elements PLUS the cost of this final slice we just made.
                // We keep the minimum score found across all possible 'j' starting points.
                dp[i] = min(dp[i], dp[j] + finalSliceCost);
            }
        }
        
        // Return the optimal cost to partition the full array of length 'n'.
        return dp[n];
    }
};

/*
================================================================================
DRY RUN (Conceptual)
================================================================================
Imagine array length N = 3: [A, B, C]
Goal: Minimize cost. 

1. i = 1 (Looking at prefix [A])
   - j = 0: Slice is [A]. Score = dp[0] + cost(A). 
   -> dp[1] finalized.

2. i = 2 (Looking at prefix [A, B])
   - j = 0: Slice is [A, B]. Score = dp[0] + cost(A, B).
   - j = 1: Slice is [B]. Score = dp[1] + cost(B). (Leftover is optimal [A])
   -> dp[2] finalized as the minimum of those two options.

3. i = 3 (Looking at full array [A, B, C])
   - j = 0: Slice is [A, B, C]. Score = dp[0] + cost(A, B, C).
   - j = 1: Slice is [B, C]. Score = dp[1] + cost(B, C). (Leftover is optimal [A])
   - j = 2: Slice is [C]. Score = dp[2] + cost(C). (Leftover is optimal [A, B])
   -> dp[3] finalized.

Return dp[3].

================================================================================
COMPLEXITY ANALYSIS
================================================================================
Time Complexity: O(N^2)
- The outer loop runs N times.
- The inner loop runs 1 + 2 + 3 + ... + N times, which is N(N+1)/2 = O(N^2).
- This assumes getSliceCost() executes in O(1) time. If getSliceCost takes O(N) 
  time to check something (like looping to check if it's a palindrome), the total 
  time becomes O(N^3). (Hence why precomputing a 2D table for slice costs is 
  a standard trick!).

Space Complexity: O(N)
- The DP array strictly requires O(N) memory, which is highly efficient. 
- (Note: if you build a 2D precomputation table for the costs, space becomes O(N^2)).

Why this is optimal:
You cannot skip checking valid cut points without losing the guarantee of correctness, 
meaning O(N^2) is the optimal lower bound for generic arbitrary partitioning.
================================================================================
*/