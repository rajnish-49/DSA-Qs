#include <bits/stdc++.h>
using namespace std;

/*
Problem: 0/1 Knapsack

We are given:
- wt[i]  = weight of item i
- val[i] = value of item i
- W      = maximum capacity of the knapsack

Each item can be chosen at most once.

Goal:
Maximize the total value of selected items while keeping
their total weight <= W.

For every item, we have two choices:
1. Skip the item.
2. Take the item, only if its weight fits in the remaining capacity.

This "take / not take" decision is the basic idea behind 0/1 Knapsack.
*/

// ============================================================
// APPROACH 1: MEMOIZATION
// ============================================================

int memo[1001][1001];

/*
State:

knapsackTopDown(wt, val, W, n)

= maximum value we can obtain using the first n items
  when the remaining knapsack capacity is W.

The current item is index n - 1.
*/

int knapsackTopDown(int wt[], int val[], int W, int n)
{
    // No items left or no capacity left means no more value can be collected.
    if (n == 0 || W == 0)
        return 0;

    if (memo[n][W] != -1)
        return memo[n][W];

    /*
        If the current item fits, we have two choices:

        Take:
            Add its value, reduce the remaining capacity by its weight,
            and move to the remaining n - 1 items.

        Skip:
            Keep the same capacity and move to the remaining n - 1 items.

        Since this is 0/1 Knapsack, after taking the item we still move
        to n - 1, which prevents the same item from being chosen again.
    */
    if (wt[n - 1] <= W)
    {
        int take =
            val[n - 1] +
            knapsackTopDown(wt, val, W - wt[n - 1], n - 1);

        int skip =
            knapsackTopDown(wt, val, W, n - 1);

        return memo[n][W] = max(take, skip);
    }

    // If the current item is too heavy, skipping it is the only choice.
    return memo[n][W] =
               knapsackTopDown(wt, val, W, n - 1);
}

// ============================================================
// APPROACH 2: 2D TABULATION
// ============================================================

int knapsackBottomUp2D(
    int W,
    vector<int>& wt,
    vector<int>& val,
    int n)
{
    /*
        State:

        dp[i][capacity] = maximum value obtainable using the first i items
                          with knapsack capacity 'capacity'.

        dp[0][capacity] = 0:
        With zero items, no value can be collected.

        dp[i][0] = 0:
        With zero capacity, no item can be selected.
    */

    vector<vector<int>> dp(
        n + 1,
        vector<int>(W + 1, 0)
    );

    for (int i = 1; i <= n; i++)
    {
        for (int capacity = 0; capacity <= W; capacity++)
        {
            /*
                Item i in the DP table corresponds to index i - 1
                in the 0-indexed wt and val arrays.
            */

            // If the current item fits, choose the better of taking it or skipping it.
            if (wt[i - 1] <= capacity)
            {
                dp[i][capacity] = max(
                    val[i - 1] + dp[i - 1][capacity - wt[i - 1]],  // Take the current item
                    dp[i - 1][capacity]                             // Skip the current item
                );
            }
            else
            {
                // If the current item does not fit, we can only skip it.
                dp[i][capacity] = dp[i - 1][capacity];
            }
        }
    }

    return dp[n][W];
}

// ============================================================
// APPROACH 3: 1D SPACE-OPTIMIZED TABULATION
// ============================================================

int knapsackBottomUp1D(
    int W,
    vector<int>& wt,
    vector<int>& val,
    int n)
{
    /*
        dp[capacity] = maximum value obtainable for this capacity
        after processing the items seen so far.

        In the 2D solution, dp[i][...] only depends on dp[i - 1][...],
        so the previous row can be compressed into one array.
    */

    vector<int> dp(W + 1, 0);

    for (int i = 0; i < n; i++)
    {
        /*
            Capacity MUST be traversed from right to left.

            When calculating:

                dp[capacity] =
                    max(dp[capacity],
                        dp[capacity - wt[i]] + val[i]);

            dp[capacity - wt[i]] must still represent the state BEFORE
            processing the current item.

            Going right-to-left guarantees that it has not yet been
            updated during the current iteration.

            If we went left-to-right, an updated state could be reused
            immediately, allowing the same item to be selected multiple
            times. That would turn it into Unbounded Knapsack.
        */

        for (int capacity = W;
             capacity >= wt[i];
             capacity--)
        {
            int skip = dp[capacity];

            int take =
                dp[capacity - wt[i]] + val[i];

            dp[capacity] = max(skip, take);
        }
    }

    return dp[W];
}

/*
Memoization:
    Time Complexity:  O(n * W)
    Space Complexity: O(n * W) + O(n) recursion stack

2D Tabulation:
    Time Complexity:  O(n * W)
    Space Complexity: O(n * W)

1D Space Optimization:
    Time Complexity:  O(n * W)
    Space Complexity: O(W)

Most important 0/1 Knapsack observation:

    TAKE:
        dp[i - 1][capacity - wt[i - 1]]

    SKIP:
        dp[i - 1][capacity]

We always move to the previous item after taking because
every item is allowed to be used only once.
*/