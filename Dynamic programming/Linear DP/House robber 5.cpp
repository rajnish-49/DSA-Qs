#include <bits/stdc++.h>
using namespace std;

/*
    Colored House Robber

    Each index contains nums[i] points and has a color colors[i].

    Adjacent elements with the same color cannot both be selected.
    Adjacent elements with different colors may both be selected.

    For every index:
    - Skip it and retain the best answer up to index - 1.
    - Select it:
        - Same color as previous: combine with answer up to index - 2.
        - Different color: combine with answer up to index - 1.
*/


// ============================================================
// MEMOIZATION
// ============================================================

class MemoizationSolution {
public:
    /*
        solve(index) = maximum points obtainable by considering
        indices from 0 to index.
    */
    long long solve(
        int index,
        vector<int>& nums,
        vector<int>& colors,
        vector<long long>& memo
    ) {
        if (index < 0) {
            return 0;
        }

        if (index == 0) {
            return nums[0];
        }

        if (memo[index] != LLONG_MIN) {
            return memo[index];
        }

        // Do not select the current index.
        long long skip = solve(index - 1, nums, colors, memo);

        long long take = nums[index];

        if (colors[index] == colors[index - 1]) {
            /*
                Current and previous elements conflict, so selecting
                the current element forces us to exclude index - 1.
            */
            take += solve(index - 2, nums, colors, memo);
        } else {
            /*
                Different colors do not conflict, so the current
                element can be combined with the best answer up to index - 1.
            */
            take += solve(index - 1, nums, colors, memo);
        }

        return memo[index] = max(skip, take);
    }

    long long rob(vector<int>& nums, vector<int>& colors) {
        int n = nums.size();

        vector<long long> memo(n, LLONG_MIN);

        return solve(n - 1, nums, colors, memo);
    }
};

/*
    Time:  O(n)
    Space: O(n) memo array + O(n) recursion stack
*/


// ============================================================
// TABULATION
// ============================================================

class TabulationSolution {
public:
    long long rob(vector<int>& nums, vector<int>& colors) {
        int n = nums.size();

        /*
            dp[index] = maximum points obtainable from
            indices 0 through index.
        */
        vector<long long> dp(n);

        dp[0] = nums[0];

        for (int index = 1; index < n; index++) {
            long long skip = dp[index - 1];
            long long take = nums[index];

            if (colors[index] == colors[index - 1]) {
                /*
                    Selecting the current element means index - 1
                    cannot be selected.
                */
                if (index >= 2) {
                    take += dp[index - 2];
                }
            } else {
                /*
                    The current element does not conflict with index - 1,
                    so combine it with the complete best answer so far.
                */
                take += dp[index - 1];
            }

            dp[index] = max(skip, take);
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
    long long rob(vector<int>& nums, vector<int>& colors) {
        int n = nums.size();

        if (n == 1) {
            return nums[0];
        }

        /*
            twoIndicesBack = dp[index - 2]
            oneIndexBack   = dp[index - 1]
        */
        long long twoIndicesBack = 0;
        long long oneIndexBack = nums[0];

        for (int index = 1; index < n; index++) {
            long long skip = oneIndexBack;
            long long take = nums[index];

            if (colors[index] == colors[index - 1]) {
                take += twoIndicesBack;
            } else {
                take += oneIndexBack;
            }

            long long currentBest = max(skip, take);

            twoIndicesBack = oneIndexBack;
            oneIndexBack = currentBest;
        }

        return oneIndexBack;
    }
};

/*
    Time:  O(n)
    Space: O(1)
*/