/*
 * ============================================================
 * PROBLEM: Shortest Common Supersequence (LeetCode 1092)
 * ============================================================
 *
 * PROBLEM STATEMENT (plain terms):
 * Given two strings str1 and str2, find the SHORTEST string
 * that contains BOTH str1 and str2 as subsequences.
 *
 * A subsequence means you can delete some characters (not
 * necessarily contiguous) from a string and get the target.
 *
 * Example: str1="abac", str2="cab" → answer="cabac"
 *   - "abac" is in "cabac": delete the first 'c'
 *   - "cab"  is in "cabac": delete the last 'a','c'
 *
 * KEY INSIGHT — why LCS?
 * The SCS must contain every character of both strings.
 * But characters that appear in the LCS can be SHARED —
 * they satisfy both strings at once, appearing only once
 * in the result instead of twice.
 *
 * So:  SCS_length = len(str1) + len(str2) - LCS_length
 *
 * Every character NOT in the LCS must appear separately
 * from its own string. Every character IN the LCS appears
 * exactly once, satisfying both strings simultaneously.
 *
 * WHY NOT BRUTE FORCE?
 * Trying all interleavings of str1 and str2 is exponential.
 * We need overlapping subproblems → DP on prefixes.
 *
 * APPROACH: LCS-based DP
 * Step 1: Build LCS dp table over all prefix pairs.
 * Step 2: Reconstruct SCS by walking back through the table:
 *         - Match (LCS char)  → include ONCE, both pointers move
 *         - No match, came from top  → include str1's char
 *         - No match, came from left → include str2's char
 */

// ============================================================
// APPROACH 1: LCS-based (build LCS table, reconstruct SCS)
// ============================================================
class Solution {
public:
    string shortestCommonSupersequence(string str1, string str2) {
        int m = str1.size(), n = str2.size();

        /*
         * dp[i][j] = length of LCS of str1[0..i-1] and str2[0..j-1]
         *
         * We use (m+1) x (n+1) to handle empty prefix base cases cleanly.
         * dp[0][anything] = 0: LCS of empty string with anything = 0
         * dp[anything][0] = 0: same — initialized by default (vector fill 0)
         */
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (str1[i-1] == str2[j-1])
                    /*
                     * Characters match → this character can be SHARED.
                     * It extends the LCS of the prefixes before this pair.
                     * Both pointers consumed → look at dp[i-1][j-1].
                     */
                    dp[i][j] = 1 + dp[i-1][j-1];
                else
                    /*
                     * No match → this character cannot be shared.
                     * Best LCS we can get is by either:
                     *   - Skipping str1[i-1]: LCS of str1[0..i-2] with str2[0..j-1] = dp[i-1][j]
                     *   - Skipping str2[j-1]: LCS of str1[0..i-1] with str2[0..j-2] = dp[i][j-1]
                     * Take the max since we want the LONGEST common subsequence.
                     */
                    dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
            }
        }

        /*
         * RECONSTRUCTION: build SCS by retracing decisions in the dp table.
         *
         * We walk from (m, n) back to (0, 0).
         * At each step we ask: how did dp[i][j] get its value?
         * That answer tells us which character belongs in the SCS here.
         *
         * We build the result in REVERSE order (collecting while walking
         * backwards), then reverse at the end.
         *
         * Three cases:
         *   MATCH        → char is in LCS, shared → write ONCE, move both i and j
         *   dp[i-1][j] was bigger → we came from top → str1[i-1] was skipped
         *                           in LCS, must include it in SCS → write str1[i-1], move i
         *   dp[i][j-1] was bigger → we came from left → str2[j-1] was skipped
         *                           in LCS, must include it in SCS → write str2[j-1], move j
         */
        string result = "";
        int i = m, j = n;

        while (i > 0 && j > 0) {
            if (str1[i-1] == str2[j-1]) {
                /*
                 * This character is part of the LCS → shared between both strings.
                 * Include it ONCE in the SCS (it satisfies both strings here).
                 * Move both pointers since both strings are "handled" at this position.
                 */
                result += str1[i-1];
                i--; j--;
            } else if (dp[i-1][j] > dp[i][j-1]) {
                /*
                 * dp[i-1][j] > dp[i][j-1] means the LCS was achieved by
                 * keeping str1[i-1] unmatched (skipped from LCS perspective).
                 * Since it's not shared, it MUST appear in the SCS on its own.
                 * Include str1[i-1] and move i — j stays because str2[j-1]
                 * is still unresolved.
                 */
                result += str1[i-1];
                i--;
            } else {
                /*
                 * Symmetric case: LCS came from dp[i][j-1], meaning str2[j-1]
                 * was the one skipped. It must appear in SCS on its own.
                 * Include str2[j-1] and move j — i stays.
                 */
                result += str2[j-1];
                j--;
            }
        }

        /*
         * One string is fully processed (its pointer hit 0).
         * The other may still have characters remaining — those characters
         * have no counterpart left, so they all go directly into the SCS.
         * No decisions needed: just flush them in reverse order.
         */
        while (i > 0) { result += str1[i-1]; i--; }
        while (j > 0) { result += str2[j-1]; j--; }

        /*
         * We collected characters while walking backwards through the table,
         * so the result string is currently reversed. Fix it.
         */
        reverse(result.begin(), result.end());
        return result;
    }
};


/*
 * ============================================================
 * APPROACH 2: Direct SCS DP (store SCS length, not LCS length)
 * ============================================================
 *
 * Instead of computing LCS and deriving SCS from it,
 * we directly store SCS lengths in the dp table.
 *
 * dp[i][j] = length of SCS of str1[0..i-1] and str2[0..j-1]
 *
 * RECURRENCE:
 *   if str1[i-1] == str2[j-1]:
 *       dp[i][j] = 1 + dp[i-1][j-1]
 *       → share the character, cost is 1 (not 2)
 *
 *   else:
 *       dp[i][j] = 1 + min(dp[i-1][j], dp[i][j-1])
 *       → must include one character right now (cost 1),
 *         then solve the remaining subproblem optimally.
 *         dp[i-1][j]: include str1[i-1], solve (i-1, j) next
 *         dp[i][j-1]: include str2[j-1], solve (i, j-1) next
 *         Take MIN because we want SHORTEST supersequence.
 *
 * BASE CASES:
 *   dp[i][0] = i → SCS of str1[0..i-1] with empty str2 = str1 itself
 *   dp[0][j] = j → SCS of empty str1 with str2[0..j-1] = str2 itself
 *
 * RECONSTRUCTION logic is identical in spirit to Approach 1,
 * but we follow MIN instead of MAX when retracing decisions.
 */
class Solution {
public:
    string shortestCommonSupersequence(string str1, string str2) {
        int m = str1.size(), n = str2.size();

        /*
         * dp[i][j] = SCS length of str1[0..i-1] and str2[0..j-1].
         * Sized (m+1) x (n+1) so index i refers to prefix of length i,
         * and i=0 cleanly represents the empty string.
         */
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

        /*
         * Base case: if one string is empty, the SCS is just the other string.
         * dp[i][0] = i means: to merge str1[0..i-1] with "", you need i characters.
         * dp[0][j] = j means: to merge "" with str2[0..j-1], you need j characters.
         *
         * Note: in Approach 1 (LCS), these were 0 by default — empty LCS with anything = 0.
         * Here they must be set explicitly because SCS with empty string = the string itself.
         */
        for (int i = 0; i <= m; i++) dp[i][0] = i;
        for (int j = 0; j <= n; j++) dp[0][j] = j;

        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (str1[i-1] == str2[j-1])
                    /*
                     * Characters match → share them.
                     * Both strings are satisfied by this one character.
                     * Cost = 1 (not 2), and we reduce both subproblems by 1.
                     */
                    dp[i][j] = 1 + dp[i-1][j-1];
                else
                    /*
                     * No match → we must include exactly one character now
                     * (we can't share it). Two options:
                     *
                     * Option A — include str1[i-1]: costs 1, then solve dp[i-1][j]
                     * Option B — include str2[j-1]: costs 1, then solve dp[i][j-1]
                     *
                     * We want the shorter result → take min of the two subproblems.
                     * The +1 accounts for the character we are including right now.
                     */
                    dp[i][j] = 1 + min(dp[i-1][j], dp[i][j-1]);
            }
        }

        /*
         * RECONSTRUCTION: retrace which decision built each cell.
         *
         * Key idea: dp[i][j]'s value came from exactly one of three places.
         * By checking which neighbour it came from, we know which character
         * was placed into the SCS at this step.
         *
         * Walk from (m, n) → (0, 0), collect characters in reverse.
         */
        string result = "";
        int i = m, j = n;

        while (i > 0 && j > 0) {
            if (str1[i-1] == str2[j-1]) {
                /*
                 * Match → this character was SHARED (came from dp[i-1][j-1] + 1).
                 * It satisfies both strings → write it once.
                 * Both pointers move because both strings are resolved here.
                 */
                result += str1[i-1];
                i--; j--;
            } else if (dp[i-1][j] < dp[i][j-1]) {
                /*
                 * dp[i][j] was built by placing str1[i-1] (option A was cheaper).
                 * Retrace that decision: write str1[i-1], move i back.
                 * j stays because str2[j-1] is still unresolved — it will be
                 * handled in a later (earlier in reconstruction) step.
                 */
                result += str1[i-1];
                i--;
            } else {
                /*
                 * dp[i][j] was built by placing str2[j-1] (option B was cheaper
                 * or equal). Retrace: write str2[j-1], move j back.
                 * i stays because str1[i-1] is still unresolved.
                 */
                result += str2[j-1];
                j--;
            }
        }

        /*
         * If str2 is exhausted (j=0) but str1 still has characters,
         * those characters have no str2 counterpart remaining.
         * They must all appear in the SCS as-is — flush them.
         * Same logic applies if str1 is exhausted but str2 is not.
         */
        while (i > 0) { result += str1[i-1]; i--; }
        while (j > 0) { result += str2[j-1]; j--; }

        /*
         * Result was built right-to-left (we walked the table backwards).
         * Reverse to get the correct left-to-right SCS string.
         */
        reverse(result.begin(), result.end());
        return result;
    }
};