// @https://leetcode.com/problems/jump-game-v/description/

/*
    Jump Game V

    From index i, we can jump at most d positions left or right.

    A jump from i to j is valid only when:
    - arr[j] < arr[i]
    - we do not cross any index whose value is greater than or equal to arr[i]

    Since every jump goes to a strictly smaller value, cycles are impossible.

    State:
    dp[i] = maximum number of indices that can be visited starting from i
*/

class Solution
{
public:
    vector<int> dp;
    int n;

    int dfs(int i, vector<int> &arr, int d)
    {
        if (dp[i] != -1)
            return dp[i];

        // The current index itself is always counted.
        int ans = 1;

        // Try jumping towards the left, up to distance d.
        for (int j = i - 1; j >= max(0, i - d); j--)
        {
            /*
                A greater or equal value blocks this direction.

                We cannot land on it and cannot jump over it,
                so no farther left index can be considered.
            */
            if (arr[j] >= arr[i])
                break;

            ans = max(ans, 1 + dfs(j, arr, d));
        }

        // Try jumping towards the right, up to distance d.
        for (int j = i + 1; j <= min(n - 1, i + d); j++)
        {
            if (arr[j] >= arr[i])
                break;

            ans = max(ans, 1 + dfs(j, arr, d));
        }

        return dp[i] = ans;
    }

    int maxJumps(vector<int> &arr, int d)
    {
        n = arr.size();
        dp.assign(n, -1);

        int res = 1;

        /*
            The jump sequence may start from any index,
            so calculate the best result for every starting position.
        */
        for (int i = 0; i < n; i++)
        {
            res = max(res, dfs(i, arr, d));
        }

        return res;
    }
};

/*
    Time:  O(n * d)
    Space: O(n) DP array + O(n) recursion stack
*/

/*
    Tabulation:

    dp[i] = maximum number of indices that can be visited
            when starting from index i.

    Since i can jump only to an index having a smaller value,
    process indices in increasing order of their values.
*/

class Solution
{
public:
    int maxJumps(vector<int> &arr, int d)
    {
        int n = arr.size();

        vector<int> indices(n);

        for (int i = 0; i < n; i++)
        {
            indices[i] = i;
        }

        // Smaller-valued indices must be processed before larger-valued ones.
        sort(indices.begin(), indices.end(),
             [&](int a, int b)
             {
                 return arr[a] < arr[b];
             });

        // Every starting index counts as one visited index.
        vector<int> dp(n, 1);

        int res = 1;

        for (int i : indices)
        {
            // Try jumping towards the left.
            for (int j = i - 1; j >= max(0, i - d); j--)
            {
                /*
                    A greater or equal value blocks this direction.
                    We cannot land on it or jump over it.
                */
                if (arr[j] >= arr[i])
                    break;

                dp[i] = max(dp[i], 1 + dp[j]);
            }

            // Try jumping towards the right.
            for (int j = i + 1; j <= min(n - 1, i + d); j++)
            {
                if (arr[j] >= arr[i])
                    break;

                dp[i] = max(dp[i], 1 + dp[j]);
            }

            res = max(res, dp[i]);
        }

        return res;
    }
};

/*
    Time:  O(n log n + n * d)
    Space: O(n)
*/