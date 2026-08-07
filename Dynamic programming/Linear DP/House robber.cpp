#include <bits/stdc++.h>
using namespace std;

/*
    House Robber

    Each house contains some money, but two adjacent houses cannot
    be robbed together. Find the maximum money that can be collected.

    At every house:
    - Pick it, then move to index + 2
    - Skip it, then move to index + 1
*/


// ============================================================
// MEMOIZATION
// ============================================================

class MemoizationSolution {
public:
    /*
        solve(index) = maximum money obtainable from
        houses in the range [index, n - 1].
    */
    int solve(int index, vector<int>& nums, vector<int>& dp) {
        int n = nums.size();

        // No houses remain.
        if (index >= n) {
            return 0;
        }

        if (dp[index] != -1) {
            return dp[index];
        }

        // Picking the current house forces us to skip the next house.
        int pick = nums[index] + solve(index + 2, nums, dp);

        // Skip the current house and consider the next one.
        int skip = solve(index + 1, nums, dp);

        return dp[index] = max(pick, skip);
    }

    int rob(vector<int>& nums) {
        vector<int> dp(nums.size(), -1);

        return solve(0, nums, dp);
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
    int rob(vector<int>& nums) {
        int n = nums.size();

        if (n == 1) {
            return nums[0];
        }

        /*
            dp[index] = maximum money obtainable from
            houses in the range [0, index].
        */
        vector<int> dp(n);

        dp[0] = nums[0];
        dp[1] = max(nums[0], nums[1]);

        for (int index = 2; index < n; index++) {

            // Pick current house and combine it with dp[index - 2].
            int pick = nums[index] + dp[index - 2];

            // Skip current house, retaining the previous best answer.
            int skip = dp[index - 1];

            dp[index] = max(pick, skip);
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
    int rob(vector<int>& nums) {
        int n = nums.size();

        if (n == 1) {
            return nums[0];
        }

        /*
            twoHousesBack = dp[index - 2]
            oneHouseBack  = dp[index - 1]
        */
        int twoHousesBack = nums[0];
        int oneHouseBack = max(nums[0], nums[1]);

        for (int index = 2; index < n; index++) {
            int pick = nums[index] + twoHousesBack;
            int skip = oneHouseBack;

            int currentBest = max(pick, skip);

            twoHousesBack = oneHouseBack;
            oneHouseBack = currentBest;
        }

        return oneHouseBack;
    }
};

/*
    Time:  O(n)
    Space: O(1)
*/