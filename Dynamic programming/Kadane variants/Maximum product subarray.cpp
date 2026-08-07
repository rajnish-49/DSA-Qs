#include <bits/stdc++.h>
using namespace std;

/*
    Problem: Maximum Product Subarray
    Link: https://leetcode.com/problems/maximum-product-subarray/

    Find the maximum product of a non-empty contiguous subarray.

    We store two values for every index:

    maxEnding[i] = maximum product of a subarray ending exactly at i
    minEnding[i] = minimum product of a subarray ending exactly at i

    The minimum product is required because multiplying a large negative
    product by another negative number can produce the maximum product.
*/

class Solution
{
public:
    int maxProduct(vector<int> &nums)
    {
        int n = nums.size();

        vector<long long> maxEnding(n);
        vector<long long> minEnding(n);

        /*
            At index 0, the only subarray ending there is [nums[0]].

            Therefore, both the maximum and minimum products
            ending at index 0 are nums[0].
        */
        maxEnding[0] = nums[0];
        minEnding[0] = nums[0];

        long long answer = nums[0];

        for (int i = 1; i < n; i++)
        {
            /*
                Any subarray ending at i is formed in one of three ways:

                1. Start a new subarray from nums[i].
                2. Extend the previous maximum-product subarray.
                3. Extend the previous minimum-product subarray.
            */
            long long startFresh = nums[i];

            long long extendMaximum = nums[i] * maxEnding[i - 1];

            long long extendMinimum = nums[i] * minEnding[i - 1];

            /*
                Store the largest product ending exactly at i.

                When nums[i] is negative, extendMinimum may become
                the maximum because negative × negative is positive.
            */
            maxEnding[i] = max(
                startFresh,
                max(extendMaximum, extendMinimum));

            /*
                Store the smallest product ending exactly at i.

                This negative product may become useful later if it
                is multiplied by another negative number.
            */
            minEnding[i] = min(
                startFresh,
                min(extendMaximum, extendMinimum));

            // The final subarray may end at any index.
            answer = max(answer, maxEnding[i]);
        }

        return answer;
    }
};

/*
    Time Complexity: O(n)
    Space Complexity: O(n)
*/