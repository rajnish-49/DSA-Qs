#include <bits/stdc++.h>
using namespace std;

/*
    Problem: Number of Paths with Max Score
    Link: https://leetcode.com/problems/number-of-paths-with-max-score/

    We start from 'S' at the bottom-right cell and must reach 'E'
    at the top-left cell.

    Allowed moves from (i, j):
    - Up:      (i - 1, j)
    - Left:    (i, j - 1)
    - Up-left: (i - 1, j - 1)

    Cells containing 'X' are blocked.
    Cells containing digits contribute that digit to the path score.
    'S' and 'E' contribute zero.

    We must return:
    1. The maximum score among all valid paths.
    2. The number of paths that achieve exactly that maximum score.

    If no path exists, return {0, 0}.
*/

class Solution
{
public:
    const int MOD = 1e9 + 7;

    // ============================================================
    // APPROACH 1: ORIGINAL DIRECTION — S TO E
    // ============================================================

    vector<int> pathsWithMaxScoreOriginal(vector<string> &board)
    {
        int n = board.size();

        /*
            dp[i][j].first:
            Maximum score collected on a path from 'S' to cell (i, j).

            dp[i][j].second:
            Number of paths from 'S' to (i, j) that achieve that maximum score.

            {-1, 0} means that the cell is unreachable from 'S'.
        */
        vector<vector<pair<int, int>>> dp(
            n,
            vector<pair<int, int>>(n, {-1, 0}));

        /*
            At 'S':
            - No digit has been collected, so the score is 0.
            - There is exactly one way to begin the path.
        */
        dp[n - 1][n - 1] = {0, 1};

        /*
            We move from bottom-right towards top-left.

            To reach the current cell (i, j) while following the original
            movement rules, the previous cell must be one of:

            - Below:      (i + 1, j),     followed by an upward move
            - Right:      (i, j + 1),     followed by a left move
            - Down-right: (i + 1, j + 1), followed by an up-left move

            Therefore, these states must be processed before (i, j).
        */
        for (int i = n - 1; i >= 0; i--)
        {
            for (int j = n - 1; j >= 0; j--)
            {
                // 'S' has already been initialized.
                if (i == n - 1 && j == n - 1)
                    continue;

                // A blocked cell cannot be part of any valid path.
                if (board[i][j] == 'X')
                    continue;

                int bestScore = -1;
                int numberOfPaths = 0;

                /*
                    Try reaching (i, j) from the cell below.

                    If its score is greater than the current best score,
                    replace both the best score and the path count.

                    If its score is equal to the current best score,
                    its paths are additional maximum-score paths.
                */
                if (i + 1 < n && dp[i + 1][j].first != -1)
                {
                    int candidateScore = dp[i + 1][j].first;

                    if (candidateScore > bestScore)
                    {
                        bestScore = candidateScore;
                        numberOfPaths = dp[i + 1][j].second;
                    }
                    else if (candidateScore == bestScore)
                    {
                        numberOfPaths =
                            (numberOfPaths + dp[i + 1][j].second) % MOD;
                    }
                }

                // Try reaching (i, j) from the cell on the right.
                if (j + 1 < n && dp[i][j + 1].first != -1)
                {
                    int candidateScore = dp[i][j + 1].first;

                    if (candidateScore > bestScore)
                    {
                        bestScore = candidateScore;
                        numberOfPaths = dp[i][j + 1].second;
                    }
                    else if (candidateScore == bestScore)
                    {
                        numberOfPaths =
                            (numberOfPaths + dp[i][j + 1].second) % MOD;
                    }
                }

                // Try reaching (i, j) from the diagonally down-right cell.
                if (i + 1 < n && j + 1 < n &&
                    dp[i + 1][j + 1].first != -1)
                {
                    int candidateScore = dp[i + 1][j + 1].first;

                    if (candidateScore > bestScore)
                    {
                        bestScore = candidateScore;
                        numberOfPaths = dp[i + 1][j + 1].second;
                    }
                    else if (candidateScore == bestScore)
                    {
                        numberOfPaths =
                            (numberOfPaths + dp[i + 1][j + 1].second) % MOD;
                    }
                }

                // None of the three possible previous cells was reachable.
                if (bestScore == -1)
                    continue;

                // Digit cells add their value, while 'E' and 'S' add zero.
                int cellValue = 0;

                if (board[i][j] >= '1' && board[i][j] <= '9')
                {
                    cellValue = board[i][j] - '0';
                }

                /*
                    Every selected previous path has the same bestScore.

                    Since all those paths now enter the same current cell,
                    cellValue is added once to their score, while their
                    path counts are combined.
                */
                dp[i][j] = {
                    bestScore + cellValue,
                    numberOfPaths};
            }
        }

        // If 'E' is unreachable from 'S', no valid path exists.
        if (dp[0][0].first == -1)
        {
            return {0, 0};
        }

        return {
            dp[0][0].first,
            dp[0][0].second};
    }

    // ============================================================
    // APPROACH 2: REVERSED DIRECTION — E TO S
    // ============================================================

    vector<int> pathsWithMaxScoreReversed(vector<string> &board)
    {
        int n = board.size();

        /*
            Every valid path from:

                S -> E using up, left and up-left

            can be reversed into a path from:

                E -> S using down, right and down-right

            The reversed path visits exactly the same cells, so its
            score is unchanged. There is also a one-to-one correspondence
            between the original paths and their reversed paths.

            Therefore, solving the reversed movement problem produces
            the same maximum score and number of maximum-score paths.
        */

        /*
            dp[i][j].first:
            Maximum score collected on a reversed path from 'E' to (i, j).

            dp[i][j].second:
            Number of reversed paths that achieve that maximum score.

            {-1, 0} means that the cell is unreachable from 'E'.
        */
        vector<vector<pair<int, int>>> dp(
            n,
            vector<pair<int, int>>(n, {-1, 0}));

        // At 'E', the score is 0 and there is exactly one starting path.
        dp[0][0] = {0, 1};

        /*
            We process cells from top-left towards bottom-right.

            In the reversed direction, cell (i, j) can be reached from:

            - Above:      (i - 1, j)
            - Left:       (i, j - 1)
            - Upper-left: (i - 1, j - 1)
        */
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                // 'E' has already been initialized.
                if (i == 0 && j == 0)
                    continue;

                if (board[i][j] == 'X')
                    continue;

                int bestScore = -1;
                int numberOfPaths = 0;

                // Try reaching (i, j) from the cell above.
                if (i - 1 >= 0 && dp[i - 1][j].first != -1)
                {
                    int candidateScore = dp[i - 1][j].first;

                    if (candidateScore > bestScore)
                    {
                        bestScore = candidateScore;
                        numberOfPaths = dp[i - 1][j].second;
                    }
                    else if (candidateScore == bestScore)
                    {
                        numberOfPaths =
                            (numberOfPaths + dp[i - 1][j].second) % MOD;
                    }
                }

                // Try reaching (i, j) from the cell on the left.
                if (j - 1 >= 0 && dp[i][j - 1].first != -1)
                {
                    int candidateScore = dp[i][j - 1].first;

                    if (candidateScore > bestScore)
                    {
                        bestScore = candidateScore;
                        numberOfPaths = dp[i][j - 1].second;
                    }
                    else if (candidateScore == bestScore)
                    {
                        numberOfPaths =
                            (numberOfPaths + dp[i][j - 1].second) % MOD;
                    }
                }

                // Try reaching (i, j) from the diagonally upper-left cell.
                if (i - 1 >= 0 && j - 1 >= 0 &&
                    dp[i - 1][j - 1].first != -1)
                {
                    int candidateScore = dp[i - 1][j - 1].first;

                    if (candidateScore > bestScore)
                    {
                        bestScore = candidateScore;
                        numberOfPaths = dp[i - 1][j - 1].second;
                    }
                    else if (candidateScore == bestScore)
                    {
                        numberOfPaths =
                            (numberOfPaths + dp[i - 1][j - 1].second) % MOD;
                    }
                }

                // The current cell cannot be reached from 'E'.
                if (bestScore == -1)
                    continue;

                // Digit cells add their value, while 'E' and 'S' add zero.
                int cellValue = 0;

                if (board[i][j] >= '1' && board[i][j] <= '9')
                {
                    cellValue = board[i][j] - '0';
                }

                dp[i][j] = {
                    bestScore + cellValue,
                    numberOfPaths};
            }
        }

        // If 'S' is unreachable in the reversed direction, no path exists.
        if (dp[n - 1][n - 1].first == -1)
        {
            return {0, 0};
        }

        return {
            dp[n - 1][n - 1].first,
            dp[n - 1][n - 1].second};
    }

    vector<int> pathsWithMaxScore(vector<string> &board)
    {
        // Either implementation returns the same answer.
        return pathsWithMaxScoreOriginal(board);
    }
};

/*
    Time Complexity: O(n²)

    Every cell is processed once and examines at most three
    possible previous cells.

    Space Complexity: O(n²)

    Each cell stores:
    - its maximum reachable score
    - the number of paths achieving that score
*/