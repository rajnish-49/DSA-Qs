#include <bits/stdc++.h>
using namespace std;

/*
    Problem: Unique Paths
    Link: https://leetcode.com/problems/unique-paths/

    A robot starts at the top-left cell (0, 0) of an m x n grid
    and wants to reach the bottom-right cell (m - 1, n - 1).

    From each cell, it can move only:
    - Right: (i, j + 1)
    - Down:  (i + 1, j)

    Return the total number of distinct paths to the destination.
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
        solve(i, j) = number of paths from cell (i, j)
                      to the destination (m - 1, n - 1).
    */
    int solve(int i, int j)
    {
        // A path leaving the grid is invalid.
        if (i >= m || j >= n)
            return 0;

        // Reaching the destination forms one valid path.
        if (i == m - 1 && j == n - 1)
            return 1;

        if (dp[i][j] != -1)
            return dp[i][j];

        /*
            Every path from (i, j) must begin by moving either right or down,
            so add the number of paths available from both next cells.
        */
        int moveRight = solve(i, j + 1);
        int moveDown = solve(i + 1, j);

        return dp[i][j] = moveRight + moveDown;
    }

    int uniquePathsMemo(int M, int N)
    {
        m = M;
        n = N;

        dp.assign(m, vector<int>(n, -1));

        return solve(0, 0);
    }

    // ============================================================
    // TABULATION: START TO DESTINATION
    // ============================================================

    int uniquePathsTabForward(int m, int n)
    {
        /*
            dp[i][j] = number of paths from the starting cell (0, 0)
                       to the current cell (i, j).
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

                // A path can reach (i, j) from the cell above or the cell on the left.
                int fromTop = (i > 0) ? dp[i - 1][j] : 0;
                int fromLeft = (j > 0) ? dp[i][j - 1] : 0;

                dp[i][j] = fromTop + fromLeft;
            }
        }

        return dp[m - 1][n - 1];
    }

/*
    Memoization:
    Time Complexity: O(m * n)
    Space Complexity: O(m * n) DP table + O(m + n) recursion stack

    Tabulation:
    Time Complexity: O(m * n)
    Space Complexity: O(m * n)
*/