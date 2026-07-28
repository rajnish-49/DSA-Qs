/*
 * ============================================================================
 * Problem: Delete and Earn (LeetCode 740)
 * ============================================================================
 * 
 * Description:
 * ------------
 * You are given an integer array nums. You want to maximize the number of 
 * points you get by performing the following operation any number of times:
 * 
 * - Pick any nums[i] and delete it to earn nums[i] points.
 * - Afterwards, you must delete every element equal to nums[i] - 1 and 
 *   every element equal to nums[i] + 1.
 * 
 * Return the maximum number of points you can earn.
 * 
 * ============================================================================
 * Key Insight:
 * ============================================================================
 * This problem reduces to the "House Robber" problem!
 * 
 * - If we pick value x, we delete ALL occurrences of x (earn x * count[x])
 * - We also delete ALL x-1 and x+1 (can't earn from adjacent values)
 * - This is like houses: if we "rob" value x, we can't "rob" x-1 or x+1
 * 
 * So we transform nums into a frequency map and apply House Robber on VALUES.
 * 
 * ============================================================================
 * Approaches:
 * ============================================================================
 * 
 * 1. Tabulation (Bottom-Up DP)
 *    - dp[x] = max points considering values 0 to x
 *    - Time: O(n + maxVal)  |  Space: O(maxVal)
 * 
 * 2. Memoization (Top-Down DP)
 *    - Recursive with memoization
 *    - Time: O(n + maxVal)  |  Space: O(maxVal) + recursion stack
 * 
 * 3. Space Optimized
 *    - Only track prev1 (dp[x-1]) and prev2 (dp[x-2])
 *    - Time: O(n + maxVal)  |  Space: O(n) for frequency map only
 * 
 * ============================================================================
 */

// ============================================================================
// Approach 1: Tabulation (Bottom-Up DP)
// ============================================================================
class Solution {
public:
    int deleteAndEarn(vector<int>& nums) {

        // We need to know the maximum value present,
        // because DP is built over VALUES, not indices.
        int maxVal = *max_element(nums.begin(), nums.end());

        /*
         * mp[x] = how many times value x appears
         *
         * Why this compression is crucial:
         * The original order of nums does not matter.
         * Only the COUNT of each value matters,
         * because deleting x deletes all x-1 and x+1 anyway.
         */
        unordered_map<int,int> mp;
        for (int x : nums) mp[x]++;

        /*
         * dp[x] = maximum points we can earn
         * considering ONLY values from 0 to x.
         *
         * This is exactly the House Robber definition:
         * dp[x] = best answer up to "house x"
         */
        vector<int> dp(maxVal + 1, 0);

        // Base case:
        // No values → no points
        dp[0] = 0;

        // If we only consider value 1,
        // the only choice is to take all 1s.
        if (maxVal >= 1)
            dp[1] = mp[1];

        /*
         * Transition:
         *
         * For each value x, we have two choices:
         *
         * 1) Skip x
         *    → then result is dp[x-1]
         *
         * 2) Take x
         *    → earn x * mp[x]
         *    → must skip x-1
         *    → so add dp[x-2]
         *
         * We take the better of the two.
         */
        for (int x = 2; x <= maxVal; x++) {
            dp[x] = max(
                dp[x - 1],              // skip x
                dp[x - 2] + x * mp[x]   // take x
            );
        }

        // dp[maxVal] contains the answer
        return dp[maxVal];
    }
};


// ============================================================================
// Approach 2: Memoization (Top-Down DP)
// ============================================================================
class Solution {
public:
    unordered_map<int,int> mp;
    vector<int> memo;

    /*
     * solve(x) = maximum points we can earn
     * considering values from 0 to x
     *
     * This mirrors the dp[x] definition,
     * but computed lazily via recursion.
     */
    int solve(int x) {

        // Base cases:
        if (x == 0) return 0;
        if (x == 1) return mp[1];

        // If already computed, reuse it.
        // This avoids exponential recomputation.
        if (memo[x] != -1)
            return memo[x];

        /*
         * Choice 1: Take x
         *   → earn x * mp[x]
         *   → must skip x-1
         *   → next allowed is x-2
         */
        int take = solve(x - 2) + x * mp[x];

        /*
         * Choice 2: Skip x
         *   → best remains whatever we got up to x-1
         */
        int skip = solve(x - 1);

        // Store and return the best choice
        return memo[x] = max(take, skip);
    }

    int deleteAndEarn(vector<int>& nums) {

        int maxVal = *max_element(nums.begin(), nums.end());

        // Build frequency map
        for (int x : nums) mp[x]++;

        /*
         * memo[x] = -1 means "not yet computed"
         * Size is maxVal + 1 because we solve for values, not indices
         */
        memo.assign(maxVal + 1, -1);

        return solve(maxVal);
    }
};


// ============================================================================
// Approach 3: Space Optimized DP
// ============================================================================
class Solution {
public:
    int deleteAndEarn(vector<int>& nums) {

        int maxVal = *max_element(nums.begin(), nums.end());

        unordered_map<int,int> mp;
        for (int x : nums) mp[x]++;

        /*
         * prev2 = dp[x-2]
         * prev1 = dp[x-1]
         *
         * At every step, we compute dp[x]
         * using only these two values.
         *
         * This is possible because:
         * dp[x] depends ONLY on dp[x-1] and dp[x-2]
         */
        int prev2 = 0;
        int prev1 = mp[1];

        for (int x = 2; x <= maxVal; x++) {

            /*
             * Same transition, just without an array:
             *
             * curr = max(
             *     prev1,              // skip x
             *     prev2 + x * mp[x]   // take x
             * )
             */
            int curr = max(prev1, prev2 + x * mp[x]);

            // Shift the window forward
            prev2 = prev1;
            prev1 = curr;
        }

        // prev1 always holds dp[maxVal]
        return prev1;
    }
};
