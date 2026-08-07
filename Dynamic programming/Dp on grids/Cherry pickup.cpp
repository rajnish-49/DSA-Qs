#include <bits/stdc++.h>
using namespace std;

/*
    Problem: Cherry Pickup
    Link: https://leetcode.com/problems/cherry-pickup/

    We have an n x n grid:

    - 0 means an empty cell.
    - 1 means the cell contains one cherry.
    - -1 means the cell is blocked.

    Originally, one person must:
    1. Travel from (0, 0) to (n - 1, n - 1) using right/down moves.
    2. Return to (0, 0) using left/up moves.

    Instead of handling the forward and return journeys separately,
    imagine two people starting at (0, 0) and moving towards
    (n - 1, n - 1) at the same time.

    Each person moves either right or down on every step.

    If both people occupy the same cell, its cherry is counted only once.
*/

class Solution
{
public:
    int cherryPickup(vector<vector<int>> &grid)
    {
        int n = grid.size();

        /*
            Each person needs exactly 2 * (n - 1) moves to travel
            from (0, 0) to (n - 1, n - 1).
        */
        int steps = 2 * (n - 1);

        /*
            After t moves:

                row + column = t

            Therefore, if we know a person's row, their column is fixed:

                c1 = t - r1
                c2 = t - r2

            State:

            dp[r1][r2][t] = maximum cherries collected after t moves,
                            when person 1 is at row r1 and
                            person 2 is at row r2.

            Their columns are derived from t.

            -1e9 represents an unreachable state.
        */
        vector<vector<vector<int>>> dp(
            n,
            vector<vector<int>>(
                n,
                vector<int>(steps + 1, -1e9)));

        // Both people begin at (0, 0), and its cherry is collected once.
        dp[0][0][0] = grid[0][0];

        for (int t = 1; t <= steps; t++)
        {
            for (int r1 = 0; r1 < n; r1++)
            {
                for (int r2 = 0; r2 < n; r2++)
                {
                    // Columns are determined by the number of moves made.
                    int c1 = t - r1;
                    int c2 = t - r2;

                    // Ignore states whose calculated columns lie outside the grid.
                    if (c1 < 0 || c1 >= n || c2 < 0 || c2 >= n)
                        continue;

                    // Neither person can stand on a blocked cell.
                    if (grid[r1][c1] == -1 || grid[r2][c2] == -1)
                        continue;

                    int best = -1e9;

                    /*
                        Each person could have reached the current cell
                        either from the left or from above.

                        dp[r1][r2]:
                        Both people moved right, so their rows did not change.
                    */
                    best = max(best, dp[r1][r2][t - 1]);

                    // Person 1 moved down, while person 2 moved right.
                    if (r1 > 0)
                        best = max(best, dp[r1 - 1][r2][t - 1]);

                    // Person 1 moved right, while person 2 moved down.
                    if (r2 > 0)
                        best = max(best, dp[r1][r2 - 1][t - 1]);

                    // Both people moved down.
                    if (r1 > 0 && r2 > 0)
                        best = max(best, dp[r1 - 1][r2 - 1][t - 1]);

                    // None of the four previous states was reachable.
                    if (best < 0)
                        continue;

                    // Person 1 collects the cherry at the current cell.
                    int cherries = best + grid[r1][c1];

                    /*
                        If both rows are different, the people are on
                        different cells and person 2's cherry is also added.

                        At the same step t, equal rows also imply equal columns,
                        so r1 == r2 means both people occupy the same cell.
                    */
                    if (r1 != r2)
                        cherries += grid[r2][c2];

                    dp[r1][r2][t] = cherries;
                }
            }
        }

        /*
            If the destination is unreachable, the stored value remains
            negative. In that case, the required answer is 0.
        */
        return max(0, dp[n - 1][n - 1][steps]);
    }
};

/*
    Time Complexity: O(n³)

    There are O(n) steps, and for every step we try all
    combinations of r1 and r2.

    Space Complexity: O(n³)

    The DP table stores:
        n possible rows for person 1
        n possible rows for person 2
        O(n) possible step values
*/