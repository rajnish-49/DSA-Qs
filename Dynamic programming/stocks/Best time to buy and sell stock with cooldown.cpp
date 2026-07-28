/*
 * ============================================================
 * PROBLEM: Best Time to Buy and Sell Stock with Cooldown
 * ============================================================
 *
 * PROBLEM STATEMENT (in simple terms):
 * -------------------------------------
 * You are given an array `prices` where prices[i] is the price
 * of a stock on day i. You want to maximize your profit by buying
 * and selling stocks, but with ONE important constraint:
 *
 *   After you SELL a stock, you must wait ONE day (cooldown)
 *   before you can buy again.
 *
 * You may only hold AT MOST ONE stock at a time.
 *
 * INPUT:
 *   - prices[]: an array of integers representing daily stock prices
 *
 * OUTPUT:
 *   - A single integer: the maximum profit achievable
 *
 * EXAMPLE:
 *   prices = [1, 2, 3, 0, 2]
 *   Buy on day 0 (price=1), Sell on day 2 (price=3) → profit = 2
 *   Cooldown on day 3
 *   Buy on day 3 (price=0), Sell on day 4 (price=2) → profit = 2
 *   Total = 4
 *
 * ============================================================
 * CORE INTUITION (shared by all three solutions):
 * ============================================================
 *
 * OBSERVATION 1 — What governs our decisions?
 *   At any point in time, our decision depends entirely on:
 *     (a) Which day we are on (index i)
 *     (b) What "state" we are currently in
 *
 * OBSERVATION 2 — There are exactly 3 meaningful states:
 *   State REST  (0): We do NOT hold a stock, and we are FREE to buy.
 *                    (Either we never bought, or we completed a cooldown)
 *   State HOLD  (1): We ARE HOLDING a stock (bought it on some earlier day).
 *                    We can either sell today or keep holding.
 *   State COOL  (2): We just SOLD, so we are in COOLDOWN.
 *                    We cannot buy today; we must skip to the next day.
 *
 * OBSERVATION 3 — Why brute force fails:
 *   A brute force approach enumerates every possible sequence of
 *   buy/sell/skip actions → exponential O(2^n) time.
 *   We'd re-solve the same (day, state) combinations repeatedly.
 *
 * OBSERVATION 4 — State Transitions (the heart of all three solutions):
 *
 *   From REST  → can stay in REST (do nothing) OR move to HOLD (buy today)
 *   From HOLD  → can stay in HOLD (keep holding) OR move to COOL (sell today)
 *   From COOL  → can ONLY move to REST (cooldown ends, now free to buy)
 *
 *   Visually:
 *
 *              [do nothing]           [keep holding]
 *         REST ──────────→ REST  HOLD ─────────────→ HOLD
 *          │                          │
 *          │ [buy today]              │ [sell today]
 *          ↓                          ↓
 *         HOLD                       COOL
 *                                     │
 *                                     │ [forced skip]
 *                                     ↓
 *                                    REST
 *
 * OBSERVATION 5 — The three solutions are the SAME idea, different forms:
 *   Solution 1 (Top-Down DP):    Recursion + Memoization
 *   Solution 2 (Bottom-Up DP):   Iterative table filling
 *   Solution 3 (Space-Optimized):Collapse the table to 3 variables
 *
 * ============================================================
 */


// ============================================================
// SOLUTION 1: Top-Down DP (Memoized Recursion)
// ============================================================
//
// APPROACH:
//   Define solve(i, state) = max profit achievable from day i
//   onwards, given we are currently in `state`.
//   We recurse forward day by day, exploring all valid choices,
//   and cache every (day, state) result to avoid re-solving.
//
// TIME  COMPLEXITY: O(n)  — n days × 3 states, each computed once
// SPACE COMPLEXITY: O(n)  — dp table + recursion call stack depth
// ============================================================

class Solution {
public:
    // State encoding:
    // 0 → REST: not holding, free to buy
    // 1 → HOLD: currently holding a stock
    // 2 → COOL: in cooldown, must skip this day

    int n;                   // total number of days, cached for convenience
    vector<vector<int>> dp;  // dp[i][state] = max profit from day i in given state
                             // -1 means "not yet computed" (our sentinel value)

    /*
     * solve(prices, i, state):
     *   Returns the maximum profit achievable from day i to day n-1,
     *   given that we are currently in `state`.
     *
     *   At each day, we enumerate all valid choices for the current state,
     *   recurse into the future for each choice, and return the best outcome.
     */
    int solve(vector<int>& prices, int i, int state) {

        // BASE CASE: We've gone past the last day.
        // No more buying or selling is possible → no more profit to gain.
        if (i == n) return 0;

        // MEMOIZATION CHECK: If this (day, state) pair was already solved,
        // return the cached answer immediately to avoid exponential blowup.
        if (dp[i][state] != -1) return dp[i][state];

        // -----------------------------------------------------------
        // STATE 0 (REST): We are free to buy today.
        // -----------------------------------------------------------
        if (state == 0) {
            return dp[i][state] = max(
                // CHOICE 1: Do nothing → stay in REST tomorrow.
                // We pass up today's opportunity to preserve flexibility.
                solve(prices, i + 1, 0),

                // CHOICE 2: Buy today → pay prices[i] (negative profit impact),
                // and transition to HOLD state starting tomorrow.
                // The minus sign captures that buying is a cash outflow.
                -prices[i] + solve(prices, i + 1, 1)
            );
        }

        // -----------------------------------------------------------
        // STATE 1 (HOLD): We are currently holding a stock.
        // -----------------------------------------------------------
        else if (state == 1) {
            return dp[i][state] = max(
                // CHOICE 1: Keep holding → stay in HOLD tomorrow.
                // Useful when today's price isn't the peak yet.
                solve(prices, i + 1, 1),

                // CHOICE 2: Sell today → gain prices[i],
                // but we MUST enter COOLDOWN (state 2) tomorrow.
                prices[i] + solve(prices, i + 1, 2)
            );
        }

        // -----------------------------------------------------------
        // STATE 2 (COOL): We just sold yesterday, so we're in cooldown.
        // -----------------------------------------------------------
        else {
            // No choice here — cooldown forces us to skip today entirely.
            // After this forced skip, we become free to buy (REST) tomorrow.
            return dp[i][state] = solve(prices, i + 1, 0);
        }
    }

    int maxProfit(vector<int>& prices) {
        n = prices.size();

        // Allocate the memoization table: n days × 3 states, all -1 (uncomputed).
        dp.assign(n, vector<int>(3, -1));

        // Start from day 0 in REST state (we begin with no stock, free to buy).
        return solve(prices, 0, 0);
    }
};


// ============================================================
// SOLUTION 2: Bottom-Up DP (Iterative Tabulation)
// ============================================================
//
// APPROACH:
//   Instead of recursing top-down and caching, we BUILD the dp
//   table iteratively from day 0 upward. Each cell dp[i][state]
//   represents: "maximum profit achievable if we are in `state`
//   at the END of day i."
//
//   We seed the table with day 0 base cases, then fill each
//   subsequent day using only the previous day's values.
//
// BASE CASES (Day 0):
//   dp[0][0] = 0          → Started free, did nothing. Profit = 0.
//   dp[0][1] = -prices[0] → Bought on day 0. Profit = -prices[0] (cash outflow).
//   dp[0][2] = 0          → Impossible to be in cooldown on day 0 (can't sell
//                           before buying). 0 is a safe sentinel — this state
//                           will never propagate incorrectly.
//
// TRANSITIONS (day i, derived from day i-1):
//   dp[i][0] = max(dp[i-1][0], dp[i-1][2])          ← REST from REST or from COOL
//   dp[i][1] = max(dp[i-1][1], dp[i-1][0]-prices[i]) ← HOLD by continuing or buying
//   dp[i][2] = dp[i-1][1] + prices[i]                ← COOL only by selling today
//
// FINAL ANSWER: max(dp[n-1][0], dp[n-1][2])
//   We exclude state 1 (HOLD) because holding unsold stock at the
//   very end gives no realized profit — we can't sell after day n-1.
//
// TIME  COMPLEXITY: O(n)  — single pass through n days
// SPACE COMPLEXITY: O(n)  — full n×3 DP table stored
// ============================================================

class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();

        // dp[i][0] = best profit ending day i in REST  (free to buy)
        // dp[i][1] = best profit ending day i in HOLD  (holding stock)
        // dp[i][2] = best profit ending day i in COOL  (cooldown, just sold)
        vector<vector<int>> dp(n, vector<int>(3, 0));

        // -----------------------------------------------------------
        // BASE CASES: Seed the table with known day-0 values.
        // -----------------------------------------------------------

        // Day 0, REST: We haven't done anything yet. Zero profit.
        dp[0][0] = 0;

        // Day 0, HOLD: We decided to buy the stock on the very first day.
        // Our running profit is -prices[0] because we've spent that much cash.
        dp[0][1] = -prices[0];

        // Day 0, COOL: Logically impossible (we'd need to have sold, which
        // requires having bought on a day before day 0). Set to 0 as a safe
        // placeholder; this won't contaminate later transitions.
        dp[0][2] = 0;

        // -----------------------------------------------------------
        // FILL THE TABLE from day 1 to day n-1.
        // Each day's values depend only on the previous day's values.
        // -----------------------------------------------------------
        for (int i = 1; i < n; i++) {

            // REST on day i: We are free to buy.
            // We arrive here from either:
            //   - Being in REST yesterday and doing nothing again today
            //   - Being in COOL yesterday (cooldown just ended, now free)
            // Pick whichever past path left us with more profit.
            dp[i][0] = max(dp[i-1][0], dp[i-1][2]);

            // HOLD on day i: We are holding a stock.
            // We arrive here from either:
            //   - Already holding yesterday and choosing NOT to sell today
            //   - Being in REST yesterday and BUYING today (costs prices[i])
            // The subtraction captures the cash outflow of purchasing.
            dp[i][1] = max(
                dp[i-1][1],
                dp[i-1][0] - prices[i]
            );

            // COOL on day i: We just sold today (entering cooldown).
            // The ONLY way to arrive here is:
            //   - We were HOLDING yesterday AND we sell today at prices[i].
            // No max needed — there is exactly one way to enter cooldown.
            dp[i][2] = dp[i-1][1] + prices[i];
        }

        // -----------------------------------------------------------
        // EXTRACT THE ANSWER from the last day.
        // -----------------------------------------------------------
        // At the end of the last day:
        //   - REST (state 0): We're not holding anything → clean exit.
        //   - COOL (state 2): We just completed a sale → realized profit.
        //   - HOLD (state 1): We're still holding → no realized gain from this.
        // So we return the better of REST and COOL.
        return max(dp[n-1][0], dp[n-1][2]);
    }
};


// ============================================================
// SOLUTION 3: Space-Optimized (Constant Space, 3 Variables)
// ============================================================
//
// APPROACH:
//   The bottom-up DP table has n rows, but each row depends ONLY
//   on the previous row. We don't need to store all n rows —
//   we can collapse the entire table into just 3 variables:
//
//     rest = dp[i][0]  →  best profit if free to buy TODAY
//     hold = dp[i][1]  →  best profit if holding a stock TODAY
//     cool = dp[i][2]  →  best profit if in cooldown TODAY
//
//   On each day, we compute NEW values for all three variables
//   simultaneously using the OLD values. We must use temporaries
//   (new_rest, new_hold, new_cool) so the old values aren't
//   overwritten before we're done using them in this iteration.
//
// WHY TEMPORARIES ARE CRITICAL:
//   If we wrote rest = max(rest, cool) first, then used the
//   updated `rest` to compute hold, we'd be mixing day i and
//   day i-1 values — corrupting the result.
//   Computing all three new values BEFORE updating ensures each
//   new value is derived purely from the PREVIOUS day's state.
//
// INITIALIZATION (mirrors bottom-up day-0 base cases):
//   rest = 0          → Day 0: haven't acted yet, profit = 0
//   hold = -prices[0] → Day 0: bought on day 0, profit = -prices[0]
//   cool = 0          → Day 0: impossible state, safe sentinel = 0
//
// TIME  COMPLEXITY: O(n)  — single pass through prices array
// SPACE COMPLEXITY: O(1)  — only 3 integer variables, no array needed
//
// This is the most production-efficient solution of the three.
// ============================================================

class Solution {
public:
    int maxProfit(vector<int>& prices) {

        // Initialize the three state variables to their day-0 values.
        // These directly mirror the base cases of the bottom-up DP table.

        // rest: max profit when we are free to buy.
        // On day 0, we haven't done anything → profit = 0.
        int rest = 0;

        // hold: max profit when we are holding a stock.
        // On day 0, the only way to hold is to have bought today → profit = -prices[0].
        int hold = -prices[0];

        // cool: max profit when we are in cooldown (just sold).
        // On day 0, impossible to have sold already → safe sentinel = 0.
        int cool = 0;

        // -----------------------------------------------------------
        // ITERATE from day 1 to day n-1, updating all 3 state values.
        // -----------------------------------------------------------
        for (int i = 1; i < prices.size(); i++) {

            // Compute all three new state values using YESTERDAY's values
            // (rest, hold, cool are still the previous day's values here).
            // We must compute all three BEFORE updating any variable.

            // NEW REST: We are free to buy today.
            // Came from: staying in REST yesterday | finishing COOL yesterday.
            // Equivalent to: dp[i][0] = max(dp[i-1][0], dp[i-1][2])
            int new_rest = max(rest, cool);

            // NEW HOLD: We are holding a stock today.
            // Came from: holding since yesterday | was in REST yesterday and buy today.
            // Buying today costs prices[i], reducing our running profit.
            // Equivalent to: dp[i][1] = max(dp[i-1][1], dp[i-1][0] - prices[i])
            int new_hold = max(hold, rest - prices[i]);

            // NEW COOL: We just sold today, entering cooldown.
            // Came from: was holding yesterday and chose to sell today at prices[i].
            // Only one way to enter cooldown, so no max() needed.
            // Equivalent to: dp[i][2] = dp[i-1][1] + prices[i]
            int new_cool = hold + prices[i];

            // Now that ALL new values are computed from the old ones,
            // it's safe to overwrite the variables for the next iteration.
            rest = new_rest;
            hold = new_hold;
            cool = new_cool;
        }

        // -----------------------------------------------------------
        // FINAL ANSWER: best profit at end of last day.
        // -----------------------------------------------------------
        // We cannot profit from still holding stock (state HOLD) at the end,
        // so we take the better of REST (free, no stock) and COOL (just sold).
        // This mirrors: return max(dp[n-1][0], dp[n-1][2])
        return max(rest, cool);
    }
};
```

---

## How the Three Solutions Relate
```
Same transitions, three different implementations:

  solve(i, state) recurses FORWARD         ← Solution 1 (Top-Down)
  dp[i][state] fills FORWARD iteratively   ← Solution 2 (Bottom-Up)
  rest/hold/cool rolls ONE ROW forward     ← Solution 3 (Space-Optimized)

         Time       Space     Best for
Sol 1    O(n)       O(n)      Understanding the logic intuitively
Sol 2    O(n)       O(n)      Tracing through the table step-by-step
Sol 3    O(n)       O(1)      Production code / interviews