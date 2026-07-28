// ============================================================
// EDIT DISTANCE (Levenshtein Distance)
// ============================================================
//
// PROBLEM:
// Given two strings word1 and word2, find the minimum number of
// single-character operations to transform word1 into word2.
// The three allowed operations are:
//   - Insert  a character anywhere into word1
//   - Delete  any character from word1
//   - Replace any character in word1 with another character
//
// Example: "horse" → "ros" in 3 operations:
//   horse → rorse (replace h with r)
//   rorse → rose  (delete r)
//   rose  → ros   (delete e)
//
// ============================================================
// CORE IDEA — TWO CURSORS:
// ============================================================
//
// Instead of thinking about the full string, think of two cursors:
// one scanning word1 (at index i), one scanning word2 (at index j).
// Both start at 0. Our goal is to "consume" both strings completely.
//
// At every position (i, j) we ask:
//   "What's the cheapest way to make word1[i..end] match word2[j..end]?"
//
// This is our subproblem: f(i, j).
// We want f(0, 0) — the answer for the full strings.
//
// ============================================================
// THE 3 OPTIONS AT A MISMATCH (word1[i] != word2[j]):
// ============================================================
//
// Say word1 = "horse", word2 = "ros", and we're at i=0, j=0.
// 'h' != 'r', so we're stuck. We pay 1 operation and pick the best move:
//
// 1) DELETE word1[i]  →  subproblem becomes f(i+1, j)
//    We throw away 'h'. word1 becomes "orse".
//    word2 is unchanged — 'r' is still sitting there unmatched.
//    So j stays put. i moves forward because 'h' no longer exists.
//    New problem: can "orse" cheaply become "ros"?
//
// 2) INSERT word2[j] before word1[i]  →  subproblem becomes f(i, j+1)
//    We insert 'r' before 'h'. word1 becomes "rhorse".
//    That inserted 'r' matches word2[j]='r' perfectly, so j advances.
//    But 'h' is still sitting there untouched, so i stays.
//    New problem: can "horse" cheaply become "os"?
//    (We don't track the inserted character — advancing j IS the match.)
//
// 3) REPLACE word1[i] with word2[j]  →  subproblem becomes f(i+1, j+1)
//    We overwrite 'h' with 'r'. Both are now handled.
//    Both cursors advance.
//    New problem: can "orse" cheaply become "os"?
//
// MATCH — word1[i] == word2[j]  →  subproblem becomes f(i+1, j+1), FREE
//    Characters align naturally. No operation needed.
//    Both cursors advance at zero cost.
//
// ┌───────────┬────────────┬────────────┬─────────────────────────────────┐
// │ Operation │ i (word1)  │ j (word2)  │ Why                             │
// ├───────────┼────────────┼────────────┼─────────────────────────────────┤
// │ Delete    │ advances   │ stays      │ word2[j] still unmatched        │
// │ Insert    │ stays      │ advances   │ word1[i] still unhandled        │
// │ Replace   │ advances   │ advances   │ both forcefully handled         │
// │ Match     │ advances   │ advances   │ both naturally aligned, no cost │
// └───────────┴────────────┴────────────┴─────────────────────────────────┘
//
// WHY TRY ALL THREE?
// We have no way of knowing locally which operation leads to the cheapest
// overall solution. Delete might look good now but create 5 problems later.
// Replace costs 1 now but might save 3 operations down the line.
// So we try all three and let the DP bubble up the best answer.
//
// ============================================================
// BASE CASES:
// ============================================================
//
// f(n, j) = m - j  →  word1 fully consumed, word2 has (m-j) chars left.
//                      No choice but to INSERT all of them. Cost = m - j.
//
// f(i, m) = n - i  →  word2 fully matched, word1 has (n-i) chars left.
//                      No choice but to DELETE all of them. Cost = n - i.
//
// ============================================================
// WHY DP?
// ============================================================
//
// Different sequences of operations can land us at the same (i, j).
// E.g., delete then insert vs insert then delete — same state, same future.
// So we cache f(i, j). Total unique states: O(n * m).
// This collapses the O(3^(n+m)) brute force down to O(n * m).
//
// ============================================================


// ============================================================
// APPROACH 1: TOP-DOWN (Memoized Recursion)
// ============================================================
// Start at (0, 0) and recurse toward the base cases.
// Cache every (i, j) result so we never solve the same suffix pair twice.

class Solution {
public:
    vector<vector<int>> dp;
    int n, m;

    int solve(string &a, string &b, int i, int j) {
        // word1 exhausted — must insert every remaining word2 character
        if (i == n) return m - j;

        // word2 exhausted — must delete every remaining word1 character
        if (j == m) return n - i;

        // this suffix pair already solved — reuse cached result
        if (dp[i][j] != -1) return dp[i][j];

        // characters align — free move, no operation spent
        if (a[i] == b[j])
            return dp[i][j] = solve(a, b, i+1, j+1);

        // mismatch — spend 1 operation, try all three moves, keep the cheapest
        return dp[i][j] = 1 + min({
            solve(a, b, i+1, j),    // DELETE:  discard word1[i], word2[j] waits
            solve(a, b, i, j+1),    // INSERT:  place word2[j], word1[i] waits
            solve(a, b, i+1, j+1)   // REPLACE: force-match, both advance
        });
    }

    int minDistance(string word1, string word2) {
        n = word1.size();
        m = word2.size();
        // -1 signals "not yet computed" — all real answers are >= 0
        dp.assign(n, vector<int>(m, -1));
        return solve(word1, word2, 0, 0);
    }
};

