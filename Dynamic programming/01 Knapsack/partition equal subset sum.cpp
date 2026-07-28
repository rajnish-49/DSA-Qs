class Solution {
public:
    int n;
    vector<vector<int>> dp;

    /* PROBLEM STATEMENT & INTUITION:
     * Given an array, can we partition it into two subsets with equal sum?
     * 
     * KEY INSIGHT: If total sum is S, and we split into two equal subsets,
     * each subset must have sum = S/2. So the problem reduces to:
     * "Can we find a subset with sum exactly equal to S/2?"
     * 
     * This is the classic SUBSET SUM problem - a decision problem where we
     * decide for each element: include it or exclude it from our subset.
     * 
     * WHY DP? At each index, we face 2 choices (take/skip). Without memoization,
     * this creates overlapping subproblems - same (index, remainingSum) state
     * gets computed multiple times. DP eliminates redundant computation.
     */

    bool solve(int i, int currSum, vector<int>& nums) {
        /* BASE CASES - The recursion endpoints:
         * 
         * currSum == 0: We've successfully formed our target subset!
         * Every element we "took" along the way has summed to exactly target.
         * This is our SUCCESS condition - return true immediately.
         */
        if (currSum == 0) return true;
        
        /* i < 0: We've exhausted all elements without reaching currSum = 0.
         * No more elements to consider, and we still need more sum.
         * This path FAILED - return false.
         */
        if (i < 0) return false;

        /* MEMOIZATION CHECK:
         * dp[i][currSum] stores: "Can we form sum=currSum using elements [0...i]?"
         * If already computed (-1 means not computed yet), return cached result.
         * This prevents re-exploring the same (index, remainingSum) state.
         */
        if (dp[i][currSum] != -1)
            return dp[i][currSum];

        /* DECISION 1 - SKIP current element:
         * Move to previous index with same target sum.
         * Intuition: "Maybe we don't need nums[i] to form our subset"
         * Recursive state: (i-1, currSum) - one less element, same sum needed
         */
        bool skip = solve(i - 1, currSum, nums);

        /* DECISION 2 - TAKE current element (if valid):
         * Only possible if nums[i] doesn't exceed remaining sum needed.
         * 
         * Constraint check: currSum >= nums[i]
         * Why? If we need sum=5 but nums[i]=10, taking it overshoots target.
         * 
         * If we take nums[i], we reduce our target by nums[i]:
         * Recursive state: (i-1, currSum - nums[i])
         * Intuition: "We used nums[i], so we need (currSum - nums[i]) from rest"
         */
        bool take = false;
        if (currSum >= nums[i])
            take = solve(i - 1, currSum - nums[i], nums);

        /* COMBINING DECISIONS - The OR logic:
         * We succeed if EITHER path (skip OR take) leads to success.
         * Only need ONE valid way to form the subset.
         * 
         * Store in dp[i][currSum] before returning for future lookups.
         */
        return dp[i][currSum] = skip || take;
    }

    bool canPartition(vector<int>& nums) {
        /* PREREQUISITE CHECK - Mathematical impossibility:
         * If total sum is odd, we CANNOT split into two equal integer sums.
         * Example: sum=15 → can't have two subsets each with sum=7.5
         * Early exit optimization - no need to run DP.
         */
        int sum = 0;
        for (int x : nums) sum += x;

        if (sum % 2 != 0) return false;

        /* PROBLEM TRANSFORMATION:
         * Original: "Split into two equal subsets"
         * Transformed: "Find ONE subset with sum = total_sum/2"
         * 
         * Why this works? If we find subset with sum=S/2, the remaining
         * elements automatically form the other subset with sum=S/2.
         */
        int target = sum / 2;
        n = nums.size();

        /* DP TABLE INITIALIZATION:
         * Dimensions: dp[n][target+1]
         * - Row (i): Represents considering elements from index 0 to i
         * - Column (currSum): The exact sum we're trying to achieve (0 to target)
         * 
         * Initialized to -1: Meaning "not yet computed"
         * Will be updated to 0 (false) or 1 (true) as we solve subproblems
         */
        dp.assign(n, vector<int>(target + 1, -1));

        /* START RECURSION from:
         * Index = n-1 (last element, will recurse backward to 0)
         * Sum = target (the full amount we need to achieve)
         * 
         * Top-down approach: Start from full problem, break into subproblems
         */
        return solve(n - 1, target, nums);
    }
};

/* TIME COMPLEXITY: O(n × target) where target = sum/2
 * Each unique (i, currSum) state computed once due to memoization.
 * Total states = n indices × (target+1) possible sums
 * 
 * SPACE COMPLEXITY: O(n × target) for dp table + O(n) recursion stack
 */


 class Solution {
public:
    bool canPartition(vector<int>& nums) {
        /* PROBLEM TRANSFORMATION (First Principles):
         * Equal partition means: subset1_sum = subset2_sum = total_sum/2
         * Key insight: If we can form ANY subset with sum = total_sum/2,
         * the remaining elements AUTOMATICALLY form the other half.
         * So we reduce "partition into 2 equal subsets" → "find 1 subset with sum = target"
         */
        int sum = 0;
        for (int x : nums) sum += x;
        
        /* IMPOSSIBILITY CHECK:
         * Odd sum cannot be divided into two equal integer parts.
         * Example: [1,2,4] → sum=7 → need 3.5 per subset (impossible)
         */
        if (sum % 2 != 0) return false;
        
        int target = sum / 2;
        int n = nums.size();
        
        /* DP TABLE DESIGN (Bottom-Up Approach):
         * 
         * dp[i][s] = "Can we form sum 's' using first 'i' elements?"
         * 
         * Dimensions: (n+1) × (target+1)
         * - Extra row (i=0): Represents "using 0 elements" (empty subset)
         * - Extra column (s=0): Represents "forming sum=0"
         * 
         * WHY (n+1) instead of n?
         * Allows natural 1-based indexing where dp[i] considers nums[0...i-1]
         * Makes base case cleaner: dp[0][...] = "no elements available"
         * 
         * State transition will build from smaller subproblems (fewer elements)
         * to larger ones (more elements), filling table row by row.
         */
        vector<vector<bool>> dp(n + 1, vector<bool>(target + 1, false));
        
        /* BASE CASE - The Foundation:
         * dp[0][0] = true means "We CAN form sum=0 using 0 elements"
         * This is trivially true: empty subset has sum=0
         * 
         * All other dp[0][s] remain false (s>0):
         * "Can't form positive sum with no elements" - impossible
         * 
         * This seed case allows the recurrence relation to build upward.
         */
        dp[0][0] = true;
        
        /* ITERATIVE DP BUILD - Processing element by element:
         * i represents "considering first i elements" (nums[0] to nums[i-1])
         * We build solutions incrementally: if we know what's possible with i-1 elements,
         * we can derive what's possible with i elements.
         */
        for (int i = 1; i <= n; i++) {
            /* For each element, try forming every possible sum from 0 to target:
             * s represents the target sum we're trying to achieve
             */
            for (int s = 0; s <= target; s++) {
                /* DECISION 1 - SKIP nums[i-1] (current element):
                 * 
                 * Inherit result from previous row: dp[i-1][s]
                 * Logic: "If we could form sum 's' without current element,
                 *         we can still form it by not including current element"
                 * 
                 * Example: If dp[2][5]=true (can make 5 with first 2 elements),
                 *          then dp[3][5]=true (can still make 5, just skip 3rd element)
                 * 
                 * This preserves all solutions from previous subproblems.
                 */
                dp[i][s] = dp[i-1][s];
                
                /* DECISION 2 - TAKE nums[i-1] (current element):
                 * 
                 * Constraint: s >= nums[i-1]
                 * Can only take element if it doesn't exceed target sum 's'
                 * Example: If s=5 and nums[i-1]=10, taking it overshoots → skip this
                 * 
                 * Recurrence Logic: dp[i-1][s - nums[i-1]]
                 * "If we take nums[i-1], we need the REMAINING sum (s - nums[i-1])
                 *  to be achievable using the PREVIOUS elements (i-1)"
                 * 
                 * Example walkthrough:
                 * - Want sum s=8, current element nums[i-1]=3
                 * - If we take 3, we need remaining 5 from previous elements
                 * - Check dp[i-1][5]: "Could we make 5 before adding current element?"
                 * - If yes, then dp[i][8] = true (take 3, get 5 from before = 8)
                 * 
                 * OR operation combines both decisions:
                 * Success if EITHER (skip works) OR (take works)
                 * Only need ONE valid path to form the sum.
                 */
                if (s >= nums[i-1]) {
                    dp[i][s] = dp[i][s] || dp[i-1][s - nums[i-1]];
                }
            }
        }
        
        /* FINAL ANSWER:
         * dp[n][target] = "Can we form 'target' sum using all 'n' elements?"
         * 
         * This is exactly what we need: a subset from all available elements
         * that sums to target (which is half the total sum).
         * 
         * If true: One subset has sum=target, other has sum=target → equal partition
         * If false: No way to split into equal sums → return false
         */
        return dp[n][target];
    }
};

/* COMPLEXITY ANALYSIS:
 * 
 * TIME: O(n × target) where target = sum/2
 * - Two nested loops: n iterations × target iterations
 * - Each cell computed once with O(1) operations
 * - Worst case: target ≈ sum/2, so O(n × sum)
 * 
 * SPACE: O(n × target) for the 2D DP table
 * 
 * SPACE OPTIMIZATION (not implemented here):
 * Since dp[i] only depends on dp[i-1], we can use rolling array technique
 * to reduce space to O(target) using just 1D array.
 * Trade-off: Slightly less intuitive but same time complexity.
 * 
 * KEY DIFFERENCE FROM TOP-DOWN:
 * - Top-down (memoization): Starts from full problem, recurses to base cases
 * - Bottom-up (this code): Starts from base cases, builds to full problem
 * - Both have same complexity, but bottom-up avoids recursion stack overhead
 */

/* EXAMPLE TRACE: nums = [1,5,11,5], target = 11
 * 
 * Initial: dp[0][0] = true, all others false
 * 
 * After i=1 (element 1):
 *   dp[1][0] = true (skip 1)
 *   dp[1][1] = true (take 1: check dp[0][0])
 *   dp[1][2..11] = false
 * 
 * After i=2 (element 5):
 *   dp[2][0] = true (skip)
 *   dp[2][1] = true (skip, inherited)
 *   dp[2][5] = true (take 5: check dp[1][0]=true)
 *   dp[2][6] = true (take 5: check dp[1][1]=true, so 1+5=6)
 * 
 * After i=3 (element 11):
 *   dp[3][11] = true (take 11: check dp[2][0]=true)
 * 
 * Result: dp[4][11] = true → can partition!
 * Subset: {11} and {1,5,5} both sum to 11
 */