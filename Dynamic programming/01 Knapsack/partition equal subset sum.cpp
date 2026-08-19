#include <bits/stdc++.h>
using namespace std;

/*
    Problem: Partition Equal Subset Sum
    Link: https://leetcode.com/problems/partition-equal-subset-sum/

    We need to divide nums into two subsets having equal sums.

    If totalSum is odd, equal partition is impossible.

    If totalSum is even:

        target = totalSum / 2

    Now we only need to check whether there exists a subset
    whose sum is exactly target.

    If such a subset exists, all remaining elements automatically
    form the other subset with the same sum.
*/


// ============================================================
// MEMOIZATION
// ============================================================

class Solution {
public:
    int n;
    vector<vector<int>> dp;

    /*
        solve(i, currSum) = whether we can form exactly currSum
                            using elements nums[0...i].
    */
    bool solve(int i, int currSum, vector<int>& nums) {

        // Required sum has been formed.
        if (currSum == 0)
            return true;

        // No elements remain while some sum is still required.
        if (i < 0)
            return false;

        if (dp[i][currSum] != -1)
            return dp[i][currSum];

        // Skip nums[i], so the required sum remains unchanged.
        bool skip = solve(i - 1, currSum, nums);

        bool take = false;

        /*
            Take nums[i] if it fits.

            After taking it, we ask the previous items whether they
            can form the remaining sum currSum - nums[i].
        */
        if (currSum >= nums[i])
            take = solve(i - 1, currSum - nums[i], nums);

        return dp[i][currSum] = skip || take;
    }

    bool canPartition(vector<int>& nums) {

        int sum = 0;

        for (int x : nums)
            sum += x;

        // Two equal integer subset sums are impossible if total sum is odd.
        if (sum % 2 != 0)
            return false;

        int target = sum / 2;
        n = nums.size();

        dp.assign(n, vector<int>(target + 1, -1));

        return solve(n - 1, target, nums);
    }
};


// ============================================================
// TABULATION
// ============================================================

class Solution {
public:
    bool canPartition(vector<int>& nums) {

        int sum = 0;

        for (int x : nums)
            sum += x;

        if (sum % 2 != 0)
            return false;

        int target = sum / 2;
        int n = nums.size();

        /*
            dp[i][s] = whether sum s can be formed using
                       the first i elements.

            Row i corresponds to nums[0...i-1].
        */
        vector<vector<bool>> dp(
            n + 1,
            vector<bool>(target + 1, false)
        );

        // With zero elements, sum 0 can be formed using the empty subset.
        dp[0][0] = true;

        for (int i = 1; i <= n; i++) {
            for (int s = 0; s <= target; s++) {

                // Skip nums[i - 1], so ask the previous row for the same sum.
                dp[i][s] = dp[i - 1][s];

                /*
                    Take nums[i - 1] if it fits.

                    After taking it, we ask the previous row whether
                    the remaining sum s - nums[i - 1] was achievable.
                */
                if (s >= nums[i - 1]) {
                    dp[i][s] =
                        dp[i][s] ||
                        dp[i - 1][s - nums[i - 1]];
                }
            }
        }

        return dp[n][target];
    }
};