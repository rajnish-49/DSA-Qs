#include <bits/stdc++.h>
using namespace std;

/*
    Problem: Length of the Longest Subsequence That Sums to Target
    Link: https://leetcode.com/problems/length-of-the-longest-subsequence-that-sums-to-target/

    We need to choose a subsequence whose sum is exactly target
    and maximize the number of elements chosen.

    For every element, we have two choices:
    - Skip it.
    - Take it, if it does not exceed the remaining target.

    Impossible states are represented using a large negative value
    so they are never selected while taking the maximum.
*/


// ============================================================
// MEMOIZATION
// ============================================================

class Solution {
public:
    int n;
    vector<vector<int>> dp;

    /*
        solve(idx, target) = maximum length of a subsequence that can be
                             formed using nums[idx...n-1]
                             whose sum is exactly target.
    */
    int solve(int idx, vector<int>& nums, int target) {

        // Required sum has been formed, so no more elements need to be taken.
        if (target == 0) {
            return 0;
        }

        // No elements remain but target is still not formed.
        if (idx == n) {
            return -1000000;
        }

        if (dp[idx][target] != -1)
            return dp[idx][target];

        // Skip the current element.
        int skip = solve(idx + 1, nums, target);

        int take = -1000000;

        /*
            Take nums[idx] if it fits in the remaining target.

            We add 1 because nums[idx] becomes part of the subsequence,
            and reduce the remaining target by nums[idx].
        */
        if (nums[idx] <= target) {
            take = 1 + solve(idx + 1, nums, target - nums[idx]);
        }

        return dp[idx][target] = max(take, skip);
    }

    int lengthOfLongestSubsequence(vector<int>& nums, int target) {
        n = nums.size();

        dp.assign(n, vector<int>(target + 1, -1));

        int ans = solve(0, nums, target);

        // Negative result means no subsequence with sum exactly target exists.
        return ans < 0 ? -1 : ans;
    }
};


// ============================================================
// TABULATION
// ============================================================

class Solution {
public:
    int lengthOfLongestSubsequence(vector<int>& nums, int target) {
        int n = nums.size();

        /*
            dp[i][sum] = maximum length of a subsequence among the first i
                         elements whose sum is exactly 'sum'.

            Row i corresponds to nums[0...i-1].

            -1000000 means that forming this exact sum is impossible.
        */
        vector<vector<int>> dp(
            n + 1,
            vector<int>(target + 1, -1000000)
        );

        /*
            Sum 0 can always be formed by choosing no elements.

            This must be initialized for every row because even after
            considering some elements, the empty subsequence still forms 0.
        */
        for (int i = 0; i <= n; i++) {
            dp[i][0] = 0;
        }

        for (int i = 1; i <= n; i++) {
            for (int sum = 1; sum <= target; sum++) {

                /*
                    nums[i - 1] is the current element because DP rows
                    are 1-indexed while nums is 0-indexed.
                */

                // Skip the current element, so use the answer from the previous row.
                dp[i][sum] = dp[i - 1][sum];

                /*
                    If the current element fits, try taking it.

                    After taking nums[i - 1], the remaining sum becomes:

                        sum - nums[i - 1]

                    dp[i - 1][...] is used because each element can be
                    selected only once.

                    +1 counts the current element in the subsequence length.
                */
                if (nums[i - 1] <= sum) {
                    dp[i][sum] = max(
                        dp[i][sum],
                        1 + dp[i - 1][sum - nums[i - 1]]
                    );
                }
            }
        }

        int ans = dp[n][target];

        return ans < 0 ? -1 : ans;
    }
};