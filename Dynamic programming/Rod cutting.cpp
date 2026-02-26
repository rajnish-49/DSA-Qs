/*
 * ============================================================
 * ROD CUTTING PROBLEM - DEEP DIVE
 * ============================================================
 *
 * UNDERSTANDING THE PROBLEM FROM SCRATCH:
 * ----------------------------------------
 * You have a rod of length n (n = price.size()).
 * price[i-1] = the fixed market price for a piece of exactly length i.
 * You can cut the rod anywhere, any number of times, in any way you want.
 * Goal: maximize total revenue from selling all the resulting pieces.
 *
 * Example: price = [1, 5, 8, 9, 10, 17, 17, 20], n = 8
 *   Selling as one piece of length 8 → price[7] = 20
 *   Cutting into 2+6 → price[1] + price[5] = 5 + 17 = 22  ← better!
 *   Cutting into 1+1+...8 times → 8 * price[0] = 8 * 1 = 8  ← worse
 *
 * WHY GREEDY FAILS:
 * -----------------
 * Your first instinct might be: "always pick the cut with the best
 * price-per-inch ratio." But this doesn't work because the remaining
 * rod length after a greedy cut might not align well with any good
 * price point. The pieces interact with each other — you need to
 * consider the COMBINATION globally, not locally.
 * Example: price = [3,5,8,9] for lengths 1,2,3,4
 *   Greedy picks length 3 (8/3 ≈ 2.67/inch), leftover = 1 → 8+3 = 11
 *   But two pieces of length 2 → 5+5 = 10, worse here
 *   But length 4 unsplit → 9, worse
 *   Actually here greedy works, but in general it won't — you need DP.
 *
 * ============================================================
 * CORE INSIGHT — WHY THIS IS A RECURSIVE PROBLEM:
 * ============================================================
 *
 * Imagine you're standing at the left end of the rod.
 * You must make your FIRST cut somewhere (or not cut at all).
 * Say you decide the first piece has length `cut` (1 ≤ cut ≤ length).
 * You sell that piece for price[cut-1].
 * Now you're left with a rod of length (length - cut).
 *
 * HERE IS THE KEY REALIZATION:
 * The leftover rod of length (length - cut) is EXACTLY the same problem
 * as the original, just smaller. You want to maximize revenue from it
 * too. This is called "optimal substructure" — the optimal solution to
 * the big problem contains optimal solutions to smaller subproblems.
 *
 * So: bestRevenue(length) = max over all cuts k from 1 to length of:
 *                           price[k-1] + bestRevenue(length - k)
 *
 * This recurrence is the entire algorithm. Everything else is just
 * making it efficient.
 *
 * ============================================================
 * WHY NAIVE RECURSION IS EXPONENTIAL (and why we need memo):
 * ============================================================
 *
 * Without memoization, consider what happens when we call solve(8):
 *   solve(8) tries cut=1 → calls solve(7)
 *   solve(8) tries cut=2 → calls solve(6)
 *   ...
 *   solve(7) tries cut=1 → calls solve(6)   ← solve(6) again!
 *   solve(7) tries cut=2 → calls solve(5)
 *   ...
 *
 * solve(6) gets called from solve(7) AND from solve(8). As n grows,
 * the number of redundant calls explodes. The naive recursion has
 * O(2^n) time complexity — completely unusable for large n.
 *
 * With memoization (top-down DP), we store the answer for each length
 * the FIRST time we compute it. Every subsequent call for that same
 * length is just an O(1) lookup. Since there are only n+1 distinct
 * subproblems (length 0, 1, 2, ..., n), and each takes O(n) work
 * (trying all cuts), total time = O(n²). Massive improvement.
 *
 * The dp[] array serves as a CACHE. dp[i] = best revenue for rod of
 * length i. Initialized to -1 meaning "not yet computed."
 *
 * ============================================================
 * CONNECTION TO UNBOUNDED KNAPSACK:
 * ============================================================
 * This problem is structurally identical to Unbounded Knapsack:
 *   - "Items" are piece lengths 1 to n, each with a value (price).
 *   - "Knapsack capacity" is the rod length.
 *   - Each item (cut length) can be used UNLIMITED times (unbounded).
 *   - We want to maximize total value fitting exactly into capacity n.
 * Recognizing this pattern helps you immediately know the recurrence
 * structure and that O(n²) DP is the right approach.
 */

int solve(int price[], int length, vector<int> &dp) {

    // BASE CASE: Rod of length 0 → nothing left to cut or sell → revenue = 0.
    // Every recursive path eventually reduces `length` to 0 by making cuts,
    // and this is where those paths "bottom out" and start returning values
    // back up the call stack. Without this, recursion would go negative and
    // loop forever.
    if (length == 0) return 0;

    // MEMOIZATION LOOKUP: Before doing any work, check if we've already
    // solved this exact subproblem. dp[length] != -1 means we previously
    // computed and stored the best revenue for a rod of this length.
    // Return it immediately — no need to re-explore all cuts again.
    // This is what makes the algorithm O(n²) instead of O(2^n).
    if (dp[length] != -1) return dp[length];

    // `best` will hold the maximum revenue found across all possible
    // first cuts for a rod of this length. We start at 0 (can't do worse
    // than 0 since prices are positive). As we try each cut, we update
    // this if we find something better.
    int best = 0;

    // TRY EVERY POSSIBLE LENGTH FOR THE FIRST PIECE:
    // `cut` ranges from 1 (minimum piece) to `length` (no cut, sell whole rod).
    // For each value of `cut`, we're asking: "What if I sell a piece of
    // length `cut` first, and then optimally handle the rest?"
    //
    // Why start from 1? A cut of length 0 makes no sense (selling nothing).
    // Why go up to length? cut == length means we don't cut at all — we
    // sell the entire remaining rod as one piece. This must be considered
    // because sometimes the whole rod is worth more unsplit.
    //
    // IMPORTANT: We only decide the FIRST cut here. The recursive call
    // handles all subsequent cuts on the leftover rod. We trust recursion
    // (+ memoization) to make those remaining decisions optimally.
    // This is the "leap of faith" in recursive thinking.
    for (int cut = 1; cut <= length; cut++) {

        // price[cut - 1]: Revenue from selling the first piece of length `cut`.
        //   Array is 0-indexed but lengths are 1-indexed, so length `cut`
        //   corresponds to price[cut-1].
        //
        // solve(price, length - cut, dp): Maximum revenue obtainable from
        //   the leftover rod of length (length - cut). This call either:
        //   (a) returns a memoized answer in O(1) if already computed, or
        //   (b) recursively explores all cuts for that smaller length.
        //
        // Together, this evaluates one complete "strategy" for this rod:
        //   sell a piece of length `cut`, then optimally sell the rest.
        // We keep track of the best strategy seen so far in `best`.
        best = max(best, price[cut - 1] + solve(price, length - cut, dp));
    }

    // STORE THE RESULT before returning.
    // Any future call to solve(..., length, ...) will hit the memo check
    // above and skip all this computation. This is the "dynamic" in
    // dynamic programming — we're building a table of answers as we go.
    dp[length] = best;

    return best;

    // CALL CONVENTION (reminder for the caller):
    // dp should be a vector of size (n+1) initialized to -1.
    // Initial call: solve(price, n, dp) where n = price.size()
    // dp[0] = 0 is handled by the base case, not pre-filled.
}

/*
 * TRACE THROUGH EXAMPLE: price = [1, 5, 8, 9], n = 4
 * -----------------------------------------------------
 * solve(4):
 *   cut=1: price[0] + solve(3) = 1 + solve(3)
 *     solve(3):
 *       cut=1: 1 + solve(2)
 *         solve(2):
 *           cut=1: 1 + solve(1)
 *             solve(1): cut=1: 1+solve(0)=1+0=1 → dp[1]=1, return 1
 *           cut=1 gives: 1+1=2
 *           cut=2: 5 + solve(0) = 5+0 = 5  ← better
 *           dp[2] = 5, return 5
 *       cut=1 gives: 1+5=6
 *       cut=2: 5 + solve(1) = 5+1=6
 *       cut=3: 8 + solve(0) = 8+0=8  ← best
 *       dp[3] = 8, return 8
 *   cut=1 gives: 1+8=9
 *   cut=2: 5 + solve(2) = 5+5=10  ← solve(2) is O(1) lookup now!
 *   cut=3: 8 + solve(1) = 8+1=9
 *   cut=4: 9 + solve(0) = 9+0=9
 *   best = 10, dp[4] = 10, return 10
 *
 * Notice solve(2) and solve(1) are reused without recomputation.
 * For n=8, the savings are even more dramatic.
 *
 * TIME COMPLEXITY:  O(n²) — n subproblems, each doing O(n) work
 * SPACE COMPLEXITY: O(n)  — dp array + O(n) recursion call stack
 */


 class Solution {
  public:
    int cutRod(vector<int> &price) {
        
        int n = price.size();

        // TABULATION (BOTTOM-UP DP):
        // ============================================================
        // Same recurrence as memoization, but we FLIP the direction.
        // Instead of starting at solve(n) and recursing DOWN to smaller
        // subproblems, we start from the smallest subproblem (length=0)
        // and BUILD UP to dp[n] iteratively.
        //
        // WHY THIS WORKS:
        // When we're computing dp[len], we need dp[len-cut] for all
        // cuts 1..len. Since len-cut is always LESS than len, those
        // values are guaranteed to already be filled in the table by
        // the time we need them. No recursion needed — just a lookup.
        // This is the core contract of bottom-up DP: "smaller problems
        // are always solved before larger ones that depend on them."
        //
        // dp[i] = maximum revenue obtainable from a rod of length i.
        // dp[0] = 0 (base case: empty rod, zero revenue) — pre-filled
        // by vector initialization. We fill dp[1], dp[2], ..., dp[n]
        // in order, each building on previously computed values.
        vector<int> dp(n + 1, 0);

        // OUTER LOOP — iterates over every rod length from 1 to n.
        // Think of `len` as: "I am now solving the subproblem: what is
        // the best revenue for a rod of exactly this length?"
        // We solve every possible rod length, not just the final answer,
        // because dp[n] depends on dp[n-1], dp[n-2], ..., dp[0].
        for (int len = 1; len <= n; len++) {
            
            int best = 0; // Best revenue found so far for rod of length `len`

            // INNER LOOP — try every possible first cut for this rod length.
            // Identical logic to the memoized version's for loop.
            // For each cut length (1 to len), we ask: "sell a piece of
            // length `cut`, then take the best possible revenue from the
            // remaining rod of length (len - cut)."
            //
            // The crucial difference from memoization:
            // In memo → dp[len-cut] was computed lazily (only when needed,
            //           via a recursive call that may or may not have been
            //           cached yet).
            // In tabulation → dp[len-cut] is GUARANTEED to be already
            //           computed because len-cut < len, and we've already
            //           completed all iterations where the outer loop
            //           variable was equal to len-cut. Pure O(1) lookup.
            for (int cut = 1; cut <= len; cut++) {
                // price[cut-1]: value of selling the piece of length `cut`
                // dp[len-cut]:  best revenue from leftover rod (already solved)
                best = max(best, price[cut - 1] + dp[len - cut]);
            }

            // Lock in the answer for this rod length.
            // Future iterations (len+1, len+2, ...) will read this value
            // when their own inner loops reach a cut that leaves len behind.
            // Example: when solving len=7, cut=2 → reads dp[5] (set here).
            dp[len] = best;
        }

        // dp[n] now holds the maximum revenue for the full rod of length n.
        // Every subproblem it depended on was solved in the iterations before it.
        return dp[n];

        // TABULATION vs MEMOIZATION — when to prefer which:
        // --------------------------------------------------------
        // Tabulation:
        //   + No recursion → no call stack overhead → no stack overflow risk
        //   + Slightly faster in practice (no function call overhead)
        //   + Memory access pattern is sequential → cache friendly
        //   - Must solve ALL subproblems even if some aren't needed
        //     (doesn't matter here since all lengths 1..n are always needed)
        //
        // Memoization:
        //   + Solves only the subproblems actually reachable from solve(n)
        //   + More natural to write — directly mirrors the recurrence
        //   - Recursion depth = O(n), risks stack overflow for large n
        //   - Random memory access pattern (less cache friendly)
        //
        // For this specific problem, all subproblems are always needed,
        // so tabulation is strictly better. Both are O(n²) time, O(n) space.
    }
};