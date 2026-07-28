/*
 * MAXIMUM NUMBER OF JUMPS TO REACH LAST INDEX
 *
 * Problem: Given nums and target, find the maximum number of jumps to reach
 * index n-1 from index 0. Jump from i to j is valid iff i < j and
 * |nums[j] - nums[i]| <= target. Return -1 if unreachable.
 *
 * Brute force: Try all paths recursively — exponential, too slow.
 *
 * Key insight: The maximum jumps to reach index i depends only on the maximum
 * jumps to reach each valid predecessor j < i, not on how we got to j.
 * This is optimal substructure — so DP works.
 *
 * State: dp[i] = maximum number of jumps to reach index i from index 0.
 *               -1 means index i is unreachable.
 *
 * Transition: for each i, scan all j < i.
 *             if j is reachable and jump j->i is valid:
 *             dp[i] = max(dp[i], dp[j] + 1)
 *             (+1 because jumping from j to i costs exactly one jump)
 *
 * Base case: dp[0] = 0 (already at index 0, zero jumps used)
 *
 * Answer: dp[n-1], which stays -1 if never updated (unreachable)
 *
 * Time: O(n^2) — for each i, scan all j < i
 * Space: O(n) — dp array
 */

class Solution {
public:
    int maximumJumps(vector<int>& nums, int target) {

        int n = nums.size();

        // -1 = unreachable; only index 0 is reachable at the start
        vector<int> dp(n, -1);
        dp[0] = 0;

        for (int i = 1; i < n; i++) {
            for (int j = 0; j < i; j++) {
                // j must be reachable, and the value difference must be within target
                if (dp[j] != -1 && abs(nums[i] - nums[j]) <= target) {
                    // one more jump on top of however many it took to reach j
                    dp[i] = max(dp[i], dp[j] + 1);
                }
            }
        }

        // -1 if n-1 was never reached, otherwise the maximum jump count
        return dp[n - 1];
    }
};