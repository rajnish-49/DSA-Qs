/*
 * ============================================================
 * MATRIX CHAIN MULTIPLICATION — MEMOIZATION + TABULATION
 * ============================================================
 *
 * PROBLEM:
 * --------
 * Given n matrices A[1], A[2], ..., A[n], find the minimum number
 * of scalar multiplications needed to multiply the entire chain.
 *
 * THE DIMENSION ARRAY dims[]:
 * ---------------------------
 * Matrix A[i] has dimensions dims[i-1] x dims[i].
 *
 * IMPORTANT: matrices are 1-indexed, dims[] is 0-indexed.
 * This is WHY there is always a -1 in the formula.
 *
 * Example: dims = [10, 20, 30, 40]  →  3 matrices (NOT 4)
 *   A[1]: dims[0] x dims[1] = 10 x 20   (row = dims[i-1] = dims[0], col = dims[i] = dims[1])
 *   A[2]: dims[1] x dims[2] = 20 x 30   (row = dims[i-1] = dims[1], col = dims[i] = dims[2])
 *   A[3]: dims[2] x dims[3] = 30 x 40   (row = dims[i-1] = dims[2], col = dims[i] = dims[3])
 *
 * Number of matrices = dims.length - 1
 * dims[] just stores boundary values, not the matrices themselves.
 *
 * WHAT dp[i][j] MEANS:
 * --------------------
 * dp[i][j] = minimum scalar multiplications to multiply A[i] through A[j].
 *
 * i = starting matrix index (where the chain starts)
 * j = ending matrix index   (where the chain ends)
 *
 * So dp[1][4] means: best cost to multiply A[1] A[2] A[3] A[4].
 * dp[2][3] means: best cost to multiply A[2] A[3].
 * dp[0][3] is INVALID when matrices are 1-indexed (A[0] doesn't exist).
 *
 * ============================================================
 * THE CUT k — WHAT IT IS AND WHY
 * ============================================================
 *
 * To solve dp[i][j], you ask: "what if the LAST multiplication
 * I perform splits the chain at position k?"
 *
 * k is the index of the LAST matrix in the LEFT group.
 * So cut at k means:
 *   Left group:  A[i] ... A[k]
 *   Right group: A[k+1] ... A[j]
 *
 * Example: dp[1][4], cut at k=2 means:
 *   Left:  [ A[1] A[2] ]   collapses to one matrix
 *   Right: [ A[3] A[4] ]   collapses to one matrix
 *   Then multiply those two resulting matrices together.
 *
 * WHY k ranges from i to j-1 (i.e. i <= k < j):
 *   k must be >= i so left group has at least A[i] (non-empty)
 *   k must be <  j so right group has at least A[j] (non-empty)
 *   k = j would make right group A[j+1]..A[j] which is empty — INVALID
 *
 * All valid cuts for dp[1][4]:
 *   k=1: [ A[1] ]          x [ A[2] A[3] A[4] ]
 *   k=2: [ A[1] A[2] ]     x [ A[3] A[4] ]
 *   k=3: [ A[1] A[2] A[3]] x [ A[4] ]
 *   k=4: INVALID — right group would be empty
 *
 * ============================================================
 * THE RECURRENCE — WHY dims[i-1] AND NOT dims[i]
 * ============================================================
 *
 * After cut at k, you have two resulting matrices:
 *   Left chunk  A[i..k]   collapses to shape: dims[i-1] x dims[k]
 *   Right chunk A[k+1..j] collapses to shape: dims[k]   x dims[j]
 *
 * WHY dims[i-1] for the left chunk's row count?
 *   The row count of the left chunk comes from the FIRST matrix A[i].
 *   A[i] has dimensions dims[i-1] x dims[i].
 *   Its ROW count is dims[i-1], not dims[i].
 *   dims[i] would be A[i]'s COLUMN count — wrong.
 *
 * Concrete example: i=1, A[1] is 10x20
 *   dims[i]   = dims[1] = 20  → column of A[1]  ← WRONG to use this as row
 *   dims[i-1] = dims[0] = 10  → row of A[1]     ← CORRECT
 *
 * WHY dims[k] for the left chunk's col AND right chunk's row?
 *   Left chunk ends at A[k]. A[k] has col = dims[k].
 *   Right chunk starts at A[k+1]. A[k+1] has row = dims[(k+1)-1] = dims[k].
 *   They share dims[k] — this is the "shared middle dimension".
 *   This shared dimension must match for the multiplication to be valid.
 *
 * Cost to merge left (dims[i-1] x dims[k]) with right (dims[k] x dims[j]):
 *   = dims[i-1] * dims[k] * dims[j]
 *
 * NOTE: dp[i][k] and dp[k+1][j] are COSTS (just numbers like 6000).
 *       dims[i-1] * dims[k] * dims[j] is computed fresh from dims[] each time.
 *       They are completely different things — dp stores costs, dims stores sizes.
 *
 * Full recurrence:
 *   dp[i][j] = min over k in [i, j-1] of:
 *              dp[i][k] + dp[k+1][j] + dims[i-1] * dims[k] * dims[j]
 *
 * BASE CASE: dp[i][i] = 0 (single matrix, no multiplication needed)
 *
 * ============================================================
 * FILL ORDER — WHY WE ITERATE BY CHAIN LENGTH
 * ============================================================
 *
 * dp[i][j] depends on dp[i][k] and dp[k+1][j].
 * Both of these are SHORTER chains than dp[i][j].
 *   dp[i][k]   has length k-i+1     < j-i+1
 *   dp[k+1][j] has length j-(k+1)+1 < j-i+1
 *
 * So: fill shorter chains before longer chains.
 * Outer loop = chain length (2 to n).
 * For each length, inner loop = all valid starting positions i.
 *
 * RANGE OF i FOR A GIVEN LENGTH:
 *   i starts at 1 (first matrix is A[1])
 *   j = i + len - 1 must not exceed n
 *   So i <= n - len + 1
 *
 * Example: n=4, len=3
 *   i=1 → j=3 ✅
 *   i=2 → j=4 ✅
 *   i=3 → j=5 ❌ A[5] doesn't exist
 *   So i goes from 1 to 4-3+1 = 2
 *
 * ============================================================
 */

#include <bits/stdc++.h>
using namespace std;

int memo[101][101];
int dims[101]; // dims[i-1] x dims[i] = shape of matrix A[i]
int n;         // number of matrices (NOT dims.length, which is n+1)

int solveTopDown(int i, int j) {
    // BASE CASE: single matrix costs nothing
    if (i == j) return 0;

    // CACHE CHECK: already solved this subchain?
    if (memo[i][j] != -1) return memo[i][j];

    memo[i][j] = INT_MAX;

    // try every cut k — k is the last matrix of the left group
    // left group  = A[i..k],   shape after collapse = dims[i-1] x dims[k]
    // right group = A[k+1..j], shape after collapse = dims[k]   x dims[j]
    for (int k = i; k < j; k++) {
        int cost = solveTopDown(i, k)             // cost to fully compute left group
                 + solveTopDown(k+1, j)           // cost to fully compute right group
                 + dims[i-1] * dims[k] * dims[j]; // cost to merge the two resulting matrices
                                                   // dims[i-1] = row of left result (row of A[i])
                                                   // dims[k]   = col of left = row of right (shared dim)
                                                   // dims[j]   = col of right result (col of A[j])

        memo[i][j] = min(memo[i][j], cost);
    }

    return memo[i][j];
}

int solveBottomUp() {
    int dp[n+1][n+1];
    memset(dp, 0, sizeof(dp));
    // dp[i][i] = 0 for all i (base case handled by memset)

    // OUTER LOOP: chain length from 2 to n
    // len=1 is base case (already 0), so start from 2
    for (int len = 2; len <= n; len++) {

        // INNER LOOP: all valid starting positions for this length
        // i goes from 1 to n-len+1 because j = i+len-1 must not exceed n
        for (int i = 1; i <= n - len + 1; i++) {

            int j = i + len - 1; // ending index — fixed once i and len are known

            dp[i][j] = INT_MAX;

            // try all cut points k in [i, j-1]
            // when we reach here, dp[i][k] and dp[k+1][j] are already filled
            // because both have length < len (guaranteed by outer loop order)
            for (int k = i; k < j; k++) {
                int cost = dp[i][k]
                         + dp[k+1][j]
                         + dims[i-1] * dims[k] * dims[j];

                dp[i][j] = min(dp[i][j], cost);
            }
        }
    }

    return dp[1][n]; // min cost to multiply entire chain A[1..n]
}

/*
 * ============================================================
 * DRY RUN (n=3, dims = [10, 30, 5, 60])
 * ============================================================
 *
 * Matrices:
 *   A[1]: dims[0] x dims[1] = 10 x 30
 *   A[2]: dims[1] x dims[2] = 30 x  5
 *   A[3]: dims[2] x dims[3] =  5 x 60
 *
 * BASE CASES: dp[1][1]=0, dp[2][2]=0, dp[3][3]=0
 *
 * LENGTH = 2:
 * -----------
 * dp[1][2]: i=1, j=2, k=1
 *   left  = A[1],    shape = dims[0] x dims[1] = 10x30
 *   right = A[2],    shape = dims[1] x dims[2] = 30x5
 *   cost  = 0 + 0 + dims[0]*dims[1]*dims[2] = 10*30*5 = 1500
 *   dp[1][2] = 1500
 *
 * dp[2][3]: i=2, j=3, k=2
 *   left  = A[2],    shape = dims[1] x dims[2] = 30x5
 *   right = A[3],    shape = dims[2] x dims[3] = 5x60
 *   cost  = 0 + 0 + dims[1]*dims[2]*dims[3] = 30*5*60 = 9000
 *   dp[2][3] = 9000
 *
 * LENGTH = 3:
 * -----------
 * dp[1][3]: i=1, j=3, try k=1 and k=2
 *
 *   k=1: left=A[1], right=A[2..3]
 *     cost = dp[1][1] + dp[2][3] + dims[0]*dims[1]*dims[3]
 *          = 0 + 9000 + 10*30*60 = 27000
 *
 *   k=2: left=A[1..2], right=A[3]
 *     cost = dp[1][2] + dp[3][3] + dims[0]*dims[2]*dims[3]
 *          = 1500 + 0 + 10*5*60 = 4500
 *
 *   dp[1][3] = min(27000, 4500) = 4500
 *   Optimal: (A[1]*A[2]) * A[3]
 *
 * ============================================================
 * COMPLEXITY
 * ============================================================
 *
 * TIME:  O(n^3) — n^2 subproblems, each with O(n) k loop
 * SPACE: O(n^2) for dp/memo table
 *        Memoization also uses O(n) recursion stack depth
 *
 * ============================================================
 */

int main() {
    cin >> n;
    for (int i = 0; i <= n; i++) cin >> dims[i];

    memset(memo, -1, sizeof(memo));
    cout << "Memoization: " << solveTopDown(1, n) << endl;
    cout << "Tabulation:  " << solveBottomUp() << endl;

    return 0;
}