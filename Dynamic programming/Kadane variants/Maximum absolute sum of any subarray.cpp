#include <bits/stdc++.h>
using namespace std;

/*
    Problem: Maximum Absolute Sum of Any Subarray
    Link: https://leetcode.com/problems/maximum-absolute-sum-of-any-subarray/

    We need the maximum value of:

        abs(sum of a non-empty contiguous subarray)

    The answer can come from:
    1. A subarray with the largest positive sum.
    2. A subarray with the smallest negative sum, whose absolute value may be large.

    Therefore, we run two versions of Kadane's algorithm together:
    - Maximum Kadane to find the maximum subarray sum.
    - Minimum Kadane to find the minimum subarray sum.
*/

class Solution
{
public:
    int maxAbsoluteSum(vector<int> &nums)
    {
        int n = nums.size();

        /*
            maxEnding[i] = maximum sum of a subarray ending exactly at index i.
            minEnding[i] = minimum sum of a subarray ending exactly at index i.

            We use "ending exactly at i" because every state is formed by either
            starting a new subarray at i or extending a subarray ending at i - 1.
        */
        vector<int> maxEnding(n);
        vector<int> minEnding(n);

        // At index 0, the only subarray ending there is [nums[0]].
        maxEnding[0] = nums[0];
        minEnding[0] = nums[0];

        // The first element itself is a valid subarray, so consider its absolute sum.
        int ans = abs(nums[0]);

        for (int i = 1; i < n; i++)
        {
            // Start a new subarray at i or extend the maximum-sum subarray ending at i - 1.
            maxEnding[i] = max(nums[i], nums[i] + maxEnding[i - 1]);

            // Start a new subarray at i or extend the minimum-sum subarray ending at i - 1.
            minEnding[i] = min(nums[i], nums[i] + minEnding[i - 1]);

            /*
                The largest absolute sum ending at i can come from:
                - maxEnding[i], if the largest positive sum has greater magnitude.
                - minEnding[i], if the most negative sum has greater magnitude.
            */
            ans = max(ans, max(abs(maxEnding[i]), abs(minEnding[i])));
        }

        return ans;
    }
};

/*
    Time Complexity: O(n)
    Each index is processed once.

    Space Complexity: O(n)
    Two DP arrays of size n are stored.
*/