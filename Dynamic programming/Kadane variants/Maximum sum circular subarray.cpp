#include <bits/stdc++.h>
using namespace std;

/*
    Problem: Maximum Sum Circular Subarray
    Link: https://leetcode.com/problems/maximum-sum-circular-subarray/

    A maximum-sum circular subarray can be of two types:

    1. Non-wrapping:
       The subarray lies normally inside the array.
       Find it using standard Kadane's algorithm.

    2. Wrapping:
       The subarray uses a suffix and a prefix of the array.

       [chosen prefix] [excluded middle] [chosen suffix]

       To maximize the chosen circular part, exclude the minimum-sum
       subarray from the total array sum.

       wrappingSum = totalSum - minimumSubarraySum

    Final answer:
        max(normal maximum sum, wrapping maximum sum)

    Special case:
    If every element is negative, minimumSubarraySum equals totalSum.
    Subtracting them would give 0, representing an empty subarray,
    which is not allowed. In that case, return the normal maximum.
*/

class Solution
{
public:
    int maxSubarraySumCircular(vector<int> &nums)
    {
        /*
            currentMax:
            Maximum subarray sum ending exactly at the current index.

            maximumSum:
            Maximum non-wrapping subarray sum found anywhere.
        */
        int currentMax = nums[0];
        int maximumSum = nums[0];

        /*
            currentMin:
            Minimum subarray sum ending exactly at the current index.

            minimumSum:
            Minimum subarray sum found anywhere. This is the section
            removed to form the maximum wrapping subarray.
        */
        int currentMin = nums[0];
        int minimumSum = nums[0];

        int totalSum = nums[0];

        for (int i = 1; i < nums.size(); i++)
        {
            /*
                Maximum Kadane:

                Either start a new subarray at i or extend the
                maximum-sum subarray ending at i - 1.
            */
            currentMax = max(
                nums[i],
                currentMax + nums[i]);

            maximumSum = max(maximumSum, currentMax);

            /*
                Minimum Kadane:

                Either start a new subarray at i or extend the
                minimum-sum subarray ending at i - 1.
            */
            currentMin = min(
                nums[i],
                currentMin + nums[i]);

            minimumSum = min(minimumSum, currentMin);

            totalSum += nums[i];
        }

        /*
            If maximumSum is negative, every element is negative.

            In that case:
                totalSum - minimumSum = 0

            But 0 would represent excluding the complete array and
            selecting an empty subarray, which is invalid.
        */
        if (maximumSum < 0)
        {
            return maximumSum;
        }

        int wrappingSum = totalSum - minimumSum;

        return max(maximumSum, wrappingSum);
    }
};

/*
    Time Complexity: O(n)
    Space Complexity: O(1)
*/