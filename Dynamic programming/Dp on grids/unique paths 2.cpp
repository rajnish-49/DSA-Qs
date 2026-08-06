class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        
        /* ═══════════════════════════════════════════════════════════════════════════
         * PROBLEM STATEMENT & INTUITION:
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * We have a robot starting at the TOP-LEFT corner of an m×n grid. The robot
         * wants to reach the BOTTOM-RIGHT corner. At any point, the robot can ONLY 
         * move RIGHT or DOWN (never left or up - this constraint is crucial).
         * 
         * Some cells contain obstacles (marked as 1), which block all paths through them.
         * Empty cells are marked as 0.
         * 
         * CORE INSIGHT - Why Dynamic Programming?
         * ────────────────────────────────────────
         * The key observation is that to reach any cell (i,j), the robot MUST have
         * come from either:
         *   1. The cell directly ABOVE it (i-1, j), OR
         *   2. The cell directly to the LEFT of it (i, j-1)
         * 
         * This creates a RECURSIVE SUBSTRUCTURE:
         *   paths_to(i,j) = paths_to(i-1,j) + paths_to(i,j-1)
         * 
         * WHY do we add? Because paths are INDEPENDENT. If there are 5 ways to reach
         * the cell above and 3 ways to reach the cell to the left, then there are
         * 5+3=8 total ways to reach the current cell (by the fundamental counting
         * principle - each path from above is distinct from each path from left).
         * 
         * We use DP to avoid recalculating the number of paths to each cell multiple
         * times. We build the solution BOTTOM-UP, starting from the origin and working
         * our way to the destination.
         * 
         * ═══════════════════════════════════════════════════════════════════════════
         */
        
        int m = obstacleGrid.size();      // Number of rows in the grid
        int n = obstacleGrid[0].size();   // Number of columns in the grid
        
        /* ───────────────────────────────────────────────────────────────────────────
         * CRITICAL EDGE CASE: Check start and end positions
         * ───────────────────────────────────────────────────────────────────────────
         * If either the starting position (0,0) or ending position (m-1,n-1) has an
         * obstacle, it's IMPOSSIBLE to complete any path. Think of it this way:
         *   - If start is blocked: Robot can't even begin its journey
         *   - If end is blocked: Robot can never reach the destination
         * In both cases, the answer is definitively 0 paths.
         */
        if (obstacleGrid[0][0] == 1 || obstacleGrid[m - 1][n - 1] == 1)
            return 0;
        
        /* ───────────────────────────────────────────────────────────────────────────
         * DP TABLE INITIALIZATION:
         * ───────────────────────────────────────────────────────────────────────────
         * dp[i][j] represents: "How many unique paths exist from (0,0) to (i,j)?"
         * 
         * We initialize all cells to 0 because initially we haven't computed any paths.
         * This also serves as the default value for obstacle cells (0 paths through them).
         */
        vector<vector<int>> dp(m, vector<int>(n, 0));
        
        /* ───────────────────────────────────────────────────────────────────────────
         * BASE CASE: Starting position
         * ───────────────────────────────────────────────────────────────────────────
         * There is exactly ONE way to be at the starting position (0,0): by starting
         * there. This is our base case that seeds the entire DP computation.
         * 
         * Think of it as: "Before taking any steps, I'm already at the start in 1 way."
         */
        dp[0][0] = 1;
        
        /* ═══════════════════════════════════════════════════════════════════════════
         * MAIN DP COMPUTATION - Building the solution iteratively
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * ITERATION ORDER MATTERS:
         * We iterate row by row, left to right. This ensures that when we compute
         * dp[i][j], we've ALREADY computed:
         *   - dp[i-1][j] (the cell above)
         *   - dp[i][j-1] (the cell to the left)
         * 
         * This is the essence of BOTTOM-UP DP: we solve smaller subproblems first
         * (cells closer to origin) and use them to solve larger subproblems (cells
         * farther from origin).
         * 
         * WHY this order works: Due to movement restrictions (only right/down), the
         * number of paths to any cell only depends on cells we've already processed
         * in this iteration pattern.
         */
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                
                /* ───────────────────────────────────────────────────────────────────
                 * Skip the starting cell - we already initialized it
                 * ───────────────────────────────────────────────────────────────────
                 * We don't want to overwrite dp[0][0] = 1. It's our base case and
                 * attempting to apply the recurrence relation would give us 0+0=0,
                 * which is wrong.
                 */
                if (i == 0 && j == 0)
                    continue;
                
                /* ───────────────────────────────────────────────────────────────────
                 * OBSTACLE HANDLING:
                 * ───────────────────────────────────────────────────────────────────
                 * If current cell has an obstacle (value = 1), it's IMPASSABLE.
                 * No paths can go THROUGH this cell, so dp[i][j] = 0.
                 * 
                 * CRITICAL UNDERSTANDING: This doesn't mean "0 paths to this cell"
                 * in isolation, but rather "0 paths that use this cell". Paths can
                 * still go AROUND obstacles through other cells.
                 * 
                 * We set it to 0 and continue because:
                 *   1. This cell contributes 0 to any cell that depends on it
                 *   2. We don't want to add paths from its neighbors (they can't
                 *      route through an obstacle)
                 */
                if (obstacleGrid[i][j] == 1) {
                    dp[i][j] = 0;
                    continue;
                }
                
                /* ───────────────────────────────────────────────────────────────────
                 * COMPUTING PATHS FROM TOP:
                 * ───────────────────────────────────────────────────────────────────
                 * If we're NOT in the first row (i > 0), we can receive paths from
                 * the cell directly above us. The number of such paths is dp[i-1][j].
                 * 
                 * WHY the check (i > 0)?
                 * - If i == 0, we're in the first row - there IS no cell above us
                 * - Accessing dp[-1][j] would be out of bounds
                 * - From a logical standpoint: cells in the first row can ONLY be
                 *   reached by moving RIGHT from (0,0), never from above
                 * 
                 * If we're in the first row, fromTop = 0 (no contribution from above)
                 */
                int fromTop = (i > 0) ? dp[i - 1][j] : 0;
                
                /* ───────────────────────────────────────────────────────────────────
                 * COMPUTING PATHS FROM LEFT:
                 * ───────────────────────────────────────────────────────────────────
                 * Similarly, if we're NOT in the first column (j > 0), we can receive
                 * paths from the cell directly to our left. The count is dp[i][j-1].
                 * 
                 * WHY the check (j > 0)?
                 * - If j == 0, we're in the first column - there IS no cell to our left
                 * - Accessing dp[i][-1] would be out of bounds
                 * - Logically: cells in the first column can ONLY be reached by moving
                 *   DOWN from (0,0), never from the left
                 * 
                 * If we're in the first column, fromLeft = 0 (no contribution from left)
                 */
                int fromLeft = (j > 0) ? dp[i][j - 1] : 0;
                
                /* ───────────────────────────────────────────────────────────────────
                 * THE CORE DP RECURRENCE RELATION:
                 * ───────────────────────────────────────────────────────────────────
                 * dp[i][j] = dp[i-1][j] + dp[i][j-1]
                 * 
                 * INTUITION BREAKDOWN:
                 * 
                 * Imagine you're standing at cell (i,j). You ask: "How many different
                 * routes brought me here from the start?"
                 * 
                 * Well, your LAST STEP before arriving at (i,j) was either:
                 *   1. A step DOWN from (i-1,j), OR
                 *   2. A step RIGHT from (i,j-1)
                 * 
                 * These are MUTUALLY EXCLUSIVE events (you can't take both steps
                 * simultaneously as your last move). Therefore, by the Addition
                 * Principle of Counting:
                 * 
                 * Total paths to (i,j) = [Paths that end with a DOWN move] +
                 *                        [Paths that end with a RIGHT move]
                 *                      = [Paths to (i-1,j)] + [Paths to (i,j-1)]
                 *                      = dp[i-1][j] + dp[i][j-1]
                 * 
                 * EXAMPLE WALKTHROUGH:
                 * If dp[i-1][j] = 5 (five ways to reach the cell above)
                 * And dp[i][j-1] = 3 (three ways to reach the cell to the left)
                 * Then dp[i][j] = 8 (eight total ways to reach current cell)
                 * 
                 * Why not multiply? Because we're not combining paths - we're counting
                 * distinct routes. Each of the 5 paths from above is a COMPLETE path
                 * from (0,0) to (i,j), as is each of the 3 paths from the left.
                 * They're alternatives, not combinations.
                 */
                dp[i][j] = fromTop + fromLeft;
            }
        }
        
        /* ═══════════════════════════════════════════════════════════════════════════
         * FINAL ANSWER:
         * ═══════════════════════════════════════════════════════════════════════════
         * After filling the entire DP table, dp[m-1][n-1] contains the number of
         * unique paths from (0,0) to the bottom-right corner.
         * 
         * WHY is this guaranteed to be correct?
         * - We've computed paths to every cell in order of dependency
         * - Each cell's value is based on correct values of cells it depends on
         * - The final cell aggregates all valid paths through the grid
         * 
         * TIME COMPLEXITY: O(m × n) - we visit each cell exactly once
         * SPACE COMPLEXITY: O(m × n) - for the DP table
         *   Note: This could be optimized to O(n) using rolling arrays since we only
         *   need the previous row, but the current approach is clearer.
         */
        return dp[m - 1][n - 1];
    }
};