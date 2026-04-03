/*
 * PROBLEM: Maximum Amount of Money Robot Can Earn
 *
 * SIMPLE RESTATEMENT:
 * A robot starts at top-left (0,0) of an m x n grid and wants to reach
 * bottom-right (m-1, n-1). It can only move right or down.
 * Each cell has a coin value:
 *   - positive: robot gains coins
 *   - negative: a robber steals that many coins
 * The robot can NEUTRALIZE at most 2 negative cells on its path,
 * meaning it collects 0 from those cells instead of losing coins.
 * Return the maximum coins the robot can collect.
 *
 * INPUT:
 *   coins[i][j] = coin value at cell (i,j), can be negative
 *
 * OUTPUT:
 *   Maximum coins collectible on any valid path from (0,0) to (m-1,n-1)
 *
 * GOAL:
 *   Find the best path AND the best cells to neutralize on that path.
 *
 * -----------------------------------------------------------------------
 * APPROACH FROM FIRST PRINCIPLES:
 *
 * OBSERVATION 1: Robot can only move right or down.
 *   So every cell (i,j) can only be reached from (i-1,j) or (i,j-1).
 *   This is a classic grid DP structure.
 *
 * OBSERVATION 2: The robot has a special ability — 2 neutralizations.
 *   This means the state of the robot is not just its position (i,j),
 *   but also HOW MANY neutralizations it has remaining.
 *   This is the key insight that leads to a 3D DP.
 *
 * BRUTE FORCE FAILS because:
 *   There are exponentially many paths (right/down combinations),
 *   and for each path we'd need to try all combinations of which
 *   2 cells to neutralize. Too slow for m,n up to 500.
 *
 * OPTIMAL APPROACH: 3D Dynamic Programming
 *   State: dp[i][j][k] = maximum coins at cell (i,j) with k neutralizations remaining
 *   k can be 0, 1, or 2.
 *
 * STATE TRANSITION:
 *   For each cell, we arrive from top or left (whichever gives more coins).
 *   Then:
 *   - If coins[i][j] >= 0: just collect it. k doesn't change.
 *       dp[i][j][k] = max(top, left) + coins[i][j]
 *   - If coins[i][j] < 0: two choices:
 *       Choice 1 - Don't neutralize: take the loss. k stays same.
 *           dp[i][j][k] = max(top, left) + coins[i][j]
 *       Choice 2 - Neutralize: collect 0, spend one neutralization. k drops by 1.
 *           dp[i][j][k-1] = max(dp[i][j][k-1], max(top, left))
 *
 * BASE CASE: (0,0) — starting cell, always have 2 neutralizations.
 *   If positive: dp[0][0][2] = coins[0][0]
 *   If negative: dp[0][0][2] = coins[0][0]  (don't neutralize)
 *                dp[0][0][1] = 0             (neutralize, spend 1)
 *
 * ANSWER: max(dp[m-1][n-1][0], dp[m-1][n-1][1], dp[m-1][n-1][2])
 *   Because we don't know how many neutralizations the optimal path used.
 * -----------------------------------------------------------------------
 */

class Solution {
public:
    int maximumAmount(vector<vector<int>>& coins) {

        int m = coins.size();
        int n = coins[0].size();

        // dp[i][j][k] = max coins at cell (i,j) with k neutralizations remaining
        // k=2: both neutralizations unused
        // k=1: one used, one remaining
        // k=0: both used, none remaining
        // Initialize everything to -1e9 meaning "unreachable state"
        vector<vector<vector<int>>> dp(
            m, vector<vector<int>>(n, vector<int>(3, -1e9)));

        // BASE CASE: cell (0,0) — robot always starts here with 2 neutralizations
        // We only set dp[0][0][2] because we always start with 2 neutralizations.
        // dp[0][0][0] and dp[0][0][1] stay -1e9 (unreachable at start).
        if (coins[0][0] >= 0) {
            // Positive cell: just collect, no reason to neutralize
            dp[0][0][2] = coins[0][0];
        } else {
            // Negative cell: two choices at the very start
            dp[0][0][2] = coins[0][0]; // Choice 1: don't neutralize, take the loss, keep 2 remaining
            dp[0][0][1] = 0;           // Choice 2: neutralize, collect 0, now 1 remaining
        }

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {

                // Skip (0,0) since it is already handled as the base case above
                // If we don't skip, the loop would overwrite our base case
                if (i == 0 && j == 0)
                    continue;

                // For each state of neutralizations remaining (0, 1, or 2)
                // We compute the best coins we can have at (i,j) with exactly k remaining
                for (int k = 0; k <= 2; k++) {

                    // The robot can arrive from above (i-1,j) or from the left (i,j-1)
                    // If the cell doesn't exist (boundary), treat it as -1e9 (unreachable)
                    // so it never wins the max comparison
                    int top  = (i > 0) ? dp[i - 1][j][k] : (int)-1e9;
                    int left = (j > 0) ? dp[i][j - 1][k] : (int)-1e9;

                    // best = maximum coins we can arrive at (i,j) with,
                    // before collecting coins[i][j]
                    int best = max(top, left);

                    if (coins[i][j] >= 0) {
                        // Positive cell: always collect it, no decision needed
                        // k stays the same — no neutralization used
                        dp[i][j][k] = best + coins[i][j];

                    } else {
                        // Negative cell: two choices

                        // Choice 1: Don't neutralize
                        // Take the loss. k stays the same.
                        // We arrived with best coins, robber steals, we end with best + coins[i][j]
                        dp[i][j][k] = best + coins[i][j];

                        // Choice 2: Neutralize (only possible if we have a neutralization left)
                        // Block the robber, collect 0 from this cell.
                        // We arrived with best coins, collect 0, still have best coins.
                        // But we spent one neutralization, so k drops to k-1.
                        // We update dp[i][j][k-1] because that is the state with one fewer remaining.
                        // We take max because dp[i][j][k-1] may have already been set
                        // by an earlier iteration of the k loop (e.g. k=1 sets k-1=0,
                        // and k=2 also sets k-1=1 which was already visited).
                        // We never want to overwrite a better value already stored there.
                        if (k > 0) {
                            dp[i][j][k - 1] = max(dp[i][j][k - 1], best);
                        }
                    }
                }
            }
        }

        // The answer is at the bottom-right cell.
        // We don't know how many neutralizations the optimal path used,
        // so we take the max across all three possible remaining counts.
        return max(
            {dp[m - 1][n - 1][0], dp[m - 1][n - 1][1], dp[m - 1][n - 1][2]});
    }
};


/*
 * TOP-DOWN MEMOIZATION APPROACH
 *
 * Same idea as bottom-up but written recursively.
 * We define solve(i, j, neu) = maximum coins collectible
 * from cell (i,j) to (m-1,n-1) with neu neutralizations remaining.
 *
 * KEY DIFFERENCE FROM BOTTOM-UP:
 *   Bottom-up fills the table iteratively from (0,0) to (m-1,n-1).
 *   Top-down starts from (0,0) and recursively explores forward,
 *   caching results so each (i,j,neu) state is computed only once.
 *
 * STATE:
 *   solve(i, j, neu) = max coins from (i,j) to destination with neu neutralizations left
 *
 * BASE CASES:
 *   1. Reached destination (m-1, n-1):
 *      - If cell is negative and neu > 0: neutralize it, return 0
 *      - Otherwise: return coins[i][j] as is
 *   2. Out of bounds: return INT_MIN (impossible state)
 *
 * TRANSITION:
 *   At every cell we have two choices:
 *   Choice 1 (take): collect coins[i][j] as is, neu unchanged
 *   Choice 2 (skip): only if cell is negative and neu > 0,
 *                    collect 0, neu decreases by 1
 *   Return max of both choices.
 *
 * MEMOIZATION:
 *   t[i][j][neu] stores the result of solve(i,j,neu)
 *   so we never recompute the same state twice.
 *   Initialized to INT_MIN meaning "not yet computed".
 */

class Solution {
public:
    int m;
    int n;
    // t[i][j][neu] = max coins from (i,j) to (m-1,n-1) with neu neutralizations remaining
    // neu can be 0, 1, or 2 — so third dimension is size 3
    int t[501][501][3];

    int solve(vector<vector<int>>& coins, int i, int j, int neu) {

        // BASE CASE 1: Reached the destination cell
        // We still need to collect from this cell before stopping
        if(i == m-1 && j == n-1) {
            // If cell is negative and we have a neutralization left, use it
            // Collect 0 instead of losing coins — always better than taking the loss
            if(coins[i][j] < 0 && neu > 0) {
                return 0;
            }
            // Otherwise collect as is (positive cell, or negative with no neu left)
            return coins[i][j];
        }

        // BASE CASE 2: Went out of bounds
        // Robot moved beyond the grid — this path is invalid
        // Return INT_MIN so this path never gets chosen in a max comparison
        if(i >= m || j >= n) {
            return INT_MIN;
        }

        // MEMOIZATION CHECK: if this state was already computed, return cached result
        // This prevents recomputing the same (i,j,neu) state multiple times
        if(t[i][j][neu] != INT_MIN) {
            return t[i][j][neu];
        }

        // CHOICE 1: Don't neutralize this cell.
        // Collect coins[i][j] as is (positive or negative), then move to the best next cell.
        // neu stays the same because we didn't spend a neutralization.
        int take = coins[i][j] + max(solve(coins, i+1, j, neu), solve(coins, i, j+1, neu));

        // CHOICE 2: Neutralize this cell.
        // Only possible if current cell is negative AND we have neutralizations remaining.
        // If we neutralize, we collect 0 from this cell (so coins[i][j] is not added).
        // We spend one neutralization so neu becomes neu-1 for all future cells.
        int skip = INT_MIN; // default to impossible in case we can't neutralize
        if(coins[i][j] < 0 && neu > 0) {
            int skipDown  = solve(coins, i+1, j, neu-1); // go down with one less neutralization
            int skipRight = solve(coins, i, j+1, neu-1); // go right with one less neutralization
            skip = max(skipDown, skipRight); // best future coins after neutralizing this cell
        }

        // Store and return the best of both choices for this state (i, j, neu)
        return t[i][j][neu] = max(take, skip);
    }

    int maximumAmount(vector<vector<int>>& coins) {
        m = coins.size();
        n = coins[0].size();

        // Initialize entire memo table to INT_MIN meaning "not yet computed"
        // We use INT_MIN and not 0 because 0 is a valid answer
        for(int i = 0; i < 501; i++) {
            for(int j = 0; j < 501; j++) {
                for(int k = 0; k < 3; k++) {
                    t[i][j][k] = INT_MIN;
                }
            }
        }

        // Start recursion from (0,0) with 2 neutralizations remaining
        return solve(coins, 0, 0, 2);
    }
};