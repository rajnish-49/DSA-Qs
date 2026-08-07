#include <bits/stdc++.h>
using namespace std;

/*
    House Robber II

    Houses are arranged in a circle, so the first and last houses
    are adjacent and cannot both be robbed.

    Break the circular arrangement into two linear cases:

    1. Rob from index 0 to n - 2  -> exclude the last house
    2. Rob from index 1 to n - 1  -> exclude the first house

    The answer is the maximum result from these two cases.

    For every linear range:
        pick current house     -> nums[index] + answer after skipping one house
        skip current house     -> answer from the next house
*/


// ============================================================
// MEMOIZATION
// ============================================================

class MemoizationSolution {
public:
    /*
        solve(index, end) = maximum money obtainable from
        houses in the range [index, end].
    */
    int solve(int index, int end, vector<int>& nums, vector<int>& dp) {
        if (index > end) {
            return 0;
        }

        if (dp[index] != -1) {
            return dp[index];
        }

        // Pick the current house, so the adjacent house must be skipped.
        int pick = nums[index] + solve(index + 2, end, nums, dp);

        // Skip the current house and consider the next one.
        int skip = solve(index + 1, end, nums, dp);

        return dp[index] = max(pick, skip);
    }

    int rob(vector<int>& nums) {
        int n = nums.size();

        if (n == 1) {
            return nums[0];
        }

        /*
            Separate memo arrays are required because the two cases
            have different ending boundaries.
        */
        vector<int> dpWithoutLast(n, -1);
        vector<int> dpWithoutFirst(n, -1);

        int withoutLast =
            solve(0, n - 2, nums, dpWithoutLast);

        int withoutFirst =
            solve(1, n - 1, nums, dpWithoutFirst);

        return max(withoutLast, withoutFirst);
    }
};

/*
    Time:  O(n)
    Space: O(n) DP arrays + O(n) recursion stack
*/


// ============================================================
// TABULATION
// ============================================================

class TabulationSolution {
public:
    /*
        Solves the normal linear House Robber problem
        for the range [start, end].
    */
    int solveLinear(int start, int end, vector<int>& nums) {
        int length = end - start + 1;

        if (length == 1) {
            return nums[start];
        }

        /*
            dp[i] = maximum money obtainable from the first
            i + 1 houses of the selected range.
        */
        vector<int> dp(length);

        dp[0] = nums[start];
        dp[1] = max(nums[start], nums[start + 1]);

        for (int i = 2; i < length; i++) {
            int currentIndex = start + i;

            int pick = nums[currentIndex] + dp[i - 2];
            int skip = dp[i - 1];

            dp[i] = max(pick, skip);
        }

        return dp[length - 1];
    }

    int rob(vector<int>& nums) {
        int n = nums.size();

        if (n == 1) {
            return nums[0];
        }

        int withoutLast = solveLinear(0, n - 2, nums);
        int withoutFirst = solveLinear(1, n - 1, nums);

        return max(withoutLast, withoutFirst);
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
    /*
        Only the previous two DP values are required:

        twoHousesBack = dp[i - 2]
        oneHouseBack  = dp[i - 1]
    */
    int solveLinear(int start, int end, vector<int>& nums) {
        int length = end - start + 1;

        if (length == 1) {
            return nums[start];
        }

        int twoHousesBack = nums[start];
        int oneHouseBack = max(nums[start], nums[start + 1]);

        for (int i = 2; i < length; i++) {
            int currentIndex = start + i;

            int pick = nums[currentIndex] + twoHousesBack;
            int skip = oneHouseBack;

            int current = max(pick, skip);

            twoHousesBack = oneHouseBack;
            oneHouseBack = current;
        }

        return oneHouseBack;
    }

    int rob(vector<int>& nums) {
        int n = nums.size();

        if (n == 1) {
            return nums[0];
        }

        int withoutLast = solveLinear(0, n - 2, nums);
        int withoutFirst = solveLinear(1, n - 1, nums);

        return max(withoutLast, withoutFirst);
    }
};

/*
    Time:  O(n)
    Space: O(1) extra space
*/