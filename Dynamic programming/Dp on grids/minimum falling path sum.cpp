#include <bits/stdc++.h>
using namespace std;

/*
    Problem: Minimum Falling Path Sum
    Link: https://leetcode.com/problems/minimum-falling-path-sum/

    We may start from any cell in the first row.

    From cell (i, j), we can move to the next row using:
    - Down-left: (i + 1, j - 1)
    - Down:      (i + 1, j)
    - Down-right:(i + 1, j + 1)

    We must reach any cell in the last row and minimize the total path sum.

    State:

    dp[i][j] = minimum falling-path sum required to reach cell (i, j)
               from any cell in the first row.

    To reach (i, j), the previous cell can be directly above,
    diagonally above-left, or diagonally above-right.
*/

class Solution
{
public:
    int minFallingPathSum(vector<vector<int>> &matrix)
    {
        int rows = matrix.size();
        int cols = matrix[0].size();

        vector<vector<int>> dp(rows, vector<int>(cols, 0));

        /*
            We may start from any cell in the first row.

            Therefore, the minimum path sum for reaching each first-row
            cell is simply the value of that cell.
        */
        for (int col = 0; col < cols; col++)
        {
            dp[0][col] = matrix[0][col];
        }

        for (int row = 1; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                /*
                    For the first column, there is no upper-left cell.

                    It can only be reached from:
                    - directly above
                    - diagonally above-right
                */
                if (col == 0)
                {
                    dp[row][col] =
                        matrix[row][col] +
                        min(dp[row - 1][col],
                            dp[row - 1][col + 1]);
                }

                /*
                    For the last column, there is no upper-right cell.

                    It can only be reached from:
                    - diagonally above-left
                    - directly above
                */
                else if (col == cols - 1)
                {
                    dp[row][col] =
                        matrix[row][col] +
                        min(dp[row - 1][col - 1],
                            dp[row - 1][col]);
                }

                /*
                    For an internal column, all three previous cells
                    are valid choices.
                */
                else
                {
                    dp[row][col] =
                        matrix[row][col] +
                        min({
                            dp[row - 1][col - 1],
                            dp[row - 1][col],
                            dp[row - 1][col + 1]});
                }
            }
        }

        /*
            A falling path may finish at any cell in the last row,
            so take the minimum among all last-row states.
        */
        int answer = dp[rows - 1][0];

        for (int col = 1; col < cols; col++)
        {
            answer = min(answer, dp[rows - 1][col]);
        }

        return answer;
    }
};

/*
    Time Complexity: O(rows * cols)

    Every cell is processed once.

    Space Complexity: O(rows * cols)

    The DP table stores the minimum path sum for every cell.
*/