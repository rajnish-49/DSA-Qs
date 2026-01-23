class Solution {
public:

    // This function answers ONE precise question:
    // "Is the k×k square with top-left corner at (i, j) a magic square?"
    //
    // Important mental model:
    // (i, j) = top-left corner
    // k      = size of square
    //
    // So the square spans:
    // rows    : i        → i + k - 1
    // columns : j        → j + k - 1
    bool checkmagic(vector<vector<int>>& grid, int i, int j, int k) {

        // -------------------------------
        // STEP 1: Reference Sum (csum)
        // -------------------------------
        // A magic square means:
        // all rows, columns, and diagonals have THE SAME sum.
        //
        // To check "all are equal", we must first pick ONE sum
        // to compare everything against.
        //
        // We choose the FIRST ROW of the square because:
        // - it always exists
        // - if the square is magic, this row must also have the magic sum
        //
        // We stay on row 'i' and take EXACTLY k elements:
        // columns j, j+1, ..., j+k-1
        //
        // Key intuition:
        // "loop from start to < start + k" → exactly k elements
        int csum = 0;
        for (int c = j; c < j + k; c++) {
            csum += grid[i][c];
        }

        // -------------------------------
        // STEP 2: Check all rows
        // -------------------------------
        // Now we verify that EVERY row in the square
        // has the same sum as csum.
        //
        // Rows involved:
        // i, i+1, ..., i+k-1  (total k rows)
        for (int r = i; r < i + k; r++) {

            int rowSum = 0;

            // For the current row 'r', we again take
            // k columns starting from j
            for (int c = j; c < j + k; c++) {
                rowSum += grid[r][c];
            }

            // If even ONE row differs,
            // the square cannot be magic
            if (rowSum != csum)
                return false;
        }

        // -------------------------------
        // STEP 3: Check all columns
        // -------------------------------
        // Same idea as rows, but now column-wise.
        //
        // Columns involved:
        // j, j+1, ..., j+k-1  (total k columns)
        for (int c = j; c < j + k; c++) {

            int colSum = 0;

            // For this column 'c', we sum k rows
            // starting from row i
            for (int r = i; r < i + k; r++) {
                colSum += grid[r][c];
            }

            // Any mismatch → not magic
            if (colSum != csum)
                return false;
        }

        // -------------------------------
        // STEP 4: Main diagonal
        // -------------------------------
        // Main diagonal moves:
        // down 1 row and right 1 column each time
        //
        // Cells are:
        // (i, j)
        // (i+1, j+1)
        // (i+2, j+2)
        // ...
        // (i+k-1, j+k-1)
        //
        // Notice how both row and column increase together.
        int diag1 = 0;
        for (int d = 0; d < k; d++) {
            diag1 += grid[i + d][j + d];
        }

        if (diag1 != csum)
            return false;

        // -------------------------------
        // STEP 5: Anti-diagonal
        // -------------------------------
        // Anti-diagonal moves:
        // down 1 row and LEFT 1 column each time
        //
        // Column starts at (j + k - 1) and decreases
        //
        // Cells are:
        // (i,     j+k-1)
        // (i+1,   j+k-2)
        // (i+2,   j+k-3)
        // ...
        // (i+k-1, j)
        //
        // The expression (k - 1 - d) ensures
        // column index moves left correctly.
        int diag2 = 0;
        for (int d = 0; d < k; d++) {
            diag2 += grid[i + d][j + (k - 1 - d)];
        }

        if (diag2 != csum)
            return false;

        // If we reached here:
        // - all rows match
        // - all columns match
        // - both diagonals match
        //
        // Therefore, this square IS magic
        return true;
    }

    int largestMagicSquare(vector<vector<int>>& grid) {

        int row = grid.size();
        int col = grid[0].size();

        // Minimum answer is 1 because
        // every 1×1 square is trivially magic
        int maxi = 1;

        // Choose every cell as a possible top-left corner
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {

                // Try increasing square sizes starting from k = 2
                //
                // Boundary intuition:
                // The bottom-right corner of the square is:
                // (i + k - 1, j + k - 1)
                //
                // It must stay inside the grid:
                // i + k - 1 < row
                // j + k - 1 < col
                for (int k = 2;
                     i + k - 1 < row && j + k - 1 < col;
                     k++) {

                    if (checkmagic(grid, i, j, k)) {
                        // Keep the largest valid size found so far
                        maxi = max(maxi, k);
                    }
                }
            }
        }

        return maxi;
    }
};


class Solution {
public:
    int largestMagicSquare(vector<vector<int>>& grid) {
        int rows = grid.size();
        int cols = grid[0].size();

        // ----------------------------------------------------
        // rowPS[r][c] = sum of grid[r][0..c] (inclusive)
        //
        // Purpose:
        //   Allows O(1) computation of sum of any row segment:
        //   grid[r][j..x] =
        //   rowPS[r][x] - rowPS[r][j-1]  (if j > 0)
        //
        // We do NOT add an extra 0 column, so we must handle
        // j == 0 explicitly later.
        // ----------------------------------------------------
        vector<vector<int>> rowPS(rows, vector<int>(cols));
        for (int i = 0; i < rows; i++) {
            rowPS[i][0] = grid[i][0];
            for (int j = 1; j < cols; j++)
                rowPS[i][j] = rowPS[i][j - 1] + grid[i][j];
        }

        // ----------------------------------------------------
        // colPS[r][c] = sum of grid[0..r][c] (inclusive)
        //
        // Purpose:
        //   Allows O(1) computation of sum of any column segment:
        //   grid[i..x][c] =
        //   colPS[x][c] - colPS[i-1][c]  (if i > 0)
        //
        // Again, no extra row of 0s, so i == 0 is handled later.
        // ----------------------------------------------------
        vector<vector<int>> colPS(rows, vector<int>(cols));
        for (int j = 0; j < cols; j++) {
            colPS[0][j] = grid[0][j];
            for (int i = 1; i < rows; i++)
                colPS[i][j] = colPS[i - 1][j] + grid[i][j];
        }

        // ----------------------------------------------------
        // We iterate square size from largest → smallest.
        //
        // Key insight:
        //   The first magic square found is guaranteed to be
        //   the largest, so we can return immediately.
        //
        // This avoids unnecessary checks for smaller squares.
        // ----------------------------------------------------
        for (int side = min(rows, cols); side >= 2; side--) {

            // ------------------------------------------------
            // Choose (i, j) as top-left corner of the square.
            //
            // The square spans:
            //   rows    i .. i + side - 1
            //   columns j .. j + side - 1
            //
            // Hence the bound:
            //   i + side - 1 < rows
            //   j + side - 1 < cols
            // ------------------------------------------------
            for (int i = 0; i + side - 1 < rows; i++) {
                for (int j = 0; j + side - 1 < cols; j++) {

                    // ------------------------------------------------
                    // Reference sum (target):
                    // Use the FIRST ROW of the square.
                    //
                    // Why:
                    //   In a magic square, all rows must have
                    //   the same sum, so the first row is a
                    //   valid baseline.
                    //
                    // grid[i][j .. j+side-1]
                    // ------------------------------------------------
                    int target =
                        rowPS[i][j + side - 1]
                        - (j > 0 ? rowPS[i][j - 1] : 0);

                    bool ok = true;

                    // ------------------------------------------------
                    // Check remaining rows.
                    //
                    // We start from r = i + 1 because row i
                    // was already used to compute target.
                    //
                    // Each row must match the same target sum.
                    // ------------------------------------------------
                    for (int r = i + 1; r < i + side; r++) {
                        int rs =
                            rowPS[r][j + side - 1]
                            - (j > 0 ? rowPS[r][j - 1] : 0);

                        if (rs != target) {
                            ok = false;
                            break;
                        }
                    }
                    if (!ok) continue;

                    // ------------------------------------------------
                    // Check columns.
                    //
                    // For each column c in [j .. j+side-1],
                    // sum grid[i .. i+side-1][c] using colPS.
                    //
                    // All column sums must equal target.
                    // ------------------------------------------------
                    for (int c = j; c < j + side; c++) {
                        int cs =
                            colPS[i + side - 1][c]
                            - (i > 0 ? colPS[i - 1][c] : 0);

                        if (cs != target) {
                            ok = false;
                            break;
                        }
                    }
                    if (!ok) continue;

                    // ------------------------------------------------
                    // Check both diagonals.
                    //
                    // Main diagonal:
                    //   (i, j),
                    //   (i+1, j+1),
                    //   ...
                    //   (i+side-1, j+side-1)
                    //
                    // Anti-diagonal:
                    //   (i, j+side-1),
                    //   (i+1, j+side-2),
                    //   ...
                    //   (i+side-1, j)
                    //
                    // Diagonals are checked directly because
                    // they are only O(side), which is acceptable.
                    // ------------------------------------------------
                    int d1 = 0, d2 = 0;
                    for (int k = 0; k < side; k++) {
                        d1 += grid[i + k][j + k];
                        d2 += grid[i + k][j + side - 1 - k];
                    }

                    // ------------------------------------------------
                    // If both diagonals match the target sum,
                    // then all magic square conditions are met.
                    //
                    // Since we are iterating from largest side
                    // to smallest, we can return immediately.
                    // ------------------------------------------------
                    if (d1 == target && d2 == target)
                        return side;
                }
            }
        }

        // If no square larger than 1x1 is found
        return 1;
    }
};
