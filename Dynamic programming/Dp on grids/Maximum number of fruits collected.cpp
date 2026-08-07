#include <bits/stdc++.h>
using namespace std;

/*
    Problem: Find the Maximum Number of Fruits Collected
    Link: https://leetcode.com/problems/find-the-maximum-number-of-fruits-collected/

    Three children start at:
    - Child 1: (0, 0)
    - Child 2: (0, n - 1)
    - Child 3: (n - 1, 0)

    All three must reach (n - 1, n - 1) in exactly n - 1 moves.

    ---------------------------------------------------------------
    CHILD 1
    ---------------------------------------------------------------

    Child 1 must increase both its row and column from 0 to n - 1.

    Since it has only n - 1 moves, every move must increase both row
    and column. Therefore, Child 1's path is fixed:

        (0,0) -> (1,1) -> (2,2) -> ... -> (n-1,n-1)

    So Child 1 collects every cell on the main diagonal.

    After collecting those fruits, we set the diagonal cells to 0 so
    they are not counted again by Child 2 or Child 3.

    ---------------------------------------------------------------
    CHILD 2
    ---------------------------------------------------------------

    Child 2 starts at the top-right corner and moves one row downward
    after every move. Its column may change by -1, 0, or +1.

    We use DP to find the maximum fruits it can collect while reaching
    the bottom-right corner.

    dp2[row][col] = maximum fruits Child 2 can collect while reaching
                    cell (row, col) from (0, n - 1).

    ---------------------------------------------------------------
    CHILD 3
    ---------------------------------------------------------------

    Child 3 starts at the bottom-left corner and moves one column right
    after every move. Its row may change by -1, 0, or +1.

    We transpose the grid so that Child 3's movement becomes identical
    to Child 2's movement:

        Original Child 3 start:   (n - 1, 0)
        After transposition:      (0, n - 1)

    Its three moves also become down-left, down, and down-right.

    Therefore, the same DP logic can be reused.

    Child 2 stays on or above the main diagonal, while Child 3 stays
    on or below it for any complete valid path. Hence, they can overlap
    only on the main diagonal, whose fruits have already been collected
    by Child 1 and set to 0.
*/

class Solution
{
public:
    int maxCollectedFruits(vector<vector<int>> &fruits)
    {
        int n = fruits.size();

        // Copy the grid because the main diagonal will be set to zero.
        vector<vector<int>> grid = fruits;

        // ---------------------------------------------------------
        // CHILD 1: FIXED MAIN-DIAGONAL PATH
        // ---------------------------------------------------------

        int child1 = 0;

        for (int i = 0; i < n; i++)
        {
            child1 += grid[i][i];

            // Prevent Child 2 and Child 3 from counting these fruits again.
            grid[i][i] = 0;
        }

        // ---------------------------------------------------------
        // CHILD 2: TOP-RIGHT TO BOTTOM-RIGHT
        // ---------------------------------------------------------

        /*
            dp2[row][col] = maximum fruits collected by Child 2
                            while reaching (row, col).

            INT_MIN means that the cell has not been reached through
            any valid path.
        */
        vector<vector<int>> dp2(
            n,
            vector<int>(n, INT_MIN));

        // Child 2 begins at the top-right corner.
        dp2[0][n - 1] = grid[0][n - 1];

        /*
            After k moves, Child 2 must be in row k because every move
            increases the row by exactly one.
        */
        for (int k = 1; k < n; k++)
        {
            /*
                Child 2 starts at column n - 1.

                After k moves, the farthest left it can reach is:

                    n - 1 - k

                Therefore, its possible columns are from
                n - 1 - k to n - 1.
            */
            for (int j = n - 1 - k; j <= n - 1; j++)
            {
                int bestPrevious = INT_MIN;

                /*
                    To reach column j in the current row, the previous
                    column could have been:

                    j - 1 -> moved down-right
                    j     -> moved straight down
                    j + 1 -> moved down-left
                */

                if (j - 1 >= 0 && dp2[k - 1][j - 1] != INT_MIN)
                {
                    bestPrevious = max(
                        bestPrevious,
                        dp2[k - 1][j - 1]);
                }

                if (dp2[k - 1][j] != INT_MIN)
                {
                    bestPrevious = max(
                        bestPrevious,
                        dp2[k - 1][j]);
                }

                if (j + 1 < n && dp2[k - 1][j + 1] != INT_MIN)
                {
                    bestPrevious = max(
                        bestPrevious,
                        dp2[k - 1][j + 1]);
                }

                // Update only when at least one valid previous state exists.
                if (bestPrevious != INT_MIN)
                {
                    dp2[k][j] = bestPrevious + grid[k][j];
                }
            }
        }

        // Child 2 must finish at the bottom-right cell.
        int child2 = dp2[n - 1][n - 1];

        // ---------------------------------------------------------
        // TRANSPOSE THE GRID FOR CHILD 3
        // ---------------------------------------------------------

        /*
            Transposing swaps rows and columns:

                tgrid[i][j] = grid[j][i]

            Child 3 originally starts at (n - 1, 0). After transposition,
            this becomes (0, n - 1), which is the same starting position
            used by Child 2.

            Child 3's movements also become:
            - down-left
            - down
            - down-right
        */
        vector<vector<int>> transposedGrid(
            n,
            vector<int>(n));

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                transposedGrid[j][i] = grid[i][j];
            }
        }

        // ---------------------------------------------------------
        // CHILD 3: SAME DP AFTER TRANSPOSITION
        // ---------------------------------------------------------

        /*
            dp3[row][col] = maximum fruits collected by Child 3 in
                            the transposed grid while reaching
                            cell (row, col).
        */
        vector<vector<int>> dp3(
            n,
            vector<int>(n, INT_MIN));

        dp3[0][n - 1] = transposedGrid[0][n - 1];

        for (int k = 1; k < n; k++)
        {
            for (int j = n - 1 - k; j <= n - 1; j++)
            {
                int bestPrevious = INT_MIN;

                if (j - 1 >= 0 && dp3[k - 1][j - 1] != INT_MIN)
                {
                    bestPrevious = max(
                        bestPrevious,
                        dp3[k - 1][j - 1]);
                }

                if (dp3[k - 1][j] != INT_MIN)
                {
                    bestPrevious = max(
                        bestPrevious,
                        dp3[k - 1][j]);
                }

                if (j + 1 < n && dp3[k - 1][j + 1] != INT_MIN)
                {
                    bestPrevious = max(
                        bestPrevious,
                        dp3[k - 1][j + 1]);
                }

                if (bestPrevious != INT_MIN)
                {
                    dp3[k][j] =
                        bestPrevious + transposedGrid[k][j];
                }
            }
        }

        // In the transposed grid, Child 3 also finishes at (n - 1, n - 1).
        int child3 = dp3[n - 1][n - 1];

        return child1 + child2 + child3;
    }
};

/*
    Time Complexity: O(n²)

    - Collecting Child 1's fruits: O(n)
    - Child 2's DP: O(n²)
    - Transposing the grid: O(n²)
    - Child 3's DP: O(n²)

    Space Complexity: O(n²)

    The grid, transposed grid, dp2, and dp3 each require O(n²) space.
*/