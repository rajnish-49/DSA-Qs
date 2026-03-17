class Solution {
public:
    /*
     * PROBLEM: Longest Common SUBSTRING (NOT subsequence!)
     * Find the length of the longest substring common to both strings.
     *
     * CRITICAL DISTINCTION — Substring vs Subsequence:
     * Subsequence: characters must appear in order, but CAN have gaps.
     * Substring:   characters must appear in order AND must be CONTIGUOUS.
     *
     * e.g., s1 = "abcde", s2 = "abfce"
     * LCS (subsequence) = "abce" → length 4  (gaps allowed)
     * Longest Common Substring = "ab" → length 2  (no gaps, breaks at 'c' vs 'f')
     *
     * This contiguity requirement is the single difference from LCS,
     * but it completely changes the DP transition.
     *
     * TABLE DEFINITION:
     * dp[i][j] = length of the longest common SUBSTRING that ENDS AT
     *            s1[i-1] and s2[j-1] (i-th char of s1, j-th char of s2).
     *
     * "ENDS AT" is the key phrase — dp[i][j] doesn't mean "best so far
     * among all substrings in s1[0..i-1] and s2[0..j-1]". It means
     * the longest common substring that is forced to include BOTH s1[i-1]
     * and s2[j-1] as its last characters.
     * This enforces contiguity — a common substring ending at (i,j) can
     * only be extended from a common substring ending at (i-1, j-1).
     *
     * Because of this definition, the final answer isn't dp[n][m] —
     * it could be anywhere in the table. So we track a running maximum.
     */
    int longCommSubstr(string &s1, string &s2) {
        int n = s1.size();
        int m = s2.size();

        // Same (n+1)x(m+1) setup as LCS — row 0, col 0 are base cases (empty string = 0).
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

        int maxi = 0; // Tracks the best common substring length seen across all (i,j)

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {

                if (s1[i - 1] == s2[j - 1]) {
                    // Characters match: we can EXTEND the common substring that
                    // ended at (i-1, j-1) by 1. The substring remains contiguous
                    // because we're appending a matching character to both ends.
                    // dp[i-1][j-1] could be 0 (no prior match) — that's fine,
                    // it just means this match starts a new substring of length 1.
                    dp[i][j] = 1 + dp[i - 1][j - 1];
                    maxi = max(maxi, dp[i][j]);
                }
                else {
                    // THIS IS THE KEY DIFFERENCE FROM LCS:
                    // In LCS, a mismatch meant: skip one char and try → max(dp[i-1][j], dp[i][j-1])
                    // Here, skipping is NOT allowed — substring must be contiguous.
                    // A mismatch means the common substring CANNOT include both s1[i-1] and s2[j-1]
                    // as its ending characters, so any substring ending here has length 0.
                    // We reset to 0, forcing any future match to start fresh.
                    dp[i][j] = 0;
                }
            }
        }

        return maxi;

        /*
         * WHY dp[n][m] IS NOT THE ANSWER (unlike LCS):
         * In LCS, dp[n][m] = LCS of full strings, because skipping is allowed
         * and the best result naturally "bubbles up" to the bottom-right corner.
         * Here, dp[n][m] only tells us the longest common substring that ends
         * at the LAST characters of both strings. The true longest might end
         * anywhere in the middle — so we must track maxi across the entire table.
         *
         * VISUAL INTUITION:
         * s1 = "abcde", s2 = "bcdf"
         *
         *       ""  b  c  d  f
         *   ""  [0, 0, 0, 0, 0]
         *   a   [0, 0, 0, 0, 0]
         *   b   [0, 1, 0, 0, 0]
         *   c   [0, 0, 2, 0, 0]
         *   d   [0, 0, 0, 3, 0]   ← diagonal streak of matches = "bcd"
         *   e   [0, 0, 0, 0, 0]
         *
         * Notice the diagonal streaks — that's contiguous matching in action.
         * A mismatch snaps the streak back to 0. The longest streak = answer.
         *
         * COMPLEXITY:
         * Time:  O(n * m) — fill every cell once
         * Space: O(n * m) — optimizable to O(m) since we only need the previous row
         *        (only dp[i-1][j-1] is used, so a single prev-row array suffices)
         */
    }
};