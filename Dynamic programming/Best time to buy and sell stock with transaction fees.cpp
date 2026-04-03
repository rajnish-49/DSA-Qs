// ============================================================
// PROBLEM: Best Time to Buy and Sell Stock with Transaction Fee
// ============================================================
//
// PROBLEM STATEMENT (in simple terms):
// -------------------------------------
// You are given an array `prices` where prices[i] is the price
// of a stock on day i. You may complete as many transactions as
// you like (buy one and sell one share of the stock multiple
// times), BUT each time you sell, you must pay a transaction fee.
//
// CONSTRAINTS:
//   - You may NOT hold more than one share at a time.
//   - You must SELL before you BUY again.
//   - Each SELL incurs a cost of `fee`.
//
// INPUT:
//   - prices: array of stock prices over n days
//   - fee: fixed cost charged on every sell transaction
//
// OUTPUT:
//   - Maximum profit achievable
//
// EXAMPLE:
//   prices = [1, 3, 2, 8, 4, 9], fee = 2
//   Output: 8
//   Explanation: Buy at 1, sell at 8 (profit = 8-1-2 = 5)
//                Buy at 4, sell at 9 (profit = 9-4-2 = 3)
//                Total = 8
//
// ============================================================
// APPROACH & INTUITION (First Principles)
// ============================================================
//
// OBSERVATION 1 — Two states at every day:
//   At any given day i, we are in one of exactly two states:
//     (a) We CAN BUY  → we don't currently hold a stock
//     (b) We MUST SELL (or hold) → we currently hold a stock
//   This binary state is the key insight that drives all 4 solutions.
//
// OBSERVATION 2 — Why brute force fails:
//   A naive approach tries every subset of buy/sell days — that's
//   exponential time O(2^n). Even a plain greedy (buy every dip,
//   sell every peak) becomes complicated once the fee changes
//   whether a small profit is worth taking.
//
// OBSERVATION 3 — Optimal Substructure for DP:
//   The maximum profit from day i onwards depends ONLY on:
//     - Which day we are on (i)
//     - Whether we currently hold stock (buy = 0 or 1)
//   This is a classic DP with 2 states per day.
//
// STATE DEFINITION:
//   dp[i][1] = max profit achievable from day i to end,
//              given we are FREE TO BUY (don't hold stock)
//   dp[i][0] = max profit achievable from day i to end,
//              given we currently HOLD a stock (must sell before buying)
//
// TRANSITIONS:
//   If we can buy (state = 1) on day i:
//     Option A: Skip day i → dp[i+1][1]  (still free to buy tomorrow)
//     Option B: Buy today  → -prices[i] + dp[i+1][0]  (spend money, now holding)
//
//   If we hold stock (state = 0) on day i:
//     Option A: Skip day i → dp[i+1][0]  (still holding tomorrow)
//     Option B: Sell today → prices[i] - fee + dp[i+1][1]  (earn money, pay fee, free to buy)
//
// BASE CASE:
//   dp[n][0] = dp[n][1] = 0  (no days left → no profit possible)
//
// ANSWER: dp[0][1]  (start at day 0, free to buy)
//
// The four solutions below implement this exact DP using:
//   1. Top-down recursion + memoization
//   2. Bottom-up tabulation (2D table)
//   3. Space-optimized tabulation (two variables per state)
//   4. Forward-pass intuitive DP
// ============================================================


// ============================================================
// SOLUTION 1: Top-Down Recursion with Memoization
// ============================================================
// We recursively ask: "What's the best profit I can make
// starting from day i, given whether I can buy or not?"
// We cache answers in dp[i][buy] to avoid recomputation.
// Time: O(n), Space: O(n) for memo table + O(n) call stack
// ============================================================

class Solution {
public:
    // dp[i][buy]:
    //   i   = current day index
    //   buy = 1 means we are free to buy; 0 means we currently hold a stock
    //   Value = max profit achievable from day i to end in that state
    vector<vector<int>> dp;

    int solve(vector<int>& prices, int i, int buy, int fee) {

        // BASE CASE: We've processed all days.
        // No more transactions possible → profit = 0.
        if (i == prices.size()) return 0;

        // MEMOIZATION CHECK: If we've already computed the answer
        // for this (day, state) combination, return it directly.
        // This prevents exponential recomputation of overlapping subproblems.
        if (dp[i][buy] != -1) return dp[i][buy];

        int profit;

        if (buy) {
            // STATE: We are FREE TO BUY on day i.
            // We have two choices and we pick whichever gives more profit:
            //   Choice 1: SKIP today — move to day i+1 still free to buy.
            //             No money changes hands.
            //   Choice 2: BUY today — spend prices[i] (hence subtract it),
            //             and move to day i+1 in the "holding" state (buy=0).
            profit = max(
                solve(prices, i + 1, 1, fee),               // skip today
                -prices[i] + solve(prices, i + 1, 0, fee)   // buy today
            );
        } else {
            // STATE: We currently HOLD a stock on day i.
            // We have two choices:
            //   Choice 1: SKIP today — hold the stock, move to i+1 still holding.
            //   Choice 2: SELL today — earn prices[i], but pay the transaction fee.
            //             After selling we are free to buy again (buy=1).
            profit = max(
                solve(prices, i + 1, 0, fee),                    // skip (keep holding)
                prices[i] + solve(prices, i + 1, 1, fee) - fee   // sell today
            );
        }

        // Store the result before returning so future calls with the
        // same (i, buy) can reuse it without recursing again.
        return dp[i][buy] = profit;
    }

    int maxProfit(vector<int>& prices, int fee) {
        int n = prices.size();

        // Initialize memo table with -1 (sentinel for "not yet computed").
        // Dimensions: n days × 2 states (can buy / holding).
        dp.assign(n, vector<int>(2, -1));

        // Start from day 0, with buy=1 (we start free to buy — no stock held).
        return solve(prices, 0, 1, fee);
    }
};


// ============================================================
// SOLUTION 2: Bottom-Up Tabulation (2D DP Table)
// ============================================================
// Instead of recursing forward, we fill the table BACKWARDS
// from the last day to the first. This eliminates recursion
// overhead and makes the dependency order explicit.
//
// We iterate i from n-1 down to 0, using dp[i+1] to fill dp[i].
// Time: O(n), Space: O(n)
// ============================================================

class Solution {
public:
    int maxProfit(vector<int>& prices, int fee) {
        int n = prices.size();

        // dp[i][buy] = max profit from day i to end, given state `buy`.
        // Size is (n+1) × 2 so that dp[n][...] = 0 serves as the base case
        // (no days left → no profit) without needing special boundary checks.
        vector<vector<int>> dp(n + 1, vector<int>(2, 0));

        // Fill the table from the last valid day backwards.
        // We process day i using results already computed for day i+1.
        for (int i = n - 1; i >= 0; i--) {

            // ----- State: buy = 1 (free to buy on day i) -----
            // Best of: skip day i (stay free to buy) OR buy today (transition to holding)
            dp[i][1] = max(
                dp[i + 1][1],                    // skip: carry forward the "free to buy" profit
                -prices[i] + dp[i + 1][0]        // buy: spend prices[i], now we hold (state 0)
            );

            // ----- State: buy = 0 (holding stock on day i) -----
            // Best of: skip day i (keep holding) OR sell today (earn prices[i], pay fee, go free)
            dp[i][0] = max(
                dp[i + 1][0],                         // skip: stay in holding state
                prices[i] - fee + dp[i + 1][1]        // sell: earn money, pay fee, now free to buy
            );
        }

        // Answer: start at day 0 in the "free to buy" state.
        return dp[0][1];
    }
};


// ============================================================
// SOLUTION 3: Space-Optimized Tabulation (Rolling Variables)
// ============================================================
// In Solution 2, dp[i] only depends on dp[i+1].
// So we don't need to store the entire table — just the
// values from the "next" day. We replace the 2D array with
// 4 scalar variables representing dp[i+1][0] and dp[i+1][1].
//
// Time: O(n), Space: O(1)
// ============================================================

class Solution {
public:
    int maxProfit(vector<int>& prices, int fee) {

        // These represent dp[i+1][1] and dp[i+1][0] respectively.
        // Initially i = n (base case): profit from beyond the last day = 0.
        int nextCanBuy  = 0;   // dp[i+1][1]: next day's profit when free to buy
        int nextHolding = 0;   // dp[i+1][0]: next day's profit when holding stock

        // Traverse days from the last to the first (same order as Solution 2).
        for (int i = prices.size() - 1; i >= 0; i--) {

            // Compute current day's "free to buy" value using next day's values.
            // Same logic as dp[i][1] in Solution 2.
            int currCanBuy = max(
                nextCanBuy,                  // skip today (remain free to buy)
                -prices[i] + nextHolding     // buy today (transition to holding)
            );

            // Compute current day's "holding" value using next day's values.
            // Same logic as dp[i][0] in Solution 2.
            int currHolding = max(
                nextHolding,                      // skip today (keep holding)
                prices[i] - fee + nextCanBuy      // sell today (earn, pay fee, go free)
            );

            // Shift the window: current day's values become the "next" for the previous day.
            nextCanBuy  = currCanBuy;
            nextHolding = currHolding;
        }

        // After processing all days back to day 0,
        // nextCanBuy holds dp[0][1] — the answer.
        return nextCanBuy;
    }
};


// ============================================================
// SOLUTION 4: Forward-Pass Intuitive DP
// ============================================================
// Instead of going backwards, we scan LEFT TO RIGHT and track
// two running values:
//   profitNoStock   = best profit so far when NOT holding stock
//   profitWithStock = best profit so far when HOLDING stock
//
// At each day, we consider selling (if holding) or buying (if not).
// This is equivalent to the same DP but framed as "what's the best
// I've achieved up to and including today."
//
// Time: O(n), Space: O(1)
// ============================================================

class Solution {
public:
    int maxProfit(vector<int>& prices, int fee) {

        // profitNoStock:   Max profit achievable up to current day
        //                  while NOT holding any stock.
        //                  Start at 0 — before any transaction, profit is 0.
        int profitNoStock = 0;

        // profitWithStock: Max profit achievable up to current day
        //                  while HOLDING a stock.
        //                  We "buy" on day 0, so we spend prices[0].
        //                  Starting value is -prices[0] (money spent).
        int profitWithStock = -prices[0];

        // Traverse from day 1 onward. Day 0 is already handled by initialization.
        for (int i = 1; i < prices.size(); i++) {

            // Save the old "not holding" value before we potentially update it.
            // We need this to correctly compute profitWithStock for this same day
            // (buying uses yesterday's "not holding" profit, not today's updated one).
            int prevNoStock = profitNoStock;

            // UPDATE "not holding" state for day i:
            // Either we were already not holding (keep as-is),
            // OR we sell today: take our profit-from-holding and add prices[i], minus fee.
            profitNoStock = max(
                profitNoStock,                          // stay: don't transact today
                profitWithStock + prices[i] - fee       // sell: convert stock to cash, pay fee
            );

            // UPDATE "holding" state for day i:
            // Either we were already holding (keep as-is),
            // OR we buy today: deduct prices[i] from our best cash-position before today.
            // We use prevNoStock (not the updated profitNoStock) to avoid using
            // today's sell proceeds to immediately buy on the same day.
            profitWithStock = max(
                profitWithStock,                        // stay: keep holding
                prevNoStock - prices[i]                 // buy: spend prices[i] from cash
            );
        }

        // At the end, the best outcome is when we are NOT holding any stock
        // (selling always makes sense on the last relevant day if profitable).
        return profitNoStock;
    }
};