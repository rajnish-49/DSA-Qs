#include <bits/stdc++.h>
using namespace std;

/*
    Problem: Maximum Path Score in a Grid
    Link: https://leetcode.com/problems/maximum-path-score-in-a-grid/

    We start at (0, 0) and must reach (m - 1, n - 1).
    From each cell, we can move only right or down.

    Cell value and contribution:

        Value 0 -> score = 0, cost = 0
        Value 1 -> score = 1, cost = 1
        Value 2 -> score = 2, cost = 1

    The total cost of the selected path must not exceed k.
    Among all valid paths, return the maximum obtainable score.

    State:

        dp[i][j][c] = maximum score collected while reaching cell (i, j)
                      with an exact total cost of c.

    A value of -1 means that the state is unreachable.

    We store the exact cost instead of only storing the best score at each
    cell because two paths reaching the same cell may have different costs.
    A path with a lower current score but lower cost may be more useful later.
*/

class Solution
{
public:
    int maxPathScore(vector<vector<int>> &grid, int k)
    {
        int m = grid.size();
        int n = grid[0].size();

        /*
            For every cell, store the maximum score for each exact cost
            from 0 to k.

            Since all scores are non-negative, -1 can safely represent
            an unreachable state.
        */
        vector<vector<vector<int>>> dp(
            m,
            vector<vector<int>>(
                n,
                vector<int>(k + 1, -1)));

        /*
            grid[0][0] is guaranteed to be 0.

            Therefore, we begin with:
            - total score = 0
            - total cost = 0
        */
        dp[0][0][0] = 0;

        // Initialize the first row, where every cell can only be reached from the left.
        for (int j = 1; j < n; j++)
        {
            int score = grid[0][j];

            // Values 1 and 2 both cost 1, while value 0 costs nothing.
            int cellCost = (score > 0) ? 1 : 0;

            for (int previousCost = 0; previousCost <= k; previousCost++)
            {
                // The previous cell cannot be reached with this exact cost.
                if (dp[0][j - 1][previousCost] == -1)
                    continue;

                int newCost = previousCost + cellCost;

                // Keep only paths whose total cost does not exceed k.
                if (newCost <= k)
                {
                    dp[0][j][newCost] =
                        dp[0][j - 1][previousCost] + score;
                }
            }
        }

        // Initialize the first column, where every cell can only be reached from above.
        for (int i = 1; i < m; i++)
        {
            int score = grid[i][0];
            int cellCost = (score > 0) ? 1 : 0;

            for (int previousCost = 0; previousCost <= k; previousCost++)
            {
                if (dp[i - 1][0][previousCost] == -1)
                    continue;

                int newCost = previousCost + cellCost;

                if (newCost <= k)
                {
                    dp[i][0][newCost] =
                        dp[i - 1][0][previousCost] + score;
                }
            }
        }

        /*
            Process the remaining cells.

            Cell (i, j) can be reached from:
            - Above: (i - 1, j)
            - Left:  (i, j - 1)

            For each exact previous cost, choose the larger score from
            these two directions and then include the current cell.
        */
        for (int i = 1; i < m; i++)
        {
            for (int j = 1; j < n; j++)
            {
                int score = grid[i][j];
                int cellCost = (score > 0) ? 1 : 0;

                for (int previousCost = 0;
                     previousCost <= k;
                     previousCost++)
                {
                    int fromAbove = dp[i - 1][j][previousCost];
                    int fromLeft = dp[i][j - 1][previousCost];

                    // Find the best path reaching a neighboring cell with this exact cost.
                    int bestPreviousScore = max(fromAbove, fromLeft);

                    // Neither neighboring cell is reachable with this cost.
                    if (bestPreviousScore == -1)
                        continue;

                    int newCost = previousCost + cellCost;

                    if (newCost <= k)
                    {
                        /*
                            Multiple previous states may produce the same new cost,
                            so preserve the maximum score among them.
                        */
                        dp[i][j][newCost] = max(
                            dp[i][j][newCost],
                            bestPreviousScore + score);
                    }
                }
            }
        }

        /*
            The destination may be reached with any exact cost from 0 to k.

            Therefore, take the maximum score among all valid costs.
        */
        int answer = -1;

        for (int cost = 0; cost <= k; cost++)
        {
            answer = max(
                answer,
                dp[m - 1][n - 1][cost]);
        }

        return answer;
    }
};

/*
    Time Complexity: O(m * n * k)

    For every cell, we examine all possible costs from 0 to k.

    Space Complexity: O(m * n * k)

    Each cell stores a maximum score for every exact cost.
*/