// @https://leetcode.com/problems/special-positions-in-a-binary-matrix/description/?envType=daily-question&envId=2026-03-06

// ============================================================
// PROBLEM: 1582. Special Positions in a Binary Matrix
// ============================================================
//
// WHAT THE PROBLEM IS ASKING:
// ----------------------------
// Given a binary matrix (only 0s and 1s) of size m x n,
// we need to count how many positions (i, j) are "special".
//
// DEFINITION OF SPECIAL POSITION:
// --------------------------------
// A position (i, j) is SPECIAL if and only if:
//   1. mat[i][j] == 1   (the cell itself must be 1)
//   2. Every other cell in row i is 0
//   3. Every other cell in column j is 0
//
// In other words, the 1 at (i, j) must be the ONLY 1 in
// its entire row AND the ONLY 1 in its entire column.
//
// CONSTRAINTS AND THEIR IMPLICATIONS:
// -------------------------------------
// - 1 <= m, n <= 100  → matrix is small; even O(m*n*(m+n)) is fine
// - mat[i][j] is 0 or 1 → no need for value checks beyond == 1
// - A cell with 0 can never be special (condition 1 fails immediately)
//
// EDGE CASES:
// -----------
// - All zeros → answer is 0 (no cell has mat[i][j] == 1)
// - All ones → answer is 0 (every row and column has multiple 1s)
// - Single cell matrix [[1]] → answer is 1 (trivially special)
// - Single row or single column of all 1s → at most 1 special (only
//   if that row/col has exactly one 1, which can't happen if all are 1)
//
// WHY NAIVE APPROACH CAN BE SLOW (but still OK here):
// ----------------------------------------------------
// A naive approach for each cell would scan its entire row and column.
// That's O(m*n) cells × O(m+n) scan = O(m*n*(m+n)).
// With m,n=100, this is 100*100*200 = 2,000,000 — totally fine.
// BUT we can do better: precompute row/column sums in O(m*n),
// then check in O(1) per cell → overall O(m*n). This is optimal.
//
// ============================================================
// THINKING FROM FIRST PRINCIPLES
// ============================================================
//
// START FROM SCRATCH:
// -------------------
// We need to identify cells where a 1 is "isolated" — alone in
// its row AND alone in its column.
//
// BUILDING INTUITION:
// -------------------
// Step 1: What makes a position special?
//   → mat[i][j] == 1, row i has no other 1, column j has no other 1.
//
// Step 2: How do we efficiently know if row i has only one 1?
//   → Count the total 1s in row i. If count == 1, row is "pure".
//
// Step 3: Similarly for column j.
//   → Count total 1s in column j. If count == 1, column is "pure".
//
// Step 4: KEY INSIGHT:
//   A position (i,j) is special ⟺
//     mat[i][j] == 1  AND  rowSum[i] == 1  AND  colSum[j] == 1
//
//   Why? If rowSum[i] == 1, the only 1 in row i is somewhere.
//   If mat[i][j] == 1, that somewhere IS (i,j).
//   Similarly colSum[j] == 1 means (i,j) is the only 1 in column j.
//   Together, this perfectly satisfies the "special" definition.
//
// CORE CONCEPT:
// -------------
// Replace per-cell scanning with precomputed aggregates (prefix sums
// over rows and columns). This converts O(m+n) work per cell into
// O(1) lookup per cell after O(m*n) preprocessing.
//
// ============================================================
// WHY THIS ALGORITHM IS CORRECT AND OPTIMAL
// ============================================================
//
// CORRECTNESS:
// ------------
// rowSum[i] counts exactly how many 1s are in row i.
// colSum[j] counts exactly how many 1s are in column j.
// If mat[i][j]==1, rowSum[i]==1, colSum[j]==1:
//   → Row i has exactly 1 total → that 1 is at column j (since mat[i][j]=1)
//   → Column j has exactly 1 total → that 1 is at row i
//   → No other 1 exists in row i or column j → SPECIAL ✓
//
// OPTIMALITY:
// -----------
// We must read every cell at least once to compute anything → Ω(m*n).
// Our algorithm does exactly O(m*n) work → optimal.
//
// ============================================================
// APPROACH 1: BRUTE FORCE (O(m*n*(m+n)))
// For each cell that is 1, scan its row and column directly.
// Included for educational contrast with the optimal approach.
// ============================================================

class Solution_BruteForce {
public:
    int numSpecial(vector<vector<int>>& mat) {

        // Capture matrix dimensions.
        // m = number of rows, n = number of columns.
        int m = mat.size();
        int n = mat[0].size();

        // ans accumulates the count of special positions found.
        int ans = 0;

        // Outer double loop: visit every cell (i, j) in the matrix.
        for(int i = 0; i < m; i++){
            for(int j = 0; j < n; j++){

                // GATE CHECK: A special position MUST have value 1.
                // If this cell is 0, it can never be special — skip immediately.
                // This avoids unnecessary row/column scans for 0-cells.
                if(mat[i][j] != 1) continue;

                // Assume this position is special until proven otherwise.
                // This flag will be set to false if any violation is found.
                bool special = true;

                // SCAN ROW i: Check all other columns in the same row.
                // If any other cell in row i is 1, this position is NOT special.
                for(int col = 0; col < n; col++){
                    if(col != j && mat[i][col] == 1){
                        // Found another 1 in the same row → violates condition.
                        special = false;
                        break; // No need to check further in this row.
                    }
                }

                // SCAN COLUMN j: Only if still possibly special (short-circuit).
                // The condition "special" in the loop header prevents wasted work
                // if the row scan already disqualified this cell.
                for(int row = 0; row < m && special; row++){
                    if(row != i && mat[row][j] == 1){
                        // Found another 1 in the same column → violates condition.
                        special = false;
                    }
                }

                // If both row and column scans passed, this position is special.
                if(special) ans++;
            }
        }

        return ans;
    }
};

// ============================================================
// DRY RUN — APPROACH 1
// mat = [[1,0,0],[0,0,1],[1,0,0]]
//
// (i=0,j=0): mat[0][0]=1. Scan row 0: cols 1,2 are 0 ✓. Scan col 0: row1=0,row2=1 ✗ → NOT special.
// (i=0,j=1): mat[0][1]=0 → skip.
// (i=0,j=2): mat[0][2]=0 → skip.
// (i=1,j=0): mat[1][0]=0 → skip.
// (i=1,j=1): mat[1][1]=0 → skip.
// (i=1,j=2): mat[1][2]=1. Scan row 1: cols 0,1 are 0 ✓. Scan col 2: row0=0,row2=0 ✓ → SPECIAL! ans=1.
// (i=2,j=0): mat[2][0]=1. Scan row 2: cols 1,2 are 0 ✓. Scan col 0: row0=1 ✗ → NOT special.
// ... remaining cells are 0, skipped.
// Final ans = 1 ✓
// ============================================================


// ============================================================
// APPROACH 2: OPTIMAL — PRECOMPUTE ROW AND COLUMN SUMS (O(m*n))
// Precompute rowSum[i] = total 1s in row i
//            colSum[j] = total 1s in column j
// Then check each cell in O(1).
// ============================================================

class Solution_Optimal {
public:
    int numSpecial(vector<vector<int>>& mat) {

        int m = mat.size();
        int n = mat[0].size();

        // row[i] = number of 1s in row i.
        // This tells us: is (i, j) the ONLY 1 in its row?
        // If row[i] == 1 and mat[i][j] == 1, then yes.
        vector<int> row(m, 0);

        // col[j] = number of 1s in column j.
        // This tells us: is (i, j) the ONLY 1 in its column?
        // If col[j] == 1 and mat[i][j] == 1, then yes.
        vector<int> col(n, 0);

        // PASS 1: Populate row sums and column sums.
        // We scan every cell exactly once — O(m*n).
        // For each 1 found, we increment the running count
        // for its row and its column simultaneously.
        for(int i = 0; i < m; i++){
            for(int j = 0; j < n; j++){
                if(mat[i][j] == 1){
                    row[i]++;  // This row now has one more 1.
                    col[j]++;  // This column now has one more 1.
                }
            }
        }

        // INVARIANT after PASS 1:
        // row[i] == total number of 1s in row i (for all i)
        // col[j] == total number of 1s in column j (for all j)

        int ans = 0;

        // PASS 2: Identify special positions.
        // A cell (i,j) is special iff all three conditions hold:
        //   1. mat[i][j] == 1     → the cell itself is 1
        //   2. row[i] == 1        → no other 1 in its row
        //   3. col[j] == 1        → no other 1 in its column
        // All three checks are O(1) — no scanning needed.
        for(int i = 0; i < m; i++){
            for(int j = 0; j < n; j++){
                if(mat[i][j] == 1 && row[i] == 1 && col[j] == 1){
                    ans++;
                }
            }
        }

        return ans;
    }
};

// ============================================================
// DRY RUN — APPROACH 2
// mat = [[1,0,0],[0,1,0],[0,0,1]]
//
// PASS 1 — Build row/col sums:
//   (0,0)=1 → row[0]=1, col[0]=1
//   (1,1)=1 → row[1]=1, col[1]=1
//   (2,2)=1 → row[2]=1, col[2]=1
//   All others are 0.
//   row = [1, 1, 1]
//   col = [1, 1, 1]
//
// PASS 2 — Check each cell:
//   (0,0): mat=1, row[0]=1, col[0]=1 → SPECIAL! ans=1
//   (0,1): mat=0 → skip
//   (0,2): mat=0 → skip
//   (1,0): mat=0 → skip
//   (1,1): mat=1, row[1]=1, col[1]=1 → SPECIAL! ans=2
//   (1,2): mat=0 → skip
//   (2,0): mat=0 → skip
//   (2,1): mat=0 → skip
//   (2,2): mat=1, row[2]=1, col[2]=1 → SPECIAL! ans=3
//
// Final ans = 3 ✓
// ============================================================


// ============================================================
// APPROACH 3: SPACE-OPTIMIZED — ONLY PRECOMPUTE COLUMN SUMS (O(m*n), O(n) space)
// Observation: We don't need to store ALL row sums.
// For each row i, we can compute rowCount on-the-fly as we scan it.
// We only need colSum precomputed (since columns span multiple rows).
// This halves the auxiliary space from O(m+n) to O(n).
// ============================================================

class Solution_SpaceOptimized {
public:
    int numSpecial(vector<vector<int>>& mat) {

        int m = mat.size();
        int n = mat[0].size();

        // colCount[j] = number of 1s in column j.
        // We need this precomputed because when we're at row i,
        // we can't know column totals without seeing all rows first.
        vector<int> colCount(n, 0);

        // PASS 1: Count 1s per column across all rows.
        // Row sums are NOT stored — we'll compute them on-the-fly in Pass 2.
        for(int i = 0; i < m; i++){
            for(int j = 0; j < n; j++){
                if(mat[i][j] == 1){
                    colCount[j]++;
                    // WHY only colCount and not rowCount?
                    // Because row sums can be recomputed during Pass 2
                    // by scanning the row in O(n) — they don't need storage.
                    // Column sums cannot be computed on-the-fly per row,
                    // so they must be precomputed.
                }
            }
        }

        // INVARIANT after PASS 1:
        // colCount[j] = total 1s in column j, for all j in [0, n).

        int ans = 0;

        // PASS 2: For each row, compute row statistics on-the-fly.
        for(int i = 0; i < m; i++){

            // rowCount = number of 1s found so far in row i.
            // Computed fresh for each row — no storage across rows needed.
            int rowCount = 0;

            // colIndex = the column where the last 1 in row i was found.
            // If rowCount ends up == 1, colIndex will hold the exact column
            // of that single 1. This is the candidate for special position.
            // Initialized to -1 as a sentinel meaning "no 1 found yet".
            int colIndex = -1;

            // Scan every cell in row i to compute rowCount and track colIndex.
            for(int j = 0; j < n; j++){
                if(mat[i][j] == 1){
                    rowCount++;   // One more 1 found in this row.
                    colIndex = j; // Remember where the last (or only) 1 is.
                }
            }

            // SPECIAL POSITION CHECK for row i:
            // Condition 1: rowCount == 1
            //   → Row i has exactly one 1, located at column colIndex.
            //   → This automatically means mat[i][colIndex] == 1.
            // Condition 2: colCount[colIndex] == 1
            //   → Column colIndex has exactly one 1.
            //   → Since mat[i][colIndex] == 1, that one must be at row i.
            // Together: position (i, colIndex) satisfies ALL special conditions.
            //
            // WHY we don't need mat[i][colIndex]==1 explicitly:
            //   rowCount==1 already guarantees colIndex was set to a cell with value 1.
            if(rowCount == 1 && colCount[colIndex] == 1){
                ans++;
            }
            // If rowCount == 0: no 1s in this row, no special position.
            // If rowCount >= 2: multiple 1s in this row, none can be special.
        }

        return ans;
    }
};

// ============================================================
// DRY RUN — APPROACH 3
// mat = [[1,0,0],[0,0,1],[1,0,0]]
//
// PASS 1 — Column sums:
//   col 0: rows 0 and 2 have 1 → colCount[0] = 2
//   col 1: no 1s              → colCount[1] = 0
//   col 2: row 1 has 1        → colCount[2] = 1
//   colCount = [2, 0, 1]
//
// PASS 2 — Row-by-row:
//   Row 0: scan → j=0: mat=1, rowCount=1, colIndex=0
//                 j=1: mat=0, j=2: mat=0
//           rowCount=1, colIndex=0, colCount[0]=2 ≠ 1 → NOT special.
//
//   Row 1: scan → j=0: mat=0, j=1: mat=0, j=2: mat=1, rowCount=1, colIndex=2
//           rowCount=1, colIndex=2, colCount[2]=1 == 1 → SPECIAL! ans=1.
//
//   Row 2: scan → j=0: mat=1, rowCount=1, colIndex=0
//                 j=1: mat=0, j=2: mat=0
//           rowCount=1, colIndex=0, colCount[0]=2 ≠ 1 → NOT special.
//
// Final ans = 1 ✓
// ============================================================


// ============================================================
// COMPLEXITY ANALYSIS
// ============================================================
//
// APPROACH 1 — BRUTE FORCE:
// --------------------------
// Time: O(m * n * (m + n))
//   - We visit m*n cells.
//   - For each cell with value 1, we scan its row (O(n)) and column (O(m)).
//   - Worst case: all cells are 1 → m*n*(m+n) operations.
//   - With m=n=100: 100*100*200 = 2,000,000 — fast enough in practice.
// Space: O(1)
//   - Only constant extra variables (special, ans) used.
//   - No auxiliary arrays.
//
// APPROACH 2 — PRECOMPUTE ROW + COLUMN SUMS:
// -------------------------------------------
// Time: O(m * n)
//   - Pass 1: scan all m*n cells once → O(m*n)
//   - Pass 2: scan all m*n cells once → O(m*n)
//   - Total: 2 * O(m*n) = O(m*n)
// Space: O(m + n)
//   - row[] array of size m
//   - col[] array of size n
//   - Negligible vs input size.
//
// APPROACH 3 — PRECOMPUTE COLUMN SUMS ONLY:
// ------------------------------------------
// Time: O(m * n)
//   - Pass 1: scan all m*n cells → O(m*n)
//   - Pass 2: scan all m*n cells row by row → O(m*n)
//   - Total: O(m*n) — same asymptotic as Approach 2.
// Space: O(n)
//   - colCount[] array of size n only.
//   - rowCount and colIndex are scalar variables reused each row.
//   - This is strictly better space than Approach 2's O(m+n).
//
// IS THIS OPTIMAL?
// ----------------
// YES. We must read every cell at least once to determine if any
// 1 exists and where → lower bound is Ω(m*n).
// Both Approaches 2 and 3 achieve O(m*n) time → optimal.
// Approach 3 further minimizes space to O(n) → best overall solution.
// ============================================================