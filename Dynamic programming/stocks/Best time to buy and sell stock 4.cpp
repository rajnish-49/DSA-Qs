/*
 * ============================================================================
 * PROBLEM: Best Time to Buy and Sell Stock IV (LeetCode 188)
 * ============================================================================
 *
 * PROBLEM STATEMENT (Simple Terms):
 * ------------------------------------
 * You are given an array `prices` where prices[i] is the price of a stock
 * on day i. You may complete AT MOST `k` transactions (each transaction =
 * one buy + one sell). Your goal is to maximize total profit.
 *
 * CONSTRAINTS:
 *   - You CANNOT hold more than one stock at a time (must sell before buying again).
 *   - Each buy-sell pair counts as ONE transaction.
 *   - You want to maximize the sum of (sell_price - buy_price) across all transactions.
 *
 * INPUT:
 *   - k       : maximum number of transactions allowed
 *   - prices  : array of daily stock prices
 *
 * OUTPUT:
 *   - Maximum profit achievable using at most k transactions.
 *
 * ============================================================================
 * INTUITION & APPROACH (First Principles)
 * ============================================================================
 *
 * 1. BRUTE FORCE OBSERVATION:
 *    At each day, you can either buy, sell, or do nothing. Trying all
 *    combinations is exponential — clearly too slow for large inputs.
 *
 * 2. KEY OBSERVATIONS:
 *    a) At any point in time, your "state" is determined by:
 *         - Which day you're on (i)
 *         - Whether you currently HOLD a stock or not (buy = 1 means you CAN buy)
 *         - How many transactions (caps) remain
 *    b) A transaction is "used up" only when you SELL (not when you buy).
 *       This is a common design choice in DP to keep transitions clean.
 *    c) If k >= n/2, you can perform unlimited transactions (every profitable
 *       consecutive day gap can be captured), so we handle this as a special
 *       greedy case.
 *
 * 3. WHY DP WORKS:
 *    The decision at each day depends only on the current state (day, holding?,
 *    transactions left), not on how we got there. This "optimal substructure"
 *    property makes DP applicable.
 *
 * 4. DP STATE DEFINITION:
 *    dp[i][buy][cap] = max profit starting from day i,
 *                      where buy=1 means we can buy (don't hold stock),
 *                      buy=0 means we hold a stock (can only sell),
 *                      and cap = transactions remaining.
 *
 * 5. TRANSITIONS:
 *    IF we can BUY (buy == 1):
 *      - Skip today:  dp[i+1][1][cap]              (do nothing)
 *      - Buy today:  -prices[i] + dp[i+1][0][cap]  (spend money, now holding)
 *
 *    IF we HOLD and can SELL (buy == 0):
 *      - Skip today:  dp[i+1][0][cap]              (do nothing)
 *      - Sell today: +prices[i] + dp[i+1][1][cap-1] (earn money, use 1 transaction)
 *
 * 6. BASE CASE:
 *    If we run out of days (i == n) or transactions (cap == 0), profit = 0.
 *
 * ============================================================================
 */


/* ============================================================================
 * SOLUTION 1: TOP-DOWN DP WITH MEMOIZATION (Recursive + Cache)
 * ============================================================================
 * Approach: Recursive exploration of all decisions with memoization to avoid
 * recomputing the same subproblems. This is the most natural way to express
 * the DP state transitions described above.
 * ============================================================================
 */
class Solution {
public:
    int n; // Total number of days (size of prices array)

    // dp[i][buy][cap]:
    //   i   = current day index (0 to n-1)
    //   buy = 1 if we are allowed to buy (don't hold stock), 0 if we hold stock
    //   cap = remaining transactions we can still make
    // Initialized to -1 to indicate "not yet computed"
    vector<vector<vector<int>>> dp;

    /*
     * solve() — Recursive function that returns maximum profit
     * starting from day `i`, with state `buy` and `cap` remaining transactions.
     *
     * Parameters:
     *   prices : stock price array
     *   i      : current day we're deciding on
     *   buy    : 1 = we can buy today (no stock in hand), 0 = we hold a stock
     *   cap    : number of transactions still available
     */
    int solve(vector<int>& prices, int i, int buy, int cap) {

        // BASE CASE 1: No more days to process — no more profit possible.
        // BASE CASE 2: No transactions left — we're not allowed to trade anymore.
        // Both cases contribute 0 to the profit.
        if (i == n || cap == 0) return 0;

        // MEMOIZATION CHECK:
        // If we've already computed the answer for this exact state (i, buy, cap),
        // return it directly. This is the key optimization over plain recursion.
        if (dp[i][buy][cap] != -1) return dp[i][buy][cap];

        // CASE 1: We currently DO NOT hold a stock (buy == 1)
        // We have two choices: skip today, or buy today.
        if (buy) {
            return dp[i][buy][cap] = max(
                // CHOICE A: Do nothing today — move to next day, still can buy
                solve(prices, i + 1, 1, cap),

                // CHOICE B: Buy today — pay prices[i], now we hold stock (buy = 0)
                // Cap stays the same; a transaction is counted only upon SELLING.
                -prices[i] + solve(prices, i + 1, 0, cap)
            );
        }

        // CASE 2: We currently HOLD a stock (buy == 0)
        // We have two choices: skip today, or sell today.
        else {
            return dp[i][buy][cap] = max(
                // CHOICE A: Do nothing today — move to next day, still holding stock
                solve(prices, i + 1, 0, cap),

                // CHOICE B: Sell today — earn prices[i], no longer holding (buy = 1)
                // Decrement cap by 1 because we've now COMPLETED one transaction.
                prices[i] + solve(prices, i + 1, 1, cap - 1)
            );
        }
    }

    int maxProfit(int k, vector<int>& prices) {
        n = prices.size();

        // Initialize the DP table with -1 (uncomputed).
        // Dimensions: n days × 2 states (hold or not) × (k+1) transaction caps
        dp.assign(n, vector<vector<int>>(2, vector<int>(k + 1, -1)));

        // Start from day 0, with buy=1 (no stock held yet), and k transactions remaining.
        return solve(prices, 0, 1, k);
    }
};


/* ============================================================================
 * SOLUTION 2: BOTTOM-UP DP (Tabulation) WITH GREEDY SHORTCUT
 * ============================================================================
 * Approach: Instead of recursing from day 0 and memoizing, we build the
 * answer table from the last day backward. This avoids recursion overhead
 * and is often faster in practice due to cache locality.
 *
 * KEY ADDITION: Greedy shortcut for large k.
 *   If k >= n/2, we can make unlimited transactions (since at most n/2
 *   non-overlapping transactions fit in n days). In this case, we simply
 *   collect every upward price movement — a classic greedy trick.
 * ============================================================================
 */
class Solution {
public:
    int maxProfit(int k, vector<int>& prices) {
        int n = prices.size();

        /*
         * GREEDY SHORTCUT:
         * If k >= n/2, the transaction limit is effectively unlimited.
         * Why? You can have at most floor(n/2) non-overlapping buy-sell pairs
         * in n days. So if k is that large or larger, just grab every
         * profitable consecutive difference — this is optimal for unlimited trades.
         */
        if (k >= n / 2) {
            int profit = 0;
            for (int i = 1; i < n; i++)
                if (prices[i] > prices[i - 1])
                    // Accumulate every upward price movement greedily.
                    profit += prices[i] - prices[i - 1];
            return profit;
        }

        /*
         * BOTTOM-UP DP TABLE:
         * dp[i][buy][cap] = max profit from day i onward,
         *                   given buy-state and cap transactions remaining.
         *
         * We use (n+1) days so that dp[n][...][...] = 0 naturally
         * represents the base case (no more days left).
         * All values initialized to 0 (base case is profit = 0).
         */
        vector<vector<vector<int>>> dp(n + 1,
            vector<vector<int>>(2, vector<int>(k + 1, 0)));

        /*
         * Fill the table BACKWARDS from day n-1 to day 0.
         * We go backwards because dp[i] depends on dp[i+1] (future days).
         * This mirrors the recursive structure but eliminates recursion overhead.
         */
        for (int i = n - 1; i >= 0; i--) {
            for (int buy = 0; buy <= 1; buy++) {

                // cap=0 is already 0 (no transactions left = no profit).
                // Start from cap=1.
                for (int cap = 1; cap <= k; cap++) {

                    // CASE 1: We can buy (don't hold stock).
                    // Same logic as the recursive version, now reading from
                    // the already-computed future states (dp[i+1]).
                    if (buy) {
                        dp[i][buy][cap] = max(
                            dp[i + 1][1][cap],           // Skip today
                            -prices[i] + dp[i + 1][0][cap] // Buy today
                        );
                    }

                    // CASE 2: We hold stock and can sell.
                    // Selling completes a transaction, so cap decreases by 1.
                    else {
                        dp[i][buy][cap] = max(
                            dp[i + 1][0][cap],               // Skip today
                            prices[i] + dp[i + 1][1][cap - 1] // Sell today
                        );
                    }
                }
            }
        }

        // Answer: starting at day 0, with buy=1 (no stock held) and k transactions left.
        return dp[0][1][k];
    }
};


/* ============================================================================
 * SOLUTION 3: SPACE-OPTIMIZED DP WITH TRANSACTION STATE ENCODING
 * ============================================================================
 * Approach: Instead of a 3D dp[day][buy][cap] table, we collapse the
 * "buy" and "cap" dimensions into a single "transaction state" index `t`,
 * using the observation that:
 *
 *   - Even indices (t = 0, 2, 4, ..., 2k-2) = BUY states
 *     (we are about to buy for transaction t/2 + 1)
 *   - Odd  indices (t = 1, 3, 5, ..., 2k-1) = SELL states
 *     (we are about to sell for transaction (t+1)/2)
 *
 * So the total number of "transaction states" is 2*k.
 * This reduces the 3D table to a 2D table: dp[day][transaction_state].
 *
 * WHY THIS WORKS:
 *   Each transaction naturally alternates: buy → sell → buy → sell...
 *   By encoding this as a linear sequence of 2k states, we don't need
 *   a separate "buy" flag or "cap" counter — they're implied by `t`.
 *
 * STILL INCLUDES THE GREEDY SHORTCUT for k >= n/2.
 * ============================================================================
 */
class Solution {
public:
    int maxProfit(int k, vector<int>& prices) {
        int n = prices.size();

        // GREEDY SHORTCUT (same as Solution 2 — see explanation above)
        if (k >= n / 2) {
            int profit = 0;
            for (int i = 1; i < n; i++)
                if (prices[i] > prices[i - 1])
                    profit += prices[i] - prices[i - 1];
            return profit;
        }

        /*
         * dp[i][t] = max profit on day i in transaction state t.
         *
         * Transaction states are interleaved as:
         *   t=0  → Buy  for 1st transaction
         *   t=1  → Sell for 1st transaction
         *   t=2  → Buy  for 2nd transaction
         *   t=3  → Sell for 2nd transaction
         *   ...
         *   t=2k-2 → Buy  for k-th transaction
         *   t=2k-1 → Sell for k-th transaction
         *
         * The final answer is dp[n-1][2k-1]:
         *   the last sell state of the k-th transaction on the last day.
         */
        vector<vector<int>> dp(n, vector<int>(2 * k + 1, 0));

        /*
         * BASE CASE: Day 0 (first day).
         * For every BUY state (even t), if we buy on day 0, profit = -prices[0].
         * For every SELL state (odd t), we can't have sold without buying first,
         * so the profit remains 0.
         *
         * Why loop up to 2k (exclusive)?
         * We have 2k states: t = 0, 1, ..., 2k-1.
         * t = 2k is an extra slot used only for safe indexing in the loop below
         * (dp[i-1][t-1] when t=0 → we use index 0 itself as a guard).
         */
        for (int t = 0; t < 2 * k; t++) {
            if (t % 2 == 0)
                dp[0][t] = -prices[0]; // BUY state: we bought at day 0 price
            else
                dp[0][t] = 0;          // SELL state: can't sell without having bought
        }

        /*
         * FILL THE TABLE from day 1 to day n-1.
         * For each day and each transaction state, decide:
         *   - Stay in the same state (do nothing today)
         *   - Transition to the next state (buy or sell today)
         */
        for (int i = 1; i < n; i++) {
            for (int t = 0; t < 2 * k; t++) {

                /*
                 * EVEN t → BUY STATE: we need to buy stock to move to t+1 (sell state).
                 *
                 * Option A: Stay in buy state → dp[i-1][t] (didn't buy yet today)
                 * Option B: Buy today → -prices[i] + (profit before this buy)
                 *
                 * "Profit before this buy" = dp[i-1][t-1] (the previous sell state).
                 * But if t == 0 (first buy ever), there's no previous sell state.
                 * Guard: use dp[i-1][0] (the initial buy state itself) when t-1 < 0.
                 * This is equivalent to saying: for the very first transaction,
                 * the profit before buying is 0 (we start with nothing).
                 * Using dp[i-1][0] (which holds -prices[0]) as the guard is a
                 * subtlety — it effectively means: "either we bought at day 0,
                 * or we don't buy at all yet." The max handles this correctly.
                 */
                if (t % 2 == 0) {
                    dp[i][t] = max(
                        dp[i - 1][t],                              // Do nothing today (stay in buy state)
                        -prices[i] + dp[i - 1][t - 1 >= 0 ? t - 1 : 0] // Buy today
                    );
                }

                /*
                 * ODD t → SELL STATE: we hold a stock and need to sell to complete a transaction.
                 *
                 * Option A: Stay in sell state → dp[i-1][t] (don't sell today, hold stock)
                 * Option B: Sell today → +prices[i] + dp[i-1][t-1]
                 *           dp[i-1][t-1] is the previous BUY state (t-1 is even),
                 *           which already accounts for the cost of buying.
                 *
                 * Note: t-1 is guaranteed >= 0 when t is odd (t >= 1), so no guard needed.
                 */
                else {
                    dp[i][t] = max(
                        dp[i - 1][t],               // Do nothing today (still holding)
                        prices[i] + dp[i - 1][t - 1] // Sell today, complete this transaction
                    );
                }
            }
        }

        /*
         * FINAL ANSWER:
         * dp[n-1][2*k-1] = max profit at the last day in the final SELL state.
         * This captures the best outcome after at most k complete transactions.
         */
        return dp[n - 1][2 * k - 1];
    }
};