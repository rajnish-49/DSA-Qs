#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

class Solution
{
public:
    int minCost(int n, vector<int> &cuts)
    {
        cuts.push_back(0);
        cuts.push_back(n);
        sort(cuts.begin(), cuts.end());
        int m = cuts.size();

        vector<vector<int>> memo(m, vector<int>(m, -1));

        function<int(int, int)> solve = [&](int i, int j) -> int
        {
            if (j - i <= 1)
                return 0;
            if (memo[i][j] != -1)
                return memo[i][j];
            memo[i][j] = 1e9;
            for (int k = i + 1; k < j; k++)
            {
                int cost = solve(i, k) + solve(k, j) + cuts[j] - cuts[i];
                memo[i][j] = min(memo[i][j], cost);
            }
            return memo[i][j];
        };

        solve(0, m - 1);

        vector<vector<int>> dp(m, vector<int>(m, 0));
        for (int len = 2; len < m; len++)
        {
            for (int i = 0; i + len <= m - 1; i++)
            {
                int j = i + len;
                dp[i][j] = 1e9;
                for (int k = i + 1; k < j; k++)
                {
                    int cost = dp[i][k] + dp[k][j] + cuts[j] - cuts[i];
                    dp[i][j] = min(dp[i][j], cost);
                }
            }
        }

        return dp[0][m - 1];
    }
};
