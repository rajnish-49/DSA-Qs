/*
 * ============================================================
 * PROBLEM: 0/1 Knapsack
 * ============================================================
 * Given n items each with weight w[i] and value v[i], and a
 * knapsack of capacity W, find the maximum value you can carry.
 * Each item is either taken (1) or skipped (0) — never split.
 *
 * BRUTE FORCE FAILURE:
 * Enumerate all 2^n subsets — exponential, dies at n ~= 20.
 *
 * KEY INSIGHT:
 * At item i with remaining capacity j, you make a binary choice:
 * skip (capacity unchanged, move to i+1) or take (capacity drops
 * by w[i], gain v[i], move to i+1). These subproblems recur
 * across different item prefixes with the same remaining capacity
 * → overlapping subproblems → memoize or tabulate.
 *
 * STATE: f(i, j) = max value using items i..n-1 with capacity j
 *   (top-down natural definition — "items remaining" framing)
 * OR
 * dp[i][j] = max value using items 0..i-1 with capacity j
 *   (bottom-up "items considered so far" framing)
 * Both encode the same recurrence, just indexed differently.
 *
 * TRANSITION:
 *   skip item i : f(i, j) = f(i+1, j)
 *   take item i : f(i, j) = v[i] + f(i+1, j - w[i])  if w[i] <= j
 *   f(i, j) = max of the two valid choices
 *
 * BASE CASES:
 *   f(n, j) = 0  — no items left, zero value regardless of capacity
 *   f(i, 0) = 0  — zero capacity, can't take anything
 * ============================================================
 */

#include <bits/stdc++.h>
using namespace std;

// ─────────────────────────────────────────────
// TOP-DOWN (memoization)
// Recurse on the natural choice tree; cache to avoid recomputation.
// memo[n][W] = -1 means not yet computed.
// ─────────────────────────────────────────────
int memo[1001][1001];

int knapsackTopDown(int wt[], int val[], int W, int n) {
    if (n == 0 || W == 0)
        return 0;

    if (memo[n][W] != -1)
        return memo[n][W];

    if (wt[n-1] <= W)
        return memo[n][W] = max(val[n-1] + knapsackTopDown(wt, val, W - wt[n-1], n-1),
                                            knapsackTopDown(wt, val, W,           n-1));
    else
        return memo[n][W] = knapsackTopDown(wt, val, W, n-1);
}

// ─────────────────────────────────────────────
// BOTTOM-UP 2D (tabulation)
// Fill the table in dependency order so every subproblem needed
// by dp[i][j] is already computed when we reach it.
// ─────────────────────────────────────────────
int knapsackBottomUp2D(int W, vector<int>& wt, vector<int>& val, int n) {
    // dp[i][j]: best value from the FIRST i items with capacity j
    // Row 0 (zero items) is all 0s by default — the base case.
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int j = 0; j <= W; j++) {
            // Skip item i: inherit the best result without it
            dp[i][j] = dp[i-1][j];

            // Take item i: only if it fits; compare against skip
            if (wt[i-1] <= j)
                dp[i][j] = max(dp[i][j], dp[i-1][j - wt[i-1]] + val[i-1]);
            // wt[i-1] / val[i-1] because items are 0-indexed but dp rows are 1-indexed
        }
    }

    return dp[n][W];
}

// ─────────────────────────────────────────────
// BOTTOM-UP 1D (space-optimized tabulation)
// dp[i][j] only ever reads from dp[i-1][...], so one array suffices.
// Iterating capacity RIGHT TO LEFT preserves the "previous row"
// values that the current item still needs.
// ─────────────────────────────────────────────
int knapsackBottomUp1D(int W, vector<int>& wt, vector<int>& val, int n) {
    vector<int> dp(W + 1, 0);

    for (int i = 0; i < n; i++) {
        // Right-to-left: dp[j - wt[i]] hasn't been touched yet in
        // this pass, so it still holds the "before item i" value.
        // Left-to-right would let item i be picked multiple times
        // (that's unbounded knapsack — a different problem).
        for (int j = W; j >= wt[i]; j--)
            dp[j] = max(dp[j], dp[j - wt[i]] + val[i]);
    }

    return dp[W];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, W;
    cin >> n >> W;

    vector<int> wt(n), val(n);
    for (int i = 0; i < n; i++) cin >> wt[i] >> val[i];

    memset(memo, -1, sizeof(memo));

    cout << "Top-Down    : " << knapsackTopDown(wt.data(), val.data(), W, n) << "\n";
    cout << "Bottom-Up 2D: " << knapsackBottomUp2D(W, wt, val, n)            << "\n";
    cout << "Bottom-Up 1D: " << knapsackBottomUp1D(W, wt, val, n)            << "\n";

    return 0;
}

/*
 * COMPLEXITY (all three):
 *   Time:  O(n * W) — every (item, capacity) pair visited once
 *   Space: O(n * W) for 2D and top-down memo; O(W) for 1D
 *
 * TOP-DOWN vs BOTTOM-UP:
 *   Top-down only computes subproblems actually reached by recursion
 *   — useful when the state space is sparse (many (i,j) never visited).
 *   Bottom-up fills the entire table unconditionally but has no
 *   recursion overhead and is cache-friendlier in practice.
 *
 * THE RIGHT-TO-LEFT INVARIANT (1D):
 *   When processing item i, dp[j] must still mean "best value with
 *   first (i-1) items and capacity j". Right-to-left guarantees this
 *   because we only write dp[j] after we've already read dp[j - wt[i]],
 *   which sits to the left and hasn't been overwritten yet this pass.
 */