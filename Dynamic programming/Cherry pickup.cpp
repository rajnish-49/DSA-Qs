// @ 

// ============================================================
// LEETCODE 741. CHERRY PICKUP
// ============================================================
//
// THE PROBLEM:
// You have an n x n grid. You start at (0,0), walk to (n-1,n-1),
// then walk back to (0,0). Collect as many cherries (1s) as
// possible. You can only move right/down going forward, and
// left/up going back. Thorns (-1) block your path. Once you
// pick a cherry, the cell becomes 0.
//
// ============================================================
// WHY THE NAIVE APPROACH FAILS:
// ============================================================
// The first thought is: find the path with max cherries going
// forward, remove those cherries, then find the max path going
// back. THIS IS WRONG.
//
// Why? Because being greedy on the first path can block a better
// overall combination. Example:
//
//     1 1 -1
//     1 -1  1      Greedy first path picks top cherries,
//    -1  1  1      but blocks the second path completely.
//
// The two paths are NOT independent. They affect each other.
// So we need to consider BOTH paths simultaneously.
//
// ============================================================
// THE KEY INSIGHT — TWO PEOPLE WALKING FORWARD:
// ============================================================
// Going (0,0) → (n-1,n-1) → (0,0) is the SAME as sending
// TWO PEOPLE from (0,0) to (n-1,n-1) at the same time.
//
// Why? Any path going back (up/left) from (n-1,n-1) to (0,0)
// can simply be REVERSED into a path going forward (down/right)
// from (0,0) to (n-1,n-1).
//
// So: round trip = two forward trips.
//
// Now the cherry rule becomes simple:
//   - If both land on SAME cell  → count cherry ONCE
//   - If both land on DIFF cells → count BOTH cherries
//
// No need to mutate the grid. No need to track which cherries
// are already picked. The "same cell" check handles everything.
//
// ============================================================
// WHY WE CAN SYNC THEM BY STEP COUNT:
// ============================================================
// Both people start at (0,0) and end at (n-1,n-1).
// Both can only move right or down.
// Every move increases (row + col) by exactly 1.
// So after t steps: row + col = t  →  col = t - row.
//
// This means: at any step t, if we know a person's ROW,
// we can DERIVE their column. We don't need to store it.
//
// So the state is just: (t, row_of_person1, row_of_person2)
// That's 3 variables instead of 4. O(n^3) instead of O(n^4).
//
// ============================================================
// THE DP STATE:
// ============================================================
// dp[r1][r2][t] = maximum cherries collected by BOTH people
//                 from step 0 up to step t, where person 1
//                 is at row r1 and person 2 is at row r2.
//
// Columns are derived:
//   c1 = t - r1
//   c2 = t - r2
//
// Base case: dp[0][0][0] = grid[0][0]
//   Both start at (0,0). Cherry counted once (same cell).
//
// Answer: dp[n-1][n-1][2*(n-1)]
//   Both end at (n-1,n-1) after 2*(n-1) steps.
//
// ============================================================
// THE TRANSITION — WHERE DID THEY COME FROM?
// ============================================================
// At step t, person 1 is at row r1. At step t-1, they were at:
//   r1     → if they moved RIGHT (row doesn't change)
//   r1 - 1 → if they moved DOWN  (row was 1 less before)
//
// Same logic for person 2 with r2.
//
// So there are 4 possible previous states:
//   (r1,   r2,   t-1) → both moved right
//   (r1,   r2-1, t-1) → P1 right, P2 down
//   (r1-1, r2,   t-1) → P1 down,  P2 right
//   (r1-1, r2-1, t-1) → both moved down
//
// We take the MAX of these 4. That's the best way to arrive
// at the current state. Then we add cherries at current cells.
//
// ============================================================
// WHY t GOES FROM 0 TO 2*(n-1):
// ============================================================
// To go from (0,0) to (n-1,n-1):
//   Down moves needed:  n-1
//   Right moves needed: n-1
//   Total steps:        2*(n-1)
//
// t counts STEPS (not cells). Steps go 0, 1, 2, ... 2*(n-1).
// Cells visited = steps + 1 = 2*n - 1. But t is steps.
//
// ============================================================
// WHY -1e9 AND NOT INT_MIN:
// ============================================================
// We add values to dp entries (cherries). If dp is INT_MIN
// and we add something, it OVERFLOWS (wraps to positive).
// -1e9 is negative enough to never become max, but
// safe to add to without overflow.
//
// ============================================================
// COMPLEXITY:
//   Time:  O(n^3) — three nested loops, each up to n
//   Space: O(n^3) — the dp table
// ============================================================

class Solution {
public:
    int cherryPickup(vector<vector<int>>& grid) {
        int n = grid.size();

        // Total steps to go from (0,0) to (n-1,n-1).
        // Need (n-1) downs + (n-1) rights = 2*(n-1) steps.
        int steps = 2 * (n - 1);

        // ================================================
        // DP TABLE CREATION
        // ================================================
        // dp[r1][r2][t] = max cherries both people collected
        // up to step t. Person 1 at row r1, person 2 at row r2.
        //
        // Why this dimension order (r1, r2, t) and not (t, r1, r2)?
        // Doesn't matter logically — both are correct.
        // This ordering is common in leetcode solutions.
        //
        // Size: n x n x (steps+1)
        // steps+1 because t goes from 0 to steps INCLUSIVE.
        // If steps=4, t can be 0,1,2,3,4 → that's 5 values.
        //
        // Filled with -1e9 (acts as -infinity).
        // Meaning: "this state is unreachable / invalid".
        // Any state that stays -1e9 was never reached by a
        // valid path, so it will never contribute to the answer.
        // ================================================
        vector<vector<vector<int>>> dp(
            n, vector<vector<int>>(n, vector<int>(steps + 1, -1e9))
        );

        // BASE CASE:
        // At step 0, both people are at (0,0).
        // They are on the SAME cell, so cherry is counted ONCE.
        // grid[0][0] is guaranteed to not be -1 (per constraints).
        dp[0][0][0] = grid[0][0];

        // ================================================
        // MAIN DP LOOP
        // ================================================
        // We fill the table step by step, from t=1 to t=steps.
        // Why start at t=1? Because t=0 is already the base case.
        //
        // For each step t, we try every possible (r1, r2) combo.
        // Most combos will be invalid (out of bounds, thorn, or
        // unreachable). We skip those with 'continue'.
        // ================================================
        for (int t = 1; t <= steps; t++) {
            for (int r1 = 0; r1 < n; r1++) {
                for (int r2 = 0; r2 < n; r2++) {

                    // ============================================
                    // DERIVE COLUMNS FROM STEP AND ROW
                    // ============================================
                    // Core identity: after t steps, row + col = t.
                    // Every move adds 1 to either row or col.
                    // Started at (0,0), so row+col starts at 0.
                    // After t moves, row+col = t.
                    // Therefore: col = t - row.
                    //
                    // This is WHY we only need (t, r1, r2) as state.
                    // Columns are not independent variables — they
                    // are fully determined by t and the row.
                    // ============================================
                    int c1 = t - r1;
                    int c2 = t - r2;

                    // ============================================
                    // BOUNDS CHECK ON COLUMNS
                    // ============================================
                    // Not every (t, r1) gives a valid column.
                    // Example: t=1, r1=3 → c1 = 1-3 = -2. Invalid.
                    // Example: t=5, r1=1, n=3 → c1 = 5-1 = 4. Invalid.
                    // Skip these — person can't be here.
                    // ============================================
                    if (c1 < 0 || c1 >= n || c2 < 0 || c2 >= n)
                        continue;

                    // ============================================
                    // THORN CHECK
                    // ============================================
                    // If either person is standing on a thorn (-1),
                    // this state is completely invalid. A person
                    // cannot enter a thorn cell at all. So no valid
                    // path can pass through this state. Skip it.
                    // ============================================
                    if (grid[r1][c1] == -1 || grid[r2][c2] == -1)
                        continue;

                    // ============================================
                    // FIND THE BEST PREVIOUS STATE
                    // ============================================
                    // Current: person 1 at row r1, person 2 at row r2.
                    // Previous step is t-1. Where were they?
                    //
                    // Person 1 is at r1 now. Last step they were at:
                    //   r1   → they moved RIGHT to get here
                    //          (right doesn't change row)
                    //   r1-1 → they moved DOWN to get here
                    //          (down increases row by 1, so before
                    //           it was 1 less)
                    //
                    // Same logic for person 2 with r2.
                    //
                    // 4 combinations of previous positions:
                    //   (r1,   r2)   → both moved right
                    //   (r1,   r2-1) → P1 right, P2 down
                    //   (r1-1, r2)   → P1 down,  P2 right
                    //   (r1-1, r2-1) → both moved down
                    //
                    // We pick the MAX among valid ones. That gives
                    // the best way to arrive at current state.
                    //
                    // Why check r1>0 and r2>0?
                    // If r1=0, then r1-1=-1, which is out of bounds.
                    // Person can't have been at row -1.
                    // So we guard against accessing invalid indices.
                    // ============================================
                    int best = -1e9;

                    // Both moved right (neither row changed)
                    best = max(best, dp[r1][r2][t - 1]);

                    // Person 1 moved down (was at r1-1), P2 right
                    if (r1 > 0)
                        best = max(best, dp[r1 - 1][r2][t - 1]);

                    // Person 1 moved right, Person 2 moved down
                    if (r2 > 0)
                        best = max(best, dp[r1][r2 - 1][t - 1]);

                    // Both moved down (both were 1 row above)
                    if (r1 > 0 && r2 > 0)
                        best = max(best, dp[r1 - 1][r2 - 1][t - 1]);

                    // ============================================
                    // IF ALL PREVIOUS STATES ARE INVALID, SKIP
                    // ============================================
                    // If best is still -1e9, it means NO valid path
                    // can reach any of the 4 previous states.
                    // So no valid path can reach current state either.
                    // Mark it as unreachable and move on.
                    // ============================================
                    if (best < 0) continue;

                    // ============================================
                    // COUNT CHERRIES AT CURRENT POSITIONS
                    // ============================================
                    // This is where the "two people" trick shines.
                    //
                    // If r1 == r2, then c1 == c2 too (since
                    // c = t - r, and t is same for both).
                    // So both are on the EXACT SAME CELL.
                    // Cherry should only be counted ONCE.
                    // (First person picks it, second person sees 0)
                    //
                    // If r1 != r2, they are on DIFFERENT cells.
                    // Both cherries are picked independently.
                    // Count both.
                    //
                    // This single check completely replaces the need
                    // to actually modify the grid and track which
                    // cherries have been picked. That's the elegance
                    // of the two-people formulation.
                    // ============================================
                    int cherries = best + grid[r1][c1];

                    if (r1 != r2)
                        cherries += grid[r2][c2];

                    // Store the result for this state.
                    // This will be used as a "previous state" when
                    // we process step t+1.
                    dp[r1][r2][t] = cherries;
                }
            }
        }

        // ================================================
        // RETURN THE ANSWER
        // ================================================
        // Both people must end at (n-1, n-1).
        // This happens at step t = 2*(n-1) = steps.
        // So answer is dp[n-1][n-1][steps].
        //
        // Why max(0, ...)?
        // If dp[n-1][n-1][steps] is negative, it means NO
        // valid path exists from (0,0) to (n-1,n-1).
        // (All paths are blocked by thorns.)
        // In that case, we collect 0 cherries.
        // ================================================
        return max(0, dp[n - 1][n - 1][steps]);
    }
};