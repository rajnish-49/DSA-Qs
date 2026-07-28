// @https://leetcode.com/problems/find-the-maximum-number-of-fruits-collected/


class Solution {
public:
    int maxCollectedFruits(vector<vector<int>>& fruits) {

        //     int n = fruits.size();

        //     vector<vector<vector<int>>> dp(
        //         n, vector<vector<int>>(n, vector<int>(n, -1)));

        //     dp[0][n - 1][n - 1] = fruits[0][0] + fruits[0][n−1] +
        //     fruits[n−1][0];

        //     for (int t = 1; t < n; t++) {
        //         for (c2 = 0; c2 < n; c2++) {
        //             for (r3 = 0; r3 < n; r3++) {

        //                 gain = fruits[t][t];

        //                 if (c2 != t)
        //                     gain += fruits[t][c2];

        //                 if (r3 != t && r3 != c2)
        //                     gain += fruits[r3][t];
        //             }
        //         }
        //     }

        int n = fruits.size();
        vector<vector<int>> grid = fruits;

        int child1 = 0;
        for (int i = 0; i < n; i++) {
            child1 += grid[i][i];
            grid[i][i] = 0;
        }

        vector<vector<int>> dp2(n, vector<int>(n, INT_MIN));
        dp2[0][n - 1] = grid[0][n - 1];

        for (int k = 1; k < n; k++) {
            for (int j = n - 1 - k; j <= n - 1; j++) {
                int best = INT_MIN;
                if (j - 1 >= 0 && dp2[k - 1][j - 1] != INT_MIN)
                    best = max(best, dp2[k - 1][j - 1]);
                if (dp2[k - 1][j] != INT_MIN)
                    best = max(best, dp2[k - 1][j]);
                if (j + 1 < n && dp2[k - 1][j + 1] != INT_MIN)
                    best = max(best, dp2[k - 1][j + 1]);

                if (best != INT_MIN)
                    dp2[k][j] = best + grid[k][j];
            }
        }
        int child2 = dp2[n - 1][n - 1];

    
        vector<vector<int>> tgrid(n, vector<int>(n));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                tgrid[j][i] = grid[i][j];


        vector<vector<int>> dp3(n, vector<int>(n, INT_MIN));
        dp3[0][n - 1] = tgrid[0][n - 1];

        for (int k = 1; k < n; k++) {
            for (int j = n - 1 - k; j <= n - 1; j++) {
                int best = INT_MIN;
                if (j - 1 >= 0 && dp3[k - 1][j - 1] != INT_MIN)
                    best = max(best, dp3[k - 1][j - 1]);
                if (dp3[k - 1][j] != INT_MIN)
                    best = max(best, dp3[k - 1][j]);
                if (j + 1 < n && dp3[k - 1][j + 1] != INT_MIN)
                    best = max(best, dp3[k - 1][j + 1]);

                if (best != INT_MIN)
                    dp3[k][j] = best + tgrid[k][j];
            }
        }
        int child3 = dp3[n - 1][n - 1];

        return child1 + child2 + child3;
    }
};