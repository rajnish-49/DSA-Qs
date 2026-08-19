#include <bits/stdc++.h>
using namespace std;

/*
    Problem: Target Sum
    Link: https://leetcode.com/problems/target-sum/

    We are given nums[] and must place either '+' or '-' before every element
    so that the final expression evaluates to target.

    Example:
        nums = [1,1,1,1,1], target = 3

        One valid assignment:
        +1 +1 +1 +1 -1 = 3

    We need to count how many such assignments are possible.

    ------------------------------------------------------------
    CONVERTING IT TO 0/1 KNAPSACK
    ------------------------------------------------------------

    Let:
        P = sum of elements given '+'
        N = sum of elements given '-'

    We need:

        P - N = target

    Also, every element belongs to either P or N:

        P + N = totalSum

    Adding the two equations:

        2P = totalSum + target

        P = (totalSum + target) / 2

    So the problem becomes:

        Count the number of subsets whose sum is
        (totalSum + target) / 2.

    Each number can be selected at most once, so this becomes
    a 0/1 Knapsack "count subsets" problem.
*/

class Solution
{
public:

    // ============================================================
    // APPROACH 1: MEMOIZATION
    // ============================================================

    vector<vector<int>> memo;

    /*
        solve(index, requiredSum)

        = number of ways to choose a subset from index ... n-1
          whose sum is exactly requiredSum.
    */
    int solve(vector<int> &nums, int index, int requiredSum)
    {
        /*
            When all elements have been considered, we found one valid
            subset only if the required sum has become exactly 0.

            This base case also handles zeros correctly because we do not
            return early merely because requiredSum becomes 0.
        */
        if (index == nums.size())
            return (requiredSum == 0) ? 1 : 0;

        if (memo[index][requiredSum] != -1)
            return memo[index][requiredSum];

        // Option 1: Do not include nums[index] in the positive subset.
        int skip = solve(nums, index + 1, requiredSum);

        // Option 2: Include nums[index], only if it does not exceed requiredSum.
        int take = 0;

        if (nums[index] <= requiredSum)
        {
            take = solve(
                nums,
                index + 1,
                requiredSum - nums[index]
            );
        }

        // Every valid subset from either choice represents one sign assignment.
        return memo[index][requiredSum] = take + skip;
    }

    int findTargetSumWaysMemo(vector<int> &nums, int target)
    {
        int totalSum = accumulate(nums.begin(), nums.end(), 0);

        /*
            P = (totalSum + target) / 2

            If |target| > totalSum, even assigning every number the same
            sign cannot produce the target.
        */
        if (abs(target) > totalSum)
            return 0;

        /*
            totalSum + target must be even because P must be an integer.
        */
        if ((totalSum + target) % 2 != 0)
            return 0;

        int subsetSum = (totalSum + target) / 2;

        memo.assign(
            nums.size(),
            vector<int>(subsetSum + 1, -1)
        );

        return solve(nums, 0, subsetSum);
    }


    // ============================================================
    // APPROACH 2: 2D TABULATION
    // ============================================================

    int findTargetSumWaysTabulation(vector<int> &nums, int target)
    {
        int n = nums.size();
        int totalSum = accumulate(nums.begin(), nums.end(), 0);

        if (abs(target) > totalSum)
            return 0;

        if ((totalSum + target) % 2 != 0)
            return 0;

        int subsetSum = (totalSum + target) / 2;

        /*
            State:

            dp[i][sum] = number of subsets among the first i elements
                         whose sum is exactly 'sum'.

            Row i corresponds to considering nums[0 ... i-1].
        */
        vector<vector<int>> dp(
            n + 1,
            vector<int>(subsetSum + 1, 0)
        );

        /*
            With zero elements, there is exactly one way to make sum 0:
            choose the empty subset.
        */
        dp[0][0] = 1;

        for (int i = 1; i <= n; i++)
        {
            for (int sum = 0; sum <= subsetSum; sum++)
            {
                /*
                    nums[i - 1] is the current element because the DP uses
                    1-based item rows while nums is 0-indexed.
                */

                // Skip the current number.
                dp[i][sum] = dp[i - 1][sum];

                /*
                    Take the current number.

                    After taking nums[i - 1], we need the remaining sum:

                        sum - nums[i - 1]

                    We use row i - 1 because each number can be used only once.
                */
                if (nums[i - 1] <= sum)
                {
                    dp[i][sum] +=
                        dp[i - 1][sum - nums[i - 1]];
                }
            }
        }

        return dp[n][subsetSum];
    }


    // ============================================================
    // APPROACH 3: 1D SPACE-OPTIMIZED TABULATION
    // ============================================================

    int findTargetSumWays1D(vector<int> &nums, int target)
    {
        int totalSum = accumulate(nums.begin(), nums.end(), 0);

        if (abs(target) > totalSum)
            return 0;

        if ((totalSum + target) % 2 != 0)
            return 0;

        int subsetSum = (totalSum + target) / 2;

        /*
            dp[sum] = number of subsets from the elements processed so far
                      whose sum is exactly 'sum'.
        */
        vector<int> dp(subsetSum + 1, 0);

        // Empty subset is one way to form sum 0.
        dp[0] = 1;

        for (int num : nums)
        {
            /*
                Traverse from right to left so the same number cannot be
                reused multiple times during the current iteration.

                This is the standard 0/1 Knapsack space optimization.
            */
            for (int sum = subsetSum; sum >= num; sum--)
            {
                dp[sum] += dp[sum - num];
            }
        }

        return dp[subsetSum];
    }


    int findTargetSumWays(vector<int> &nums, int target)
    {
        return findTargetSumWaysTabulation(nums, target);
    }
};

/*
    Memoization:
        Time Complexity:  O(n * subsetSum)
        Space Complexity: O(n * subsetSum) + O(n) recursion stack

    2D Tabulation:
        Time Complexity:  O(n * subsetSum)
        Space Complexity: O(n * subsetSum)

    1D Tabulation:
        Time Complexity:  O(n * subsetSum)
        Space Complexity: O(subsetSum)
*/