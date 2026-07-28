class Solution {
public:
    int n;
    // dp[idx][target] = maximum length of subsequence starting from index 'idx' that sums to 'target'
    // We store -1 for uncomputed states
    // Negative values (like -1000000) represent impossible states (cannot achieve target sum)
    vector<vector<int>> dp;
    
    /*
     * PROBLEM UNDERSTANDING:
     * ---------------------
     * Given an array of positive integers and a target sum, find the LENGTH (not the subsequence itself)
     * of the longest subsequence whose elements sum exactly to the target.
     * Return -1 if no such subsequence exists.
     * 
     * Example: nums = [1, 2, 3, 4, 5], target = 9
     * Possible subsequences: [4, 5] (length 2), [1, 3, 5] (length 3), [2, 3, 4] (length 3)
     * Answer: 3 (we want maximum length)
     * 
     * KEY INSIGHT: This is NOT the classic subset sum problem (which asks "can we make target?")
     * We want to MAXIMIZE the number of elements while still hitting the target exactly.
     * 
     * WHY THIS APPROACH?
     * ------------------
     * For each element, we have 2 choices: include it or skip it.
     * This creates a decision tree with 2^n possibilities - too many for brute force.
     * However, many subproblems repeat: "from index i, can we make sum s?"
     * This overlapping subproblems structure makes it perfect for Dynamic Programming.
     * 
     * STATE DEFINITION:
     * -----------------
     * dp[idx][target] = "Starting from index idx, with remaining target to achieve,
     *                    what's the maximum number of elements we can select?"
     * 
     * We use top-down DP (memoization) - start from the problem and recurse down,
     * caching results to avoid recomputation.
     */
    
    int solve(int idx, vector<int>& nums, int target) {
        
        // BASE CASE 1: We've successfully reduced target to 0
        // This means the elements we've selected so far sum exactly to original target
        // Return 0 because from this point, we're not adding any more elements
        // The count of elements comes from the "1 + solve(...)" in the recursive calls above
        if (target == 0) {
            return 0;
        }
        
        // BASE CASE 2: We've exhausted all elements but target is still not 0
        // This path is INVALID - we can't achieve the target with remaining elements
        // Return a large negative number (-1000000) to indicate impossibility
        // Why -1000000? Because we're taking max() later, this ensures invalid paths
        // don't get selected. It's more negative than any valid answer (which is at most n)
        if (idx == n) {
            return -1000000;
        }
        
        // MEMOIZATION CHECK: Have we already solved this subproblem?
        // If dp[idx][target] is not -1, we've computed it before - return cached result
        // This is the "Dynamic" part of DP - avoiding redundant calculations
        // Time complexity drops from O(2^n) to O(n * target)
        if (dp[idx][target] != -1)
            return dp[idx][target];
        
        // DECISION 1: SKIP the current element
        // Move to next index while target remains unchanged
        // This explores the branch where nums[idx] is NOT in our subsequence
        int skip = solve(idx + 1, nums, target);
        
        // DECISION 2: TAKE the current element (if possible)
        // Initialize to -1000000 (impossible) assuming we can't take it
        int take = -1000000;
        
        // We can only take nums[idx] if it doesn't exceed our remaining target
        // Taking an element larger than target would make target negative = impossible
        if (nums[idx] <= target) {
            // If we TAKE nums[idx]:
            // 1. Add 1 to our count (we're including one more element)
            // 2. Reduce target by nums[idx] (we've "used up" that much of our sum)
            // 3. Move to next index (can't reuse same element)
            // The "+1" here is crucial - it counts THIS element in our subsequence length
            take = 1 + solve(idx + 1, nums, target - nums[idx]);
        }
        
        // OPTIMAL SUBSTRUCTURE: Choose the better option
        // max(take, skip) gives us the path that yields MAXIMUM length
        // If 'take' is -1000000 (impossible), skip will be chosen
        // If both are negative (both paths fail), the less negative one propagates up
        // Store result in dp table before returning (memoization)
        return dp[idx][target] = max(take, skip);
    }
    
    int lengthOfLongestSubsequence(vector<int>& nums, int target) {
        n = nums.size();
        
        // Initialize DP table with -1 (uncomputed state marker)
        // Dimensions: n rows (one per element) × (target + 1) columns (sums from 0 to target)
        // Space complexity: O(n * target)
        dp.assign(n, vector<int>(target + 1, -1));
        
        // Start recursion from index 0 with full target to achieve
        int ans = solve(0, nums, target);
        
        // FINAL ANSWER INTERPRETATION:
        // If ans < 0, all paths to achieve target failed (returned large negative values)
        // This means no subsequence can sum to target - return -1
        // Otherwise, ans contains the maximum length of a valid subsequence
        return ans < 0 ? -1 : ans;
    }
    
    /*
     * TIME COMPLEXITY: O(n * target)
     * - We have n × target unique states in our DP table
     * - Each state is computed exactly once (due to memoization)
     * - Each state computation does O(1) work
     * 
     * SPACE COMPLEXITY: O(n * target)
     * - DP table size: n × (target + 1)
     * - Recursion stack depth: O(n) in worst case
     * - Total: O(n * target) dominated by DP table
     * 
     * ALTERNATE APPROACHES:
     * - Bottom-up DP: Build table iteratively instead of recursively
     * - Space-optimized: Since we only need previous row, can reduce to O(target) space
     * - Backtracking with pruning: Explore all 2^n possibilities with early termination
     */
};





class Solution {
public:
    int lengthOfLongestSubsequence(vector<int>& nums, int target) {
        int n = nums.size();
        
        /*
         * ═══════════════════════════════════════════════════════════════════════════
         * PROBLEM STATEMENT & FUNDAMENTAL UNDERSTANDING
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * Given: An array of positive integers and a target sum
         * Find: LENGTH of the longest subsequence that sums EXACTLY to target
         * Return: -1 if impossible
         * 
         * Example Walkthrough:
         * nums = [1, 1, 5, 4, 5], target = 9
         * 
         * Possible subsequences summing to 9:
         * - [4, 5] → length 2
         * - [1, 1, 5] → length 3  ← This is NOT valid (sums to 7, not 9)
         * - [1, 4, 4] → Can't use 4 twice (it appears once)
         * - [5, 4] → length 2
         * 
         * Wait, let me recalculate:
         * - [4, 5] → 4+5 = 9 ✓ length 2
         * - [1, 1, 5] → 1+1+5 = 7 ✗
         * - [5, 4] → same as [4,5]
         * 
         * Actually for [1, 1, 5, 4, 5]:
         * We need subsets summing to 9:
         * - Take indices {0,2,3}: nums[0]=1, nums[2]=5, nums[3]=4 → 1+5+4=10 ✗
         * - Take indices {2,3}: nums[2]=5, nums[3]=4 → 5+4=9 ✓ length=2
         * - Take indices {0,1,3,4}: 1+1+4+5=11 ✗
         * 
         * Answer: 2 (we want MAXIMUM length among all valid subsequences)
         * 
         * ═══════════════════════════════════════════════════════════════════════════
         * WHY DYNAMIC PROGRAMMING?
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * 1. OPTIMAL SUBSTRUCTURE:
         *    The problem can be broken into smaller subproblems.
         *    If we know the answer for "elements from index i+1 to n with target T",
         *    we can easily compute "elements from index i to n with target T" by
         *    deciding whether to include nums[i] or not.
         * 
         * 2. OVERLAPPING SUBPROBLEMS:
         *    Example: nums = [2, 3, 5], target = 10
         *    
         *    Path 1: Skip 2, Skip 3 → subproblem: "from index 2, make sum 10"
         *    Path 2: Skip 2, Take 3 → subproblem: "from index 2, make sum 7"
         *    Path 3: Take 2, Skip 3 → subproblem: "from index 2, make sum 8"
         *    
         *    Notice: The subproblem "from index 2, make sum X" appears multiple times!
         *    Without DP, we'd recalculate it every time → exponential complexity O(2^n)
         *    With DP, we calculate once and reuse → polynomial complexity O(n × target)
         * 
         * ═══════════════════════════════════════════════════════════════════════════
         * TOP-DOWN vs BOTTOM-UP: THE PARADIGM SHIFT
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * TOP-DOWN (Recursion + Memoization):
         * - Start with the ORIGINAL problem: solve(0, target)
         * - Break it down into smaller problems recursively
         * - Cache results as we go (memoization)
         * - Think: "To solve this, I need to solve that first"
         * - Natural flow: matches how we think about the problem
         * 
         * BOTTOM-UP (Tabulation):
         * - Start with the SMALLEST subproblems (base cases)
         * - Build up to the original problem iteratively
         * - Fill the table systematically
         * - Think: "If I know all smaller answers, I can build bigger answers"
         * - More efficient: no recursion overhead, better cache locality
         * 
         * ═══════════════════════════════════════════════════════════════════════════
         * DP STATE DEFINITION - THE HEART OF THE SOLUTION
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * dp[idx][t] = "Starting from index idx to end of array (n-1),
         *               with a remaining target of t,
         *               what is the MAXIMUM number of elements we can select
         *               such that they sum EXACTLY to t?"
         * 
         * Return value meanings:
         * - Positive number (0 to n): Maximum length achievable
         * - Large negative (-1000000): IMPOSSIBLE to achieve target t
         * 
         * Why use -1000000 for impossible states?
         * - We're taking max() to find the best option
         * - Valid answers range from 0 to n (at most n elements)
         * - -1000000 is so negative that max(valid_answer, -1000000) always picks valid_answer
         * - It propagates upward to mark entire paths as invalid
         * 
         * ═══════════════════════════════════════════════════════════════════════════
         * TABLE DIMENSIONS & INITIALIZATION
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * Dimensions: (n+1) × (target+1)
         * 
         * Why n+1 rows?
         * - Rows represent "starting index": 0, 1, 2, ..., n-1, n
         * - Row 'n' represents "no elements left" (base case)
         * - This extra row simplifies boundary conditions in our loops
         * 
         * Why target+1 columns?
         * - Columns represent "remaining target": 0, 1, 2, ..., target
         * - Column '0' means "we need sum of 0" (base case - already achieved!)
         * 
         * Initial value: -1000000
         * - Means "by default, assume all states are impossible"
         * - We'll overwrite with actual values as we compute
         * - Only valid/reachable states will get real values
         */
        vector<vector<int>> dp(n + 1, vector<int>(target + 1, -1000000));
        
        /*
         * ═══════════════════════════════════════════════════════════════════════════
         * BASE CASE INITIALIZATION
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * dp[i][0] = 0 for all i
         * 
         * MEANING: "From any index i, if we need to make sum 0, we can do it by
         *           selecting 0 elements (select nothing)"
         * 
         * WHY IS THIS TRUE?
         * - Target 0 means we don't need to pick any more elements
         * - Empty subsequence has sum 0 and length 0
         * - This is ALWAYS achievable regardless of which index we're at
         * 
         * INTUITION:
         * Imagine you're at a buffet with target calories = 0 (you're full).
         * No matter where you are in line (index 0, 1, ..., n), you can achieve
         * "0 more calories" by simply not taking anything → 0 items selected.
         * 
         * Row n (dp[n][0] = 0):
         * - Particularly important: "no elements left, need sum 0"
         * - This is the successful termination condition
         * - Matches the base case in recursive version: if (target == 0) return 0;
         * 
         * All other dp[n][t] remain -1000000:
         * - "No elements left, but still need positive sum t"
         * - IMPOSSIBLE → keeps the -1000000 value
         * - Matches recursive base case: if (idx == n && target > 0) return -1000000;
         */
        for (int i = 0; i <= n; i++) {
            dp[i][0] = 0;
        }
        
        /*
         * ═══════════════════════════════════════════════════════════════════════════
         * ITERATION ORDER: WHY BACKWARDS?
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * for (int idx = n - 1; idx >= 0; idx--)
         * 
         * CRITICAL INSIGHT: Bottom-up DP must process states in REVERSE dependency order
         * 
         * Dependency Analysis:
         * - dp[idx][t] depends on dp[idx+1][t] and dp[idx+1][t-nums[idx]]
         * - To compute row idx, we MUST have already computed row idx+1
         * - Therefore, we process from row n-1 down to row 0
         * 
         * Visual Dependency:
         * 
         *        t-nums[idx]    t
         *              ↓        ↓
         *   idx+1:   [X]──────[Y]
         *              ↖      ↗
         *                [Z]
         *   idx:         ↑
         *                t
         * 
         * To compute dp[idx][t] (Z), we need:
         * - dp[idx+1][t] (Y) - if we skip
         * - dp[idx+1][t-nums[idx]] (X) - if we take
         * 
         * Both are in the NEXT row (idx+1), so we must compute that row first!
         * 
         * Why does target (t) go forward (0 to target)?
         * - No dependency between different columns in same row
         * - dp[idx][t] only depends on row idx+1, not other values in row idx
         * - Can process columns in any order (forward is conventional)
         * 
         * ═══════════════════════════════════════════════════════════════════════════
         * THE RECURRENCE RELATION - CORE LOGIC
         * ═══════════════════════════════════════════════════════════════════════════
         */
        for (int idx = n - 1; idx >= 0; idx--) {
            for (int t = 0; t <= target; t++) {
                
                /*
                 * ───────────────────────────────────────────────────────────────────
                 * DECISION 1: SKIP nums[idx]
                 * ───────────────────────────────────────────────────────────────────
                 * 
                 * skip = dp[idx + 1][t]
                 * 
                 * MEANING: "Don't include nums[idx] in our subsequence"
                 * 
                 * Effect:
                 * - Move to next index (idx+1)
                 * - Target remains unchanged (t stays t)
                 * - Length doesn't increase (we didn't select this element)
                 * 
                 * Example:
                 * nums = [3, 5, 7], currently at idx=0 (value 3), target=10
                 * 
                 * Skip 3 → Problem becomes: "From index 1, make sum 10"
                 *          → dp[1][10]
                 * 
                 * If dp[1][10] = 2 (we can select 2 elements from index 1 onwards to make 10)
                 * Then skip = 2 (skipping doesn't add to count)
                 */
                int skip = dp[idx + 1][t];
                
                /*
                 * ───────────────────────────────────────────────────────────────────
                 * DECISION 2: TAKE nums[idx]
                 * ───────────────────────────────────────────────────────────────────
                 * 
                 * take = 1 + dp[idx + 1][t - nums[idx]]
                 * 
                 * PRECONDITION: nums[idx] <= t (we can't take more than we need)
                 * 
                 * Why check nums[idx] <= t?
                 * - If nums[idx] > t, taking it would overshoot our target
                 * - Example: nums[idx] = 7, remaining target t = 5
                 *   Taking 7 would give sum of 7, but we only need 5 → INVALID
                 * - Also prevents negative index: dp[idx+1][t-nums[idx]] would be dp[idx+1][negative]
                 * 
                 * MEANING: "Include nums[idx] in our subsequence"
                 * 
                 * Effect:
                 * - Move to next index (idx+1) - can't reuse same element
                 * - Reduce target by nums[idx] (we've "consumed" that amount)
                 * - Add 1 to length (we selected this element: +1)
                 * 
                 * Example:
                 * nums = [3, 5, 7], currently at idx=0 (value 3), target=10
                 * 
                 * Take 3 → Problem becomes: "From index 1, make sum 10-3=7"
                 *        → 1 + dp[1][7]
                 *        → The +1 counts the current element (3) we just took
                 * 
                 * If dp[1][7] = 1 (we can select 1 element from index 1 onwards to make 7)
                 * Then take = 1 + 1 = 2 (we selected nums[0] plus 1 more element)
                 * 
                 * The "+1" is CRUCIAL:
                 * - It's how we COUNT elements in bottom-up approach
                 * - Each time we take an element, we increment the count
                 * - This accumulates as we build up the solution
                 */
                int take = -1000000;
                if (nums[idx] <= t) {
                    take = 1 + dp[idx + 1][t - nums[idx]];
                }
                
                /*
                 * ───────────────────────────────────────────────────────────────────
                 * COMBINING DECISIONS: THE BELLMAN EQUATION
                 * ───────────────────────────────────────────────────────────────────
                 * 
                 * dp[idx][t] = max(take, skip)
                 * 
                 * PRINCIPLE OF OPTIMALITY:
                 * - We want the MAXIMUM length subsequence
                 * - At each element, we have 2 choices: take or skip
                 * - The optimal solution is the BETTER of these two choices
                 * - This is the essence of dynamic programming!
                 * 
                 * Why max() and not min() or sum()?
                 * - Problem asks for LONGEST (maximum length)
                 * - We're not counting total ways (would need sum)
                 * - We're not minimizing anything (would need min)
                 * 
                 * Case Analysis:
                 * 
                 * Case 1: Both take and skip are valid (positive)
                 *   → max() picks the one giving more elements
                 *   Example: take=3, skip=2 → choose take (3 elements is better)
                 * 
                 * Case 2: Only skip is valid
                 *   → take=-1000000 (can't take element, maybe nums[idx] > t)
                 *   → max(-1000000, skip) = skip
                 * 
                 * Case 3: Only take is valid
                 *   → skip=-1000000 (skipping leads to impossible state)
                 *   → max(take, -1000000) = take
                 * 
                 * Case 4: Both impossible
                 *   → take=-1000000, skip=-1000000
                 *   → max(-1000000, -1000000) = -1000000
                 *   → This state is impossible, propagates upward
                 * 
                 * CONCRETE EXAMPLE TRACE:
                 * nums = [2, 3], target = 5, currently computing dp[0][5]
                 * 
                 * Option 1 - Skip nums[0]=2:
                 *   skip = dp[1][5]
                 *   Suppose dp[1][5] = 1 (we can take nums[1]=3 to make... wait, 3≠5)
                 *   Actually dp[1][5] = -1000000 (can't make 5 from just [3])
                 * 
                 * Option 2 - Take nums[0]=2:
                 *   take = 1 + dp[1][5-2] = 1 + dp[1][3]
                 *   dp[1][3] = 1 (take nums[1]=3 to make 3)
                 *   take = 1 + 1 = 2
                 * 
                 * Result: dp[0][5] = max(2, -1000000) = 2
                 * Interpretation: Best subsequence from index 0 summing to 5 has length 2
                 *                 (which is [2, 3])
                 */
                dp[idx][t] = max(take, skip);
            }
        }
        
        /*
         * ═══════════════════════════════════════════════════════════════════════════
         * EXTRACTING FINAL ANSWER
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * ans = dp[0][target]
         * 
         * MEANING: "From index 0 (start of array) to end, with full target,
         *           what's the maximum length subsequence summing to target?"
         * 
         * This is exactly what the original problem asks!
         * 
         * Why dp[0][target] specifically?
         * - Index 0: We want to consider ALL elements (entire array)
         * - Target: We want subsequences summing to the original target
         * 
         * return ans < 0 ? -1 : ans
         * 
         * FINAL VALIDATION:
         * - If ans < 0 (specifically -1000000), no valid subsequence exists
         * - Return -1 as per problem requirements
         * - Otherwise, ans contains the maximum length (valid answer)
         * 
         * Why not just check ans == -1000000?
         * - Using ans < 0 is more robust
         * - Covers any negative sentinel value
         * - Valid lengths are always >= 0
         * 
         * ═══════════════════════════════════════════════════════════════════════════
         * COMPLETE EXAMPLE WALKTHROUGH
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * nums = [1, 2, 3], target = 4
         * 
         * Initial DP table (3+1 rows, 4+1 columns):
         *        t=0    t=1    t=2    t=3    t=4
         * idx=0   0    -1e6   -1e6   -1e6   -1e6
         * idx=1   0    -1e6   -1e6   -1e6   -1e6
         * idx=2   0    -1e6   -1e6   -1e6   -1e6
         * idx=3   0    -1e6   -1e6   -1e6   -1e6  (base case row)
         * 
         * Fill idx=2 (element nums[2]=3):
         *   For t=3: skip=dp[3][3]=-1e6, take=1+dp[3][0]=1+0=1 → dp[2][3]=1
         *   For t=4: skip=dp[3][4]=-1e6, take=1+dp[3][1]=1+(-1e6)=-1e6+1 → dp[2][4]=-1e6
         * 
         * Fill idx=1 (element nums[1]=2):
         *   For t=2: skip=dp[2][2]=-1e6, take=1+dp[2][0]=1 → dp[1][2]=1
         *   For t=3: skip=dp[2][3]=1, take=1+dp[2][1]=-1e6 → dp[1][3]=1
         *   For t=4: skip=dp[2][4]=-1e6, take=1+dp[2][2]=-1e6 → dp[1][4]=-1e6
         * 
         * Fill idx=0 (element nums[0]=1):
         *   For t=4: skip=dp[1][4]=-1e6, take=1+dp[1][3]=1+1=2 → dp[0][4]=2
         * 
         * Answer: dp[0][4] = 2
         * Subsequence: [1, 3] (length 2, sum 4) ✓
         * 
         * ═══════════════════════════════════════════════════════════════════════════
         * COMPLEXITY ANALYSIS
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * TIME: O(n × target)
         * - Two nested loops: idx from n-1 to 0 (n iterations)
         *                     t from 0 to target (target+1 iterations)
         * - Each cell computation is O(1) (just comparisons and arithmetic)
         * - Total: n × (target+1) = O(n × target)
         * 
         * SPACE: O(n × target)
         * - DP table: (n+1) × (target+1) integers
         * - No recursion stack (unlike top-down approach)
         * - Can be optimized to O(target) using space optimization (not done here)
         * 
         * ═══════════════════════════════════════════════════════════════════════════
         */
        
        int ans = dp[0][target];
        return ans < 0 ? -1 : ans;
    }
};


/* --------------------------  ANOTHER DP STATE----------------------- */


class Solution {
public:
    int lengthOfLongestSubsequence(vector<int>& nums, int target) {
        int n = nums.size();
        
        /*
         * ═══════════════════════════════════════════════════════════════════════════
         * PROBLEM STATEMENT & CORE UNDERSTANDING
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * Given: Array of positive integers + target sum
         * Find: Maximum LENGTH of subsequence that sums EXACTLY to target
         * Return: -1 if impossible
         * 
         * Example: nums = [1, 2, 3, 4, 5], target = 9
         * Valid subsequences:
         * - [4, 5] → sum=9, length=2
         * - [1, 3, 5] → sum=9, length=3 ✓ MAXIMUM
         * - [2, 3, 4] → sum=9, length=3 ✓ MAXIMUM
         * - [1, 2, 1, 5] → Can't use 1 twice if only one 1 exists
         * Answer: 3
         * 
         * This is a VARIANT of classic subset sum problem with a twist:
         * - Classic subset sum: "Can we make target?" (YES/NO)
         * - This problem: "What's the MAXIMUM count of elements to make target?"
         * 
         * ═══════════════════════════════════════════════════════════════════════════
         * COMPARING TWO DP FORMULATIONS: INDEX-BASED PERSPECTIVE SHIFT
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * FORMULATION 1 (Previous code - suffix-based):
         * dp[idx][t] = "From index idx to END, max elements to make sum t"
         * - Think: "What can I do with elements [idx, idx+1, ..., n-1]?"
         * - Process BACKWARDS: idx from n-1 down to 0
         * - Depends on dp[idx+1][...] (next element)
         * - Natural for top-down recursion
         * 
         * FORMULATION 2 (This code - prefix-based):
         * dp[i][j] = "Using FIRST i elements, max elements to make sum j"
         * - Think: "What can I do with elements [0, 1, ..., i-1]?"
         * - Process FORWARDS: i from 1 to n
         * - Depends on dp[i-1][...] (previous elements)
         * - Natural for bottom-up tabulation
         * - More intuitive: build solution incrementally
         * 
         * Both are correct! They're just different ways to think about the problem.
         * Prefix-based (this one) is more common in competitive programming.
         * 
         * ═══════════════════════════════════════════════════════════════════════════
         * STATE DEFINITION - PREFIX-BASED FORMULATION
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * dp[i][j] = "Considering the FIRST i elements (nums[0] to nums[i-1]),
         *             what is the MAXIMUM number of elements we can select
         *             such that their sum equals EXACTLY j?"
         * 
         * Key Index Mapping (CRUCIAL for avoiding off-by-one errors):
         * - dp[0][j] → considering 0 elements (empty set)
         * - dp[1][j] → considering first 1 element (only nums[0])
         * - dp[2][j] → considering first 2 elements (nums[0], nums[1])
         * - dp[i][j] → considering first i elements (nums[0] to nums[i-1])
         * - dp[n][j] → considering all n elements (entire array)
         * 
         * When accessing nums array from dp[i]:
         * - Current element being "added" is nums[i-1]
         * - This is because dp[i] represents "after considering i elements"
         * - Element at position i-1 is the i-th element (0-indexed)
         * 
         * Return values:
         * - Positive (0 to i): Valid answer, maximum length achievable
         * - -1000000: IMPOSSIBLE to achieve sum j with these elements
         * 
         * Why -1000000?
         * - Must be more negative than any valid answer (valid: 0 to n)
         * - When taking max(), ensures invalid states don't get selected
         * - Clear sentinel value indicating "this state is unreachable"
         * 
         * ═══════════════════════════════════════════════════════════════════════════
         * TABLE INITIALIZATION PHILOSOPHY
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * Dimensions: (n+1) rows × (target+1) columns
         * 
         * Row interpretation:
         * - Row 0: No elements considered yet (base case)
         * - Row i (1 ≤ i ≤ n): First i elements considered
         * - Extra row (row 0) makes iteration cleaner, no special cases
         * 
         * Column interpretation:
         * - Column 0: Need to make sum 0 (special base case)
         * - Column j (1 ≤ j ≤ target): Need to make sum j
         * 
         * Default value: -1000000 (assume everything is impossible)
         * - We'll overwrite cells that are actually reachable
         * - Unreachable cells stay at -1000000
         * - Defensive programming: catches logic errors
         */
        vector<vector<int>> dp(n + 1, vector<int>(target + 1, -1000000));
        
        /*
         * ═══════════════════════════════════════════════════════════════════════════
         * BASE CASE: THE FOUNDATION OF DYNAMIC PROGRAMMING
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * dp[i][0] = 0 for all i (0 ≤ i ≤ n)
         * 
         * LOGICAL MEANING:
         * "To make sum 0, regardless of how many elements we've considered,
         *  we select 0 elements (the empty subsequence)"
         * 
         * Mathematical proof:
         * - Empty set ∅ has sum = 0
         * - |∅| = 0 (cardinality/length is 0)
         * - This is always valid and achievable
         * 
         * Why for ALL rows i?
         * 
         * dp[0][0] = 0:
         * "With 0 elements available, to make sum 0, select 0 elements"
         * - This is the fundamental base case
         * - Everything builds from here
         * 
         * dp[1][0] = 0:
         * "With 1 element available (nums[0]), to make sum 0, select 0 elements"
         * - Don't use nums[0], just pick nothing
         * - Still valid!
         * 
         * dp[i][0] = 0:
         * "With i elements available, to make sum 0, select 0 elements"
         * - Same logic: empty subsequence always sums to 0
         * 
         * Critical insight:
         * - Column 0 represents "mission already accomplished"
         * - We don't need any more elements
         * - This termination condition is crucial for the recurrence
         * 
         * What about dp[0][j] for j > 0?
         * - "With 0 elements, make positive sum j"
         * - IMPOSSIBLE! We have nothing to work with
         * - Stays at -1000000 (default initialization)
         * - Represents unreachable/invalid states
         * 
         * ═══════════════════════════════════════════════════════════════════════════
         * ITERATION ORDER: BUILDING FROM SMALLER SUBPROBLEMS
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * Outer loop: i from 1 to n (forward, building up)
         * Inner loop: j from 1 to target (forward, any order works here)
         * 
         * Why i from 1 to n (not 0)?
         * - Row 0 is base case (already initialized)
         * - We're building row i from row i-1
         * - Start at row 1 (first element), end at row n (all elements)
         * 
         * Why j from 1 to target (not 0)?
         * - Column 0 is base case (already initialized to 0)
         * - We compute columns 1 through target
         * - Could go 0 to target, but j=0 is already known
         * 
         * Dependency graph:
         * 
         *         j-nums[i-1]      j
         *              ↓           ↓
         *   i-1:    [prev1]     [prev2]
         *              ↖         ↗
         *                [current]
         *   i:            ↑
         *                 j
         * 
         * To compute dp[i][j], we need:
         * - dp[i-1][j] (directly above)
         * - dp[i-1][j-nums[i-1]] (above-left, diagonal)
         * 
         * Both are in row i-1, so we must process row i-1 before row i.
         * Processing rows forward (1 to n) ensures dependencies are met.
         * 
         * Within a row, columns are independent of each other.
         * dp[i][j] doesn't depend on dp[i][j-1] or dp[i][j+1].
         * So column order doesn't matter (convention: left to right).
         */
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= target; j++) {
                
                /*
                 * ───────────────────────────────────────────────────────────────────
                 * DECISION 1: DON'T INCLUDE nums[i-1] IN SUBSEQUENCE
                 * ───────────────────────────────────────────────────────────────────
                 * 
                 * dp[i][j] = dp[i - 1][j]
                 * 
                 * INTUITION: "Skip the i-th element (nums[i-1])"
                 * 
                 * Meaning:
                 * "The best we can do with first i elements to make sum j
                 *  is the same as the best we could do with first i-1 elements"
                 * 
                 * Why does this make sense?
                 * - If we don't use nums[i-1], it's as if it doesn't exist
                 * - Our answer is entirely determined by nums[0..i-2]
                 * - So we inherit the answer from dp[i-1][j]
                 * 
                 * Example:
                 * nums = [2, 3, 5], currently at i=3 (all elements), j=5
                 * Computing dp[3][5]
                 * 
                 * Option: Skip nums[2]=5
                 * - Use only first 2 elements [2, 3]
                 * - dp[3][5] inherits dp[2][5]
                 * - If dp[2][5] = 2 (subsequence [2,3] gives sum 5, length 2)
                 * - Then skipping gives us the same: length 2
                 * 
                 * This represents the "conservative" choice:
                 * - Don't gamble on the new element
                 * - Stick with what we already know works
                 * - Guarantees we're at least as good as before
                 * 
                 * Index mapping reminder:
                 * - When at dp[i][j], the "new" element is nums[i-1]
                 * - dp[i-1][j] represents solution WITHOUT nums[i-1]
                 * - This is why we use i-1 in the recurrence
                 */
                dp[i][j] = dp[i - 1][j];
                
                /*
                 * ───────────────────────────────────────────────────────────────────
                 * DECISION 2: INCLUDE nums[i-1] IN SUBSEQUENCE
                 * ───────────────────────────────────────────────────────────────────
                 * 
                 * if (nums[i - 1] <= j):
                 *     dp[i][j] = max(dp[i][j], 1 + dp[i - 1][j - nums[i - 1]])
                 * 
                 * PRECONDITION CHECK: Why nums[i-1] <= j?
                 * 
                 * If nums[i-1] > j:
                 * - We need sum j, but this single element exceeds it
                 * - Taking it would give sum > j (overshoot)
                 * - INVALID choice, can't even consider it
                 * - Also prevents array index error: dp[i-1][j-nums[i-1]] with j-nums[i-1] < 0
                 * 
                 * Example of invalid case:
                 * nums[i-1] = 7, j = 5
                 * - Can't use 7 to make sum 5
                 * - Skip this option entirely
                 * 
                 * If nums[i-1] <= j (valid to consider):
                 * 
                 * INTUITION: "Include the i-th element (nums[i-1]) in our subsequence"
                 * 
                 * Step-by-step breakdown:
                 * 
                 * 1. We COMMIT to using nums[i-1]
                 *    - This contributes nums[i-1] to our sum
                 *    - Reduces remaining target from j to j-nums[i-1]
                 * 
                 * 2. New subproblem: dp[i-1][j-nums[i-1]]
                 *    - "With first i-1 elements, make sum j-nums[i-1]"
                 *    - Why i-1? We've "used up" element i-1, can't reuse it
                 *    - Why j-nums[i-1]? We've allocated nums[i-1] to the sum
                 * 
                 * 3. Add 1 to count: 1 + dp[i-1][j-nums[i-1]]
                 *    - The "1" represents nums[i-1] that we just included
                 *    - dp[i-1][...] tells us how many MORE elements we can add
                 *    - Total = 1 (this element) + (elements from remaining)
                 * 
                 * 4. Take maximum: max(skip_option, take_option)
                 *    - dp[i][j] already has skip_option (from line above)
                 *    - Now we compare: is taking better or skipping better?
                 *    - Keep whichever gives MORE elements (maximize length)
                 * 
                 * DETAILED EXAMPLE:
                 * nums = [1, 2, 3], currently at i=2, j=3
                 * Computing dp[2][3] (first 2 elements: [1, 2], make sum 3)
                 * 
                 * Current element: nums[i-1] = nums[1] = 2
                 * 
                 * Skip option (already computed):
                 * - dp[2][3] = dp[1][3]
                 * - "With just [1], can we make 3?"
                 * - dp[1][3] = -1000000 (impossible, only have 1)
                 * 
                 * Take option:
                 * - Include nums[1] = 2
                 * - New target: j - nums[1] = 3 - 2 = 1
                 * - Subproblem: dp[1][1] = "With [1], make sum 1"
                 * - dp[1][1] = 1 (take the single element 1)
                 * - Our answer: 1 + dp[1][1] = 1 + 1 = 2
                 * 
                 * Final: dp[2][3] = max(-1000000, 2) = 2
                 * Interpretation: Use [1, 2], length 2, sum 3 ✓
                 * 
                 * WHY THIS WORKS - THE INDUCTIVE PRINCIPLE:
                 * 
                 * Assume dp[i-1][*] is correctly computed (inductive hypothesis).
                 * 
                 * For dp[i][j]:
                 * - If optimal solution doesn't use nums[i-1] → it's dp[i-1][j]
                 * - If optimal solution uses nums[i-1] → it's 1 + dp[i-1][j-nums[i-1]]
                 * - One of these must be optimal (we try both)
                 * - max() picks the better one
                 * 
                 * This is the BELLMAN PRINCIPLE OF OPTIMALITY:
                 * "An optimal solution contains optimal solutions to subproblems"
                 * 
                 * ───────────────────────────────────────────────────────────────────
                 * HANDLING IMPOSSIBLE STATES
                 * ───────────────────────────────────────────────────────────────────
                 * 
                 * What if dp[i-1][j-nums[i-1]] = -1000000?
                 * - Means subproblem is impossible
                 * - 1 + (-1000000) = -999999 (still very negative)
                 * - max(dp[i][j], -999999) will pick dp[i][j] if it's better
                 * - If both options are negative, the less negative one survives
                 * 
                 * Propagation of impossibility:
                 * - If no way to make sum j with first i elements
                 * - Both skip and take will be negative
                 * - dp[i][j] stays negative
                 * - Eventually manifests as -1 in final answer
                 * 
                 * Example:
                 * nums = [5, 10], target = 3
                 * - Can't make 3 from [5] or [10] or [5,10]
                 * - All dp[i][3] remain -1000000
                 * - dp[2][3] = -1000000
                 * - Final answer: -1 (correctly indicates impossible)
                 */
                if (nums[i - 1] <= j) {
                    dp[i][j] = max(dp[i][j], 1 + dp[i - 1][j - nums[i - 1]]);
                }
            }
        }
        
        /*
         * ═══════════════════════════════════════════════════════════════════════════
         * EXTRACTING THE FINAL ANSWER
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * ans = dp[n][target]
         * 
         * INTERPRETATION:
         * "Using all n elements (first n elements = entire array),
         *  what's the maximum number of elements we can select to make sum = target?"
         * 
         * This is EXACTLY the original problem statement!
         * 
         * Why dp[n][target] specifically?
         * - Row n: Represents considering ALL elements nums[0..n-1]
         * - Column target: Represents the exact sum we want
         * - This cell contains the solution to the entire problem
         * 
         * The beauty of bottom-up DP:
         * - We built the solution piece by piece
         * - Started with base cases (0 elements, sum 0)
         * - Gradually incorporated more elements
         * - Final cell naturally contains the answer to the original problem
         * 
         * ───────────────────────────────────────────────────────────────────────────
         * CONVERTING TO OUTPUT FORMAT
         * ───────────────────────────────────────────────────────────────────────────
         * 
         * return ans < 0 ? -1 : ans
         * 
         * PROBLEM REQUIREMENT:
         * - Return -1 if no valid subsequence exists
         * - Otherwise return the maximum length
         * 
         * Why check "ans < 0"?
         * - Valid lengths are always non-negative (0, 1, 2, ..., n)
         * - If ans = -1000000, it means impossible
         * - Any negative value indicates impossibility
         * - Convert to -1 as per problem specification
         * 
         * Edge cases handled:
         * 
         * Case 1: ans = 0
         * - Means empty subsequence (length 0) achieves target
         * - Only possible if target = 0
         * - Return 0 (valid answer)
         * 
         * Case 2: ans = positive number
         * - Valid subsequence found
         * - Return the length directly
         * 
         * Case 3: ans = -1000000 (or any negative)
         * - No valid subsequence exists
         * - Return -1
         * 
         * Example outcomes:
         * 
         * nums = [1, 2, 3], target = 6
         * - Best: [1, 2, 3], sum = 6, length = 3
         * - ans = 3, return 3 ✓
         * 
         * nums = [1, 2, 3], target = 10
         * - No subsequence sums to 10 (max possible is 1+2+3=6)
         * - ans = -1000000, return -1 ✓
         * 
         * nums = [5], target = 0
         * - Empty subsequence sums to 0
         * - ans = 0, return 0 ✓
         * 
         * ═══════════════════════════════════════════════════════════════════════════
         * COMPLETE TRACE EXAMPLE
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * nums = [1, 2, 3], target = 4
         * 
         * Initial state (after base case):
         *        j=0    j=1      j=2      j=3      j=4
         * i=0     0    -1e6     -1e6     -1e6     -1e6
         * i=1     0    -1e6     -1e6     -1e6     -1e6
         * i=2     0    -1e6     -1e6     -1e6     -1e6
         * i=3     0    -1e6     -1e6     -1e6     -1e6
         * 
         * Process i=1 (element nums[0]=1):
         *   j=1: skip=dp[0][1]=-1e6, take=1+dp[0][0]=1 → dp[1][1]=1
         *   j=2: skip=dp[0][2]=-1e6, take=1+dp[0][1]=-1e6 → dp[1][2]=-1e6
         *   j=3: skip=dp[0][3]=-1e6, take=1+dp[0][2]=-1e6 → dp[1][3]=-1e6
         *   j=4: skip=dp[0][4]=-1e6, take=1+dp[0][3]=-1e6 → dp[1][4]=-1e6
         * 
         * After i=1:
         *        j=0    j=1      j=2      j=3      j=4
         * i=1     0      1      -1e6     -1e6     -1e6
         * 
         * Process i=2 (element nums[1]=2):
         *   j=1: skip=dp[1][1]=1, nums[1]=2>1 → can't take → dp[2][1]=1
         *   j=2: skip=dp[1][2]=-1e6, take=1+dp[1][0]=1 → dp[2][2]=1
         *   j=3: skip=dp[1][3]=-1e6, take=1+dp[1][1]=1+1=2 → dp[2][3]=2
         *   j=4: skip=dp[1][4]=-1e6, take=1+dp[1][2]=-1e6 → dp[2][4]=-1e6
         * 
         * After i=2:
         *        j=0    j=1      j=2      j=3      j=4
         * i=2     0      1        1        2      -1e6
         * 
         * Process i=3 (element nums[2]=3):
         *   j=1: skip=dp[2][1]=1, nums[2]=3>1 → can't take → dp[3][1]=1
         *   j=2: skip=dp[2][2]=1, nums[2]=3>2 → can't take → dp[3][2]=1
         *   j=3: skip=dp[2][3]=2, take=1+dp[2][0]=1 → max(2,1)=2 → dp[3][3]=2
         *   j=4: skip=dp[2][4]=-1e6, take=1+dp[2][1]=1+1=2 → dp[3][4]=2
         * 
         * Final table:
         *        j=0    j=1      j=2      j=3      j=4
         * i=3     0      1        1        2        2
         * 
         * Answer: dp[3][4] = 2
         * This represents subsequence [1, 3]: 1+3=4, length=2 ✓
         * (or [2, 2] if we had duplicate 2s, but we don't)
         * 
         * Wait, let me recalculate j=4 for i=3:
         *   skip = dp[2][4] = -1e6
         *   nums[2] = 3 <= 4 ✓
         *   take = 1 + dp[2][4-3] = 1 + dp[2][1] = 1 + 1 = 2
         *   dp[3][4] = max(-1e6, 2) = 2 ✓
         * 
         * Actual subsequence: nums[0]=1, nums[2]=3 → [1,3], sum=4, length=2 ✓
         */
        
        int ans = dp[n][target];
        return ans < 0 ? -1 : ans;
    }
};

