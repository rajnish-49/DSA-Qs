#include <bits/stdc++.h>
using namespace std;

/*
    Problem: Minimum Subset Sum Difference

    We have to divide the array into two subsets such that the absolute
    difference between their sums is minimum.

    Let:
        s1 = sum of first subset
        s2 = sum of second subset

    Since every element belongs to exactly one of the two subsets:

        s1 + s2 = totalSum
        s2 = totalSum - s1

    Therefore:

        difference = |s2 - s1|
                   = |totalSum - 2 * s1|

    We only need to check possible values of s1 from 0 to totalSum / 2.
    If s1 goes beyond half, the two subsets simply swap roles.
*/


// ============================================================
// MEMOIZATION
// ============================================================

class Solution
{
public:

    /*
        solve(i, target) = whether we can form exactly 'target'
                           using elements from index i to n - 1.
    */
    bool solve(int i, int target, vector<int> &arr,
               vector<vector<int>> &dp)
    {
        // After considering all elements, the subset is valid only if target became 0.
        if (i == arr.size())
            return (target == 0);

        if (dp[i][target] != -1)
            return dp[i][target];

        // Do not include arr[i] in the subset.
        bool notTake = solve(i + 1, target, arr, dp);

        // Include arr[i] only if it does not exceed the remaining target.
        bool take = false;

        if (arr[i] <= target)
            take = solve(i + 1, target - arr[i], arr, dp);

        // Target is achievable if either taking or skipping the current element works.
        return dp[i][target] = (take || notTake);
    }

    int minSubsetSumDifference(vector<int> &arr, int n)
    {
        int totalSum = 0;

        for (int x : arr)
            totalSum += x;

        /*
            We only search subset sums up to totalSum / 2.

            The closer s1 is to half of totalSum, the closer the
            two subset sums will be and hence the smaller the difference.
        */
        int half = totalSum / 2;

        vector<vector<int>> dp(
            n,
            vector<int>(half + 1, -1)
        );

        int minDiff = INT_MAX;

        /*
            Try every possible sum s1 up to half.

            If a subset with sum s1 exists, the remaining elements
            automatically form the other subset with sum:

                s2 = totalSum - s1
        */
        for (int s1 = 0; s1 <= half; s1++)
        {
            if (solve(0, s1, arr, dp))
            {
                int s2 = totalSum - s1;

                minDiff = min(
                    minDiff,
                    abs(s2 - s1)
                );
            }
        }

        return minDiff;
    }
};


// ============================================================
// TABULATION
// ============================================================

class Solution
{
public:
    int minSubsetSumDifference(vector<int> &arr, int n)
    {
        int totalSum = 0;

        for (int x : arr)
            totalSum += x;

        int half = totalSum / 2;

        /*
            dp[i][target] = true if we can form exactly 'target'
                            using elements from arr[0...i].

            We only store target values up to totalSum / 2 because
            larger subset sums are not needed for finding the minimum difference.
        */
        vector<vector<bool>> dp(
            n,
            vector<bool>(half + 1, false)
        );

        /*
            Sum 0 is always possible by selecting no elements,
            regardless of how many elements are available.
        */
        for (int i = 0; i < n; i++)
            dp[i][0] = true;

        // Using only arr[0], we can form the sum arr[0].
        if (arr[0] <= half)
            dp[0][arr[0]] = true;

        for (int i = 1; i < n; i++)
        {
            for (int target = 0; target <= half; target++)
            {
                // Skip arr[i], so use the result from the previous row.
                bool notTake = dp[i - 1][target];

                /*
                    Take arr[i].

                    After taking it, we need to know whether the remaining
                    sum target - arr[i] could be formed using previous elements.
                */
                bool take = false;

                if (arr[i] <= target)
                    take = dp[i - 1][target - arr[i]];

                dp[i][target] = take || notTake;
            }
        }

        int minDiff = INT_MAX;

        /*
            dp[n - 1][s1] tells us which subset sums are achievable.

            For every achievable s1:
                s2 = totalSum - s1

            Choose the pair having the smallest absolute difference.
        */
        for (int s1 = 0; s1 <= half; s1++)
        {
            if (dp[n - 1][s1])
            {
                int s2 = totalSum - s1;

                minDiff = min(
                    minDiff,
                    abs(s2 - s1)
                );
            }
        }

        return minDiff;
    }
};