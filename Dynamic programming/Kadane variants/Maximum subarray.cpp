#include <bits/stdc++.h>
using namespace std;

/*
    Problem: Maximum Subarray
    Link: https://leetcode.com/problems/maximum-subarray/

    Find the maximum sum among all non-empty contiguous subarrays.

    State:
        dp[i] = maximum subarray sum ending exactly at index i

    For nums[i], either:
    - start a new subarray from i
    - extend the best subarray ending at i - 1
*/

class Solution
{
public:
    int maxSubArray(vector<int> &nums)
    {
        int n = nums.size();

        vector<int> dp(n);

        // The only subarray ending at index 0 is [nums[0]].
        dp[0] = nums[0];

        int maximumSum = dp[0];

        for (int i = 1; i < n; i++)
        {
            /*
                Start fresh from nums[i], or extend the best
                subarray that ended at index i - 1.
            */
            dp[i] = max(
                nums[i],
                nums[i] + dp[i - 1]);

            maximumSum = max(maximumSum, dp[i]);
        }

        return maximumSum;
    }
};

/*
    Time:  O(n)
    Space: O(n)
*/