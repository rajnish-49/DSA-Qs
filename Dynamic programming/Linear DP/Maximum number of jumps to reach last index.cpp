#include <bits/stdc++.h>
using namespace std;

/*
    Problem: Maximum Number of Jumps to Reach the Last Index
    Link: https://leetcode.com/problems/maximum-number-of-jumps-to-reach-the-last-index/

    We start at index 0 and can jump from index i to a later index j when:

        abs(nums[j] - nums[i]) <= target

    Find the maximum number of jumps required to reach index n - 1.
    Return -1 if the last index cannot be reached.
*/

class Solution
{
public:
    /*
        Memoization state:

        dp[i] = maximum number of jumps possible from index i
                to the last index.

        -2 means the state has not been calculated.
        -1 means the last index is unreachable from i.
    */
    int solve(int i, vector<int> &nums, int target, vector<int> &dp)
    {
        int n = nums.size();

        // Already at the destination, so no more jumps are required.
        if (i == n - 1)
        {
            return 0;
        }

        if (dp[i] != -2)
        {
            return dp[i];
        }

        int maximumJumps = -1;

        // Try jumping from i to every later index j.
        for (int j = i + 1; j < n; j++)
        {
            if (abs(nums[j] - nums[i]) <= target)
            {
                int remainingJumps = solve(j, nums, target, dp);

                // Use this jump only when j can eventually reach the end.
                if (remainingJumps != -1)
                {
                    maximumJumps = max(
                        maximumJumps,
                        1 + remainingJumps);
                }
            }
        }

        return dp[i] = maximumJumps;
    }

    int maximumJumpsMemo(vector<int> &nums, int target)
    {
        int n = nums.size();

        vector<int> dp(n, -2);

        return solve(0, nums, target, dp);
    }

    int maximumJumpsTabulation(vector<int> &nums, int target)
    {
        int n = nums.size();

        /*
            dp[i] = maximum number of jumps used to reach index i.

            -1 means index i is unreachable.
        */
        vector<int> dp(n, -1);
        dp[0] = 0;

        for (int i = 1; i < n; i++)
        {
            // Try reaching i from every previous index j.
            for (int j = 0; j < i; j++)
            {
                if (dp[j] != -1 &&
                    abs(nums[i] - nums[j]) <= target)
                {
                    dp[i] = max(dp[i], dp[j] + 1);
                }
            }
        }

        return dp[n - 1];
    }

    int maximumJumps(vector<int> &nums, int target)
    {
        // Use either implementation.
        return maximumJumpsTabulation(nums, target);
    }
};

/*
    Memoization:
    Time:  O(n²)
    Space: O(n) DP array + O(n) recursion stack

    Tabulation:
    Time:  O(n²)
    Space: O(n)
*/