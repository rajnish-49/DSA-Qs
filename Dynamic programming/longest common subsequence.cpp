class Solution {
public:
    /*
     * PROBLEM: Longest Common Subsequence (LCS)
     * Given two strings, find the length of their longest common subsequence.
     * A subsequence is a sequence derived by deleting some (or no) characters
     * WITHOUT changing the relative order of remaining characters.
     * e.g., "ace" is a subsequence of "abcde", but "aec" is NOT.
     *
     * KEY INSIGHT: Unlike substring (contiguous), subsequence allows gaps.
     * So "abcde" and "ace" share LCS = "ace" (length 3).
     *
     * WHY RECURSION + MEMOIZATION?
     * At every pair of indices (i, j), we face a CHOICE that affects the future.
     * The number of such choices is exponential in brute force (2^n),
     * but the number of UNIQUE subproblems is only O(n1 * n2),
     * making memoization extremely effective here.
     */

    int n1, n2;
    vector<vector<int>> dp; // dp[i][j] = LCS length of text1[i..] and text2[j..]

    /*
     * CORE RECURSIVE LOGIC — think of it as: 
     * "What's the LCS of the SUFFIX of 'a' starting at i, and SUFFIX of 'b' starting at j?"
     *
     * BASE CASE: If either string is exhausted, no more characters to match → return 0.
     *
     * CASE 1 — Characters MATCH (a[i] == b[j]):
     *   This is a GREEDY-SAFE decision. It is never suboptimal to take a match
     *   when characters are equal. Why? Because including this match can only
     *   help or be neutral — skipping it can't give a longer result.
     *   So: take the match and move BOTH pointers forward.
     *   → 1 + solve(i+1, j+1)
     *
     * CASE 2 — Characters DON'T MATCH:
     *   We can't use both a[i] and b[j] as a matching pair.
     *   So one of them must be "skipped" (not part of the LCS at this step).
     *   We try BOTH options and take the best:
     *     Option A: skip a[i] → solve(i+1, j)   [advance in text1]
     *     Option B: skip b[j] → solve(i, j+1)   [advance in text2]
     *   This is the classic "explore all paths" idea — we don't know which
     *   skip leads to a better future, so we try both.
     *   → max(solve(i+1, j), solve(i, j+1))
     *
     * WHY THIS COVERS ALL CASES:
     *   Any LCS must at some point either match a[i] with b[j], or skip one of them.
     *   These two cases are exhaustive and mutually exclusive at each step.
     *   The recursion tree explores all valid alignments implicitly.
     */
    int solve(string &a, string &b, int i, int j) {
        // Base case: one of the strings is fully traversed, no matches possible
        if (i == n1 || j == n2)
            return 0;

        // Return cached result if this subproblem was already solved
        // This is what converts exponential time to O(n1 * n2)
        if (dp[i][j] != -1)
            return dp[i][j];

        // CASE 1: Match found — greedily take it and move both pointers
        // It's provably safe to always take a match (can be shown via exchange argument)
        if (a[i] == b[j]) {
            return dp[i][j] = 1 + solve(a, b, i + 1, j + 1);
        }

        // CASE 2: No match — we must skip one character from either string.
        // We don't know which skip is better, so we try both and take the max.
        // This is the heart of the DP: optimal substructure + overlapping subproblems.
        return dp[i][j] = max(
            solve(a, b, i + 1, j),   // skip current char of text1
            solve(a, b, i, j + 1)    // skip current char of text2
        );
    }

    int longestCommonSubsequence(string text1, string text2) {
        n1 = text1.size();
        n2 = text2.size();

        // Initialize DP table with -1 (unvisited)
        // dp[i][j] represents LCS of text1[i..n1-1] and text2[j..n2-1]
        // Total unique states = n1 * n2, each computed once → O(n1 * n2) time & space
        dp.assign(n1, vector<int>(n2, -1));

        // Start from the beginning of both strings
        return solve(text1, text2, 0, 0);
    }

    /*
     * COMPLEXITY ANALYSIS:
     * Time:  O(n1 * n2) — each (i, j) pair computed exactly once
     * Space: O(n1 * n2) for the dp table + O(n1 + n2) recursion stack depth
     *
     * INTUITION RECAP:
     * Think of it as aligning two strings and deciding character by character:
     * - If characters align (match), great — count it and move on.
     * - If not, one of them is "wasted" at this step — try skipping each and
     *   take whichever path yields a longer subsequence downstream.
     * The DP table ensures we never recompute the same future state twice.
     *
     * RELATION TO EDIT DISTANCE:
     * LCS and Edit Distance are deeply related — both use the same
     * match/skip structure. In fact, Edit Distance = n1 + n2 - 2 * LCS
     * (when only insert/delete operations are allowed).
     */
};



class Solution {
public:
    /*
     * BOTTOM-UP (TABULATION) APPROACH TO LCS
     * Same problem, same logic as the memoized recursion — but flipped upside down.
     * Instead of starting at (0,0) and recursing toward base cases,
     * we START at the base cases and BUILD UP toward the answer.
     *
     * TABLE DEFINITION (crucial to get right):
     * dp[i][j] = length of LCS of text1[0..i-1] and text2[0..j-1]
     *            i.e., considering first i chars of text1, first j chars of text2.
     *
     * WHY 1-INDEXED WITH (n+1) x (m+1) TABLE?
     * The extra row and column (index 0) represent the empty string case.
     * dp[0][anything] = 0: LCS of empty string with anything = 0 (base case)
     * dp[anything][0] = 0: same reasoning.
     * This elegantly handles the base case without separate if-checks,
     * and lets us safely access dp[i-1][j-1] without going out of bounds.
     * So text1[i-1] is the i-th character — the -1 bridges the 1-indexed
     * dp table with the 0-indexed string.
     *
     * TRANSITION (same logic as recursion, just rewritten iteratively):
     *
     * CASE 1 — text1[i-1] == text2[j-1] (characters match):
     *   The current characters form a valid match. The LCS using these two chars
     *   = 1 + LCS of the strings WITHOUT these chars = 1 + dp[i-1][j-1].
     *   We look diagonally back because we "consumed" one char from BOTH strings.
     *
     * CASE 2 — characters don't match:
     *   Current chars can't be matched together. Best we can do:
     *   Option A: dp[i-1][j] → ignore text1[i-1], use first i-1 chars of text1
     *   Option B: dp[i][j-1] → ignore text2[j-1], use first j-1 chars of text2
     *   Take the max of both options.
     *   Think of it as: "which string should I retreat one step in?"
     *   We try both and take the better outcome.
     *
     * FILL ORDER: left-to-right, top-to-bottom.
     * This works because dp[i][j] only depends on:
     *   dp[i-1][j-1] (up-left diagonal)
     *   dp[i-1][j]   (directly above)
     *   dp[i][j-1]   (directly left)
     * All three are guaranteed to be already filled when we reach (i, j).
     */
    int longestCommonSubsequence(string text1, string text2) {
        int n = text1.size();
        int m = text2.size();

        // (n+1) x (m+1) table so row 0 and col 0 can serve as base cases (empty string).
        // All initialized to 0, which correctly encodes: LCS with empty string = 0.
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {

                if (text1[i - 1] == text2[j - 1])
                    // Characters match: extend the LCS found without either of these chars.
                    // Diagonal dp[i-1][j-1] = LCS of text1[0..i-2] and text2[0..j-2].
                    dp[i][j] = 1 + dp[i - 1][j - 1];

                else
                    // No match: best LCS achievable by skipping one char from either string.
                    // dp[i-1][j]: skip text1[i-1] — look one row up (shorter text1 prefix).
                    // dp[i][j-1]: skip text2[j-1] — look one col left (shorter text2 prefix).
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }

        // dp[n][m] = LCS of full text1 and full text2 — our final answer.
        return dp[n][m];

        /*
         * COMPLEXITY:
         * Time:  O(n * m) — fill every cell exactly once
         * Space: O(n * m) — the dp table
         *        Can be optimized to O(m) using two 1D arrays (prev row + curr row),
         *        since we only ever look at the current and previous row.
         *
         * BOTTOM-UP vs TOP-DOWN (memoization):
         * Both are O(n*m) time and space — same asymptotic complexity.
         * Bottom-up avoids recursion stack overhead and is cache-friendlier
         * (sequential memory access), making it faster in practice.
         * Top-down is more intuitive to derive and only computes needed states.
         *
         * VISUAL INTUITION (small example):
         * text1 = "ace", text2 = "abcde"
         *
         *       ""  a  b  c  d  e
         *   ""  [0, 0, 0, 0, 0, 0]
         *   a   [0, 1, 1, 1, 1, 1]
         *   c   [0, 1, 1, 2, 2, 2]
         *   e   [0, 1, 1, 2, 2, 3]  ← answer is 3 ("ace")
         *
         * Each cell builds on previous cells, and the diagonal moves represent matches.
         */
    }
};