#include <bits/stdc++.h>
using namespace std;

/*
    Problem: Triangle
    Link: https://leetcode.com/problems/triangle/

    We start from the top element of the triangle and must reach
    any element in the last row.

    From position (row, col), we can move to:

        (row + 1, col)       -> directly below
        (row + 1, col + 1)   -> diagonally below-right

    Return the minimum possible path sum from top to bottom.

    Forward DP state:

        dp[row][col] = minimum path sum required to reach
                       triangle[row][col] from the top.

    To reach (row, col), the previous position can be:

        (row - 1, col)       -> the element directly above
        (row - 1, col - 1)   -> the element diagonally above-left
*/

class Solution
{
public:
    int minimumTotal(vector<vector<int>> &triangle)
    {
        int n = triangle.size();

        // Create a DP table with the same triangular structure as the input.
        vector<vector<int>> dp(n);

        for (int row = 0; row < n; row++)
        {
            dp[row].resize(row + 1);
        }

        // The only path reaching the top element begins at the top itself.
        dp[0][0] = triangle[0][0];

        for (int row = 1; row < n; row++)
        {
            for (int col = 0; col <= row; col++)
            {
                /*
                    The first element of a row can only be reached from
                    the first element of the previous row.
                */
                if (col == 0)
                {
                    dp[row][col] =
                        dp[row - 1][col] + triangle[row][col];
                }

                /*
                    The last element of a row can only be reached from
                    the last element of the previous row.
                */
                else if (col == row)
                {
                    dp[row][col] =
                        dp[row - 1][col - 1] + triangle[row][col];
                }

                /*
                    An internal element can be reached from either the
                    element directly above or the element above-left.
                */
                else
                {
                    int fromAbove = dp[row - 1][col];
                    int fromAboveLeft = dp[row - 1][col - 1];

                    dp[row][col] =
                        triangle[row][col] +
                        min(fromAbove, fromAboveLeft);
                }
            }
        }

        // The path may finish at any element in the last row.
        int answer = dp[n - 1][0];

        for (int col = 1; col < n; col++)
        {
            answer = min(answer, dp[n - 1][col]);
        }

        return answer;
    }
};

/*
    Time Complexity: O(n²)

    Every element in the triangle is processed once.

    Space Complexity: O(n²)

    The DP table stores the minimum path sum for every triangle element.
*/