class Solution {
public:
    int m, n;
    vector<vector<int>> dp;

    /*
    ============================================================================
    PROBLEM: UNIQUE PATHS (Grid Navigation)
    ============================================================================
    
    Given an m×n grid, find the number of unique paths from top-left (0,0) 
    to bottom-right (m-1, n-1). You can only move RIGHT or DOWN.
    
    Example: 3×3 grid
    START → → →
      ↓   ↓   ↓
      ↓   ↓   ↓
      ↓   ↓   ↓ END
    
    ============================================================================
    CORE INTUITION: Path Decomposition Principle
    ============================================================================
    
    KEY INSIGHT: Any path to destination = sum of paths through adjacent cells
    
    Think of it like water flowing through a grid:
    - Water at cell (i,j) can flow RIGHT to (i, j+1) OR DOWN to (i+1, j)
    - Total water reaching destination = water through right cell + water through down cell
    - Each "unit of water" represents one unique path
    
    MATHEMATICAL FOUNDATION:
    If we denote paths(i,j) as "number of ways to reach (m-1, n-1) from (i,j)"
    Then: paths(i,j) = paths(i, j+1) + paths(i+1, j)
    
    This is a RECURRENCE RELATION - solving a big problem by solving smaller
    subproblems. The brilliance is that we're working BACKWARDS from destination.
    
    WHY THIS WORKS:
    1. Principle of Optimality: Any path from (i,j) to destination MUST go 
       through either (i, j+1) or (i+1, j) as its next step
    2. Non-overlapping subproblems: Paths going right are distinct from paths 
       going down
    3. Additive property: Total paths = sum of paths through each possibility
    
    ============================================================================
    WHY DYNAMIC PROGRAMMING?
    ============================================================================
    
    Without DP, we'd recalculate the same cell multiple times:
    
    To reach (0,0):
      - We calculate paths from (0,1) and (1,0)
      - To calculate (0,1), we need (0,2) and (1,1)
      - To calculate (1,0), we need (1,1) and (2,0)
      - Notice (1,1) is calculated TWICE!
    
    In a 3×3 grid, without memoization, we'd make ~20 recursive calls
    With memoization, we make exactly 9 calls (one per cell)
    
    COMBINATORICS CONNECTION:
    This problem is actually a disguised combinatorics problem!
    To go from (0,0) to (m-1, n-1):
    - We need exactly (m-1) DOWN moves
    - We need exactly (n-1) RIGHT moves
    - Total moves = (m-1) + (n-1) = m+n-2
    
    The question becomes: "In how many ways can we arrange (m-1) D's and (n-1) R's?"
    This is: C(m+n-2, m-1) = (m+n-2)! / ((m-1)! × (n-1)!)
    
    Our DP solution computes this same value by recursively counting paths.
    
    ============================================================================
    */

    // ========================================================================
    // APPROACH 1: TOP-DOWN MEMOIZATION (Recursion + Cache)
    // ========================================================================
    /*
    MENTAL MODEL: "From here, where can I go?"
    - Start at (0,0), ask "how many ways to reach end?"
    - Try going RIGHT or DOWN
    - Recursively solve smaller subproblems
    - Cache results to avoid recomputation
    
    dp[i][j] = "Number of paths FROM (i,j) TO destination (m-1,n-1)"
    
    Time: O(m×n) - each cell computed once
    Space: O(m×n) for dp + O(m+n) for recursion stack
    */
    
    int solve(int i, int j) {
        // BASE CASE 1: Out of Bounds (Boundary Violation)
        // If we've stepped outside the grid, this path is INVALID
        // Return 0 because there are ZERO ways to reach destination from 
        // an invalid position
        // Think of it like hitting a wall - you can't continue
        if (i >= m || j >= n) return 0;

        // BASE CASE 2: Destination Reached (Success Condition)
        // We've successfully reached the bottom-right corner!
        // Return 1 because we've found EXACTLY ONE valid path
        // This is the "leaf node" of our recursion tree where we say:
        // "Yes! This sequence of moves is a valid path, count it!"
        if (i == m - 1 && j == n - 1) return 1;

        // MEMOIZATION CHECK: Have we solved this subproblem before?
        // dp[i][j] stores: "How many paths exist from (i,j) to destination?"
        // If dp[i][j] != -1, we've already computed this during a 
        // previous recursive call. Return cached result immediately.
        // This is the HEART of dynamic programming - trading space for time
        if (dp[i][j] != -1)
            return dp[i][j];

        // RECURSIVE CASE: The Path Decomposition
        // We're at cell (i,j) and asking: "How many ways to reach the end?"
        // 
        // We have exactly TWO choices:
        // 1. Move RIGHT to cell (i, j+1)
        // 2. Move DOWN to cell (i+1, j)
        // 
        // WHY ADD THEM?
        // These are INDEPENDENT choices - every path that goes right 
        // first is DIFFERENT from every path that goes down first.
        // 
        // If there are X paths from (i, j+1) to end, and Y paths from 
        // (i+1, j) to end, then there are X+Y total paths from (i,j) to end.
        // 
        // We calculate AND store the result in dp[i][j] simultaneously
        return dp[i][j] = solve(i, j + 1) + solve(i + 1, j);
        //     └─────┬─────┘   └──────┬──────┘   └──────┬──────┘
        //          Store        Paths going        Paths going
        //          result         RIGHT              DOWN
    }

    int uniquePaths_TopDown(int M, int N) {
        m = M;
        n = N;
        
        // Create m×n table initialized with -1
        // WHY -1? We need to distinguish:
        //   - "Not yet computed" (marked as -1)
        //   - "Computed and result is 0" (a cell might have 0 paths)
        dp.assign(m, vector<int>(n, -1));
        
        // Start from top-left corner (0,0)
        // The recursion will:
        // 1. Explore all possible paths (right and down moves)
        // 2. Hit base cases (destination or out of bounds)
        // 3. Bubble up counts through recursion stack
        // 4. Memoize results to avoid redundant work
        return solve(0, 0);
    }
};

/*
===============================================================================
EXAMPLE WALKTHROUGH - TOP-DOWN: 3×2 Grid
===============================================================================

Grid visualization:
(0,0) → (0,1)
  ↓       ↓
(1,0) → (1,1)
  ↓       ↓
(2,0) → (2,1) ← DESTINATION

Call tree (simplified):
solve(0,0)
├── solve(0,1)
│   ├── solve(0,2) → OUT OF BOUNDS → returns 0
│   └── solve(1,1)
│       ├── solve(1,2) → OUT OF BOUNDS → returns 0
│       └── solve(2,1) → DESTINATION → returns 1
│       Result: 0 + 1 = 1
│   Result: 0 + 1 = 1
└── solve(1,0)
    ├── solve(1,1) → ALREADY COMPUTED → returns 1 (memoized!)
    └── solve(2,0)
        ├── solve(2,1) → DESTINATION → returns 1
        └── solve(3,0) → OUT OF BOUNDS → returns 0
        Result: 1 + 0 = 1
    Result: 1 + 1 = 2
Result: 1 + 2 = 3

Answer: 3 unique paths
1. (0,0)→(0,1)→(1,1)→(2,1)  [Right, Down, Down]
2. (0,0)→(1,0)→(1,1)→(2,1)  [Down, Right, Down]
3. (0,0)→(1,0)→(2,0)→(2,1)  [Down, Down, Right]

===============================================================================
*/

class Solution {
public:
    // ========================================================================
    // APPROACH 2: FORWARD TABULATION (Bottom-Up DP from Start)
    // ========================================================================
    /*
    PARADIGM SHIFT: Top-Down vs Bottom-Up Thinking
    
    TOP-DOWN (Recursion + Memoization):
    - "From (i,j), how many ways to reach END?"
    - Works BACKWARDS from current position
    - Natural recursion but uses call stack
    
    FORWARD TABULATION (This approach):
    - "To reach (i,j), how many ways FROM START?"
    - Works FORWARDS from start to goal
    - Iterative, no recursion stack overhead
    
    ========================================================================
    THE CORE INSIGHT: Path Accumulation (Building Up Solutions)
    ========================================================================
    
    Think of dp[i][j] as a "path counter" that accumulates paths:
    
    WATER FLOW ANALOGY (Forward Direction):
    Imagine water starting at (0,0) and flowing through the grid:
    - Water can only flow RIGHT or DOWN
    - Each cell accumulates water from cells above and left
    - Amount of water = number of different paths that reached this cell
    
    MATHEMATICAL FORMULATION:
    dp[i][j] = "Number of unique paths from START (0,0) to cell (i,j)"
    
    Recurrence Relation (Building Forward):
    dp[i][j] = dp[i-1][j] + dp[i][j-1]
              └────┬────┘   └────┬────┘
           Paths coming    Paths coming
            from ABOVE      from LEFT
    
    WHY THIS WORKS:
    - To reach cell (i,j), you MUST come from either:
      1. Cell above: (i-1, j) by moving DOWN
      2. Cell left: (i, j-1) by moving RIGHT
    - No other cell can reach (i,j) directly (only right/down moves allowed)
    - Total paths TO (i,j) = paths TO top neighbor + paths TO left neighbor
    
    ========================================================================
    ADVANTAGES OVER MEMOIZATION:
    ========================================================================
    1. NO RECURSION STACK: Saves O(m+n) space from call stack
    2. CACHE-FRIENDLY: Sequential memory access (better CPU cache hits)
    3. PREDICTABLE: Fills table in systematic order
    4. EASIER TO OPTIMIZE: Can reduce space from O(m×n) to O(n)
    5. NO STACK OVERFLOW: Safe for very large grids
    
    Time: O(m×n) - fill entire table once
    Space: O(m×n) - the dp table
    */
    
    int uniquePaths(int m, int n) {
        // TABLE INITIALIZATION: Creating Our Canvas
        // Create an m×n table initialized to 0
        // 
        // WHY INITIALIZE TO 0?
        // Zero means "no paths have reached this cell yet"
        // As we iterate, we'll accumulate paths from previous cells
        // 
        // SEMANTIC MEANING:
        // dp[i][j] = "Total number of unique paths from START (0,0) to (i,j)"
        // 
        // This is OPPOSITE to memoization where dp[i][j] meant 
        // "paths from (i,j) to END"
        vector<vector<int>> dp(m, vector<int>(n, 0));
        
        // BASE CASE: The Seed of All Paths
        // Mark the starting cell with 1
        // 
        // INTUITION:
        // "There is exactly ONE way to reach the start from the start: 
        //  by already being there!"
        // 
        // WHY IS THIS CRUCIAL?
        // This is the SEED value that propagates through the entire grid
        // Without this, every cell would remain 0 (no paths anywhere)
        // 
        // DOMINO EFFECT:
        // dp[0][0] = 1 → dp[0][1] gets 1 → dp[0][2] gets 1 → ...
        //            ↓
        //         dp[1][0] gets 1 → dp[1][1] gets 2 → ...
        dp[0][0] = 1;
        
        // MAIN ITERATION: Building the Solution Bottom-Up
        // We iterate through EVERY cell in the grid systematically
        // 
        // ITERATION ORDER MATTERS:
        // We go left-to-right, top-to-bottom (row by row)
        // 
        // WHY THIS ORDER?
        // Because dp[i][j] depends on dp[i-1][j] (above) and dp[i][j-1] (left)
        // By going left-to-right, top-to-bottom, we GUARANTEE that when we 
        // compute dp[i][j], the cells it depends on are ALREADY computed
        // 
        // DEPENDENCY GRAPH:
        // Each cell depends on cells that are "behind" it in our iteration
        // This is called a TOPOLOGICAL ORDER of dependencies
        // 
        //   (i-1,j-1) → (i-1,j)     ← Already processed
        //        ↓          ↓
        //    (i,j-1)  → (i,j)       ← Current cell depends on above and left
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                // SKIP THE BASE CASE: Already Handled
                // We already set dp[0][0] = 1 above
                // If we process it again, we'd overwrite it with 0+0=0
                // This skip preserves our seed value
                if (i == 0 && j == 0) continue;
                
                // GATHER PATHS FROM ABOVE: Vertical Contribution
                // How many paths reached the cell directly ABOVE current cell?
                // 
                // BOUNDARY CHECK: (i > 0)
                // If i == 0, we're in the TOP ROW - there's no cell above!
                // In that case, contribution from above is 0
                // 
                // LOGIC:
                // If cell (i-1, j) has X paths reaching it, then all X of those
                // paths can extend DOWN to reach (i, j)
                // So cell (i,j) inherits X paths from above
                // 
                // EDGE CASE - Top row (i==0):
                // Can only be reached from the left
                // dp[0][j] = dp[0][j-1] (one path: right→right→right)
                int fromTop  = (i > 0) ? dp[i - 1][j] : 0;
                
                // GATHER PATHS FROM LEFT: Horizontal Contribution
                // How many paths reached the cell directly LEFT of current cell?
                // 
                // BOUNDARY CHECK: (j > 0)
                // If j == 0, we're in LEFTMOST COLUMN - no cell to the left!
                // In that case, contribution from left is 0
                // 
                // LOGIC:
                // If cell (i, j-1) has Y paths reaching it, then all Y of those
                // paths can extend RIGHT to reach (i, j)
                // So cell (i,j) inherits Y paths from left
                // 
                // EDGE CASE - Left column (j==0):
                // Can only be reached from above
                // dp[i][0] = dp[i-1][0] (one path: down→down→down)
                int fromLeft = (j > 0) ? dp[i][j - 1] : 0;
                
                // THE ACCUMULATION STEP: Combining Path Counts
                // This is the HEART of the algorithm!
                // 
                // COMBINATORIAL PRINCIPLE:
                // Total ways to reach (i,j) = ways from above + ways from left
                // 
                // WHY ADD?
                // Because these are DISJOINT sets of paths:
                // - Paths coming from above took their last step as DOWN
                // - Paths coming from left took their last step as RIGHT
                // - No path can be in both categories
                // - Together, they represent ALL possible paths to (i,j)
                // 
                // MATHEMATICAL PROOF:
                // Let A = set of paths from (0,0) to (i,j) via (i-1,j)
                // Let B = set of paths from (0,0) to (i,j) via (i,j-1)
                // A and B are disjoint (A ∩ B = ∅)
                // A ∪ B = all paths to (i,j)
                // Therefore: |A ∪ B| = |A| + |B|
                // 
                // CONCRETE EXAMPLE:
                // If dp[i-1][j] = 3 and dp[i][j-1] = 2
                // Then dp[i][j] = 3 + 2 = 5
                // Meaning: 3 distinct paths arrive from above, 2 from left,
                //          totaling 5 unique paths to this cell
                dp[i][j] = fromTop + fromLeft;
                
                // IMPLICIT PROPAGATION: The Beauty of DP
                // Notice we don't track individual paths - just COUNT them!
                // Each cell becomes a "stepping stone" for future cells
                // The count propagates forward like a wave through the grid
            }
        }
        
        // RETURN THE FINAL ANSWER: Destination Cell
        // dp[m-1][n-1] now contains the total number of unique paths
        // from (0,0) to (m-1, n-1)
        // 
        // WHY THIS CELL?
        // Because it's the bottom-right corner - our destination!
        // As the last cell to be filled, it has accumulated path counts
        // from all preceding cells that could reach it
        return dp[m - 1][n - 1];
    }
};

/*
===============================================================================
EXAMPLE WALKTHROUGH - FORWARD TABULATION: 3×3 Grid
===============================================================================

Initial State (after base case):
     j=0  j=1  j=2
i=0   1    0    0
i=1   0    0    0
i=2   0    0    0

Step-by-Step Filling:

(0,1): fromTop=0, fromLeft=dp[0][0]=1 → dp[0][1]=1
(0,2): fromTop=0, fromLeft=dp[0][1]=1 → dp[0][2]=1

     j=0  j=1  j=2
i=0   1    1    1     ← First row: only ONE path (keep going right)
i=1   0    0    0
i=2   0    0    0

(1,0): fromTop=dp[0][0]=1, fromLeft=0 → dp[1][0]=1
(1,1): fromTop=dp[0][1]=1, fromLeft=dp[1][0]=1 → dp[1][1]=2
       Two paths reach (1,1):
       Path 1: (0,0) → RIGHT → (0,1) → DOWN → (1,1)
       Path 2: (0,0) → DOWN → (1,0) → RIGHT → (1,1)

(1,2): fromTop=dp[0][2]=1, fromLeft=dp[1][1]=2 → dp[1][2]=3

     j=0  j=1  j=2
i=0   1    1    1
i=1   1    2    3
i=2   0    0    0

(2,0): fromTop=dp[1][0]=1, fromLeft=0 → dp[2][0]=1
(2,1): fromTop=dp[1][1]=2, fromLeft=dp[2][0]=1 → dp[2][1]=3
(2,2): fromTop=dp[1][2]=3, fromLeft=dp[2][1]=3 → dp[2][2]=6

Final DP Table:
     j=0  j=1  j=2
i=0   1    1    1
i=1   1    2    3
i=2   1    3    6    ← Answer: 6 unique paths!

The 6 paths: R→R→D→D, R→D→R→D, R→D→D→R, D→R→R→D, D→R→D→R, D→D→R→R

===============================================================================
*/

class Solution {
public:
    // ========================================================================
    // APPROACH 3: REVERSE TABULATION (Bottom-Up DP from End)
    // ========================================================================
    /*
    CONCEPTUAL BREAKTHROUGH: Three Ways to Think About DP
    
    1. TOP-DOWN MEMOIZATION:
       - Recursion from start, cache results
       - dp[i][j] = "paths FROM (i,j) TO end"
       - Uses call stack
    
    2. FORWARD TABULATION:
       - Iteration from start to end
       - dp[i][j] = "paths FROM start TO (i,j)"
       - Asks: "How did I get here?"
    
    3. REVERSE TABULATION (This approach):
       - Iteration from end to start
       - dp[i][j] = "paths FROM (i,j) TO end"
       - Asks: "Where can I go from here?"
    
    ========================================================================
    WHY REVERSE TABULATION?
    ========================================================================
    
    MENTAL MODEL: "Planning a Journey Backward"
    
    Imagine you're at various points in the grid and asking:
    "If I'm here, how many different ways can I reach the destination?"
    
    NATURAL INTUITION:
    - At destination: "I'm already here! 1 way"
    - One cell away: "I can reach it in 1 step, so 1 way"
    - Two cells away: "I can go through 2 neighbors, so sum their ways"
    
    SEMANTIC ALIGNMENT:
    - Matches the recursive/memoization approach EXACTLY
    - Same subproblem definition: "paths from (i,j) to end"
    - Easier conversion from top-down to bottom-up
    
    WHEN TO PREFER THIS:
    - Converting from top-down memoization
    - Problem naturally asks "where can I go?"
    - Dependencies flow from bottom-right to top-left
    
    ========================================================================
    THE CORE RECURRENCE
    ========================================================================
    
    dp[i][j] = dp[i][j+1] + dp[i+1][j]
              └─────┬────┘   └────┬────┘
           Paths going      Paths going
              RIGHT            DOWN
    
    INTERPRETATION:
    "To find paths from (i,j) to destination, sum paths from:
     - The cell to my RIGHT (if I move right)
     - The cell BELOW me (if I move down)"
    
    BASE CASE: dp[m-1][n-1] = 1 (already at destination)
    
    ITERATION ORDER CRITICAL:
    - Must go RIGHT-to-LEFT, BOTTOM-to-TOP
    - Ensures cells we depend on (right & below) are already computed
    
    Time: O(m×n)
    Space: O(m×n)
    */
    
    int uniquePaths(int m, int n) {
        // TABLE INITIALIZATION: Preparing the Canvas
        // Create an m×n table initialized to 0
        // 
        // SEMANTIC MEANING:
        // dp[i][j] = "Number of unique paths FROM (i,j) TO destination"
        // 
        // This is the SAME semantic as recursive memoization!
        // Difference: we build iteratively instead of recursively
        // 
        // WHY INITIALIZE TO 0?
        // Zero means "no paths known yet from this cell"
        // As we iterate backward, we'll fill in actual counts
        // Cells outside bounds naturally remain 0
        vector<vector<int>> dp(m, vector<int>(n, 0));
        
        // BASE CASE: The Anchor Point
        // Mark the destination cell with 1
        // 
        // PROFOUND INTUITION:
        // "If I'm already at the destination, there's exactly ONE way to 
        //  'reach' it: I'm already there!"
        // 
        // SEED VALUE:
        // This is the ONLY cell we initialize explicitly
        // All other values will be DERIVED from this seed
        // Like dropping a stone in water - ripples propagate outward
        // 
        // DOMINO EFFECT (working backward):
        // dp[m-1][n-1] = 1 ← dp[m-1][n-2] reads this
        //                  ↑
        //             dp[m-2][n-1] reads this
        //                  ↑
        //             dp[m-2][n-2] reads both (gets 2)
        dp[m - 1][n - 1] = 1;
        
        // MAIN ITERATION: Building Backward from Destination
        // We iterate through EVERY cell in the grid
        // 
        // *** CRITICAL: ITERATION ORDER ***
        // We go RIGHT-TO-LEFT, BOTTOM-TO-TOP (reverse order)
        // 
        // WHY THIS ORDER IS ESSENTIAL:
        // Because dp[i][j] depends on:
        //   - dp[i][j+1] (cell to the RIGHT, larger j)
        //   - dp[i+1][j] (cell BELOW, larger i)
        // 
        // By iterating i from m-1 down to 0, and j from n-1 down to 0,
        // we GUARANTEE that when we compute dp[i][j], the cells it depends 
        // on (further right and down) are ALREADY computed
        // 
        // DEPENDENCY GRAPH:
        //    (i,j)    → (i,j+1)        Current depends on cells
        //      ↓          ↓            "ahead" in reverse iteration
        //   (i+1,j)  → (i+1,j+1)       
        // 
        // COMPUTATION ORDER:
        // Start: (m-1,n-1) [base case]
        // Then : (m-1,n-2) ← depends on (m-1,n-1) already done
        // Then : (m-2,n-1) ← depends on (m-1,n-1) already done
        // Then : (m-2,n-2) ← depends on both neighbors already done
        // ...
        // End  : (0,0) ← all dependencies satisfied
        for (int i = m - 1; i >= 0; i--) {
            for (int j = n - 1; j >= 0; j--) {
                // SKIP THE BASE CASE: Preserve the Seed
                // We already set dp[m-1][n-1] = 1
                // If we process it again:
                //   right = 0 (no cell to right)
                //   down  = 0 (no cell below)
                //   dp[m-1][n-1] = 0 + 0 = 0  ← WRONG!
                // This skip protects our base case
                if (i == m - 1 && j == n - 1) continue;
                
                // GATHER PATHS FROM RIGHT: Horizontal Forward
                // If I move RIGHT from (i,j) to (i,j+1), how many paths 
                // exist from that next cell to destination?
                // 
                // BOUNDARY CHECK: (j + 1 < n)
                // If j+1 >= n, we'd step OUT OF BOUNDS (off right edge)
                // Can't go right from rightmost column
                // This direction contributes 0 paths
                // 
                // LOGIC CHAIN:
                // 1. If (i,j+1) has X paths to destination
                // 2. I can reach (i,j+1) by moving right from (i,j)
                // 3. Those X paths are accessible via right move
                // 4. So (i,j) inherits X paths through "right" option
                // 
                // EDGE - RIGHTMOST COLUMN (j = n-1):
                // Can't go right, must go down
                // dp[i][n-1] = 0 + dp[i+1][n-1]
                // Creates "straight line" of 1's down right edge
                int right = (j + 1 < n) ? dp[i][j + 1] : 0;
                
                // GATHER PATHS FROM BELOW: Vertical Forward
                // If I move DOWN from (i,j) to (i+1,j), how many paths 
                // exist from that next cell to destination?
                // 
                // BOUNDARY CHECK: (i + 1 < m)
                // If i+1 >= m, we'd step OUT OF BOUNDS (off bottom edge)
                // Can't go down from bottom row
                // This direction contributes 0 paths
                // 
                // LOGIC CHAIN:
                // 1. If (i+1,j) has Y paths to destination
                // 2. I can reach (i+1,j) by moving down from (i,j)
                // 3. Those Y paths are accessible via down move
                // 4. So (i,j) inherits Y paths through "down" option
                // 
                // EDGE - BOTTOM ROW (i = m-1):
                // Can't go down, must go right
                // dp[m-1][j] = dp[m-1][j+1] + 0
                // Creates "straight line" of 1's across bottom edge
                int down  = (i + 1 < m) ? dp[i + 1][j] : 0;
                
                // THE ACCUMULATION: Combining Path Counts
                // 
                // FUNDAMENTAL PRINCIPLE:
                // Total paths from (i,j) = paths via right + paths via down
                // 
                // WHY ADD?
                // These represent MUTUALLY EXCLUSIVE choices:
                // - From (i,j), I choose: go RIGHT or go DOWN
                // - If RIGHT: access all paths through (i,j+1)
                // - If DOWN: access all paths through (i+1,j)
                // - Sets are DISJOINT (no overlap)
                // - Together: represent EVERY path from (i,j)
                // 
                // MATHEMATICAL FORMALIZATION:
                // Let R = paths from (i,j) that move right first
                // Let D = paths from (i,j) that move down first
                // 
                // Properties:
                // 1. R ∩ D = ∅ (disjoint: can't go both ways first)
                // 2. R ∪ D = all paths from (i,j)
                // 3. |R| = dp[i][j+1]
                // 4. |D| = dp[i+1][j]
                // 5. |R ∪ D| = |R| + |D|
                // 
                // Therefore: dp[i][j] = dp[i][j+1] + dp[i+1][j]
                // 
                // EXAMPLE:
                // If dp[i][j+1] = 3 and dp[i+1][j] = 5
                // Then dp[i][j] = 8
                // From (i,j): 3 ways starting right, 5 ways starting down
                // 
                // RIPPLE EFFECT:
                // Once computed, dp[i][j] becomes available for cells
                // that depend on it (cells above and left)
                dp[i][j] = right + down;
                
                // IMPLICIT STATE PROPAGATION
                // We never track which paths are which - just COUNT!
                // Each cell aggregates counts from cells ahead
                // By time we reach (0,0), it has accumulated ALL paths
            }
        }
        
        // RETURN THE FINAL ANSWER: Starting Cell
        // dp[0][0] = answer to "How many paths from (0,0) to (m-1,n-1)?"
        // 
        // WHY THIS CELL?
        // It's the top-left corner - our starting point!
        // As LAST cell computed in reverse iteration,
        // it has accumulated info from ALL cells in grid
        // 
        // CONCEPTUAL CLOSURE:
        // Started at END, worked backward to START
        // Now return value at START, telling us how to get to END
        return dp[0][0];
    }
};

/*
===============================================================================
EXAMPLE WALKTHROUGH - REVERSE TABULATION: 3×3 Grid
===============================================================================

Initial (after base): dp[2][2] = 1

     j=0  j=1  j=2
i=0   0    0    0
i=1   0    0    0
i=2   0    0    1    ← Start here

Iterate RIGHT-TO-LEFT, BOTTOM-TO-TOP:

(2,1): right=dp[2][2]=1, down=0 → dp[2][1]=1
(2,0): right=dp[2][1]=1, down=0 → dp[2][0]=1

     j=0  j=1  j=2
i=0   0    0    0
i=1   0    0    0
i=2   1    1    1    ← Bottom: one path (go right)

(1,2): right=0, down=dp[2][2]=1 → dp[1][2]=1
(1,1): right=dp[1][2]=1, down=dp[2][1]=1 → dp[1][1]=2
       From (1,1): 2 ways to destination
       Way 1: RIGHT→DOWN
       Way 2: DOWN→RIGHT
(1,0): right=dp[1][1]=2, down=dp[2][0]=1 → dp[1][0]=3

     j=0  j=1  j=2
i=0   0    0    0
i=1   3    2    1
i=2   1    1    1

(0,2): right=0, down=dp[1][2]=1 → dp[0][2]=1
(0,1): right=dp[0][2]=1, down=dp[1][1]=2 → dp[0][1]=3
(0,0): right=dp[0][1]=3, down=dp[1][0]=3 → dp[0][0]=6

Final:
     j=0  j=1  j=2
i=0   6    3    1    ← Answer!
i=1   3    2    1
i=2   1    1    1

Same 6 paths as other approaches!

===============================================================================
COMPARISON: THREE APPROACHES SIDE-BY-SIDE
===============================================================================

FEATURE             | TOP-DOWN       | FORWARD TAB    | REVERSE TAB
--------------------|----------------|----------------|----------------
Direction           | Backward       | Forward        | Backward
Semantic            | FROM (i,j) TO  | FROM start TO  | FROM (i,j) TO
                    | end            | (i,j)          | end
Iteration           | Recursive      | L→R, T→B       | R→L, B→T
Base Case           | Destination=1  | Start=1        | Destination=1
Depends On          | Right & Below  | Top & Left     | Right & Below
Natural When        | Thinking ahead | Building up    | Converting
                    |                |                | from recursion
Space (extra)       | O(m+n) stack   | None           | None
Implementation      | Most intuitive | Standard       | Matches recursion

All three: Time O(m×n), Space O(m×n) for dp table

===============================================================================
SPACE OPTIMIZATION (Works for Forward & Reverse)
===============================================================================

Since each cell only depends on current/previous row, reduce to O(n):

vector<int> prev(n, 0), curr(n, 0);
prev[0] = 1;  // For forward
for (int i = 0; i < m; i++) {
    curr[0] = 1;
    for (int j = 1; j < n; j++) {
        curr[j] = curr[j-1] + prev[j];  // left + top
    }
    swap(prev, curr);
}
return prev[n-1];

This is the ULTIMATE optimization - O(n) space!

===============================================================================
*/