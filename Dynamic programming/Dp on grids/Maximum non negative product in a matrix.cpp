/*
 * ============================================================================
 * PROBLEM: Maximum Non-Negative Product in a Matrix
 * ============================================================================
 *
 * PROBLEM STATEMENT (in simple terms):
 * -------------------------------------
 * You are given a 2D grid of integers (which may include negative numbers,
 * zero, and positive numbers). Starting from the top-left corner (0,0), you
 * must reach the bottom-right corner (m-1, n-1). At each step, you can only
 * move RIGHT or DOWN (no backtracking, no diagonal moves).
 *
 * Along the path, you multiply together all the values you step on (including
 * the start and end cell). Your goal is to find the MAXIMUM PRODUCT among all
 * possible paths from top-left to bottom-right.
 *
 * If the maximum product is negative, return -1.
 * Otherwise, return the maximum product modulo 1,000,000,007.
 *
 * INPUT:
 *   - grid: an m x n 2D vector of integers
 *           (values can be negative, zero, or positive)
 *
 * OUTPUT:
 *   - The maximum non-negative path product modulo 10^9+7,
 *     or -1 if every possible path yields a negative product.
 *
 * EXAMPLE:
 *   grid = [[-1, -2, -3],
 *            [1,  -1,  1]]
 *   Paths and their products:
 *     (0,0)→(0,1)→(0,2)→(1,2): -1 * -2 * -3 *  1 = -6
 *     (0,0)→(0,1)→(1,1)→(1,2): -1 * -2 * -1 *  1 = -2
 *     (0,0)→(1,0)→(1,1)→(1,2): -1 *  1 * -1 *  1 =  1  ← max non-negative
 *   Answer: 1 % MOD = 1
 *
 * ============================================================================
 * WHY BRUTE FORCE FAILS
 * ============================================================================
 *
 * There are C(m+n-2, m-1) distinct paths in an m×n grid — this grows
 * exponentially. For a 15×15 grid that's already ~40 million paths.
 * Enumerating and computing the product of each path explicitly is far
 * too slow for large inputs.
 *
 * ============================================================================
 * KEY INSIGHT: Why We Must Track Both Maximum AND Minimum
 * ============================================================================
 *
 * If all grid values were positive, tracking only the maximum product at
 * each cell would be sufficient. But negative numbers break this assumption:
 *
 *   Two negatives multiplied together give a POSITIVE.
 *
 * This means the path with the most negative product right now could
 * suddenly become the most positive product if the next cell is negative.
 *
 * Therefore, at every cell we must remember:
 *   - The MAXIMUM product reachable so far (best case for positive values)
 *   - The MINIMUM product reachable so far (worst case, but could flip
 *     to best case upon encountering a negative value)
 *
 * ============================================================================
 * APPROACH: Dynamic Programming with (max, min) Pairs
 * ============================================================================
 *
 * STATE DEFINITION:
 *   dp[i][j] = pair {maxProd, minProd} where:
 *     maxProd — the largest product achievable on any path from (0,0) to (i,j)
 *     minProd — the smallest product achievable on any path from (0,0) to (i,j)
 *
 * TRANSITION LOGIC (the elegant core of this version):
 *   When extending a path to cell (i,j) with value `val`, we multiply `val`
 *   against every candidate product arriving from valid predecessors.
 *   Rather than checking the sign of `val` explicitly, we simply compute ALL
 *   possible products and take the max/min among them:
 *
 *     candidates = {predMax * val, predMin * val}   (for a single predecessor)
 *     new_max = max(candidates)
 *     new_min = min(candidates)
 *
 *   This automatically handles sign flipping:
 *     - If val > 0: predMax * val > predMin * val → max picks predMax * val ✓
 *     - If val < 0: predMax * val < predMin * val → max picks predMin * val ✓
 *     - If val = 0: both products are 0, so max = min = 0 ✓
 *
 *   For interior cells with TWO predecessors (above and left), we have
 *   FOUR candidates total and simply take max/min of all four.
 *
 * BASE CASES:
 *   dp[0][0] = {grid[0][0], grid[0][0]}  — only one cell, product is itself
 *   First column: each cell has only one predecessor (directly above)
 *   First row:    each cell has only one predecessor (directly to the left)
 *
 * CRITICAL NOTE ON MODULO:
 *   We do NOT apply MOD during the DP computation. Applying MOD mid-way
 *   would corrupt the values used in min/max comparisons (since MOD wraps
 *   large numbers to small ones, destroying relative ordering). We apply
 *   MOD only once at the very end, on the confirmed final answer.
 *
 * TIME COMPLEXITY:  O(m × n) — each cell processed exactly once
 * SPACE COMPLEXITY: O(m × n) — dp table storing a pair per cell
 * ============================================================================
 */

class Solution {
public:
    int maxProductPath(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();

        // Type alias to keep the code concise — products can overflow int
        // easily (e.g., 15 cells each with value -15 → product ~4.4 × 10^17)
        using ll = long long;

        /*
         * dp[i][j] = {maxProduct, minProduct} for all paths from (0,0) to (i,j).
         * Pair is default-initialized to {0, 0}; real values are filled below.
         */
        vector<vector<pair<ll, ll>>> dp(m, vector<pair<ll, ll>>(n));

        /*
         * BASE CASE: top-left corner.
         * Only one "path" exists to (0,0) — the cell itself — so the max
         * and min products are both just grid[0][0].
         */
        dp[0][0] = {grid[0][0], grid[0][0]};

        /*
         * FILL FIRST COLUMN (j = 0, varying i):
         * These cells have exactly ONE predecessor — the cell directly above.
         * There's no left neighbor, so the only incoming path comes from (i-1, 0).
         *
         * We compute both possible products (prevMax * val, prevMin * val) and
         * assign max/min accordingly. This handles sign flips without any
         * explicit if-else on the sign of val.
         */
        for (int i = 1; i < m; i++) {
            ll val = grid[i][0];

            // Two candidate products: extend the previous max-path and min-path
            ll a = dp[i-1][0].first  * val;   // previous max product × val
            ll b = dp[i-1][0].second * val;   // previous min product × val

            // Whichever is larger becomes the new max; whichever is smaller
            // becomes the new min — the sign of val determines which is which
            dp[i][0].first  = max(a, b);
            dp[i][0].second = min(a, b);
        }

        /*
         * FILL FIRST ROW (i = 0, varying j):
         * These cells have exactly ONE predecessor — the cell directly to the left.
         * Same logic as the first column, but predecessor is (0, j-1).
         */
        for (int j = 1; j < n; j++) {
            ll val = grid[0][j];

            // Two candidate products from the only predecessor (to the left)
            ll a = dp[0][j-1].first  * val;   // previous max product × val
            ll b = dp[0][j-1].second * val;   // previous min product × val

            dp[0][j].first  = max(a, b);
            dp[0][j].second = min(a, b);
        }

        /*
         * FILL INTERIOR CELLS (i > 0, j > 0):
         * Each interior cell has TWO predecessors: above (i-1,j) and left (i,j-1).
         * Any path to (i,j) came through exactly one of these two cells.
         *
         * We generate ALL FOUR candidate products by combining:
         *   - The max and min from the cell above, each multiplied by val
         *   - The max and min from the cell to the left, each multiplied by val
         *
         * Taking the overall max and min of these four candidates ensures we
         * capture the true best/worst outcomes regardless of which predecessor
         * path we came from and regardless of the sign of val.
         *
         * This is more general than comparing predecessors first, then
         * multiplying — it correctly handles cases where, e.g., the smaller
         * of two predecessors' max-products yields a larger result after
         * multiplication by a negative val.
         */
        for (int i = 1; i < m; i++) {
            for (int j = 1; j < n; j++) {
                ll val = grid[i][j];

                // Four candidates: (prevMax from above, prevMin from above,
                //                   prevMax from left,  prevMin from left)
                // each extended by multiplying with the current cell's value
                ll a = dp[i-1][j].first  * val;   // above's max-path × val
                ll b = dp[i-1][j].second * val;   // above's min-path × val
                ll c = dp[i][j-1].first  * val;   // left's  max-path × val
                ll d = dp[i][j-1].second * val;   // left's  min-path × val

                /*
                 * The best possible product reaching (i,j) is the largest
                 * among all four candidates; the worst is the smallest.
                 * Using initializer-list max/min elegantly covers all cases
                 * without needing explicit sign checks on val.
                 */
                dp[i][j].first  = max({a, b, c, d});
                dp[i][j].second = min({a, b, c, d});
            }
        }

        // The maximum product of any full path is stored at the destination cell
        ll ans = dp[m-1][n-1].first;

        const ll MOD = 1e9 + 7;

        /*
         * If the best achievable product is negative, the problem requires us
         * to return -1 (no valid non-negative path product exists).
         *
         * We check this BEFORE applying MOD — if we had applied MOD during
         * the DP, a large negative could have wrapped to a positive, masking
         * the true sign. Deferring MOD to this final step keeps correctness.
         */
        if (ans < 0) return -1;

        return ans % MOD;
    }
};