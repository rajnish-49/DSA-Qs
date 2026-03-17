/*
 * ============================================================
 * PROBLEM: Best Time to Buy and Sell Stock II (LeetCode 122)
 * ============================================================
 *
 * PROBLEM STATEMENT (in simple terms):
 * -------------------------------------
 * You are given an array `prices` where prices[i] is the price
 * of a stock on day i. You want to maximize your TOTAL profit
 * by buying and selling the stock as many times as you want.
 *
 * CONSTRAINT: You may not hold more than one share at a time.
 * You must SELL before you BUY again. No simultaneous holdings.
 *
 * INPUT:  prices[] → array of integers, prices[i] = stock price on day i
 * OUTPUT: maximum TOTAL profit across ALL transactions combined
 *
 * EXAMPLE:
 *   prices = [7, 1, 5, 3, 6, 4]
 *   Buy day 1 (price=1), sell day 2 (price=5) → profit = 4
 *   Buy day 3 (price=3), sell day 4 (price=6) → profit = 3
 *   Total = 7
 *
 * ============================================================
 * HOW IS THIS DIFFERENT FROM STOCK I (LeetCode 121)?
 * ============================================================
 *
 * STOCK I  → Exactly ONE transaction allowed (one buy, one sell)
 *            Goal: find the single best buy-sell window
 *            Answer: max(prices[j] - prices[i]) for j > i
 *
 * STOCK II → UNLIMITED transactions allowed
 *            Goal: capture EVERY profitable price upswing
 *            Answer: sum of ALL positive day-over-day differences
 *
 * This single difference — one vs unlimited transactions — has a
 * profound impact on BOTH the DP transitions AND the final code:
 *
 *  ┌─────────────────┬──────────────────────┬──────────────────────┐
 *  │                 │     STOCK I           │     STOCK II         │
 *  ├─────────────────┼──────────────────────┼──────────────────────┤
 *  │ Transactions    │ Exactly 1             │ Unlimited            │
 *  ├─────────────────┼──────────────────────┼──────────────────────┤
 *  │ Buy transition  │ dp[i][1] =            │ dp[i][1] =           │
 *  │                 │ max(hold, -prices[i]) │ max(hold,            │
 *  │                 │                       │ notHold - prices[i]) │
 *  ├─────────────────┼──────────────────────┼──────────────────────┤
 *  │ Why different?  │ Buying resets profit  │ Buying carries ALL   │
 *  │                 │ to -prices[i] because │ prior profits forward│
 *  │                 │ no prior sell allowed │ because prior sells  │
 *  │                 │ before this buy       │ ARE allowed          │
 *  ├─────────────────┼──────────────────────┼──────────────────────┤
 *  │ Sell transition │ IDENTICAL in both     │ IDENTICAL in both    │
 *  │                 │ dp[i][0] =            │ dp[i][0] =           │
 *  │                 │ max(notHold,          │ max(notHold,         │
 *  │                 │ hold + prices[i])     │ hold + prices[i])    │
 *  ├─────────────────┼──────────────────────┼──────────────────────┤
 *  │ Greedy?         │ Track running minimum │ Collect every upward │
 *  │                 │ price seen so far     │ day-over-day tick    │
 *  └─────────────────┴──────────────────────┴──────────────────────┘
 *
 * THE ONE LINE THAT CHANGES EVERYTHING:
 * ----------------------------------------
 * Stock I  buy: dp[i][1] = max(dp[i-1][1],  -prices[i])
 *                                            ↑
 *                             No prior profit — this is a fresh start.
 *                             Buying here = first and only transaction.
 *
 * Stock II buy: dp[i][1] = max(dp[i-1][1],  dp[i-1][0] - prices[i])
 *                                            ↑
 *                             Carry forward ALL profit from prior sells.
 *                             Buying here = just another in a chain of trades.
 *
 * ============================================================
 * FIRST PRINCIPLES & INTUITION BUILDING
 * ============================================================
 *
 * OBSERVATION 1 — Unlimited Transactions = Capture Every Upswing:
 * ----------------------------------------------------------------
 *   Since there's no cap on transactions, we are not looking for
 *   a single best window. We want to participate in EVERY profitable
 *   price movement — every day the price rises, we want a piece of it.
 *
 * OBSERVATION 2 — Why Brute Force Fails:
 * ----------------------------------------
 *   Trying every combination of buy/sell sequences across n days
 *   is exponential — O(2^n). Even for n=30, that's over a billion
 *   combinations. We need a smarter structure.
 *
 * OBSERVATION 3 — State Machine View (Two States Per Day):
 * ---------------------------------------------------------
 *   Just like Stock I, on any given day i we are in exactly one
 *   of two states:
 *
 *     State 0 → NOT holding stock
 *               (free to buy, or just watching the market)
 *     State 1 → HOLDING stock
 *               (bought on some prior day, haven't sold yet)
 *
 *   The key question: what's the MAXIMUM profit achievable by
 *   the END of day i in each state?
 *
 * OBSERVATION 4 — DP State Definition & Transitions:
 * ----------------------------------------------------
 *   dp[i][0] = max profit on day i while NOT holding stock
 *   dp[i][1] = max profit on day i while HOLDING stock
 *
 *   TRANSITION — "not holding" on day i:
 *   ┌─────────────────────────────────────────────────────────┐
 *   │  dp[i][0] = max(dp[i-1][0],                             │
 *   │                 dp[i-1][1] + prices[i])                 │
 *   │             ↑ rest (stayed not-holding)                 │
 *   │                              ↑ sold today               │
 *   └─────────────────────────────────────────────────────────┘
 *
 *   TRANSITION — "holding" on day i:
 *   ┌─────────────────────────────────────────────────────────┐
 *   │  dp[i][1] = max(dp[i-1][1],                             │
 *   │                 dp[i-1][0] - prices[i])                 │
 *   │             ↑ rest (kept holding)                       │
 *   │                              ↑ bought today             │
 *   │                                                         │
 *   │  dp[i-1][0] - prices[i] means:                         │
 *   │  "Take ALL profit from prior sells, then subtract the   │
 *   │   cost of buying today." This is what makes Stock II    │
 *   │   different — prior profits CARRY FORWARD into new buys.│
 *   └─────────────────────────────────────────────────────────┘
 *
 *   BASE CASES (Day 0):
 *     dp[0][0] = 0           → did nothing on day 0
 *     dp[0][1] = -prices[0]  → bought on day 0, cash balance negative
 *
 *   FINAL ANSWER: dp[n-1][0]
 *   End without holding stock — unsold shares generate no profit.
 *
 * OBSERVATION 5 — Space Optimization:
 * -------------------------------------
 *   Day i only depends on day i-1, so we can replace the full
 *   n×2 table with two rolling scalar variables: hold and notHold.
 *   Space drops from O(n) to O(1). Logic stays identical.
 *
 * OBSERVATION 6 — The Greedy Insight (Why It Works Here):
 * ---------------------------------------------------------
 *   With unlimited transactions, any multi-day gain can be
 *   decomposed into a sum of consecutive daily gains:
 *
 *   Buy day 1, sell day 4:
 *     profit = prices[4] - prices[1]
 *            = (prices[2]-prices[1]) + (prices[3]-prices[2]) + (prices[4]-prices[3])
 *            = sum of DAILY differences
 *
 *   So instead of finding optimal windows, just collect every
 *   positive daily difference. Negative or zero days = no trade.
 *   This greedy is ONLY valid when transactions are unlimited —
 *   it does NOT apply to Stock I where you must pick one window.
 *
 * ============================================================
 */


/* ============================================================
 * APPROACH 1: DP — Full 2D Table
 * Time: O(n)  |  Space: O(n)
 *
 * Most explicit form. Stores the best profit for both states
 * (holding / not-holding) across every single day.
 * Best for understanding the structure before optimizing.
 *
 * The ONLY line that differs from Stock I is the buy transition:
 *   Stock I:  dp[i][1] = max(dp[i-1][1], -prices[i])
 *   Stock II: dp[i][1] = max(dp[i-1][1], dp[i-1][0] - prices[i])
 *                                         ↑
 *                         Prior profit from earlier sells is carried in.
 * ============================================================ */
class Solution {
public:
    int maxProfit(vector<int>& prices) {

        int n = prices.size();

        // dp[i][0] = max profit achievable by end of day i, NOT holding stock
        // dp[i][1] = max profit achievable by end of day i, HOLDING stock
        //
        // Note: dp[i][1] CAN be positive here (unlike Stock I where it's always ≤ 0)
        // because it includes profit from previous completed transactions.
        vector<vector<int>> dp(n, vector<int>(2));

        // --- BASE CASES: Day 0 ---
        // If we choose NOT to buy on day 0: zero activity, zero profit.
        dp[0][0] = 0;
        // If we choose TO buy on day 0: we spent prices[0].
        // No prior profit exists, so balance is simply negative.
        dp[0][1] = -prices[0];

        // --- FILL DP TABLE: Day 1 through Day n-1 ---
        for(int i = 1; i < n; i++) {

            // TRANSITION — "Not holding stock" on day i:
            // Path A: We weren't holding yesterday → do nothing → dp[i-1][0]
            // Path B: We were holding yesterday and SELL today → dp[i-1][1] + prices[i]
            //         (cash increases by today's sell price)
            // Pick whichever path yields more total profit.
            dp[i][0] = max(dp[i-1][0], dp[i-1][1] + prices[i]);

            // TRANSITION — "Holding stock" on day i:
            // Path A: We were already holding yesterday → keep it → dp[i-1][1]
            // Path B: We were NOT holding yesterday and BUY today
            //         → dp[i-1][0] - prices[i]
            //
            // CRITICAL DIFFERENCE FROM STOCK I:
            // Stock I  uses: max(dp[i-1][1], -prices[i])
            //   → buying resets profit to -prices[i] (no prior sell allowed)
            // Stock II uses: max(dp[i-1][1], dp[i-1][0] - prices[i])
            //   → buying ADDS to prior profit (prior sells already happened)
            //
            // dp[i-1][0] carries forward all earnings from previous transactions.
            // Subtracting prices[i] then reflects the cost of this new purchase.
            dp[i][1] = max(dp[i-1][1], dp[i-1][0] - prices[i]);
        }

        // Return best profit when NOT holding on the final day.
        // Being in the "holding" state on day n-1 means we never sold —
        // that stock is worthless to us, so state 0 always wins.
        return dp[n-1][0];
    }
};


/* ============================================================
 * APPROACH 2: DP — Space Optimized (Rolling Variables)
 * Time: O(n)  |  Space: O(1)
 *
 * Replace the n×2 table with just two scalars.
 * Each variable "rolls forward" — it always represents the
 * best profit achievable in that state up to the current day.
 *
 * We must snapshot BOTH old values before updating either,
 * because each update formula depends on the other variable's
 * OLD value. Updating one first would corrupt the other.
 * ============================================================ */
class Solution {
public:
    int maxProfit(vector<int>& prices) {

        // Rolling equivalents of dp[i][1] and dp[i][0].
        // Initialized to day 0 base case values.
        int hold    = -prices[0]; // Best profit while currently HOLDING stock
                                  // (day 0: just bought, spent prices[0])
        int notHold = 0;          // Best profit while NOT holding stock
                                  // (day 0: did nothing, zero profit)

        for (int i = 1; i < prices.size(); i++) {

            // SNAPSHOT both old values before any update.
            // This is the rolling-variable equivalent of reading
            // dp[i-1][0] and dp[i-1][1] from the table.
            // Without snapshotting, the second assignment would
            // accidentally use today's already-updated notHold
            // instead of yesterday's — a silent correctness bug.
            int prevNotHold = notHold;
            int prevHold    = hold;

            // UPDATE "not holding" for today:
            // Option A: stay not-holding → keep prevNotHold
            // Option B: sell today → prevHold + prices[i]
            notHold = max(prevNotHold, prevHold + prices[i]);

            // UPDATE "holding" for today:
            // Option A: keep holding → keep prevHold
            // Option B: buy today → prevNotHold - prices[i]
            //
            // Stock II KEY: prevNotHold carries all prior sell profits.
            // This is what allows chaining multiple transactions.
            // Compare to Stock I where this line is: max(prevHold, -prices[i])
            // — there, buying always resets to a fresh -prices[i] with no history.
            hold = max(prevHold, prevNotHold - prices[i]);
        }

        // notHold = best profit when ending without any stock on the last day.
        return notHold;
    }
};


/* ============================================================
 * APPROACH 3: Greedy — Collect Every Upward Price Tick
 * Time: O(n)  |  Space: O(1)
 *
 * The most elegant reduction. Works ONLY because transactions
 * are unlimited — this approach is IMPOSSIBLE for Stock I.
 *
 * MATHEMATICAL FOUNDATION:
 * Any gain from buying on day i and selling on day k equals
 * the sum of all consecutive daily gains between i and k:
 *
 *   prices[k] - prices[i]
 *   = (prices[i+1]-prices[i]) + (prices[i+2]-prices[i+1]) + ... + (prices[k]-prices[k-1])
 *
 * So: capturing each positive daily difference individually
 * is IDENTICAL to finding the optimal buy/sell windows.
 * We never need to identify actual buy/sell days — just accumulate
 * positive differences as we scan left to right.
 *
 * WHY DOESN'T THIS WORK FOR STOCK I?
 * Stock I allows only ONE transaction. We can't collect multiple
 * daily ticks separately — we're forced to commit to a single
 * contiguous [buy day, sell day] window. The greedy tick-collection
 * implicitly makes many tiny trades, which Stock I forbids.
 * ============================================================ */
class Solution {
public:
    int maxProfit(vector<int>& prices) {

        // Accumulates total profit from all profitable day-over-day moves.
        // Each "upward tick" we collect here corresponds to a micro-transaction
        // that is mathematically equivalent to an optimal buy/sell window.
        int profit = 0;

        // Start at day 1 so we can compare today's price with yesterday's.
        for (int i = 1; i < prices.size(); i++) {

            // If today is higher than yesterday, we could have bought
            // yesterday and sold today for a gain. Collect this gain.
            //
            // If today is lower or equal, no trade would be profitable
            // for this specific day-over-day window — skip it (add nothing).
            //
            // The beauty: this single condition implicitly handles ALL
            // optimal buy/sell windows. We never explicitly track buy days.
            if (prices[i] > prices[i-1]) {
                profit += prices[i] - prices[i-1];
            }
        }

        // Sum of all positive daily movements = maximum achievable profit.
        // If prices only fell, profit correctly stays 0 (no trades made).
        return profit;
    }
};