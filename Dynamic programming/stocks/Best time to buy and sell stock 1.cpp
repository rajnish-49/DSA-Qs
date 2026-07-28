/*
 * ============================================================
 * PROBLEM: Best Time to Buy and Sell Stock (LeetCode 121)
 * ============================================================
 *
 * PROBLEM STATEMENT (in simple terms):
 * -------------------------------------
 * You are given an array `prices` where prices[i] is the price
 * of a stock on day i. You want to maximize profit by choosing
 * EXACTLY ONE day to buy and a LATER day to sell.
 * You cannot sell before you buy (j > i is mandatory).
 * If no profit is possible (prices only fall), return 0.
 *
 * INPUT:  prices[] → array of integers, prices[i] = stock price on day i
 * OUTPUT: maximum profit from a SINGLE buy-sell transaction
 *
 * GOAL: Find max(prices[j] - prices[i]) for all valid pairs where j > i
 *
 * EXAMPLE:
 *   prices = [7, 1, 5, 3, 6, 4]
 *   Buy on day 1 (price=1), sell on day 4 (price=6) → profit = 5
 *   This is the best single transaction possible.
 *
 *   prices = [7, 6, 4, 3, 1]
 *   Prices only fall → no profitable trade → return 0
 *
 * ============================================================
 * FIRST PRINCIPLES & INTUITION BUILDING
 * ============================================================
 *
 * OBSERVATION 1 — Brute Force (Why It Fails):
 * ---------------------------------------------
 *   The naive approach tries every possible (buy day, sell day) pair
 *   where buy day < sell day, and tracks the maximum difference.
 *   That's roughly n*(n-1)/2 pairs → O(n²) time.
 *   For n = 100,000 that's 5 billion operations — far too slow.
 *
 * OBSERVATION 2 — The Core Insight (What Drives Maximum Profit):
 * ---------------------------------------------------------------
 *   For any sell day j, the best possible profit is:
 *       prices[j] - (cheapest price seen on any day before j)
 *
 *   This means we don't need to try all buy days for each sell day.
 *   If we scan left to right and remember the MINIMUM price seen so
 *   far, we can instantly compute the best profit for each sell day.
 *   This collapses the O(n²) search into a single O(n) pass.
 *
 * OBSERVATION 3 — DP Framing (State Machine View):
 * --------------------------------------------------
 *   On any given day i, we are in EXACTLY one of two states:
 *
 *     State 0 → NOT holding stock
 *               (either never bought, or already sold)
 *     State 1 → HOLDING stock
 *               (bought on some earlier day, haven't sold yet)
 *
 *   At each day, we choose the action that maximizes our cash balance.
 *
 *   TRANSITIONS:
 *   ┌─────────────────────────────────────────────────────────────┐
 *   │  notHolding[i] = max(notHolding[i-1],                       │
 *   │                      holding[i-1] + prices[i])              │
 *   │                  ↑ rest (did nothing)  ↑ sold today         │
 *   │                                                             │
 *   │  holding[i]    = max(holding[i-1],                          │
 *   │                      -prices[i])                            │
 *   │                  ↑ rest (kept stock)   ↑ bought today fresh │
 *   └─────────────────────────────────────────────────────────────┘
 *
 *   CRITICAL DIFFERENCE FROM STOCK II:
 *   In Stock II (unlimited transactions), buying today would be:
 *       notHolding[i-1] - prices[i]   ← carry forward prior profits
 *   Here, since only ONE transaction is allowed, buying always resets
 *   the cash basis to -prices[i] with NO prior profit carried forward.
 *   This ensures we never count a "second buy" after a prior sell.
 *
 *   BASE CASES (Day 0 — before the loop starts):
 *     dp[0][0] = 0           → did nothing on day 0, zero profit
 *     dp[0][1] = -prices[0]  → bought on day 0, cash is now negative
 *
 *   FINAL ANSWER: dp[n-1][0]
 *   We must end in state 0 (not holding) to have realized any profit.
 *   Holding stock on the last day means we never sold → no profit.
 *
 * OBSERVATION 4 — Space Optimization (Rolling Variables):
 * ---------------------------------------------------------
 *   Each day's state depends ONLY on the previous day's state.
 *   We don't need to store all n rows of the DP table —
 *   just two variables that "roll forward" with each iteration.
 *   This reduces space from O(n) → O(1) with identical logic.
 *
 * OBSERVATION 5 — Greedy Simplification (Final Reduction):
 * ----------------------------------------------------------
 *   In the space-optimized DP, notice what `holding` simplifies to:
 *       holding = max(-prices[0], -prices[1], ..., -prices[i])
 *               = -(minimum price seen so far)
 *
 *   Substituting this into the `notHolding` transition:
 *       notHolding = max(notHolding, holding + prices[i])
 *                  = max(notHolding, prices[i] - minPriceSoFar)
 *
 *   This IS the greedy approach — just tracking minPrice and maxProfit.
 *   All three approaches below are mathematically identical.
 *   They differ only in how explicitly the DP structure is expressed.
 *
 * ============================================================
 */


/* ============================================================
 * APPROACH 1: DP — Full 2D Table
 * Time: O(n)  |  Space: O(n)
 *
 * The most explicit formulation. Stores the optimal cash balance
 * for BOTH states (holding / not holding) across ALL days.
 * Best for understanding the DP structure before optimizing.
 * ============================================================ */
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();

        // dp[i][0] = maximum cash balance on day i while NOT holding stock
        // dp[i][1] = maximum cash balance on day i while HOLDING stock
        //
        // "Cash balance" here means total profit earned so far.
        // dp[i][1] is always ≤ 0 because buying costs money with no
        // prior profit to offset it (single transaction constraint).
        vector<vector<int>> dp(n, vector<int>(2));

        // --- BASE CASES: Day 0 ---
        // We haven't seen any prices yet except prices[0].
        // If we chose NOT to buy on day 0: profit = 0, nothing happened.
        dp[0][0] = 0;
        // If we chose TO buy on day 0: we spent prices[0], balance is negative.
        // This is the starting point for the "holding" state.
        dp[0][1] = -prices[0];

        // --- FILL DP TABLE: Day 1 to Day n-1 ---
        for(int i = 1; i < n; i++) {

            // TRANSITION — "Not holding stock" on day i:
            // We arrive here via one of two paths:
            //   Path A: We weren't holding yesterday either → dp[i-1][0] (do nothing)
            //   Path B: We were holding yesterday and SELL today at prices[i]
            //           → dp[i-1][1] + prices[i]
            // We pick whichever path leaves us with more cash.
            dp[i][0] = max(dp[i-1][0], dp[i-1][1] + prices[i]);

            // TRANSITION — "Holding stock" on day i:
            // We arrive here via one of two paths:
            //   Path A: We were already holding yesterday → dp[i-1][1] (do nothing)
            //   Path B: We BUY fresh today → cash balance = -prices[i]
            //
            // KEY: Unlike Stock II, buying today does NOT add dp[i-1][0] as a base.
            // That would allow "sell then buy again" (two transactions).
            // Resetting to -prices[i] enforces the single-transaction constraint —
            // buying today means this is our FIRST and ONLY buy.
            dp[i][1] = max(dp[i-1][1], -prices[i]);
        }

        // The answer is the best cash balance on the last day WITHOUT holding stock.
        // If we're still holding on day n-1, we never sold → profit = 0 at best,
        // and dp[n-1][0] will correctly reflect that (it's initialized to 0).
        return dp[n-1][0];
    }
};


/* ============================================================
 * APPROACH 2: DP — Space Optimized (Rolling Variables)
 * Time: O(n)  |  Space: O(1)
 *
 * Since each day depends only on the PREVIOUS day's two values,
 * the full n×2 table is wasteful. We keep just two scalars and
 * update them in lockstep, day by day.
 *
 * The "prev" snapshots are essential — they freeze yesterday's
 * values before we overwrite them, preventing cross-contamination
 * between the two transitions within the same iteration.
 * ============================================================ */
class Solution {
public:
    int maxProfit(vector<int>& prices) {

        // These are the rolling equivalents of dp[i][0] and dp[i][1].
        // Initialized to base case values for day 0 (same as Approach 1).
        int profitWithoutStock = 0;        // Best cash while NOT holding (starts at 0)
        int profitWithStock = -prices[0];  // Best cash while HOLDING (starts negative:
                                           // we assume we bought on day 0)

        for (int i = 1; i < prices.size(); i++) {

            // SNAPSHOT: Freeze today's "yesterday" values before overwriting.
            // If we updated profitWithoutStock first and then used it in the
            // profitWithStock transition, we'd be using TODAY's value instead
            // of yesterday's — a subtle but critical bug.
            int prevProfitWithoutStock = profitWithoutStock;
            int prevProfitWithStock    = profitWithStock;

            // UPDATE "not holding" state for today:
            // Option A: stay not-holding (carry yesterday's balance forward)
            // Option B: sell today — take yesterday's holding balance + today's price
            profitWithoutStock = max(prevProfitWithoutStock,
                                     prevProfitWithStock + prices[i]);

            // UPDATE "holding" state for today:
            // Option A: keep holding (carry yesterday's holding balance forward)
            // Option B: buy fresh today — reset basis to -prices[i]
            //           (no accumulated profit from a prior sell, single transaction only)
            profitWithStock = max(prevProfitWithStock,
                                  -prices[i]);
        }

        // Best profit when we end the last day without holding any stock.
        // If prices never rose, profitWithoutStock stays 0 (correct — no trade made).
        return profitWithoutStock;
    }
};


/* ============================================================
 * APPROACH 3: Greedy — Track Running Minimum Price
 * Time: O(n)  |  Space: O(1)
 *
 * The most concise form. Directly implements the mathematical
 * reduction derived in Observation 5 above:
 *
 *   profitWithStock   → simplifies to → -minPrice
 *   profitWithoutStock → simplifies to → maxProfit
 *
 * Instead of thinking in DP states, we ask at each sell day:
 *   "What's the best profit if I sell TODAY?"
 *   Answer: prices[i] - (minimum price seen so far)
 *
 * Track the global best of these across all sell days.
 * ============================================================ */
class Solution {
public:
    int maxProfit(vector<int>& prices) {

        // The lowest stock price encountered from day 0 up to day i-1.
        // This is the best possible buy price for any sell day we haven't
        // reached yet. Directly equivalent to -profitWithStock in Approach 2.
        int minPrice = prices[0];

        // The best profit discovered so far across all examined sell days.
        // Starts at 0: if no profitable sell day exists, we return 0.
        int maxProfit = 0;

        for (int i = 1; i < prices.size(); i++) {

            // COMPUTE today's profit assuming we sell today:
            //   prices[i] - minPrice = profit from buying at the historical
            //   low and selling right now.
            // If this beats our current best, update maxProfit.
            // We check this BEFORE updating minPrice to ensure the buy day
            // always strictly precedes the sell day (enforces j > i).
            maxProfit = max(maxProfit, prices[i] - minPrice);

            // UPDATE the running minimum for future sell days.
            // If today's price is lower than anything seen before,
            // future sell days should use today as their buy point.
            // Order matters: this must come AFTER the profit check above.
            minPrice = min(minPrice, prices[i]);
        }

        // If prices only decreased, no trade was ever profitable,
        // and maxProfit correctly remains 0.
        return maxProfit;
    }
};