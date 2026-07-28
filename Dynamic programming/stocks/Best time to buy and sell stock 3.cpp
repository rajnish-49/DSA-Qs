/*
 * ============================================================
 * PROBLEM: Best Time to Buy and Sell Stock III (LeetCode 123)
 * ============================================================
 *
 * PROBLEM STATEMENT (in simple terms):
 * -------------------------------------
 * You are given an array `prices` where prices[i] is the price of a stock
 * on day i. You may complete AT MOST 2 transactions (buy + sell = 1 transaction).
 * You CANNOT hold more than one stock at a time (must sell before buying again).
 * Find the maximum profit you can achieve.
 *
 * INPUT:
 *   - prices: a vector of integers, prices[i] = stock price on day i
 *
 * OUTPUT:
 *   - A single integer: the maximum profit achievable with at most 2 transactions
 *
 * GOAL (intuitively):
 *   - Pick at most 2 non-overlapping intervals [buy1,sell1] and [buy2,sell2]
 *     such that (prices[sell1]-prices[buy1]) + (prices[sell2]-prices[buy2])
 *     is maximized. You can also do 1 transaction or 0 if no profit is possible.
 *
 * ============================================================
 * APPROACH — BUILDING INTUITION FROM FIRST PRINCIPLES
 * ============================================================
 *
 * OBSERVATION 1: State depends on three things at every day
 *   At any point in time, what matters is:
 *     (a) Which day are we on?            → index i
 *     (b) Are we currently holding stock? → buy/sell flag
 *     (c) How many transactions remain?   → capacity (0, 1, or 2)
 *   These three facts fully describe our situation — no other history matters.
 *
 * OBSERVATION 2: Why brute force fails
 *   Trying every subset of buy/sell pairs is O(n^4) — too slow for large n.
 *   We'd recompute overlapping subproblems exponentially.
 *
 * OBSERVATION 3: Overlapping subproblems → Dynamic Programming
 *   "What's the best profit from day i onward, given I can buy/sell and
 *    have `cap` transactions left?" — this question recurs with the SAME
 *    (i, buy, cap) triple many times. Memoization collapses repeated work.
 *
 * KEY DP STATE DEFINITION:
 *   dp[i][buy][cap] = maximum profit achievable from day i to day n-1,
 *                     where `buy=1` means we CAN buy next (not holding),
 *                     `buy=0` means we MUST sell next (currently holding),
 *                     and `cap` = number of complete transactions still allowed.
 *
 * TRANSITIONS:
 *   If buy == 1 (not holding, can buy):
 *     Option A: Skip day i → dp[i+1][1][cap]          (do nothing)
 *     Option B: Buy today  → -prices[i] + dp[i+1][0][cap]  (spend money)
 *
 *   If buy == 0 (holding, must sell eventually):
 *     Option A: Skip day i → dp[i+1][0][cap]          (hold longer)
 *     Option B: Sell today → +prices[i] + dp[i+1][1][cap-1] (earn money, use 1 transaction)
 *
 * BASE CASES:
 *   - i == n: no more days, profit = 0
 *   - cap == 0: no transactions left, profit = 0
 *
 * ============================================================
 * FOUR IMPLEMENTATIONS ARE PROVIDED:
 *   1. Top-down DP (memoization) with [i][buy][cap] state
 *   2. Bottom-up DP with [i][buy][cap] state
 *   3. Top-down DP (memoization) with stage encoding (alternative state design)
 *   4. Bottom-up DP with stage encoding
 * ============================================================
 */


/* ============================================================
 * APPROACH 1: TOP-DOWN MEMOIZATION — [day][holding?][capacity]
 * ============================================================
 * We define a recursive function solve(i, buy, cap) that returns the
 * maximum profit achievable from day i onward.
 *
 * State space: n days × 2 holding states × 3 capacity levels = O(6n)
 * Time:  O(n)  — each state computed exactly once
 * Space: O(n)  — memoization table + recursion stack
 */
class Solution {
public:
    int n;

    /*
     * dp[i][buy][cap]:
     *   i   = current day index
     *   buy = 1 → we do NOT hold stock right now (free to buy)
     *         0 → we DO hold stock right now (must sell before buying again)
     *   cap = how many full transactions (buy+sell pairs) we still may complete
     *         ranges from 0 to 2 (since at most 2 transactions allowed)
     * Initialized to -1 to indicate "not yet computed".
     */
    vector<vector<vector<int>>> dp;

    /*
     * solve(prices, i, buy, cap):
     *   Recursively computes max profit from day i to end,
     *   given current holding state `buy` and remaining capacity `cap`.
     */
    int solve(vector<int>& prices, int i, int buy, int cap) {

        // BASE CASE 1: No more days to trade — no profit possible from here.
        // BASE CASE 2: No transactions remaining — we cannot complete any more
        //              buy-sell pairs, so no profit is achievable.
        if (i == n || cap == 0) return 0;

        // MEMOIZATION CHECK: If we've already solved this exact subproblem,
        // return the cached answer immediately to avoid redundant recursion.
        if (dp[i][buy][cap] != -1) return dp[i][buy][cap];

        int profit;

        if (buy) {
            /*
             * STATE: buy == 1 → We are NOT holding any stock.
             * CHOICE A: Skip today — move to day i+1, still free to buy, cap unchanged.
             *           This is useful when today's price seems high.
             * CHOICE B: Buy today — pay prices[i] now (subtract from profit),
             *           then move to day i+1 in the "holding" state (buy=0), cap unchanged.
             *           Cap is NOT decremented here — a transaction only completes on SELL.
             * We take the maximum of both choices.
             */
            profit = max(
                solve(prices, i + 1, 1, cap),              // skip: don't buy today
                -prices[i] + solve(prices, i + 1, 0, cap)  // buy: pay prices[i], now holding
            );
        } else {
            /*
             * STATE: buy == 0 → We ARE holding stock, waiting to sell.
             * CHOICE A: Hold — don't sell today, continue holding into day i+1, cap unchanged.
             *           Useful when price might rise further.
             * CHOICE B: Sell today — earn prices[i], flip to non-holding state (buy=1),
             *           and decrement cap by 1 because one full transaction just completed.
             * We take the maximum of both choices.
             */
            profit = max(
                solve(prices, i + 1, 0, cap),                   // hold: don't sell today
                prices[i] + solve(prices, i + 1, 1, cap - 1)    // sell: earn prices[i], cap used
            );
        }

        // Store result in memo table before returning, so future calls with the
        // same (i, buy, cap) triple don't recompute this subproblem.
        return dp[i][buy][cap] = profit;
    }

    int maxProfit(vector<int>& prices) {
        n = prices.size();

        // Allocate the 3D memo table: n days × 2 states (holding/not) × 3 cap levels (0,1,2).
        // -1 is our sentinel value meaning "not computed yet".
        dp.assign(n, vector<vector<int>>(2, vector<int>(3, -1)));

        // Start at day 0, free to buy (buy=1), with 2 transactions remaining (cap=2).
        return solve(prices, 0, 1, 2);
    }
};


/* ============================================================
 * APPROACH 2: BOTTOM-UP TABULATION — [day][holding?][capacity]
 * ============================================================
 * Same state space as Approach 1, but filled iteratively from the base cases
 * forward through all days. Avoids recursion overhead entirely.
 *
 * NOTE: Here `buy` convention is FLIPPED vs Approach 1 for this standalone function.
 *       buy=1 → currently HOLDING (opposite of above).
 *       buy=0 → NOT holding.
 *   (This is a common alternate encoding — both are valid.)
 *
 * dp[i][0][cap] = max profit up to day i, NOT holding stock, cap transactions left
 * dp[i][1][cap] = max profit up to day i, HOLDING stock,     cap transactions left
 *
 * Time:  O(n)   Space: O(n)
 */
int maxProfit_bottomup_3d(vector<int>& prices) {
    int n = prices.size();

    // Initialize all states to -1e9 (negative infinity) as placeholder.
    // We will fill valid states from day 0 forward.
    vector<vector<vector<int>>> dp(n, vector<vector<int>>(2, vector<int>(3, -1e9)));

    /*
     * BASE CASES for day 0:
     * dp[0][1][2]: On day 0, holding stock, 2 transactions remaining.
     *   We haven't sold yet, so profit contribution is 0 (the -prices[0] cost
     *   is handled in the "buy" transition of dp[i][0][cap]).
     *   Set to 0 — we haven't realized any profit yet.
     *
     * dp[0][0][2]: On day 0, NOT holding stock, 2 transactions remaining.
     *   Wait — this actually represents having BOUGHT on day 0 (to be consistent
     *   with the transition below where buying flips state from 1→0).
     *   So we record -prices[0] as the cost paid today.
     *
     * Note: cap=1 base cases are implicitly handled during iteration.
     */
    dp[0][1][2] = 0;          // Day 0: not holding (or freshly initialized), no profit
    dp[0][0][2] = -prices[0]; // Day 0: just bought stock, cap=2 still (transaction not complete)

    for (int i = 1; i < n; i++) {
        for (int cap = 1; cap <= 2; cap++) {

            /*
             * TRANSITION for "NOT holding" state (dp[i][1][cap]):
             *   Option A: We were also not holding yesterday → dp[i-1][1][cap]  (did nothing)
             *   Option B: We were holding yesterday and sell today → dp[i-1][0][cap] + prices[i]
             *             Selling completes a transaction, but cap was already "pre-decremented"
             *             when we bought (see buying transition below), so cap stays same here.
             *
             * This represents: max profit by end of day i, currently NOT holding, cap remaining.
             */
            dp[i][1][cap] = max(
                dp[i-1][1][cap],                   // carry forward: didn't trade today
                dp[i-1][0][cap] + prices[i]        // sold today: add today's price
            );

            /*
             * TRANSITION for "holding" state (dp[i][0][cap]):
             *   Option A: We were also holding yesterday → dp[i-1][0][cap]  (didn't sell)
             *   Option B: We were not holding yesterday and BUY today → dp[i-1][1][cap-1] - prices[i]
             *             Buying uses up one transaction slot (cap-1), costs prices[i].
             *             Note: cap is decremented on BUY here (alternative to decrement on sell).
             *
             * This represents: max profit by end of day i, currently HOLDING, cap remaining.
             */
            dp[i][0][cap] = max(
                dp[i-1][0][cap],                   // carry forward: still holding
                dp[i-1][1][cap-1] - prices[i]      // bought today: spend prices[i], use 1 cap
            );
        }
    }

    // Final answer: last day, not holding stock (all positions closed), 2 transactions allowed.
    // We want the state where we've maximized profit and are not holding anything.
    return dp[n-1][1][2];
}


/* ============================================================
 * APPROACH 3: TOP-DOWN MEMOIZATION — STAGE ENCODING
 * ============================================================
 * ALTERNATIVE STATE DESIGN: Instead of tracking (holding, capacity) separately,
 * we encode the trading lifecycle as a single "stage" integer from 0 to 3:
 *
 *   Stage 0: Haven't bought yet (1st buy allowed)       → even stage → BUY action
 *   Stage 1: Holding stock from 1st buy                 → odd stage  → SELL action
 *   Stage 2: Completed 1st transaction, can buy again   → even stage → BUY action
 *   Stage 3: Holding stock from 2nd buy                 → odd stage  → SELL action
 *   Stage 4: Completed 2nd transaction — DONE (return 0)
 *
 * Pattern: even stage = BUY phase, odd stage = SELL phase.
 * This elegantly encodes at-most-2-transactions without a separate cap variable.
 *
 * dp[i][stage] = max profit from day i onward, given we are at lifecycle `stage`.
 * Time: O(n)   Space: O(n)
 */
class Solution2 {
public:
    int n;

    /*
     * dp[i][stage]:
     *   i     = current day index (0 to n-1)
     *   stage = which phase of the trading lifecycle we're in (0 to 4)
     * Initialized to -1 as "not yet computed" sentinel.
     */
    vector<vector<int>> dp;

    /*
     * solve(prices, i, stage):
     *   Returns max profit from day i onward, given the current trading stage.
     */
    int solve(vector<int>& prices, int i, int stage) {

        // BASE CASE 1: Past all days — no more trading possible.
        // BASE CASE 2: stage == 4 means both transactions fully completed.
        //              No more actions permitted; return 0.
        if (i == n || stage == 4) return 0;

        // MEMOIZATION: If this (day, stage) pair was already solved, reuse it.
        if (dp[i][stage] != -1) return dp[i][stage];

        // CHOICE A (always available): Skip today entirely.
        // Move to day i+1 at the same stage — we didn't take any action today.
        // This allows us to wait for a better price.
        int skip = solve(prices, i + 1, stage);

        int act;
        if (stage % 2 == 0) {
            /*
             * EVEN STAGE = BUY PHASE (stages 0 and 2)
             * We are currently not holding stock and may buy.
             * Action: Buy at today's price → subtract prices[i] from profit,
             *         advance to next stage (holding state).
             * After buying, stage becomes odd (sell phase).
             */
            act = -prices[i] + solve(prices, i + 1, stage + 1);
        } else {
            /*
             * ODD STAGE = SELL PHASE (stages 1 and 3)
             * We are currently holding stock and may sell.
             * Action: Sell at today's price → add prices[i] to profit,
             *         advance to next stage (next buy or done).
             * After selling, stage becomes even (or 4 if done).
             */
            act = prices[i] + solve(prices, i + 1, stage + 1);
        }

        // Take the best of skipping vs acting on this day.
        return dp[i][stage] = max(skip, act);
    }

    int maxProfit(vector<int>& prices) {
        n = prices.size();

        // dp table: n days × 5 stages (0..4). Stage 4 is terminal, so 5 slots.
        // -1 means "not yet computed".
        dp.assign(n, vector<int>(5, -1));

        // Begin at day 0, stage 0 (no transactions done, free to buy).
        return solve(prices, 0, 0);
    }
};


/* ============================================================
 * APPROACH 4: BOTTOM-UP TABULATION — STAGE ENCODING
 * ============================================================
 * Same stage encoding as Approach 3, but computed iteratively.
 * We use a compact dp[i][stage] table with 4 stages (0 to 3).
 * Stage 4 (terminal) is represented implicitly by returning 0.
 *
 * dp[i][0] = max profit up to day i, at stage 0 (bought once, currently holding 1st)
 * dp[i][1] = max profit up to day i, at stage 1 (sold once, 1st transaction complete)
 * dp[i][2] = max profit up to day i, at stage 2 (bought twice, currently holding 2nd)
 * dp[i][3] = max profit up to day i, at stage 3 (sold twice, both transactions complete)
 *
 * NOTE: In this bottom-up version, the stage semantics shift slightly:
 *   Stage 0: Holding stock from 1st buy  (profit = -prices when entered)
 *   Stage 1: After 1st sell              (profit = +prices when entered)
 *   Stage 2: Holding stock from 2nd buy  (profit = -prices when entered)
 *   Stage 3: After 2nd sell              (profit = +prices when entered)
 *
 * Time: O(n)   Space: O(n)
 */
int maxProfit_bottomup_stage(vector<int>& prices) {
    int n = prices.size();

    // dp[i][s] = best profit achievable by end of day i at stage s.
    // Initialize to 0 (will be overwritten from base cases).
    vector<vector<int>> dp(n, vector<int>(4, 0));

    /*
     * BASE CASES for day 0:
     * dp[0][0]: We bought on day 0 (1st buy). Cost = prices[0], so profit so far = -prices[0].
     * dp[0][1]: We bought AND sold on day 0 (same day, 1st transaction). Profit = 0.
     *           (Selling same day you bought gives zero profit.)
     * dp[0][2]: We bought twice on day 0 (both buys). Profit so far = -prices[0].
     *           (Buying twice same day — second buy also costs prices[0].)
     * dp[0][3]: We completed both transactions on day 0. Profit = 0.
     *           (Both buy+sell pairs on same day give zero total profit.)
     */
    dp[0][0] = -prices[0]; // Stage 0 entered: paid prices[0] for 1st buy
    dp[0][1] = 0;          // Stage 1 entered: sold same day, net profit = 0
    dp[0][2] = -prices[0]; // Stage 2 entered: bought again same day, net = 0 - prices[0]
    dp[0][3] = 0;          // Stage 3 entered: sold again same day, net = 0

    for (int i = 1; i < n; i++) {

        /*
         * STAGE 0: Currently holding stock from 1st buy.
         * Option A: We were already at stage 0 yesterday → dp[i-1][0]  (still holding, no change)
         * Option B: We just bought today (entering stage 0) → -prices[i]
         *           This represents buying on day i for the first time.
         *           (The "came from not holding" state is implicitly 0, since before stage 0
         *            we had $0 profit, and we now pay prices[i].)
         */
        dp[i][0] = max(dp[i-1][0], -prices[i]);

        /*
         * STAGE 1: Completed 1st transaction (sold once).
         * Option A: We were already at stage 1 yesterday → dp[i-1][1]  (resting, no new trade)
         * Option B: We just sold today from stage 0 → dp[i-1][0] + prices[i]
         *           We held stock (from some earlier buy), now sell at today's price.
         */
        dp[i][1] = max(dp[i-1][1], dp[i-1][0] + prices[i]);

        /*
         * STAGE 2: Currently holding stock from 2nd buy.
         * Option A: We were already at stage 2 yesterday → dp[i-1][2]  (still holding)
         * Option B: We just bought today coming from stage 1 → dp[i-1][1] - prices[i]
         *           We had profit dp[i-1][1] from 1st transaction, now we spend prices[i]
         *           to initiate the 2nd transaction.
         */
        dp[i][2] = max(dp[i-1][2], dp[i-1][1] - prices[i]);

        /*
         * STAGE 3: Completed both transactions (sold twice). This is our final state.
         * Option A: We were already at stage 3 yesterday → dp[i-1][3]  (done trading, rest)
         * Option B: We just completed the 2nd sell today from stage 2 → dp[i-1][2] + prices[i]
         *           We held the 2nd stock (bought at some earlier price), now sell at prices[i].
         */
        dp[i][3] = max(dp[i-1][3], dp[i-1][2] + prices[i]);
    }

    /*
     * FINAL ANSWER: dp[n-1][3]
     * After all n days, the maximum profit when both transactions are complete.
     * Stage 3 is the terminal stage — both buy-sell pairs have been executed.
     * This naturally handles cases with 0 or 1 transaction if they're more profitable
     * (those profits propagate forward into stage 3 via the "rest" options).
     */
    return dp[n-1][3];
}