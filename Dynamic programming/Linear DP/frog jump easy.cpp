#include <bits/stdc++.h>
using namespace std;

/*
    Frog Jump

    A frog starts at index 0 and wants to reach index n - 1.
    It may jump either one or two indices forward.

    Cost of jumping from i to j:
        abs(height[i] - height[j])

    Find the minimum total cost required to reach the last index.

    Pattern:
    Linear DP where each state depends on the previous two states.
*/


// ============================================================
// MEMOIZATION
// ============================================================

class MemoizationSolution {
public:
    /*
        solve(index) = minimum cost required to travel
        from 'index' to the last index.
    */
    int solve(int index, vector<int>& height, vector<int>& dp) {
        int n = height.size();

        // No additional cost after reaching the last index.
        if (index == n - 1) {
            return 0;
        }

        if (dp[index] != -1) {
            return dp[index];
        }

        // Jump one index forward.
        int oneJump =
            abs(height[index] - height[index + 1])
            + solve(index + 1, height, dp);

        int twoJumps = INT_MAX;

        // Jump two indices forward when that index exists.
        if (index + 2 < n) {
            twoJumps =
                abs(height[index] - height[index + 2])
                + solve(index + 2, height, dp);
        }

        return dp[index] = min(oneJump, twoJumps);
    }

    int frogJump(vector<int>& height) {
        int n = height.size();

        vector<int> dp(n, -1);

        return solve(0, height, dp);
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
    int frogJump(vector<int>& height) {
        int n = height.size();

        /*
            dp[index] = minimum cost required to reach 'index'
            starting from index 0.
        */
        vector<int> dp(n, 0);

        // Starting index requires no cost.
        dp[0] = 0;

        for (int index = 1; index < n; index++) {

            // Reach the current index by jumping from index - 1.
            int oneJump =
                dp[index - 1]
                + abs(height[index] - height[index - 1]);

            int twoJumps = INT_MAX;

            // Reach the current index by jumping from index - 2.
            if (index > 1) {
                twoJumps =
                    dp[index - 2]
                    + abs(height[index] - height[index - 2]);
            }

            dp[index] = min(oneJump, twoJumps);
        }

        return dp[n - 1];
    }
};

/*
    Time:  O(n)
    Space: O(n)
*/


// ============================================================
// SPACE OPTIMIZATION
// ============================================================

class SpaceOptimizedSolution {
public:
    int frogJump(vector<int>& height) {
        int n = height.size();

        if (n == 1) {
            return 0;
        }

        /*
            twoStepsBack = dp[index - 2]
            oneStepBack  = dp[index - 1]
        */
        int twoStepsBack = 0;
        int oneStepBack = abs(height[1] - height[0]);

        for (int index = 2; index < n; index++) {
            int oneJump =
                oneStepBack
                + abs(height[index] - height[index - 1]);

            int twoJumps =
                twoStepsBack
                + abs(height[index] - height[index - 2]);

            int currentCost = min(oneJump, twoJumps);

            twoStepsBack = oneStepBack;
            oneStepBack = currentCost;
        }

        return oneStepBack;
    }
};

/*
    Time:  O(n)
    Space: O(1)
*/