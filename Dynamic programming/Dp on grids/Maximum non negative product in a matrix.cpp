#include <bits/stdc++.h>
using namespace std;

/*
    Problem: Maximum Non-Negative Product in a Matrix
    Link: https://leetcode.com/problems/maximum-non-negative-product-in-a-matrix/

    We start at (0, 0) and must reach (m - 1, n - 1).
    From each cell, we can move only right or down.

    We need the maximum non-negative product among all valid paths.
    If every path has a negative product, return -1.

    Why store both maximum and minimum products?

    A negative cell can reverse the order of products:
    - A large positive product multiplied by a negative number becomes very negative.
    - A very negative product multiplied by a negative number becomes very positive.

    Therefore, for every cell, we must remember both:
    - the maximum product reaching that cell
    - the minimum product reaching that cell

    State:

    dp[i][j].first  = maximum product of any path from (0, 0) to (i, j)
    dp[i][j].second = minimum product of any path from (0, 0) to (i, j)
*/

class Solution
{
public:
    int maxProductPath(vector<vector<int>> &grid)
    {
        int m = grid.size();
        int n = grid[0].size();

        using ll = long long;

        vector<vector<pair<ll, ll>>> dp(
            m,
            vector<pair<ll, ll>>(n));

        // The only path reaching the starting cell contains grid[0][0] itself.
        dp[0][0] = {grid[0][0], grid[0][0]};

        /*
            Initialize the first column.

            Cells in the first column can only be reached from the cell above,
            so there is only one possible path to each of these cells.
        */
        for (int i = 1; i < m; i++)
        {
            ll val = grid[i][0];

            /*
                Multiply the current value with both the maximum and minimum
                products from the cell above.

                We still use max and min because a negative current value can
                swap which product becomes the maximum and minimum.
            */
            ll a = dp[i - 1][0].first * val;
            ll b = dp[i - 1][0].second * val;

            dp[i][0].first = max(a, b);
            dp[i][0].second = min(a, b);
        }

        /*
            Initialize the first row.

            Cells in the first row can only be reached from the cell on the left.
        */
        for (int j = 1; j < n; j++)
        {
            ll val = grid[0][j];

            ll a = dp[0][j - 1].first * val;
            ll b = dp[0][j - 1].second * val;

            dp[0][j].first = max(a, b);
            dp[0][j].second = min(a, b);
        }

        /*
            Every remaining cell can be reached from:
            - the cell above: (i - 1, j)
            - the cell on the left: (i, j - 1)

            Both previous cells store a maximum and a minimum product,
            giving four possible products for the current cell.
        */
        for (int i = 1; i < m; i++)
        {
            for (int j = 1; j < n; j++)
            {
                ll val = grid[i][j];

                // Products obtained by entering the current cell from above.
                ll a = dp[i - 1][j].first * val;
                ll b = dp[i - 1][j].second * val;

                // Products obtained by entering the current cell from the left.
                ll c = dp[i][j - 1].first * val;
                ll d = dp[i][j - 1].second * val;

                // Keep both extremes because either may become useful later.
                dp[i][j].first = max({a, b, c, d});
                dp[i][j].second = min({a, b, c, d});
            }
        }

        // The maximum product reaching the destination is the required candidate.
        ll ans = dp[m - 1][n - 1].first;

        const ll MOD = 1e9 + 7;

        // No non-negative path product exists.
        if (ans < 0)
            return -1;

        /*
            Apply modulo only after finding the actual maximum product.

            Applying modulo during the DP would change product values and could
            destroy their correct ordering while comparing maximum and minimum.
        */
        return ans % MOD;
    }
};

/*
    Time Complexity: O(m * n)

    Every cell is processed once and considers four candidate products.

    Space Complexity: O(m * n)

    Each cell stores its maximum and minimum reachable products.
*/