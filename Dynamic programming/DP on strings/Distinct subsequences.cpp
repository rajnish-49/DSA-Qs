/*
 * ============================================================
 * PROBLEM: Distinct Subsequences (LeetCode #115)
 * ============================================================
 *
 * PROBLEM STATEMENT (in simple terms):
 * -------------------------------------
 * Given two strings `s` (source) and `t` (target), count the number
 * of distinct ways you can form string `t` by deleting some (or no)
 * characters from `s` — WITHOUT reordering the remaining characters.
 *
 * INPUT:
 *   - s : A string (the larger "pool" of characters we pick from)
 *   - t : A string (the pattern we want to form)
 *
 * OUTPUT:
 *   - An integer: the total number of distinct subsequences of `s`
 *     that equal `t`
 *
 * EXAMPLE:
 *   s = "rabbbit", t = "rabbit"
 *   → Output: 3
 *   The three 'b's in "rabbbit" give us 3 ways to pick 2 b's for "rabbit"
 *
 * ============================================================
 * INTUITION & APPROACH FROM FIRST PRINCIPLES
 * ============================================================
 *
 * OBSERVATION 1 — Subsequence Counting:
 *   We need to count ways to "match" every character of t using
 *   characters of s in left-to-right order. At each character of s,
 *   we face a binary choice:
 *     (a) Use s[i] to match t[j]  →  move both pointers forward
 *     (b) Skip s[i]               →  move only the s pointer forward
 *
 * OBSERVATION 2 — Overlapping Subproblems:
 *   The same pair (i, j) — meaning "how many ways can s[i..] form t[j..]"
 *   — gets computed repeatedly in a naive recursion. This screams DP.
 *
 * OBSERVATION 3 — Why Brute Force Fails:
 *   Pure recursion without memoization explores every subset of s,
 *   giving O(2^n) time. For strings of length 1000, this is infeasible.
 *
 * MOVING TO DYNAMIC PROGRAMMING:
 *   Define dp[i][j] = number of distinct ways to form t[j..n2-1]
 *                     using characters from s[i..n1-1]
 *
 *   TRANSITIONS:
 *     If s[i] == t[j]:
 *       We have two choices — use s[i] to match t[j], OR skip s[i].
 *       dp[i][j] = dp[i+1][j+1]   (use s[i] to match t[j])
 *                + dp[i+1][j]     (skip s[i], look for t[j] later)
 *
 *     If s[i] != t[j]:
 *       s[i] is useless for matching t[j], must skip it.
 *       dp[i][j] = dp[i+1][j]
 *
 *   BASE CASES:
 *     j == n2  → We've matched all of t. This is 1 valid way.   → return 1
 *     i == n1  → We've exhausted s but t isn't matched yet.     → return 0
 *
 * BOTTOM-UP REFRAMING (Solution 2):
 *   Flip indexing: dp[i][j] = ways to form t[0..j-1] using s[0..i-1]
 *   This lets us fill a table iteratively, avoiding recursion overhead.
 *   Base case: dp[i][0] = 1 for all i (empty t is always achievable).
 *
 * TIME COMPLEXITY:  O(n1 * n2)
 * SPACE COMPLEXITY: O(n1 * n2)  [can be optimized to O(n2) with 1D DP]
 * ============================================================
 */


// ============================================================
// SOLUTION 1: TOP-DOWN DP (Memoized Recursion)
// ============================================================
// Approach: Recurse from the beginning of both strings.
// At each step, decide whether to use or skip s[i] for matching t[j].
// Cache results in a 2D dp table to avoid recomputation.
// ============================================================

class Solution {
public:
    vector<vector<int>> dp;
    int n1, n2;

    /*
     * solve(s, t, i, j):
     *   Returns the number of distinct ways to form t[j..n2-1]
     *   using characters from s[i..n1-1].
     *
     *   Think of i as "how far we've consumed s"
     *   and j as "how far we've matched t".
     */
    int solve(string s, string t, int i, int j) {

        // BASE CASE 1: j has reached the end of t.
        // This means we've successfully matched all characters of t.
        // This counts as exactly 1 valid distinct subsequence.
        if (j >= n2) {
            return 1;
        }

        // BASE CASE 2: i has reached the end of s, but t isn't fully matched.
        // We've run out of source characters — no valid match possible from here.
        if (i >= n1) {
            return 0;
        }

        // MEMOIZATION CHECK: If this (i, j) state was already solved,
        // return the cached answer to avoid exponential recomputation.
        if (dp[i][j] != -1) {
            return dp[i][j];
        }

        // TRANSITION: Current characters match — we have two independent choices.
        if (s[i] == t[j]) {
            /*
             * Choice 1 — USE s[i] to match t[j]:
             *   Both pointers advance → solve(i+1, j+1)
             *   This "consumes" both s[i] and t[j].
             *
             * Choice 2 — SKIP s[i]:
             *   Only s's pointer advances → solve(i+1, j)
             *   We ignore s[i] and look for t[j] later in s.
             *
             * The total count is the SUM of both paths,
             * because each path represents a distinct set of index choices.
             */
            return dp[i][j] = solve(s, t, i + 1, j + 1) + solve(s, t, i + 1, j);
        }
        else {
            /*
             * TRANSITION: Characters don't match.
             * s[i] cannot contribute to matching t[j], so we must skip s[i].
             * Only s's pointer moves forward; t's pointer stays to retry t[j].
             */
            return dp[i][j] = solve(s, t, i + 1, j);
        }
    }

    int numDistinct(string s, string t) {
        n1 = s.size();
        n2 = t.size();

        // Initialize the memoization table with -1 to indicate "not yet computed".
        // Size is n1 x n2 because states are defined by position in s and position in t.
        dp.assign(n1, vector<int>(n2, -1));

        // Start matching from the very beginning of both strings.
        return solve(s, t, 0, 0);
    }
};


// ============================================================
// SOLUTION 2: BOTTOM-UP DP (Iterative Tabulation)
// ============================================================
// Approach: Build the answer table from small subproblems up to the full problem.
// dp[i][j] = number of ways to form t[0..j-1] using s[0..i-1].
//
// NOTE: We use (n1+1) x (n2+1) table with 1-indexed logic so that
// index 0 represents the "empty prefix" — a clean base case.
// long long is used to avoid integer overflow for large inputs.
// ============================================================

class Solution {
public:
    int numDistinct(string s, string t) {
        int n1 = s.size();
        int n2 = t.size();

        /*
         * dp[i][j] = number of distinct subsequences of s[0..i-1]
         *            that equal t[0..j-1]
         *
         * The extra row/column (index 0) handles the empty-string base case cleanly.
         * We use long long to prevent overflow when counts become very large.
         */
        vector<vector<long long>> dp(n1 + 1, vector<long long>(n2 + 1, 0));

        /*
         * BASE CASE: dp[i][0] = 1 for all i (including i = 0).
         * An empty target string t can always be matched in exactly 1 way
         * (by choosing no characters from s) — regardless of how long s is.
         */
        for (int i = 0; i <= n1; i++) {
            dp[i][0] = 1;
        }

        // dp[0][j] = 0 for j > 0 (already set by initialization):
        // An empty s cannot form any non-empty t. No action needed.

        /*
         * FILL THE TABLE: Process each character of s (outer loop)
         * against each character of t (inner loop).
         *
         * i represents: we're considering s[0..i-1] as our source pool.
         * j represents: we're trying to form t[0..j-1] as our target.
         */
        for (int i = 1; i <= n1; i++) {
            for (int j = 1; j <= n2; j++) {

                if (s[i - 1] == t[j - 1]) {
                    /*
                     * Characters match — mirror the top-down logic:
                     *
                     * dp[i-1][j-1]: Ways to form t[0..j-2] using s[0..i-2].
                     *   → Represents using s[i-1] to match t[j-1].
                     *
                     * dp[i-1][j]:   Ways to form t[0..j-1] using s[0..i-2].
                     *   → Represents skipping s[i-1] entirely.
                     *
                     * We add both because they are independent contributions.
                     */
                    dp[i][j] = dp[i - 1][j - 1] + dp[i - 1][j];
                }
                else {
                    /*
                     * Characters don't match — s[i-1] cannot match t[j-1].
                     * The only option is to skip s[i-1].
                     * So the count equals whatever we could form using s[0..i-2].
                     */
                    dp[i][j] = dp[i - 1][j];
                }
            }
        }

        /*
         * FINAL ANSWER: dp[n1][n2]
         * = number of distinct ways to form the full t using the full s.
         * Cast to int for the return type (LeetCode guarantees it fits).
         */
        return dp[n1][n2];
    }
};