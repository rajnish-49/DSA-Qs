class Solution {
public:
    int m, n;
    int dp[71][71][71];
    vector<int> colDir{-1, 0, 1};
    
    bool isSafe(int& i) {
        return i >= 0 && i < n;
    }

    /* ============================================================================
       PROBLEM: Cherry Pickup II (LeetCode 1463)
       ============================================================================
       
       VISUAL PROBLEM SETUP:
       ---------------------
       Imagine a grid filled with cherries (numbers represent cherry counts):
       
       Row 0:    [3,  1,  1,  5]    
                  ↓           ↓
              Robot1       Robot2
              starts       starts
              here         here
       
       Row 1:    [2,  4,  6,  1]
       Row 2:    [1,  7,  3,  2]
       Row 3:    [8,  9,  5,  4]    ← Both must reach bottom
       
       MOVEMENT RULES:
       ---------------
       From any cell, a robot can move to 3 cells in the next row:
       - Diagonal left  (column - 1)
       - Straight down  (column)
       - Diagonal right (column + 1)
       
       Example: If robot is at column 2 (row i), it can go to:
       columns 1, 2, or 3 in row i+1
       
       CRITICAL CONSTRAINT - THE OVERLAP RULE:
       ---------------------------------------
       If both robots land on the SAME cell, that cell's cherries are counted
       ONLY ONCE, not twice!
       
       Example:
       - Robot1 at (2, 1) collects grid[2][1] = 7
       - Robot2 at (2, 1) collects nothing (already taken!)
       - Total from this row = 7 (not 14!)
       
       But if they're on different cells:
       - Robot1 at (2, 1) collects grid[2][1] = 7
       - Robot2 at (2, 3) collects grid[2][3] = 2
       - Total from this row = 7 + 2 = 9
       
       GOAL: Maximize total cherries when both reach the bottom row.
       
       ============================================================================
       SOLUTION INTUITION - WHY DYNAMIC PROGRAMMING?
       ============================================================================
       
       FIRST PRINCIPLE: Why can't we just be greedy?
       ----------------------------------------------
       A greedy approach (always pick the locally best move) fails because:
       
       Example scenario:
       Row 0: Robot1 sees [5, 10] ahead
              → Greedy choice: Go to 10
       Row 1: But now Robot2 also needs that 10, they collide!
              → Total: 10 (wasted opportunity)
       
       Better approach:
       Row 0: Robot1 goes to 5, Robot2 goes to 10
              → Total: 15 (both happy!)
       
       We need to consider JOINT decisions, not individual greedy moves.
       
       
       WHY SIMULTANEOUS MOVEMENT MATTERS:
       ----------------------------------
       Key insight: Both robots are ALWAYS on the same row at any given time.
       They move down together, row by row.
       
       This means we can think of the problem as:
       - At row 0: Choose positions (c1, c2) and collect cherries
       - At row 1: Choose positions (c1', c2') and collect cherries
       - At row 2: Choose positions (c1'', c2'') and collect cherries
       - And so on...
       
       Each row's decision depends on the previous row's positions (which cells
       are reachable based on movement rules).
       
       
       THE STATE SPACE:
       ----------------
       At each row, we need to track TWO positions: (c1, c2)
       - c1: Robot 1's column
       - c2: Robot 2's column
       
       Total possible states per row: n × n (where n = number of columns)
       Total states in entire grid: m × n × n (where m = number of rows)
       
       
       WHY DP (OVERLAPPING SUBPROBLEMS):
       ---------------------------------
       Consider this scenario at row 2:
       
       Path A: (0,3) → (0,2) → (1,2)
                                  ↓
                          Both reach state (row=2, c1=1, c2=2)
                                  ↑
       Path B: (0,3) → (1,3) → (1,2)
       
       Question: "From state (2, 1, 2), what's the max cherries to the bottom?"
       
       Answer is THE SAME regardless of how we reached (2, 1, 2)!
       
       Without DP: We'd recalculate this answer every time we encounter (2, 1, 2)
       With DP: Calculate once, store in dp[2][1][2], reuse forever
       
       This is the essence of overlapping subproblems!
       
       
       DP STATE DEFINITION:
       --------------------
       dp[row][c1][c2] = Maximum cherries collectible starting from:
                         - Current row: 'row'
                         - Robot 1 at column: 'c1'  
                         - Robot 2 at column: 'c2'
                         All the way down to the bottom row.
       
       This is a "suffix" definition - we're asking "what's the best from
       HERE onwards?" not "what's the best to reach here?"
       
       ============================================================================
    */

    int solve(vector<vector<int>>& grid, int row, int c1, int c2) {
        
        /* BASE CASE: Beyond the grid
           --------------------------
           If we've moved past the last row, we're done.
           No more cherries to collect, return 0.
           
           Think: "I've fallen off the bottom of the grid, game over, 
                   I collect 0 more cherries from here."
        */
        if(row >= m)
            return 0;
        
        
        /* MEMOIZATION CHECK: Already computed this state?
           ------------------------------------------------
           Before doing ANY work, check if we've solved this exact problem before.
           
           dp[row][c1][c2] == -1 means: "Never calculated this before"
           dp[row][c1][c2] != -1 means: "Already know the answer!"
           
           Why this matters:
           ----------------
           Without this check, if we reach state (2, 1, 2) a thousand times,
           we'd recalculate it a thousand times!
           
           With this check, we calculate it ONCE, and the next 999 times we
           just return the stored answer instantly.
           
           This is what transforms our exponential recursion O(9^m) into
           polynomial time O(m × n² × 9).
        */
        if(dp[row][c1][c2] != -1) {
            return dp[row][c1][c2];
        }
        
        
        /* ====================================================================
           STEP 1: CALCULATE CHERRIES COLLECTED AT CURRENT ROW
           ====================================================================
           
           This is the "count" variable - it represents cherries collected
           RIGHT NOW at the current row, given both robots' positions.
           
           THE OVERLAP HANDLING LOGIC:
           ---------------------------
        */
        
        // Start with Robot 1's cherries
        int count = grid[row][c1];
        
        /* Now the CRITICAL part - Robot 2's contribution:
           
           CASE 1: c1 != c2 (Robots are on DIFFERENT cells)
           -------------------------------------------------
           
           Visual:
           Row i:  [3,  1,  5,  2]
                    ↑       ↑
                   R1(c1=0) R2(c2=2)
           
           They're on different cells, so we get cherries from BOTH:
           count = grid[row][0] + grid[row][2] = 3 + 5 = 8
           
           
           CASE 2: c1 == c2 (Robots are on the SAME cell)
           -----------------------------------------------
           
           Visual:
           Row i:  [3,  1,  5,  2]
                         ↑
                        R1 & R2 both at c1=c2=1
           
           They're on the same cell! Problem says we count cherries ONCE.
           count = grid[row][1] = 1 (not 1 + 1 = 2!)
           
           
           WHY THE IF CHECK WORKS:
           -----------------------
           We ALWAYS add grid[row][c1] (Robot 1's cell)
           We CONDITIONALLY add grid[row][c2] (Robot 2's cell) only if c1 != c2
           
           When c1 == c2:
           - count = grid[row][c1]  (just once)
           
           When c1 != c2:
           - count = grid[row][c1] + grid[row][c2]  (both cells)
           
           This elegantly handles the overlap constraint!
        */
        if(c1 != c2) {
            count += grid[row][c2];
        }
        
        /* At this point, 'count' represents:
           "How many cherries we collect at THIS row with the current configuration"
           
           But we're not done! We still need to figure out the FUTURE.
        */
        
        
        /* ====================================================================
           STEP 2: EXPLORE ALL POSSIBLE FUTURES AND FIND THE BEST ONE
           ====================================================================
           
           Now comes the "ans" variable - this will store the BEST possible
           cherries we can collect from the NEXT row onwards.
           
           THE DECISION TREE:
           ------------------
           At the current row, each robot has 3 movement choices:
           - Move diagonal left  (-1)
           - Move straight down  (0)
           - Move diagonal right (+1)
           
           Since we have 2 robots, total combinations = 3 × 3 = 9
           
           Visual decision tree from current state (row, c1, c2):
           
                                    (row, c1, c2)
                                         |
                    +--------------------+--------------------+
                    |                    |                    |
                Robot1 goes         Robot1 goes         Robot1 goes
                  left(-1)            straight(0)         right(+1)
                    |                    |                    |
            +-------+-------+    +-------+-------+    +-------+-------+
            |       |       |    |       |       |    |       |       |
           R2:-1   R2:0   R2:+1 R2:-1   R2:0   R2:+1 R2:-1   R2:0   R2:+1
            |       |       |    |       |       |    |       |       |
         (row+1, (row+1, (row+1, ...   ...   (row+1,  ...   ...   (row+1,
          c1-1,   c1-1,   c1-1,                c1+1,                 c1+1,
          c2-1)   c2)     c2+1)                c2+1)                 c2+1)
          
          Each leaf represents a POSSIBLE NEXT STATE.
          
          
          WHAT IS 'ans'?
          --------------
          'ans' will track the MAXIMUM cherries obtainable from any of these
          9 possible next states.
          
          Think of it as:
          "I'm standing at a fork in the road with 9 paths. I send scouts
           down each path to report back. 'ans' will remember which path
           gave the best treasure report."
        */
        
        int ans = 0;  // Initialize to 0 (worst case: all paths blocked/invalid)
        
        
        /* THE NESTED LOOP - TRYING ALL 9 COMBINATIONS:
           --------------------------------------------
           
           Outer loop: Robot 1's movement choice (x ∈ {-1, 0, 1})
           Inner loop: Robot 2's movement choice (y ∈ {-1, 0, 1})
        */
        for(int& x : colDir) {           // x represents Robot 1's column delta
            for(int& y : colDir) {       // y represents Robot 2's column delta
                
                /* Calculate next column positions:
                   
                   c1_ = next column for Robot 1 = current column + movement delta
                   c2_ = next column for Robot 2 = current column + movement delta
                   
                   Example:
                   Current: c1 = 1, c2 = 3
                   Movement: x = 1 (Robot 1 goes right), y = -1 (Robot 2 goes left)
                   Next: c1_ = 1 + 1 = 2, c2_ = 3 + (-1) = 2
                   
                   Both robots will be at column 2 in the next row!
                */
                int c1_ = x + c1;  // Robot 1's next column
                int c2_ = y + c2;  // Robot 2's next column
                
                
                /* BOUNDARY VALIDATION:
                   --------------------
                   Before we explore this path, we must ensure BOTH robots
                   stay within the grid boundaries.
                   
                   Invalid scenarios to avoid:
                   - Robot 1 moves to column -1 (fell off left edge)
                   - Robot 2 moves to column n (fell off right edge)
                   - Either robot goes out of bounds
                   
                   We only proceed if BOTH next positions are valid.
                */
                if(isSafe(c1_) && isSafe(c2_)) {
                    
                    /* ========================================================
                       THE CRITICAL RECURSIVE CALL - UNDERSTANDING 'ans'
                       ========================================================
                       
                       solve(grid, row+1, c1_, c2_) returns:
                       "Maximum cherries collectible from state (row+1, c1_, c2_)
                        all the way to the bottom"
                       
                       Let's trace through a concrete example:
                       ---------------------------------------
                       
                       Suppose we're at state (row=1, c1=0, c2=3) and trying
                       the 9 combinations:
                       
                       Iteration 1: x=0, y=-1
                       ----------------------
                       c1_ = 0+0 = 0
                       c2_ = 3+(-1) = 2
                       Call: solve(grid, 2, 0, 2)
                       Returns: 45 (let's say)
                       ans = max(0, 45) = 45
                       
                       Meaning: "If Robot1 goes straight and Robot2 goes left,
                                 we can get 45 cherries from row 2 to bottom"
                       
                       
                       Iteration 2: x=0, y=0
                       ---------------------
                       c1_ = 0+0 = 0
                       c2_ = 3+0 = 3
                       Call: solve(grid, 2, 0, 3)
                       Returns: 52 (let's say)
                       ans = max(45, 52) = 52
                       
                       Meaning: "Wait! If both go straight, we get 52 cherries!
                                 That's better than 45. Update best path!"
                       
                       
                       Iteration 3: x=1, y=-1
                       ----------------------
                       c1_ = 0+1 = 1
                       c2_ = 3+(-1) = 2
                       Call: solve(grid, 2, 1, 2)
                       Returns: 60 (let's say)
                       ans = max(52, 60) = 60
                       
                       Meaning: "Even better! Robot1 right, Robot2 left gives
                                 us 60 cherries from row 2 onwards. New best!"
                       
                       
                       ... continue for all 9 iterations ...
                       
                       
                       Final ans = 60 (after checking all 9 paths)
                       
                       This means: "Among ALL possible move combinations from
                                    this state, the best future path gives us
                                    60 cherries from row 2 to the bottom."
                       
                       
                       WHY max()?
                       ----------
                       We use max() because we want the BEST future, not just
                       any future.
                       
                       Think of it as:
                       - ans is your "high score" tracker
                       - Each recursive call is a different game outcome
                       - max() keeps only the best outcome, discarding worse ones
                       
                       
                       THE BEAUTY OF RECURSION:
                       ------------------------
                       Notice we don't manually calculate what happens at row 2,
                       row 3, etc. We TRUST that solve(grid, row+1, c1_, c2_)
                       will correctly compute the max cherries from there.
                       
                       This is the recursive leap of faith:
                       "If my function works correctly for smaller problems
                        (deeper rows), then it will work for the current problem
                        by just combining the results."
                    */
                    
                    ans = max(ans, solve(grid, row+1, c1_, c2_));
                    
                    /* After this line:
                       ans = best cherries obtainable from the next row onwards
                             considering this particular movement choice
                    */
                }
                
                /* If isSafe check fails, we simply skip this combination.
                   It doesn't contribute to our answer since it's an invalid path.
                   
                   This naturally prunes the search space - we don't waste time
                   exploring paths that lead robots off the grid.
                */
            }
        }
        
        /* After both loops complete:
           ---------------------------
           ans = MAXIMUM cherries obtainable from (row+1) to bottom,
                 considering ALL valid movement combinations
           
           This is the "optimal future" from this state.
        */
        
        
        /* ====================================================================
           STEP 3: COMBINE PRESENT + BEST FUTURE = TOTAL OPTIMAL VALUE
           ====================================================================
           
           Now we have two pieces:
           1. count = cherries collected at current row
           2. ans = best cherries from next row to bottom
           
           THE FINAL CALCULATION:
           ----------------------
           
           dp[row][c1][c2] = count + ans
           
           This represents: "Total maximum cherries from THIS state to bottom"
           
           
           CONCRETE EXAMPLE:
           -----------------
           Let's say we computed:
           - count = 8 (we collect 8 cherries at current row)
           - ans = 60 (best future path gives 60 cherries)
           
           Total optimal value from this state = 8 + 60 = 68
           
           
           INTUITION:
           ----------
           Think of it like a journey:
           
           You're at a hotel (current row)
           - You find $8 in the room (count)
           - You look at the map ahead and the best route gives you $60 more (ans)
           - Total money you'll make from here: $8 + $60 = $68
           
           
           WHY STORE IN dp[row][c1][c2]?
           -----------------------------
           MEMOIZATION! 
           
           Imagine later in the recursion tree, we reach this same state again:
           (row, c1, c2)
           
           Without memoization:
           - Recalculate count (easy)
           - Recalculate ans (requires exploring all 9 paths again!)
           - Lots of redundant work
           
           With memoization:
           - Check dp[row][c1][c2]
           - It's already 68!
           - Return immediately, no recalculation needed
           
           
           THE ASSIGNMENT CHAIN:
           ---------------------
           return dp[row][c1][c2] = count + ans;
           
           This single line does THREE things:
           
           1. COMPUTE: count + ans = 68
           2. STORE: dp[row][c1][c2] = 68 (cache for future lookups)
           3. RETURN: 68 (send back to parent caller)
           
           It's syntactic sugar for:
           int result = count + ans;
           dp[row][c1][c2] = result;
           return result;
           
           
           WHAT GETS RETURNED TO THE CALLER?
           ----------------------------------
           The caller asked: "What's the max cherries from state (row, c1, c2)?"
           We answer: "68 cherries!"
           
           The caller might be a parent state at (row-1, c1', c2') that's
           trying different move combinations and comparing results.
        */
        
        return dp[row][c1][c2] = count + ans;
        
        /* At this point:
           - We've explored all possible futures from this state
           - We've found the optimal path
           - We've cached the result
           - We've returned the answer
           
           This state is now FULLY SOLVED and will never be recalculated!
        */
    }
    
    
    int cherryPickup(vector<vector<int>>& grid) {
        // Store grid dimensions for easy access throughout recursion
        m = grid.size();        // number of rows
        n = grid[0].size();     // number of columns
        
        /* Initialize DP table with -1:
           -----------------------------
           -1 serves as a sentinel value meaning "not yet computed"
           
           When we check: if(dp[row][c1][c2] != -1)
           We're asking: "Have we solved this state before?"
           
           Any other value (0 or positive) means "already solved, use cached value"
        */
        memset(dp, -1, sizeof(dp));
        
        /* KICKSTART THE RECURSION:
           ------------------------
           Initial configuration according to problem:
           - Both robots start at row 0 (top of grid)
           - Robot 1 starts at leftmost column (column 0)
           - Robot 2 starts at rightmost column (column n-1)
           
           solve(grid, 0, 0, n-1) will:
           1. Collect cherries at row 0, columns 0 and n-1
           2. Try all 9 possible move combinations
           3. Recursively solve for each combination
           4. Return the maximum total cherries achievable
        */
        return solve(grid, 0, 0, n-1);
    }
    
    /* ========================================================================
       COMPLEXITY ANALYSIS
       ========================================================================
       
       TIME COMPLEXITY: O(m × n² × 9) = O(m × n²)
       ------------------------------------------
       - Total states: m × n × n
         (m rows, n choices for c1, n choices for c2)
       
       - Work per state: O(9)
         (trying 9 movement combinations with O(1) work each)
       
       - Each state computed EXACTLY ONCE due to memoization
       
       - Total: m × n × n × 9 = O(m × n²)
       
       
       SPACE COMPLEXITY: O(m × n²)
       ---------------------------
       - DP table: dp[71][71][71] = O(71³) = O(m × n²) when m, n ≤ 71
       - Recursion stack: O(m) depth (we go row by row downward)
       - Overall: O(m × n²) dominated by the DP table
       
    
    */
};

class Solution {
public:
    int cherryPickup(vector<vector<int>>& grid) {
        int m = grid.size();        // number of rows
        int n = grid[0].size();     // number of columns

        // dp[r][c1][c2] =
        // maximum cherries collectable from row r to bottom
        // when robot1 is at column c1 and robot2 at column c2
        static int dp[71][71][71];
        memset(dp, 0, sizeof(dp));

        // -------------------------
        // Base Case: Last Row
        // -------------------------
        // If robots are already on the last row,
        // they cannot move further.
        // So they simply collect cherries at their current cells.
        for (int c1 = 0; c1 < n; c1++) {
            for (int c2 = 0; c2 < n; c2++) {

                // Take cherries at robot1 position
                dp[m - 1][c1][c2] = grid[m - 1][c1];

                // If robots are in different columns,
                // also add cherries at robot2 position
                if (c1 != c2)
                    dp[m - 1][c1][c2] += grid[m - 1][c2];
            }
        }

        // -------------------------
        // Fill DP from bottom to top
        // -------------------------
        // Each state depends only on the row below (r + 1)
        for (int r = m - 2; r >= 0; r--) {

            for (int c1 = 0; c1 < n; c1++) {
                for (int c2 = 0; c2 < n; c2++) {

                    int bestNext = 0;  
                    // Will store the maximum cherries
                    // obtainable from next row

                    // Try all 3 possible moves for robot1
                    for (int d1 = -1; d1 <= 1; d1++) {

                        // Try all 3 possible moves for robot2
                        for (int d2 = -1; d2 <= 1; d2++) {

                            int nc1 = c1 + d1;  // next column for robot1
                            int nc2 = c2 + d2;  // next column for robot2

                            // Check boundary validity
                            if (nc1 >= 0 && nc1 < n &&
                                nc2 >= 0 && nc2 < n) {

                                // Choose the best among 9 possibilities
                                bestNext = max(bestNext,
                                               dp[r + 1][nc1][nc2]);
                            }
                        }
                    }

                    // Cherries collected at current row
                    int curr = grid[r][c1];

                    // Avoid double counting if both robots are at same cell
                    if (c1 != c2)
                        curr += grid[r][c2];

                    // Total cherries =
                    // current row cherries + best future cherries
                    dp[r][c1][c2] = curr + bestNext;
                }
            }
        }

        // Robots start at row 0:
        // robot1 at column 0
        // robot2 at column n-1
        return dp[0][0][n - 1];
    }
};



// BOTTOM UP 

class Solution {
public:
    int cherryPickup(vector<vector<int>>& grid) {
        /*
         * PROBLEM UNDERSTANDING:
         * =====================
         * - Two robots start simultaneously from top row: Robot1 at column 0, Robot2 at column n-1
         * - Both move downward row by row, collecting cherries from cells they visit
         * - Each robot can move to: (row+1, col-1), (row+1, col), or (row+1, col+1)
         * - If both robots land on same cell, cherries are counted only ONCE (not twice)
         * - Goal: Maximize total cherries collected when both reach the last row
         * 
         * CORE INSIGHT - Why 3D DP?:
         * ==========================
         * - At any row, we need to track BOTH robots' positions simultaneously
         * - State must capture: "Robot1 is at column c1 AND Robot2 is at column c2 in same row"
         * - We can't optimize them independently because their positions interact (overlap case)
         * - Hence: dp[row][c1][c2] represents the state of both robots together
         * 
         * STATE DEFINITION:
         * =================
         * t[row][c1][c2] = Maximum cherries collected when:
         *                  - Both robots have moved from row 0 to current 'row'
         *                  - Robot1 is currently at position (row, c1)
         *                  - Robot2 is currently at position (row, c2)
         * 
         * WHY THIS WORKS - The Subproblem Structure:
         * ===========================================
         * - Once we know the best way to reach (row, c1, c2), we can build on it
         * - For next row, we consider all 9 possible moves (3 for R1 × 3 for R2)
         * - This creates overlapping subproblems perfect for DP memoization
         */
        
        int m = grid.size();
        int n = grid[0].size();
        
        int t[71][71][71] = {0};  // Initialize all states to 0 (unreachable by default)
        
        /*
         * BASE CASE - Row 0 Initialization:
         * ==================================
         * Robot1 starts at (0, 0), Robot2 starts at (0, n-1)
         * 
         * Special handling for n==1:
         * - If grid has only 1 column, both robots are at same cell (0,0)
         * - Count cherries only once: grid[0][0]
         * 
         * Otherwise:
         * - Robots at different cells, sum both: grid[0][0] + grid[0][n-1]
         */
        t[0][0][n - 1] = (n == 1) ? grid[0][0] : grid[0][0] + grid[0][n - 1]; 
        
        /*
         * BOTTOM-UP DP - Building Solutions Row by Row:
         * ==============================================
         * Process each row sequentially, computing all reachable (c1, c2) states
         */
        for (int row = 1; row < m; row++) {
            
            /*
             * OPTIMIZATION - Pruning Unreachable States:
             * ==========================================
             * Not all column combinations are reachable at each row!
             * 
             * Robot1 analysis (starts at column 0):
             * - In 'row' moves, can move at most 'row' steps right (always choosing +1)
             * - Therefore: c1 ≤ min(n-1, row)
             * - Can't reach c1=50 if we're only at row=10
             * 
             * Robot2 analysis (starts at column n-1):
             * - In 'row' moves, can move at most 'row' steps left (always choosing -1)
             * - Starting from n-1, after 'row' moves left: n-1-row
             * - Therefore: c2 ≥ max(0, n-1-row)
             * - Can't reach c2=0 if we're at row=2 and n=100
             * 
             * This pruning dramatically reduces computation from O(n²) to O(row²) states per row
             */
            for (int c1 = 0; c1 <= min(n-1, row); c1++) {
                for (int c2 = max(0, n-1-row); c2 < n; c2++) {
                    
                    /*
                     * TRANSITION - Finding Best Previous State:
                     * ==========================================
                     * To reach current (row, c1, c2), we examine ALL valid previous states
                     * 
                     * Each robot independently could have come from:
                     * - column-1 (moved diagonally right)
                     * - column   (moved straight down)
                     * - column+1 (moved diagonally left)
                     * 
                     * This gives us 3 × 3 = 9 possible previous configurations
                     * We take the MAXIMUM because we want optimal path
                     */
                    int prev_max = 0;
                    
                    // Iterate through all valid previous columns for Robot1
                    for (int column1 = max(0, c1 - 1); column1 <= min(n - 1, c1 + 1); column1++) {
                        
                        // Iterate through all valid previous columns for Robot2
                        for (int column2 = max(0, c2 - 1); column2 <= min(n - 1, c2 + 1); column2++) {
                            
                            /*
                             * Consider transition from state t[row-1][column1][column2]
                             * The max() ensures we pick the best among all 9 possibilities
                             */
                            prev_max = max(prev_max, t[row - 1][column1][column2]);
                        }
                    }
                    
                    /*
                     * CHERRY COLLECTION - Handling Overlap:
                     * =====================================
                     * Critical case distinction:
                     * 
                     * If c1 == c2 (both robots at same cell):
                     * - Only ONE robot's cherries count (can't double-count same cell)
                     * - Add: prev_max + grid[row][c1]
                     * 
                     * If c1 != c2 (robots at different cells):
                     * - Both robots collect independently
                     * - Add: prev_max + grid[row][c1] + grid[row][c2]
                     * 
                     * This single condition elegantly handles the overlap constraint
                     */
                    if (c1 == c2)
                        t[row][c1][c2] = prev_max + grid[row][c1];
                    else
                        t[row][c1][c2] = prev_max + grid[row][c1] + grid[row][c2];
                }
            }
        }
        
        /*
         * ANSWER EXTRACTION - Finding Optimal Ending:
         * ============================================
         * Unlike the start (fixed positions), robots can end at ANY valid column pair
         * We must check all possible (c1, c2) combinations in the last row
         * and return the maximum among them
         * 
         * This reflects that we don't control where robots end, only optimize their paths
         */
        int res = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                res = max(res, t[m - 1][i][j]);    
            }
        }
        return res;
        
        /*
         * TIME COMPLEXITY: O(m × n² × 9) = O(m × n²)
         * - m rows to process
         * - For each row: O(row²) → O(n²) reachable states in worst case
         * - For each state: 9 transition checks (constant)
         * 
         * SPACE COMPLEXITY: O(m × n²)
         * - 3D DP table of size m × n × n
         * - Could optimize to O(n²) by keeping only current and previous row
         */
    }
};