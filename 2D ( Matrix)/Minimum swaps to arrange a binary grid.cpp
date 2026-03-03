/*
 * ============================================================
 * PROBLEM — MINIMUM SWAPS TO ARRANGE A BINARY GRID
 * ============================================================
 *
 * IN PLAIN TERMS:
 *   You have an n×n binary grid. You can swap adjacent rows any number
 *   of times. Find the minimum swaps to make every cell above the main
 *   diagonal equal to zero. Return -1 if it's impossible.
 *
 * WHAT "VALID" ACTUALLY REQUIRES:
 *   The main diagonal cell at row i is at column i.
 *   Every cell to its right (columns i+1 ... n-1) must be 0.
 *   This means row i must end with at least (n-1-i) consecutive zeros:
 *
 *       Row 0 → needs ≥ (n-1) trailing zeros
 *       Row 1 → needs ≥ (n-2) trailing zeros
 *       ...
 *       Row n-2 → needs ≥ 1  trailing zero
 *       Row n-1 → needs ≥ 0  trailing zeros (always satisfied)
 *
 * WHY BRUTE FORCE FAILS:
 *   There are n! possible row orderings. For n=200, evaluating all
 *   permutations is completely infeasible. Even with pruning, the
 *   search space is too large. We need a structural insight.
 *
 * KEY OBSERVATION — COMPRESS TO TRAILING ZEROS:
 *   A row's eligibility for position i depends ONLY on how many trailing
 *   zeros it has — nothing else about its content matters. This collapses
 *   the entire n×n problem into a 1D array of n integers.
 *
 * WHY GREEDY IS CORRECT:
 *   Process positions i = 0, 1, ..., n-1 strictly in order.
 *   For position i, scan downward from i and pick the NEAREST row
 *   (smallest index j ≥ i) whose trailing zero count meets the requirement.
 *
 *   "Nearest" is always optimal because:
 *   → Bringing a farther row up wastes extra swaps.
 *   → Rows displaced downward (shifted from i to i+1) only need FEWER
 *     trailing zeros as i increases — they never become harder to place.
 *   → No future position is ever made worse by this local choice.
 *   → Local optimum = global optimum. DP is unnecessary.
 *
 * INVARIANT MAINTAINED:
 *   After processing position i, every row 0..i satisfies its trailing
 *   zero requirement, and only adjacent swaps have been performed.
 *   The relative order of all unprocessed rows is preserved.
 *
 * ============================================================
 */

class Solution {
public:
    int minSwaps(vector<vector<int>>& grid) {
        int n = grid.size();
        vector<int> trailing(n);

        /*
         * COMPRESSION STEP — reduce the grid to a 1D problem.
         * For each row, count how many zeros trail from the right.
         * This single number fully determines whether a row can sit
         * at position i (it must have trailing[row] >= n-1-i).
         * All other cell values are irrelevant and can be discarded.
         */
        for (int i = 0; i < n; ++i) {
            int cnt = 0;
            for (int j = n - 1; j >= 0 && grid[i][j] == 0; --j)
                cnt++;
            trailing[i] = cnt;
        }

        int ans = 0;

        for (int i = 0; i < n; ++i) {
            /*
             * REQUIREMENT — how many trailing zeros row at position i must have.
             * Derived directly from the "valid grid" definition:
             * columns i+1 through n-1 (that's n-1-i columns) must all be zero.
             */
            int required = n - 1 - i;

            /*
             * GREEDY SCAN — find the nearest qualifying row at or below i.
             * We don't search globally; we stop at the first valid row
             * because bringing it up costs exactly (j-i) swaps — the minimum
             * possible cost to fill position i correctly.
             */
            int j = i;
            while (j < n && trailing[j] < required) j++;

            /*
             * IMPOSSIBILITY CHECK — if no row from i to n-1 qualifies,
             * no amount of swapping can ever fix position i. The answer
             * is -1. This happens when the grid lacks a row with sufficient
             * trailing zeros entirely (e.g., all rows are identical and short).
             */
            if (j == n) return -1;

            /*
             * BUBBLE UP — simulate (j-i) adjacent swaps to move row j to position i.
             * We operate on trailing[] directly rather than the full grid,
             * since that's the only information we need going forward.
             * Each swap shifts the qualifying row one step closer to i,
             * and the displaced rows slide down by one — preserving their
             * relative order and their eligibility for future positions.
             */
            while (j > i) {
                swap(trailing[j], trailing[j - 1]);
                ans++;
                j--;
            }
        }

        return ans;
    }
};

/*
 * ============================================================
 * COMPLEXITY
 * ============================================================
 *
 * Time:  O(n²)
 *   → Trailing zero computation: O(n²) across all rows.
 *   → Main loop: each of n positions scans and bubbles ≤ n rows → O(n²).
 *
 * Space: O(n)
 *   → Only the trailing[] array is used; full grid is not copied or modified.
 *
 * ============================================================
 * EDGE CASES
 * ============================================================
 *
 * → Already valid (Example 3): trailing[] already satisfies requirements
 *   in-order → the inner while loops never execute → returns 0.
 *
 * → All rows identical and insufficient (Example 2): the scan always hits
 *   j == n at some position → correctly returns -1.
 *
 * → n = 1: required = n-1-0 = 0, always satisfied → returns 0.
 *
 * ============================================================
 * COMMON CONCEPTUAL MISTAKES
 * ============================================================
 *
 * 1. Off-by-one on `required`:
 *    Row i needs (n-1-i) trailing zeros, not (n-i). Using (n-i) makes
 *    row 0 demand n trailing zeros — impossible for any n-wide row —
 *    causing the algorithm to return -1 on valid inputs.
 *
 * 2. Choosing the farthest qualifying row instead of the nearest:
 *    Picking the row with the MOST trailing zeros greedily seems safe
 *    but isn't — it incurs unnecessary swap costs and can displace rows
 *    that were already close to their target positions.
 *
 * 3. Swapping full rows in the grid instead of trailing[]:
 *    Since only trailing zero counts drive all future decisions, swapping
 *    entire rows is wasteful (O(n) per swap vs O(1)), inflating the
 *    overall complexity from O(n²) to O(n³) for no benefit.
 *
 * ============================================================
 */