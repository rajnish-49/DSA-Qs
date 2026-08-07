#include <bits/stdc++.h>
using namespace std;

/*
    Problem: Unique Paths II
    Link: https://leetcode.com/problems/unique-paths-ii/

    A robot starts at the top-left cell (0, 0) and wants to reach
    the bottom-right cell (m - 1, n - 1).

    It can move only:
    - Right: (i, j + 1)
    - Down:  (i + 1, j)

    obstacleGrid[i][j] == 1 means the cell is blocked and cannot be used.
    Return the total number of valid paths to the destination.
*/

class Solution
{
public:
    int m, n;
    vector<vector<int>> dp;

    // ============================================================
    // MEMOIZATION
    // ============================================================

    /*
        solve(i, j) = number of valid paths from cell (i, j)
                      to the destination.
    */
    int solve(int i, int j, vector<vector<int>> &obstacleGrid)
    {
        // Moving outside the grid does not form a valid path.
        if (i >= m || j >= n)
            return 0;

        // A blocked cell cannot be part of any path.
        if (obstacleGrid[i][j] == 1)
            return 0;

        // Reaching the destination forms one valid path.
        if (i == m - 1 && j == n - 1)
            return 1;

        if (dp[i][j] != -1)
            return dp[i][j];

        // Every valid path continues either towards the right or down.
        int moveRight = solve(i, j + 1, obstacleGrid);
        int moveDown = solve(i + 1, j, obstacleGrid);

        return dp[i][j] = moveRight + moveDown;
    }

    int uniquePathsWithObstaclesMemo(vector<vector<int>> &obstacleGrid)
    {
        m = obstacleGrid.size();
        n = obstacleGrid[0].size();

        // No path exists if the starting or destination cell is blocked.
        if (obstacleGrid[0][0] == 1 ||
            obstacleGrid[m - 1][n - 1] == 1)
        {
            return 0;
        }

        dp.assign(m, vector<int>(n, -1));

        return solve(0, 0, obstacleGrid);
    }

    // ============================================================
    // TABULATION
    // ============================================================

    int uniquePathsWithObstaclesTabulation(
        vector<vector<int>> &obstacleGrid)
    {
        int m = obstacleGrid.size();
        int n = obstacleGrid[0].size();

        // No path exists if the starting or destination cell is blocked.
        if (obstacleGrid[0][0] == 1 ||
            obstacleGrid[m - 1][n - 1] == 1)
        {
            return 0;
        }

        /*
            dp[i][j] = number of valid paths from the starting cell
                       to cell (i, j).
        */
        vector<vector<int>> dp(m, vector<int>(n, 0));

        // There is one way to be at the starting cell: begin there.
        dp[0][0] = 1;

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (i == 0 && j == 0)
                    continue;

                // A blocked cell cannot be reached through any valid path.
                if (obstacleGrid[i][j] == 1)
                {
                    dp[i][j] = 0;
                    continue;
                }

                // Reach the current cell from the cell above or the cell on the left.
                int fromTop = (i > 0) ? dp[i - 1][j] : 0;
                int fromLeft = (j > 0) ? dp[i][j - 1] : 0;

                dp[i][j] = fromTop + fromLeft;
            }
        }

        return dp[m - 1][n - 1];
    }

    int uniquePathsWithObstacles(vector<vector<int>> &obstacleGrid)
    {
        return uniquePathsWithObstaclesTabulation(obstacleGrid);
    }
};

/*
    Memoization:
    Time Complexity: O(m * n)
    Space Complexity: O(m * n) DP table + O(m + n) recursion stack

    Tabulation:
    Time Complexity: O(m * n)
    Space Complexity: O(m * n)
*/