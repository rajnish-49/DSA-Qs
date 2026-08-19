#include <bits/stdc++.h>
using namespace std;

/*
    Problem: Count Subsets with Sum K / Perfect Sum

    Given an array arr and a target, count how many subsets have
    sum exactly equal to target.

    For every element, we have two choices:
    - Skip the element.
    - Take the element, if it does not exceed the remaining target.

    Unlike normal Subset Sum, where we only check whether a subset exists,
    here we add the number of ways from both choices.
*/


// ============================================================
// MEMOIZATION
// ============================================================

class Solution {
public:

    int n;
    vector<vector<int>> dp;

    /*
        solve(idx, target) = number of subsets that can be formed
                             using arr[idx...n-1]
                             whose sum is exactly target.
    */
    int solve(int idx, vector<int>& arr, int target) {

        /*
            All elements have been considered.

            If target became 0, the choices made formed one valid subset.
            Otherwise, this path does not form the required sum.

            Keeping this base case at idx == n also handles zeros correctly.
        */
        if (idx == n) {
            return (target == 0) ? 1 : 0;
        }

        if (dp[idx][target] != -1)
            return dp[idx][target];

        // Skip arr[idx], so the required target remains unchanged.
        int skip = solve(idx + 1, arr, target);

        int take = 0;

        /*
            Take arr[idx] if it fits in the remaining target.

            After taking it, we need to form:
                target - arr[idx]

            using only the remaining elements.
        */
        if (arr[idx] <= target)
            take = solve(idx + 1, arr, target - arr[idx]);

        // Total ways = ways obtained by skipping + ways obtained by taking.
        return dp[idx][target] = skip + take;
    }

    int perfectSum(vector<int>& arr, int target) {

        n = arr.size();

        dp.assign(n, vector<int>(target + 1, -1));

        return solve(0, arr, target);
    }
};


// ============================================================
// TABULATION
// ============================================================

class Solution {
public:

    int perfectSum(vector<int>& arr, int target) {

        int n = arr.size();

        /*
            dp[i][sum] = number of subsets among the first i elements
                         whose sum is exactly 'sum'.

            Row i corresponds to considering arr[0...i-1].
        */
        vector<vector<long long>> dp(
            n + 1,
            vector<long long>(target + 1, 0)
        );

        /*
            With zero elements, there is exactly one way to make sum 0:
            choose the empty subset.
        */
        dp[0][0] = 1;

        for (int i = 1; i <= n; i++) {
            for (int sum = 0; sum <= target; sum++) {

                /*
                    Skip the current element arr[i - 1].

                    Ask the previous row:
                    how many subsets already formed this same sum?
                */
                dp[i][sum] = dp[i - 1][sum];

                /*
                    Take arr[i - 1].

                    After taking it, the remaining required sum is:

                        sum - arr[i - 1]

                    So we ask the previous row how many subsets formed
                    that remaining sum.

                    We use i - 1 because the current element can be used only once.
                */
                if (arr[i - 1] <= sum) {
                    dp[i][sum] +=
                        dp[i - 1][sum - arr[i - 1]];
                }
            }
        }

        return dp[n][target];
    }
};