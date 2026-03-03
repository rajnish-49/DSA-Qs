// ============================================================
// PROBLEM: Largest Magic Square
// LeetCode 1895
// ============================================================
//
// WHAT THE PROBLEM ASKS:
// Given an m x n integer grid, find the largest "magic square"
// that can be found as a contiguous sub-grid within it.
// Return the SIZE (side length) of that magic square.
//
// DEFINITION OF MAGIC SQUARE:
// A k x k magic square is a grid filled with distinct integers
// such that every row sum, every column sum, the main diagonal
// sum, and the anti-diagonal sum are ALL equal to one another.
// This common value is called the "magic constant".
//
// IMPORTANT CLARIFICATION FOR THIS PROBLEM:
// LeetCode's version does NOT require the numbers to be distinct.
// The only requirement is that all row sums, column sums, and
// both diagonal sums are equal within the sub-grid.
//
// CONSTRAINTS AND IMPLICATIONS:
// - 1 <= m, n <= 50   (small grid, allows O(n^5) brute force)
// - 1 <= grid[i][j] <= 10^6
// - A 1x1 square is always magic (trivially), so answer >= 1
// - The maximum possible side is min(m, n)
//
// EDGE CASES:
// - Entire grid is 1x1 → return 1
// - No sub-grid of size >= 2 qualifies → return 1
// - Multiple magic squares of same size → return the size once
//
// WHY NAIVE APPROACH IS SLOW (but still acceptable here):
// Brute force: for every top-left corner (i,j) and every size k,
// check all k rows, k columns, and 2 diagonals.
// This is O(m * n * min(m,n) * k) ≈ O(n^5) worst case.
// For n=50, that's 50^5 = 312,500,000 → tight but passable.
// We can optimize row/column checks using prefix sums → O(n^4).
//
// ============================================================
// THINKING FROM FIRST PRINCIPLES
// ============================================================
//
// STEP 1: What defines a magic square check?
// For a sub-grid starting at (i,j) with side k:
//   - Compute the "target" sum = sum of first row of sub-grid
//   - Every other row must match that target
//   - Every column must match that target
//   - Main diagonal (top-left to bottom-right) must match
//   - Anti-diagonal (top-right to bottom-left) must match
//
// STEP 2: How to speed up row/column sum queries?
// If we precompute prefix sums along rows and columns separately,
// any contiguous segment sum becomes an O(1) lookup.
//
// Row prefix sum:   rowPS[i][j] = sum of grid[i][0..j]
//   → sum of row i from col a to col b = rowPS[i][b] - rowPS[i][a-1]
//
// Column prefix sum: colPS[i][j] = sum of grid[0..i][j]
//   → sum of col j from row a to row b = colPS[b][j] - colPS[a-1][j]
//
// STEP 3: Search order matters for early exit
// If we search from the LARGEST possible side down to 2,
// we can return the FIRST valid magic square we find.
// This avoids tracking a maximum variable.
//
// STEP 4: Diagonal sums cannot be further optimized beyond O(k)
// because diagonals aren't axis-aligned in both dimensions.
// We must iterate k elements for each diagonal check.
// However, they're checked LAST (after row/col fast checks),
// so we rarely reach them for invalid squares.
//
// CORE INSIGHT:
// By using prefix sums, each row/col check drops from O(k) to O(1).
// The dominant cost becomes the diagonal check at O(k),
// and we only reach it when all rows and columns already match.
// Total complexity: O(min(m,n) * m * n * min(m,n)) = O(m*n*min^2)
//
// ============================================================
// WHY THIS ALGORITHM IS CORRECT:
// - Prefix sums give exact range sums with no approximation.
// - We check ALL rows, ALL columns, BOTH diagonals → complete check.
// - Searching largest-first means first valid answer = largest answer.
// - The 1x1 base case is returned at the end if nothing larger found.
//
// ============================================================
// SOLUTION 1: BRUTE FORCE (O(n^5)) — Educational Reference
// ============================================================

class Solution1 {
public:

    // checkmagic: verifies if the k×k sub-grid starting at (i,j) is magic.
    // This function runs in O(k^2) per call — used in the brute force version.
    bool checkmagic(vector<vector<int>> &grid, int i, int j, int k)
    {
        // Establish the "target" magic constant using the FIRST ROW of the sub-grid.
        // Every other row, column, and diagonal must equal this value.
        // WHY first row? It's an arbitrary but consistent reference point.
        // Any valid magic square would have all rows equal, so the first is fine.
       int target = 0;
       for (int c = j; c < j + k; c++){
            target += grid[i][c];   // sum of first ROW of the sub-grid
        }

        // CHECK ALL ROWS: each row sum must equal csum.
        // We start from row i (first row is already our baseline, but
        // re-checking it doesn't hurt correctness and simplifies the loop).
        for (int r = i; r < i + k; r++)
        {
            int rowSum = 0;
            for (int c = j; c < j + k; c++)
            {
                rowSum += grid[r][c];
            }
            // If any row deviates, immediately disqualify this sub-grid.
            // Returning early avoids wasted column/diagonal checks.
            if (rowSum != csum)
                return false;
        }

        // CHECK ALL COLUMNS: each column sum must equal csum.
        for (int c = j; c < j + k; c++)
        {
            int colSum = 0;
            for (int r = i; r < i + k; r++)
            {
                colSum += grid[r][c];
            }
            if (colSum != csum)
                return false;
        }

        // CHECK MAIN DIAGONAL (top-left → bottom-right):
        // grid[i][j], grid[i+1][j+1], ..., grid[i+k-1][j+k-1]
        int diag1 = 0;
        for (int d = 0; d < k; d++)
        {
            diag1 += grid[i + d][j + d];
        }
        if (diag1 != csum)
            return false;

        // CHECK ANTI-DIAGONAL (top-right → bottom-left):
        // grid[i][j+k-1], grid[i+1][j+k-2], ..., grid[i+k-1][j]
        // The column index for step d is: j + (k-1-d)
        int diag2 = 0;
        for (int d = 0; d < k; d++)
        {
            diag2 += grid[i + d][j + (k - 1 - d)];
        }
        if (diag2 != csum)
            return false;

        // All checks passed → this is a valid magic square
        return true;
    }

    int largestMagicSquare(vector<vector<int>> &grid)
    {
        int row = grid.size();
        int col = grid[0].size();

        // Start answer at 1 because a 1x1 square is always magic.
        int maxi = 1;

        // Iterate over all possible top-left corners (i, j)
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                // Try all sizes k >= 2 that fit within the grid from (i,j).
                // Condition: the square's last row is i+k-1 < row,
                //            the square's last col is j+k-1 < col.
                for (int k = 2;
                     i + k - 1 < row && j + k - 1 < col;
                     k++)
                {
                    if (checkmagic(grid, i, j, k))
                    {
                        // Update maximum if this square is larger
                        maxi = max(maxi, k);
                    }
                }
            }
        }

        return maxi;
    }
};

// ============================================================
// SOLUTION 2: PREFIX SUM OPTIMIZED (O(m * n * min(m,n)^2))
// ============================================================
//
// KEY OPTIMIZATION: Precompute row and column prefix sums so that
// any row segment sum or column segment sum is fetched in O(1).
// This converts the O(k) per-row/col check to O(1).
// The only remaining O(k) cost is diagonal traversal.
//
// SEARCH STRATEGY: Iterate sizes from LARGEST down to 2.
// The moment we find a valid magic square, return immediately.
// This avoids maintaining a "maxi" variable and exits early.
//
// ============================================================

class Solution {
public:
    int largestMagicSquare(vector<vector<int>> &grid)
    {
        int rows = grid.size();
        int cols = grid[0].size();

        // -------------------------------------------------------
        // BUILD ROW PREFIX SUMS
        // rowPS[i][j] = grid[i][0] + grid[i][1] + ... + grid[i][j]
        // Enables O(1) query: sum of grid[i][a..b] =
        //   rowPS[i][b] - (a > 0 ? rowPS[i][a-1] : 0)
        // WHY: Avoids recomputing row sums from scratch for every (i,j,k).
        // -------------------------------------------------------
        vector<vector<int>> rowPS(rows, vector<int>(cols));
        for (int i = 0; i < rows; i++)
        {
            rowPS[i][0] = grid[i][0]; // Base case: prefix sum of single element
            for (int j = 1; j < cols; j++)
                rowPS[i][j] = rowPS[i][j - 1] + grid[i][j];
            // Each cell accumulates the running sum along row i.
        }

        // -------------------------------------------------------
        // BUILD COLUMN PREFIX SUMS
        // colPS[i][j] = grid[0][j] + grid[1][j] + ... + grid[i][j]
        // Enables O(1) query: sum of grid[a..b][j] =
        //   colPS[b][j] - (a > 0 ? colPS[a-1][j] : 0)
        // WHY: Same reason — avoids recomputing col sums repeatedly.
        // -------------------------------------------------------
        vector<vector<int>> colPS(rows, vector<int>(cols));
        for (int j = 0; j < cols; j++)
        {
            colPS[0][j] = grid[0][j]; // Base case: prefix sum of single element
            for (int i = 1; i < rows; i++)
                colPS[i][j] = colPS[i - 1][j] + grid[i][j];
            // Each cell accumulates the running sum along column j.
        }

        // -------------------------------------------------------
        // SEARCH FROM LARGEST SIDE DOWN TO 2
        // We try side = min(rows,cols) first, then decrement.
        // WHY descending: the first valid square found is the largest.
        // We can return immediately without tracking a maximum.
        // -------------------------------------------------------
        for (int side = min(rows, cols); side >= 2; side--)
        {
            // Iterate over all valid top-left corners (i, j)
            // for a sub-grid of this 'side' size.
            for (int i = 0; i + side - 1 < rows; i++)
            {
                for (int j = 0; j + side - 1 < cols; j++)
                {
                    // -------------------------------------------
                    // ESTABLISH TARGET: first row sum of sub-grid
                    // Row i, from col j to col j+side-1
                    // Using prefix sum: rowPS[i][j+side-1] - rowPS[i][j-1]
                    // Special case: if j==0, there's no rowPS[i][-1],
                    // so we just use rowPS[i][j+side-1] directly.
                    // -------------------------------------------
                    int target =
                        rowPS[i][j + side - 1] - (j > 0 ? rowPS[i][j - 1] : 0);

                    bool ok = true;

                    // -------------------------------------------
                    // CHECK ALL ROWS (rows i+1 to i+side-1)
                    // Row i is the target itself, skip it.
                    // Each row sum computed in O(1) using rowPS.
                    // If any row doesn't match, set ok=false and break.
                    // WHY check rows before columns: rows use same rowPS,
                    // and we'll fail fast without touching colPS at all.
                    // -------------------------------------------
                    for (int r = i + 1; r < i + side; r++)
                    {
                        int rs =
                            rowPS[r][j + side - 1] - (j > 0 ? rowPS[r][j - 1] : 0);

                        if (rs != target)
                        {
                            ok = false;
                            break; // Short-circuit: no need to check remaining rows
                        }
                    }
                    if (!ok)
                        continue; // Skip to next (i,j) immediately

                    // -------------------------------------------
                    // CHECK ALL COLUMNS (cols j to j+side-1)
                    // Each column sum from row i to row i+side-1.
                    // Computed in O(1) using colPS.
                    // WHY after rows: we only reach here if rows passed,
                    // meaning columns are the next filter.
                    // -------------------------------------------
                    for (int c = j; c < j + side; c++)
                    {
                        int cs =
                            colPS[i + side - 1][c] - (i > 0 ? colPS[i - 1][c] : 0);

                        if (cs != target)
                        {
                            ok = false;
                            break; // Short-circuit
                        }
                    }
                    if (!ok)
                        continue; // Skip to next (i,j)

                    // -------------------------------------------
                    // CHECK BOTH DIAGONALS (O(side) each)
                    // These can't be further optimized without
                    // diagonal prefix sums (more complex to implement).
                    // We check them LAST since they're less likely
                    // to filter; most invalid squares are caught by
                    // row/col checks above.
                    //
                    // d1: main diagonal   grid[i+k][j+k]
                    // d2: anti-diagonal   grid[i+k][j+side-1-k]
                    // -------------------------------------------
                    int d1 = 0, d2 = 0;
                    for (int k = 0; k < side; k++)
                    {
                        d1 += grid[i + k][j + k];
                        d2 += grid[i + k][j + side - 1 - k];
                    }

                    // If both diagonals match target → valid magic square!
                    // Since we're searching largest-first, this is our answer.
                    if (d1 == target && d2 == target)
                        return side;
                }
            }
        }

        // -------------------------------------------------------
        // FALLBACK: A 1×1 sub-grid is always a magic square.
        // Every single cell trivially satisfies all conditions
        // (its single "row", "column", and "diagonals" are equal).
        // -------------------------------------------------------
        return 1;
    }
};

// ============================================================
// DRY RUN EXAMPLE
// ============================================================
//
// Grid:
//   [ 7, 1, 4, 5, 6 ]
//   [ 2, 5, 1, 6, 4 ]
//   [ 1, 5, 4, 3, 2 ]
//   [ 1, 2, 7, 3, 2 ]
//
// rows=4, cols=5, min=4 → try side=4,3,2
//
// rowPS[0] = [7, 8, 12, 17, 23]
// rowPS[1] = [2, 7,  8, 14, 18]
// rowPS[2] = [1, 6, 10, 13, 15]
// rowPS[3] = [1, 3, 10, 13, 15]
//
// colPS[0] = [7, 1,  4,  5,  6]
// colPS[1] = [9, 6,  5, 11, 10]
// colPS[2] = [10,11, 9, 14, 12]
// colPS[3] = [11,13,16, 17, 14]
//
// Try side=4, i=0, j=0:
//   target = rowPS[0][3] - 0 = 17
//   Row 1 sum: rowPS[1][3] - 0 = 14 ≠ 17 → fail, continue
//
// Try side=4, i=0, j=1:
//   target = rowPS[0][4] - rowPS[0][0] = 23 - 7 = 16
//   Row 1 sum: rowPS[1][4] - rowPS[1][0] = 18 - 2 = 16 ✓
//   Row 2 sum: rowPS[2][4] - rowPS[2][0] = 15 - 1 = 14 ≠ 16 → fail
//
// ... (side=4 fails everywhere)
//
// Try side=3, i=0, j=2:
//   target = rowPS[0][4] - rowPS[0][1] = 23 - 8 = 15
//   Row 1: rowPS[1][4] - rowPS[1][1] = 18 - 7 = 11 ≠ 15 → fail
//
// Try side=3, i=1, j=1:
//   target = rowPS[1][3] - rowPS[1][0] = 14 - 2 = 12
//   Row 2: rowPS[2][3] - rowPS[2][0] = 13 - 1 = 12 ✓
//   Row 3: rowPS[3][3] - rowPS[3][0] = 13 - 1 = 12 ✓
//   Col 1: colPS[3][1] - colPS[0][1] = 13 - 1 = 12 ✓
//   Col 2: colPS[3][2] - colPS[0][2] = 16 - 4 = 12 ✓
//   Col 3: colPS[3][3] - colPS[0][3] = 17 - 5 = 12 ✓
//   d1: grid[1][1]+grid[2][2]+grid[3][3] = 5+4+3 = 12 ✓
//   d2: grid[1][3]+grid[2][2]+grid[3][1] = 6+4+2 = 12 ✓
//   → ALL PASS! Return 3.
//
// ============================================================
// COMPLEXITY ANALYSIS
// ============================================================
//
// SOLUTION 1 (BRUTE FORCE):
//   Time:  O(m * n * min(m,n) * min(m,n))
//          = O(m * n * min²)
//          For m=n=50: 50*50*50*50 = 6,250,000 iterations of inner loop
//          But each checkmagic call is O(k²) → total O(m*n*min³) ~ O(n^5)
//          For n=50: 50^5 = 312,500,000 (acceptable given small n)
//   Space: O(1) extra (no prefix arrays)
//
// SOLUTION 2 (PREFIX SUM OPTIMIZED):
//   Preprocessing:
//     rowPS: O(m * n)
//     colPS: O(m * n)
//   Main search:
//     3 nested loops: O(min * m * n) for corner and size selection
//     Row checks: O(side) per corner → total still O(min * m * n * min)
//       but each step is O(1) not O(k) → constant factor improvement
//     Diagonal check: O(side) per valid corner (rare to reach)
//   Total Time: O(m * n * min(m,n)²)
//               For m=n=50: ~6.25 million O(1) ops + occasional O(k) diagonals
//               Significantly faster in practice.
//   Space: O(m * n) for rowPS and colPS arrays.
//
// IS THIS OPTIMAL?
//   For the given constraints (m,n ≤ 50), O(m*n*min²) is efficient enough.
//   A theoretically better bound might use diagonal prefix sums too,
//   making all checks O(1) and reducing to O(m*n*min) total,
//   but the implementation complexity isn't warranted here.
//   The prefix sum approach is considered optimal for this problem's constraints.
//
// ============================================================