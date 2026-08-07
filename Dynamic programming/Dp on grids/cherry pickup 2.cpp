#include <bits/stdc++.h>
using namespace std;

/*
    Problem: Cherry Pickup II
    Link: https://leetcode.com/problems/cherry-pickup-ii/

    Two robots start in the first row:

        Robot 1 starts at (0, 0)
        Robot 2 starts at (0, n - 1)

    From row r, each robot must move to row r + 1 and may choose:

        Down-left:  column - 1
        Down:       same column
        Down-right: column + 1

    Both robots continue until they reach the last row.

    When the robots visit different cells, cherries from both cells are collected.
    When they visit the same cell, that cell's cherries are collected only once.

    Unlike Cherry Pickup I, the two robots are actually given in this problem;
    no round-trip or path-reversal transformation is required.
*/

class Solution
{
public:
    int m, n;

    // Maximum constraints are 70 x 70, so indices up to 70 are sufficient.
    int dp[71][71][71];

    vector<int> colDir{-1, 0, 1};

    // Check whether a column remains inside the grid.
    bool isSafe(int &col)
    {
        return col >= 0 && col < n;
    }

    // ============================================================
    // APPROACH 1: MEMOIZATION
    // ============================================================

    /*
        State:

        solve(row, c1, c2) = maximum cherries that can be collected
                             from the current row to the last row when:

                             Robot 1 is at (row, c1)
                             Robot 2 is at (row, c2)

        We do not need separate row values for both robots because both
        robots move to the next row after every move and always remain
        in the same row.
    */
    int solve(vector<vector<int>> &grid, int row, int c1, int c2)
    {
        // Both robots have already crossed the final row.
        if (row >= m)
            return 0;

        if (dp[row][c1][c2] != -1)
            return dp[row][c1][c2];

        /*
            Collect cherries from the current cells.

            If both robots occupy the same cell, its cherries must be
            counted only once.
        */
        int currentCherries = grid[row][c1];

        if (c1 != c2)
        {
            currentCherries += grid[row][c2];
        }

        int bestNext = 0;

        /*
            Robot 1 has three possible column movements.
            Robot 2 also has three possible column movements.

            Therefore, there are 3 × 3 = 9 possible combinations
            of moves for the next row.
        */
        for (int &move1 : colDir)
        {
            for (int &move2 : colDir)
            {
                int nextC1 = c1 + move1;
                int nextC2 = c2 + move2;

                // Both robots must remain inside the grid.
                if (isSafe(nextC1) && isSafe(nextC2))
                {
                    bestNext = max(
                        bestNext,
                        solve(grid, row + 1, nextC1, nextC2));
                }
            }
        }

        // Add the current row's cherries to the best result from future rows.
        return dp[row][c1][c2] = currentCherries + bestNext;
    }

    int cherryPickupMemoization(vector<vector<int>> &grid)
    {
        m = grid.size();
        n = grid[0].size();

        memset(dp, -1, sizeof(dp));

        // Robot 1 begins at column 0 and Robot 2 at column n - 1.
        return solve(grid, 0, 0, n - 1);
    }

    // ============================================================
    // APPROACH 2: BACKWARD TABULATION
    // ============================================================

    int cherryPickupBackwardTabulation(vector<vector<int>> &grid)
    {
        int m = grid.size();
        int n = grid[0].size();

        /*
            State:

            dp[row][c1][c2] = maximum cherries collected from this row
                              to the last row when Robot 1 is at c1
                              and Robot 2 is at c2.

            This is the same state used in memoization.
        */
        static int dp[71][71][71];
        memset(dp, 0, sizeof(dp));

        /*
            Base case: last row.

            Once both robots are in the last row, there are no future rows.
            Therefore, the answer is only the cherries in their current cells.
        */
        for (int c1 = 0; c1 < n; c1++)
        {
            for (int c2 = 0; c2 < n; c2++)
            {
                dp[m - 1][c1][c2] = grid[m - 1][c1];

                if (c1 != c2)
                {
                    dp[m - 1][c1][c2] += grid[m - 1][c2];
                }
            }
        }

        /*
            Since the current row depends on the next row,
            process rows from bottom to top.
        */
        for (int row = m - 2; row >= 0; row--)
        {
            for (int c1 = 0; c1 < n; c1++)
            {
                for (int c2 = 0; c2 < n; c2++)
                {
                    int bestNext = 0;

                    // Try all nine combinations of next columns.
                    for (int move1 = -1; move1 <= 1; move1++)
                    {
                        for (int move2 = -1; move2 <= 1; move2++)
                        {
                            int nextC1 = c1 + move1;
                            int nextC2 = c2 + move2;

                            if (nextC1 >= 0 && nextC1 < n &&
                                nextC2 >= 0 && nextC2 < n)
                            {
                                bestNext = max(
                                    bestNext,
                                    dp[row + 1][nextC1][nextC2]);
                            }
                        }
                    }

                    // Collect cherries from the robots' current cells.
                    int currentCherries = grid[row][c1];

                    if (c1 != c2)
                    {
                        currentCherries += grid[row][c2];
                    }

                    dp[row][c1][c2] = currentCherries + bestNext;
                }
            }
        }

        // Initial positions of the two robots.
        return dp[0][0][n - 1];
    }

    // ============================================================
    // APPROACH 3: FORWARD TABULATION
    // ============================================================

    int cherryPickupForwardTabulation(vector<vector<int>> &grid)
    {
        int m = grid.size();
        int n = grid[0].size();

        /*
            State:

            dp[row][c1][c2] = maximum cherries collected from row 0
                              to the current row when Robot 1 is at c1
                              and Robot 2 is at c2.

            This approach builds the answer from the starting positions
            towards the last row.
        */
        int dp[71][71][71] = {0};

        /*
            Both robots begin in row 0.

            If n == 1, they occupy the same cell, so its cherries
            must be counted only once.
        */
        dp[0][0][n - 1] =
            (n == 1)
                ? grid[0][0]
                : grid[0][0] + grid[0][n - 1];

        for (int row = 1; row < m; row++)
        {
            /*
                After 'row' moves, Robot 1 can move at most 'row'
                columns away from column 0.

                Therefore, its reachable columns are:

                    0 to min(n - 1, row)
            */
            for (int c1 = 0; c1 <= min(n - 1, row); c1++)
            {
                /*
                    Robot 2 begins at column n - 1 and can move at most
                    'row' positions towards the left.

                    Therefore, its reachable columns are:

                    max(0, n - 1 - row) to n - 1
                */
                for (int c2 = max(0, n - 1 - row); c2 < n; c2++)
                {
                    int bestPrevious = 0;

                    /*
                        To arrive at column c1, Robot 1's previous column
                        could have been c1 - 1, c1, or c1 + 1.

                        The same applies to Robot 2.
                    */
                    for (int previousC1 = max(0, c1 - 1);
                         previousC1 <= min(n - 1, c1 + 1);
                         previousC1++)
                    {
                        for (int previousC2 = max(0, c2 - 1);
                             previousC2 <= min(n - 1, c2 + 1);
                             previousC2++)
                        {
                            bestPrevious = max(
                                bestPrevious,
                                dp[row - 1][previousC1][previousC2]);
                        }
                    }

                    // Add cherries from the two cells in the current row.
                    if (c1 == c2)
                    {
                        dp[row][c1][c2] =
                            bestPrevious + grid[row][c1];
                    }
                    else
                    {
                        dp[row][c1][c2] =
                            bestPrevious + grid[row][c1] + grid[row][c2];
                    }
                }
            }
        }

        /*
            The robots are not required to finish in particular columns.
            Therefore, take the maximum over every pair of columns
            in the final row.
        */
        int answer = 0;

        for (int c1 = 0; c1 < n; c1++)
        {
            for (int c2 = 0; c2 < n; c2++)
            {
                answer = max(answer, dp[m - 1][c1][c2]);
            }
        }

        return answer;
    }

    int cherryPickup(vector<vector<int>> &grid)
    {
        // Any of the three implementations can be used.
        return cherryPickupMemoization(grid);
    }
};

/*
    Number of states:

        m rows × n columns for Robot 1 × n columns for Robot 2
        = O(m × n²)

    Each state examines 9 combinations of robot movements.

    Memoization:
        Time Complexity:  O(m × n² × 9) = O(m × n²)
        Space Complexity: O(m × n²) DP + O(m) recursion stack

    Backward Tabulation:
        Time Complexity:  O(m × n² × 9) = O(m × n²)
        Space Complexity: O(m × n²)

    Forward Tabulation:
        Time Complexity:  O(m × n² × 9) = O(m × n²)
        Space Complexity: O(m × n²)
*/