#include <bits/stdc++.h>
using namespace std;

/*
    Problem: Maximum Amount of Money Robot Can Earn
    Link: https://leetcode.com/problems/maximum-amount-of-money-robot-can-earn/

    A robot starts at the top-left cell (0, 0) and must reach
    the bottom-right cell (m - 1, n - 1).

    From each cell, it can move only:
    - Right: (i, j + 1)
    - Down:  (i + 1, j)

    Cell values:
    - A positive value means the robot earns that amount.
    - A negative value means a robber takes that amount from the robot.

    The robot can neutralize at most two robbers. If a robber is
    neutralized, the negative value of that cell is treated as 0.

    The final answer may be negative because the robot is not allowed
    to avoid reaching the destination.
*/

class Solution
{
private:
    static constexpr int NEG_INF = -1000000000;

    int m, n;
    int memo[501][501][3];

    // ============================================================
    // APPROACH 1: MEMOIZATION
    // ============================================================

    /*
        solve(i, j, remaining) = maximum money the robot can collect
                                 from cell (i, j) to the destination,
                                 when it still has 'remaining'
                                 neutralizations available.

        The contribution of the current cell is included in this state.
    */
    int solve(vector<vector<int>> &coins, int i, int j, int remaining)
    {
        // Moving outside the grid does not produce a valid path.
        if (i >= m || j >= n)
            return NEG_INF;

        /*
            At the destination, the robot must collect its value.

            If it contains a robber and a neutralization is available,
            the robot may neutralize it and collect 0 instead.
        */
        if (i == m - 1 && j == n - 1)
        {
            if (coins[i][j] < 0 && remaining > 0)
                return max(coins[i][j], 0);

            return coins[i][j];
        }

        if (memo[i][j][remaining] != INT_MIN)
            return memo[i][j][remaining];

        /*
            Option 1: Do not neutralize the current cell.

            Add its value normally and then take the better path
            between moving down and moving right.
        */
        int bestNext = max(
            solve(coins, i + 1, j, remaining),
            solve(coins, i, j + 1, remaining));

        int takeNormally = coins[i][j] + bestNext;

        /*
            Option 2: Neutralize the current robber.

            This option is available only when:
            - the current cell is negative, and
            - at least one neutralization remains.

            The current cell then contributes 0, and one neutralization
            is consumed before moving to the next cell.
        */
        int neutralize = NEG_INF;

        if (coins[i][j] < 0 && remaining > 0)
        {
            neutralize = max(
                solve(coins, i + 1, j, remaining - 1),
                solve(coins, i, j + 1, remaining - 1));
        }

        return memo[i][j][remaining] =
                   max(takeNormally, neutralize);
    }

public:
    int maximumAmountMemoization(vector<vector<int>> &coins)
    {
        m = coins.size();
        n = coins[0].size();

        // INT_MIN means that the state has not been calculated yet.
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                for (int remaining = 0; remaining <= 2; remaining++)
                {
                    memo[i][j][remaining] = INT_MIN;
                }
            }
        }

        // Initially, both neutralizations are available.
        return solve(coins, 0, 0, 2);
    }

    // ============================================================
    // APPROACH 2: FORWARD TABULATION
    // ============================================================

    int maximumAmountTabulation(vector<vector<int>> &coins)
    {
        int m = coins.size();
        int n = coins[0].size();

        /*
            dp[i][j][remaining] = maximum money collected while reaching
                                  cell (i, j), with 'remaining'
                                  neutralizations still available.

            remaining can be:
            2 -> no neutralization used
            1 -> one neutralization used
            0 -> both neutralizations used

            NEG_INF means that the state is unreachable.
        */
        vector<vector<vector<int>>> dp(
            m,
            vector<vector<int>>(
                n,
                vector<int>(3, NEG_INF)));

        /*
            Initialize the starting cell.

            Option 1: Do not neutralize it.
            Both neutralizations remain available.
        */
        dp[0][0][2] = coins[0][0];

        /*
            Option 2: If the starting cell is negative, neutralize it.

            Its contribution becomes 0 and one neutralization remains.
        */
        if (coins[0][0] < 0)
        {
            dp[0][0][1] = 0;
        }

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                // The starting cell has already been initialized.
                if (i == 0 && j == 0)
                    continue;

                for (int remaining = 0; remaining <= 2; remaining++)
                {
                    /*
                        Reach the current cell from either:
                        - the cell above
                        - the cell on the left

                        The number of remaining neutralizations does not
                        change while moving between cells.
                    */
                    int fromTop =
                        (i > 0) ? dp[i - 1][j][remaining] : NEG_INF;

                    int fromLeft =
                        (j > 0) ? dp[i][j - 1][remaining] : NEG_INF;

                    int bestPrevious = max(fromTop, fromLeft);

                    // No valid path reaches this cell with this state.
                    if (bestPrevious == NEG_INF)
                        continue;

                    /*
                        Option 1: Do not neutralize the current cell.

                        Its value is added normally, whether it is
                        positive or negative.
                    */
                    dp[i][j][remaining] = max(
                        dp[i][j][remaining],
                        bestPrevious + coins[i][j]);

                    /*
                        Option 2: Neutralize the current cell.

                        Only a negative cell can be neutralized.
                        The cell contributes 0 and the number of remaining
                        neutralizations decreases by one.
                    */
                    if (coins[i][j] < 0 && remaining > 0)
                    {
                        dp[i][j][remaining - 1] = max(
                            dp[i][j][remaining - 1],
                            bestPrevious);
                    }
                }
            }
        }

        /*
            The robot may use zero, one, or both neutralizations.

            Therefore, take the best value among all possible numbers
            of neutralizations remaining at the destination.
        */
        return max({
            dp[m - 1][n - 1][0],
            dp[m - 1][n - 1][1],
            dp[m - 1][n - 1][2]});
    }

    int maximumAmount(vector<vector<int>> &coins)
    {
        // Either implementation can be used.
        return maximumAmountTabulation(coins);
    }
};

/*
    Number of states:

        m × n cells × 3 neutralization states

    Memoization:
        Time Complexity:  O(m × n)
        Space Complexity: O(m × n) DP + O(m + n) recursion stack

    Tabulation:
        Time Complexity:  O(m × n)
        Space Complexity: O(m × n)
*/