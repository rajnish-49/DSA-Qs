// @ https://leetcode.com/problems/longest-increasing-subsequence/

class Solution {
public:

    /*
        This function computes the LIS length using recursion + memoization.

        Core idea:
        -----------
        At every index, we face a binary decision:
        1) Take the current element into the subsequence (if valid)
        2) Skip the current element

        The challenge (and the confusion point) is that:
        The validity of "take" depends on what we took LAST.

        That is why we need:
        - idx : where we are in the array
        - p   : index of the previously taken element

        Together, (idx, p) uniquely define the DP state.
    */
    int solve(vector<int>& nums, vector<vector<int>>& dp, int idx, int p) {

        int n = nums.size();

        /*
            BASE CASE:
            ----------
            If idx reaches n, there are no elements left to consider.
            The LIS length from here is 0 because:
            - A subsequence must come from remaining elements
            - No elements => no length contribution
        */
        if (idx >= n) {
            return 0;
        }

        /*
            MEMOIZATION CHECK:
            ------------------
            dp[idx][p] represents:
            "The LIS length starting at index idx,
             given that the previous chosen element is at index p"

            Important detail:
            We do NOT store states where p == -1.

            Why?
            ----
            p == -1 is a virtual state meaning "no element has been chosen yet".
            Storing it would require shifting indices or wasting space.
            More importantly:
            - p == -1 only occurs at the start
            - Its reuse benefit is minimal
            - Avoiding it keeps indexing clean

            Hence: memoize only when p != -1.
        */
        if (p != -1 && dp[idx][p] != -1) {
            return dp[idx][p];
        }

        int take = 0;

        /*
            TAKE DECISION:
            --------------
            We are allowed to take nums[idx] if:
            - No previous element was chosen (p == -1), OR
            - Current element is strictly greater than the last chosen element

            This enforces the STRICTLY increasing property.

            If we take nums[idx]:
            - LIS length increases by 1
            - Current index becomes the new "previous element"
        */
        if (p == -1 || nums[idx] > nums[p]) {
            take = 1 + solve(nums, dp, idx + 1, idx);
        }

        /*
            SKIP DECISION:
            --------------
            We always have the option to skip nums[idx].

            Skipping means:
            - We do NOT include nums[idx] in the subsequence
            - The previous chosen element remains the same (p)
            - We move to the next index
        */
        int skip = solve(nums, dp, idx + 1, p);

        /*
            STORE RESULT:
            -------------
            The best LIS from this state is the maximum of:
            - Taking the current element
            - Skipping the current element

            Again, we store only when p != -1.
        */
        if (p != -1) {
            dp[idx][p] = max(take, skip);
        }

        return max(take, skip);
    }

    /*
        Entry function.

        We start with:
        - idx = 0  → beginning of the array
        - p   = -1 → no element has been chosen yet

        dp size:
        --------
        dp is sized (n+1) x (n+1) to safely handle indexing.
        dp[idx][p] is only meaningful when p != -1.
    */
    int lengthOfLIS(vector<int>& nums) {

        int n = nums.size();

        // Initialize DP table with -1 (uncomputed states)
        vector<vector<int>> dp(n + 1, vector<int>(n + 1, -1));

        return solve(nums, dp, 0, -1);
    }
};
