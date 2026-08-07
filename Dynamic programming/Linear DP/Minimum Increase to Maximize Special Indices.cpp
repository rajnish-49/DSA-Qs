#include <bits/stdc++.h>
using namespace std;

/*
    Problem: Minimum Increase to Maximize Special Indices
    Link: https://leetcode.com/problems/minimum-increase-to-maximize-special-indices/

    An interior index i is special when:

        nums[i] > nums[i - 1]
        nums[i] > nums[i + 1]

    In one operation, we can increase any array element by 1.

    We must:
    1. Maximize the number of special indices.
    2. Among all ways that achieve this maximum count,
       minimize the total number of operations.

    Two adjacent indices cannot both be special because that would require:

        nums[i] > nums[i + 1]
        nums[i + 1] > nums[i]

    Therefore, when index i is chosen as special,
    index i - 1 and index i + 1 cannot be chosen.
*/

class Solution
{
public:
    /*
        Each result stores:

        first  = number of special indices created
        second = total operations required

        More special indices are always preferred.
        If the counts are equal, the smaller cost is preferred.
    */
    using State = pair<int, long long>;

    vector<State> memo;

    State best(State a, State b)
    {
        if (a.first != b.first)
        {
            return a.first > b.first ? a : b;
        }

        return a.second < b.second ? a : b;
    }

    /*
        Minimum increase required to make index i special.

        nums[i] must become strictly greater than both neighbours,
        so its minimum required value is:

            max(nums[i - 1], nums[i + 1]) + 1
    */
    long long cost(int i, vector<int> &nums)
    {
        long long requiredValue =
            (long long)max(nums[i - 1], nums[i + 1]) + 1;

        return max(0LL, requiredValue - nums[i]);
    }

    // ============================================================
    // MEMOIZATION
    // ============================================================

    /*
        solve(i) returns the best result obtainable by considering
        the interior indices from i to n - 2.
    */
    State solve(int i, vector<int> &nums)
    {
        int n = nums.size();

        // No interior index remains to be considered.
        if (i > n - 2)
        {
            return {0, 0};
        }

        if (memo[i].first != -1)
        {
            return memo[i];
        }

        /*
            Skip index i.

            Since i is not made special, index i + 1 remains
            available for consideration.
        */
        State skip = solve(i + 1, nums);

        /*
            Make index i special.

            Index i + 1 cannot also be special, so continue
            from index i + 2.
        */
        State take = solve(i + 2, nums);

        take.first += 1;
        take.second += cost(i, nums);

        return memo[i] = best(skip, take);
    }

    long long minIncreaseMemo(vector<int> &nums)
    {
        int n = nums.size();

        /*
            A special-index count is never negative, so first = -1
            safely represents an uncomputed memoization state.
        */
        memo.assign(n, {-1, 0});

        State answer = solve(1, nums);

        return answer.second;
    }

    // ============================================================
    // TABULATION
    // ============================================================

    long long minIncreaseTabulation(vector<int> &nums)
    {
        int n = nums.size();

        /*
            dp[i][0]:
            Best result considering interior indices from 1 to i
            when index i is not made special.

            dp[i][1]:
            Best result considering interior indices from 1 to i
            when index i is made special.
        */
        vector<array<State, 2>> dp(n);

        /*
            Index 1 is the first possible special index.

            dp[1][0]:
            Do not make index 1 special.

            dp[1][1]:
            Make index 1 special and pay its required cost.
        */
        dp[1][0] = {0, 0};
        dp[1][1] = {1, cost(1, nums)};

        for (int i = 2; i <= n - 2; i++)
        {
            /*
                Index i is not special.

                Index i - 1 may have been either selected or skipped,
                so choose the better previous result.
            */
            dp[i][0] = best(
                dp[i - 1][0],
                dp[i - 1][1]);

            /*
                Index i is special.

                Index i - 1 must not be special, so this state
                can extend only dp[i - 1][0].
            */
            dp[i][1] = {
                dp[i - 1][0].first + 1,
                dp[i - 1][0].second + cost(i, nums)};
        }

        /*
            The last interior index n - 2 may either be selected
            or skipped. Choose the better final state.
        */
        State answer = best(
            dp[n - 2][0],
            dp[n - 2][1]);

        return answer.second;
    }

    long long minIncrease(vector<int> &nums)
    {
        return minIncreaseTabulation(nums);
    }
};

/*
    Memoization:
    Time:  O(n)
    Space: O(n) memoization array + O(n) recursion stack

    Tabulation:
    Time:  O(n)
    Space: O(n)
*/