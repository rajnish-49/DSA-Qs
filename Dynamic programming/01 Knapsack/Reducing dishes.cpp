#include <bits/stdc++.h>
using namespace std;

/*
    Problem: Reducing Dishes
    Link: https://leetcode.com/problems/reducing-dishes/

    Each chosen dish contributes:

        cookingTime * satisfaction

    We may discard any dishes and arrange the chosen dishes in any order.

    For any chosen set of dishes, it is optimal to cook them in increasing
    order of satisfaction so that larger satisfaction values get larger times.

    Therefore, first sort satisfaction and then decide for every dish
    whether to take it or skip it.
*/


// ============================================================
// MEMOIZATION
// ============================================================

class Solution {
public:
    int n;
    vector<vector<int>> dp;

    /*
        solve(i, cnt) = maximum like-time coefficient we can obtain
                        using dishes from index i onward when we have
                        already selected cnt dishes.

        If we take satisfaction[i], it becomes the (cnt + 1)th cooked dish,
        so its contribution is satisfaction[i] * (cnt + 1).
    */
    int solve(int i, int cnt, vector<int>& satisfaction) {

        // No dishes remain to consider.
        if (i == n)
            return 0;

        if (dp[i][cnt] != -1)
            return dp[i][cnt];

        // Discard the current dish, so the number of selected dishes stays the same.
        int skip = solve(i + 1, cnt, satisfaction);

        /*
            Take the current dish.

            cnt dishes have already been chosen, so this dish is cooked
            at time cnt + 1. After taking it, the selected count becomes cnt + 1.
        */
        int take = satisfaction[i] * (cnt + 1) +
                   solve(i + 1, cnt + 1, satisfaction);

        return dp[i][cnt] = max(skip, take);
    }

    int maxSatisfaction(vector<int>& satisfaction) {

        // Sorting ensures chosen dishes are considered in increasing satisfaction order.
        sort(satisfaction.begin(), satisfaction.end());

        n = satisfaction.size();

        dp.assign(n + 1, vector<int>(n + 1, -1));

        // Initially no dish has been selected.
        return solve(0, 0, satisfaction);
    }
};


// ============================================================
// TABULATION
// ============================================================

class Solution {
public:
    int maxSatisfaction(vector<int>& satisfaction) {
        int n = satisfaction.size();

        sort(satisfaction.begin(), satisfaction.end());

        /*
            dp[i][k] = maximum like-time coefficient obtainable using
                       the first i sorted dishes while selecting exactly k dishes.

            Since the selected dishes are processed in sorted order,
            when the current dish becomes the kth selected dish,
            its cooking time is k.

            INT_MIN represents an impossible state.
        */
        vector<vector<int>> dp(
            n + 1,
            vector<int>(n + 1, INT_MIN)
        );

        // With zero dishes considered, selecting zero dishes gives score 0.
        dp[0][0] = 0;

        for (int i = 1; i <= n; i++) {

            // We can always select zero dishes by discarding everything considered so far.
            dp[i][0] = 0;

            for (int k = 1; k <= i; k++) {

                /*
                    Skip satisfaction[i - 1].

                    We still need exactly k selected dishes, all coming
                    from the previous i - 1 dishes.
                */
                dp[i][k] = dp[i - 1][k];

                /*
                    Take satisfaction[i - 1].

                    Before taking it, we must already have selected k - 1 dishes.
                    The current dish becomes the kth cooked dish, so its
                    contribution is satisfaction[i - 1] * k.
                */
                if (dp[i - 1][k - 1] != INT_MIN) {
                    dp[i][k] = max(
                        dp[i][k],
                        dp[i - 1][k - 1] + satisfaction[i - 1] * k
                    );
                }
            }
        }

        /*
            We are allowed to cook any number of dishes, including zero,
            so take the best answer over every possible selected count k.
        */
        int ans = 0;

        for (int k = 0; k <= n; k++) {
            ans = max(ans, dp[n][k]);
        }

        return ans;
    }
};