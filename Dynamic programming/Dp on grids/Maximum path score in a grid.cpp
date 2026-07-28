class Solution {
public:
    int maxPathScore(vector<vector<int>>& grid, int k) {

        int m = grid.size();
        int n = grid[0].size();

        vector<vector<vector<int>>> dp(
            m, vector<vector<int>>(n, vector<int>(k + 1, -1)));

        dp[0][0][0] = 0;

        for (int i = 1; i < n; i++) {
            int score = grid[0][i];
            int cost = (score > 0) ? 1 : 0;
            for (int c = 0; c <= k; c++) {
                if (dp[0][i - 1][c] == -1)
                    continue;
                int costhere = c + cost;
                if (costhere <= k) {
                    dp[0][i][costhere] = dp[0][i - 1][c] + score;
                }
            }
        }

        for (int i = 1; i < m; i++) {
            int score = grid[i][0];
            int cost = (score > 0) ? 1 : 0;
            for (int c = 0; c <= k; c++) {
                if (dp[i - 1][0][c] == -1)
                    continue;
                int costhere = c + cost;
                if (costhere <= k) {
                    dp[i][0][costhere] = dp[i - 1][0][c] + score;
                }
            }
        }

        for (int i = 1; i < m; i++) {
            for (int j = 1; j < n; j++) {
                int score = grid[i][j];
                int cost = (score > 0) ? 1 : 0;

                for (int c = 0; c <= k; c++) {

                    int from_above = dp[i - 1][j][c];
                    int from_left = dp[i][j - 1][c];

                    int best = -1;
                    if (from_above != -1)
                        best = max(best, from_above);
                    if (from_left != -1)
                        best = max(best, from_left);

                    if (best == -1)
                        continue;

                    int costhere = c + cost;
                    if (costhere <= k) {
                        dp[i][j][costhere] =
                            max(dp[i][j][costhere], best + score);
                    }
                }
            }
        }
    

    int ans = -1;
    for (int c = 0; c <= k; c++) {
        if (dp[m - 1][n - 1][c] != -1)
            ans = max(ans, dp[m - 1][n - 1][c]);
    }
    return ans;
}
};