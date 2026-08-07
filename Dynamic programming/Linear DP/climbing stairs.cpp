/*
    Climbing Stairs

    There are n stairs. From each stair, we may move either 1 or 2 steps.
    Return the number of distinct ways to reach stair n.

    Pattern:
    A state depends on the previous two states, similar to Fibonacci.
*/


// ============================================================
// MEMOIZATION
// ============================================================

class MemoizationSolution {
public:
    vector<int> dp;

    /*
        solve(stair) = number of ways to reach the given stair.

        The final move to 'stair' must come from:
        - stair - 1 using a one-step move
        - stair - 2 using a two-step move
    */
    int solve(int stair) {
        if (stair <= 1) {
            return 1;
        }

        if (dp[stair] != -1) {
            return dp[stair];
        }

        return dp[stair] =
            solve(stair - 1) + solve(stair - 2);
    }

    int climbStairs(int n) {
        dp.assign(n + 1, -1);

        return solve(n);
    }
};

/*
    Time:  O(n)
    Space: O(n) DP array + O(n) recursion stack
*/


// ============================================================
// TABULATION
// ============================================================

class TabulationSolution {
public:
    int climbStairs(int n) {
        if (n <= 1) {
            return 1;
        }

        /*
            dp[stair] = number of distinct ways to reach 'stair'.
        */
        vector<int> dp(n + 1);

        /*
            dp[0] = 1 represents one way to remain at the starting position.
            dp[1] = 1 because the first stair can only be reached using one step.
        */
        dp[0] = 1;
        dp[1] = 1;

        for (int stair = 2; stair <= n; stair++) {

            /*
                Reach the current stair from either:
                - stair - 1 by taking one step
                - stair - 2 by taking two steps
            */
            dp[stair] = dp[stair - 1] + dp[stair - 2];
        }

        return dp[n];
    }
};

/*
    Time:  O(n)
    Space: O(n)
*/