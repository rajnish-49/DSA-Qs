class Solution {
public:
    vector<int> pathsWithMaxScore(vector<string>& board) {
        int n = board.size();
        const int MOD = 1e9 + 7;

        // dp[i][j] = {maxSum, ways}; maxSum == -1 means cell unreachable
        vector<vector<pair<long long,long long>>> dp(n, vector<pair<long long,long long>>(n, {-1, 0}));

        dp[n-1][n-1] = {0, 1}; // base case: S

        for (int i = n - 1; i >= 0; i--) {
            for (int j = n - 1; j >= 0; j--) {
                if (i == n-1 && j == n-1) continue; // S already set
                if (board[i][j] == 'X') continue;   // obstacle, stays unreachable

                long long best = -1, ways = 0;

                // from below (i+1, j) -- first candidate, direct assign
                if (i + 1 < n && dp[i+1][j].first != -1) {
                    best = dp[i+1][j].first;
                    ways = dp[i+1][j].second;
                }

                // from right (i, j+1) -- compare against current best
                if (j + 1 < n && dp[i][j+1].first != -1) {
                    long long score = dp[i][j+1].first;
                    if (score > best) {
                        best = score;
                        ways = dp[i][j+1].second;
                    } else if (score == best) {
                        ways = (ways + dp[i][j+1].second) % MOD;
                    }
                }

                // from diagonal (i+1, j+1) -- compare against current best
                if (i + 1 < n && j + 1 < n && dp[i+1][j+1].first != -1) {
                    long long score = dp[i+1][j+1].first;
                    if (score > best) {
                        best = score;
                        ways = dp[i+1][j+1].second;
                    } else if (score == best) {
                        ways = (ways + dp[i+1][j+1].second) % MOD;
                    }
                }

                if (best == -1) continue; // no valid predecessor -> unreachable

                // add this cell's own digit value (0 for 'E')
                int val = (board[i][j] >= '1' && board[i][j] <= '9') ? board[i][j] - '0' : 0;
                dp[i][j] = {best + val, ways};
            }
        }

        // answer sits at E = (0,0)
        if (dp[0][0].first == -1) return {0, 0};
        return {(int)dp[0][0].first, (int)dp[0][0].second};
    }
};