// @https://leetcode.com/problems/shortest-common-supersequence/description/


// ============================================================
// PROBLEM: 1092. Shortest Common Supersequence
// ============================================================
//
// WHAT THE PROBLEM IS ASKING:
// Given two strings str1 and str2, find the SHORTEST string
// such that BOTH str1 and str2 are subsequences of it.
//
// A subsequence is formed by deleting some (or zero) characters
// from a string WITHOUT changing the relative order of remaining
// characters.
//
// Example: str1 = "abac", str2 = "cab"
// Answer:  "cabac"
//   - "abac" is a subsequence of "cabac" (delete the first 'c')
//   - "cab"  is a subsequence of "cabac" (delete the last 'a','c')
//
// KEY DEFINITIONS:
// - Supersequence: A string that contains both str1 and str2
//   as subsequences.
// - Shortest Common Supersequence (SCS): The smallest such string.
//
// CONSTRAINTS & IMPLICATIONS:
// - 1 <= str1.length, str2.length <= 1000
//   => O(m*n) DP is perfectly fine (at most 10^6 cells).
// - Only lowercase English letters, so no special char handling needed.
//
// EDGE CASES:
// - If one string is a subsequence of the other, the longer one
//   IS the SCS.
// - If both strings are identical, the SCS is just that string.
// - Strings of length 1: trivially handled.
//
// WHY NAIVE APPROACHES FAIL:
// - Trying all possible supersequences: exponential in length.
// - Greedy character-by-character selection: doesn't guarantee
//   globally minimum length because local choices can block
//   better future merges.
//
// ============================================================
// THINKING FROM FIRST PRINCIPLES
// ============================================================
//
// CORE OBSERVATION:
// Any character in the SCS comes from str1, str2, or BOTH
// (when a character is shared at aligned positions).
// The LONGER the SCS, the more characters we're "duplicating".
// To MINIMIZE the SCS length, we want to MAXIMIZE the characters
// that appear in BOTH str1 and str2 at the same position in the
// supersequence — i.e., we want to share as many characters as
// possible.
//
// The characters we can "share" (use once for both strings) are
// exactly the characters of the Longest Common Subsequence (LCS).
//
// FORMULA:
//   Length of SCS = len(str1) + len(str2) - len(LCS)
//
// Why? Every character of str1 must appear. Every character of
// str2 must appear. But characters in the LCS can be shared —
// used once to satisfy both strings — so we subtract the LCS
// length to avoid double-counting.
//
// TWO APPROACHES TO RECONSTRUCT THE ACTUAL SCS STRING:
//
// APPROACH 1: Via LCS DP Table (uses LCS length at each cell)
//   - Build LCS table.
//   - Backtrack: when chars match (LCS character), include once.
//     When they don't match, include the character from whichever
//     direction had the longer LCS.
//
// APPROACH 2: Via Edit-Distance-style DP (SCS length at each cell)
//   - dp[i][j] = length of SCS of str1[0..i-1] and str2[0..j-1]
//   - Backtrack similarly but compare SCS lengths.
//
// Both approaches yield the same result. We implement BOTH below.
//
// ============================================================
// DRY RUN (for both approaches)
// str1 = "abac" (m=4), str2 = "cab" (n=3)
//
// LCS DP TABLE (Approach 1):
//       ""  c  a  b
//   ""   0  0  0  0
//   a    0  0  1  1
//   b    0  0  1  2
//   a    0  0  1  2
//   c    0  1  1  2
//
// Backtracking from dp[4][3]=2:
//   str1[3]='c', str2[2]='b' → not equal, dp[3][3]=2 > dp[4][2]=1
//     → take str1[3]='c', move i=3
//   str1[2]='a', str2[2]='b' → not equal, dp[2][3]=2 > dp[3][2]=1
//     → take str1[2]='a', move i=2
//   str1[1]='b', str2[2]='b' → equal! take 'b', i=1, j=2
//   str1[0]='a', str2[1]='a' → equal! take 'a', i=0, j=1
//   i=0, drain j: take str2[0]='c'
//   ans (reversed) = "cabac" ✓
//
// ============================================================
// APPROACH 1: LCS-based reconstruction
// ============================================================

class Solution1 {
public:
    string shortestCommonSupersequence(string str1, string str2) {

        int m = str1.size();
        int n = str2.size();

        // dp[i][j] = length of LCS of str1[0..i-1] and str2[0..j-1]
        // We use (m+1) x (n+1) to handle empty prefix base cases.
        // dp[0][*] = 0: LCS of empty str1 with anything is 0.
        // dp[*][0] = 0: LCS of anything with empty str2 is 0.
        // These are already 0 by default initialization.
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

        // FILL THE LCS TABLE
        // State: dp[i][j] = LCS length of first i chars of str1
        //                    and first j chars of str2.
        // Transition:
        //   If str1[i-1] == str2[j-1]: characters match, extend LCS by 1
        //     dp[i][j] = 1 + dp[i-1][j-1]  (use this shared char)
        //   Else: best of ignoring current char of str1 or str2
        //     dp[i][j] = max(dp[i-1][j], dp[i][j-1])
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (str1[i - 1] == str2[j - 1])
                    dp[i][j] = 1 + dp[i - 1][j - 1];
                else
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }

        // BACKTRACK TO BUILD THE SCS STRING
        // We start at (m, n) — the full strings — and move toward (0,0).
        // We build ans in REVERSE (appending then reversing at end).
        int i = m, j = n;
        string ans = "";

        // While both strings have unprocessed characters:
        while (i > 0 && j > 0) {

            if (str1[i - 1] == str2[j - 1]) {
                // Characters match → this is part of the LCS.
                // We SHARE this character (include it once in SCS).
                // Move both pointers since both strings are satisfied here.
                ans.push_back(str1[i - 1]);
                i--;
                j--;
            }
            else if (dp[i - 1][j] > dp[i][j - 1]) {
                // LCS is longer when we "skip" str1[i-1] and come from above.
                // This means str1[i-1] is NOT in LCS → must include it in SCS
                // separately (it can't be shared with str2).
                // We include str1[i-1] and move up (i--).
                ans.push_back(str1[i - 1]);
                i--;
            }
            else {
                // LCS is longer when we "skip" str2[j-1] and come from left.
                // str2[j-1] is NOT in LCS → include it separately in SCS.
                ans.push_back(str2[j - 1]);
                j--;
            }
        }

        // DRAIN REMAINING CHARACTERS
        // If str1 has remaining characters (j exhausted first),
        // all of them must be included in SCS (they can't match anything).
        while (i > 0) {
            ans.push_back(str1[i - 1]);
            i--;
        }

        // If str2 has remaining characters (i exhausted first),
        // include all of them in SCS.
        while (j > 0) {
            ans.push_back(str2[j - 1]);
            j--;
        }

        // We built ans in REVERSE ORDER (from the end of strings to start),
        // so reverse to get the correct SCS.
        reverse(ans.begin(), ans.end());
        return ans;
    }
};

// ============================================================
// APPROACH 2: Edit-Distance-Style DP (SCS length at each cell)
// ============================================================
//
// Instead of storing LCS lengths, store SCS lengths directly.
//
// STATE: dp[i][j] = length of SCS of str1[0..i-1] and str2[0..j-1]
//
// BASE CASES:
//   dp[i][0] = i  → SCS of str1[0..i-1] with empty str2 is str1 itself
//   dp[0][j] = j  → SCS of empty str1 with str2[0..j-1] is str2 itself
//
// TRANSITION:
//   If str1[i-1] == str2[j-1]:
//     dp[i][j] = 1 + dp[i-1][j-1]
//     (share the common character, cost = cost of aligning both minus 1)
//   Else:
//     dp[i][j] = 1 + min(dp[i-1][j], dp[i][j-1])
//     (include either str1[i-1] or str2[j-1], take the cheaper option)
//
// BACKTRACKING:
//   Same logic but now we compare SCS lengths (smaller = better).
//   - If chars match: shared → include once, move both.
//   - If dp[i-1][j] < dp[i][j-1]: came from top → str1[i-1] was added.
//   - Else: came from left → str2[j-1] was added.
//
// WHY THIS IS CORRECT:
//   The SCS length formula guarantees we always take the optimal merge.
//   The backtracking retraces exactly which choices were made optimally.
//
// ============================================================

class Solution {
public:
    string shortestCommonSupersequence(string str1, string str2) {

        int m = str1.size();
        int n = str2.size();

        // dp[i][j] stores length of SCS of str1[0..i-1] and str2[0..j-1].
        // (m+1) x (n+1) to accommodate empty string base cases.
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

        // BASE CASE: dp[i][0] = i
        // SCS of first i characters of str1 with empty str2
        // is simply str1[0..i-1] (length i). We must include all of str1.
        for (int i = 0; i <= m; i++)
            dp[i][0] = i;

        // BASE CASE: dp[0][j] = j
        // SCS of empty str1 with first j characters of str2
        // is simply str2[0..j-1] (length j). We must include all of str2.
        for (int j = 0; j <= n; j++)
            dp[0][j] = j;

        // FILL DP TABLE
        // For each pair (i, j), compute the SCS length.
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {

                if (str1[i - 1] == str2[j - 1]) {
                    // Characters match → share this character.
                    // SCS = SCS of (i-1, j-1) + this shared character.
                    // Cost = dp[i-1][j-1] + 1
                    dp[i][j] = 1 + dp[i - 1][j - 1];
                }
                else {
                    // Characters don't match → must include one separately.
                    // Option A: Include str1[i-1] → then solve (i-1, j): dp[i-1][j]
                    // Option B: Include str2[j-1] → then solve (i, j-1): dp[i][j-1]
                    // Choose whichever gives shorter SCS, add 1 for current char.
                    dp[i][j] = 1 + min(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }

        // BACKTRACK FROM (m, n) TO (0, 0) TO RECONSTRUCT THE SCS
        // We reverse-engineer which choices were made during DP fill.
        int i = m, j = n;
        string ans = "";

        // Process until one of the strings is fully consumed.
        while (i > 0 && j > 0) {

            if (str1[i - 1] == str2[j - 1]) {
                // Characters match → this was a shared character in the SCS.
                // Include it once, move both pointers back.
                ans.push_back(str1[i - 1]);
                i--;
                j--;
            }
            else if (dp[i - 1][j] < dp[i][j - 1]) {
                // The DP came from dp[i-1][j] (moving i pointer).
                // This means str1[i-1] was added to SCS as a standalone character.
                // Include it in answer, move i back.
                ans.push_back(str1[i - 1]);
                i--;
            }
            else {
                // The DP came from dp[i][j-1] (moving j pointer).
                // str2[j-1] was added standalone. Include it, move j back.
                ans.push_back(str2[j - 1]);
                j--;
            }
        }

        // If str1 still has characters remaining (str2 was exhausted),
        // they must all appear in SCS — append them in reverse order.
        while (i > 0) {
            ans.push_back(str1[i - 1]);
            i--;
        }

        // If str2 still has characters remaining (str1 was exhausted),
        // they must all appear in SCS — append them in reverse order.
        while (j > 0) {
            ans.push_back(str2[j - 1]);
            j--;
        }

        // We built from the END of strings to START (reverse direction).
        // Reverse to restore correct order.
        reverse(ans.begin(), ans.end());
        return ans;
    }
};

// ============================================================
// DRY RUN (Approach 2)
// str1 = "abac" (m=4), str2 = "cab" (n=3)
//
// SCS DP TABLE:
//       ""  c  a  b
//   ""   0  1  2  3
//   a    1  1  1  2
//   b    2  2  2  1  ← wait, let's recalculate
//
// Correct fill:
//       j=0  j=1(c)  j=2(a)  j=3(b)
// i=0:   0      1       2       3
// i=1(a):1      2       2       3
// i=2(b):2      3       3       2  ← str1[1]='b'==str2[2]='b': 1+dp[1][2]=1+2=3? No
//   Wait: i=2 means str1[0..1]="ab", j=3 means str2[0..2]="cab"
//   str1[1]='b'==str2[2]='b' → dp[2][3]=1+dp[1][2]=1+2=3
//   Hmm. Let me redo:
//
//   dp[1][1]: str1[0]='a' vs str2[0]='c' → no match → 1+min(dp[0][1],dp[1][0])=1+min(1,1)=2
//   dp[1][2]: str1[0]='a' == str2[1]='a' → match! 1+dp[0][1]=1+1=2
//   dp[1][3]: str1[0]='a' vs str2[2]='b' → no match → 1+min(dp[0][3],dp[1][2])=1+min(3,2)=3
//   dp[2][1]: str1[1]='b' vs str2[0]='c' → no → 1+min(dp[1][1],dp[2][0])=1+min(2,2)=3
//   dp[2][2]: str1[1]='b' vs str2[1]='a' → no → 1+min(dp[1][2],dp[2][1])=1+min(2,3)=3
//   dp[2][3]: str1[1]='b' == str2[2]='b' → match! 1+dp[1][2]=1+2=3
//   dp[3][1]: str1[2]='a' vs str2[0]='c' → no → 1+min(dp[2][1],dp[3][0])=1+min(3,3)=4
//   dp[3][2]: str1[2]='a' == str2[1]='a' → match! 1+dp[2][1]=1+3=4
//   dp[3][3]: str1[2]='a' vs str2[2]='b' → no → 1+min(dp[2][3],dp[3][2])=1+min(3,4)=4
//   dp[4][1]: str1[3]='c' == str2[0]='c' → match! 1+dp[3][0]=1+3=4
//   dp[4][2]: str1[3]='c' vs str2[1]='a' → no → 1+min(dp[3][2],dp[4][1])=1+min(4,4)=5
//   dp[4][3]: str1[3]='c' vs str2[2]='b' → no → 1+min(dp[3][3],dp[4][2])=1+min(4,5)=5
//
// dp[4][3] = 5 = length of SCS ✓ (|SCS|=5 for "cabac")
//
// BACKTRACK from (4,3):
//   str1[3]='c' vs str2[2]='b' → no match
//   dp[3][3]=4 < dp[4][2]=5 → take str1[3]='c', i=3
//   str1[2]='a' vs str2[2]='b' → no match
//   dp[2][3]=3 < dp[3][2]=4 → take str1[2]='a', i=2  [Wait: str1[2]='a', str2[2]='b']
//   Hmm, dp[i-1][j]=dp[2][3]=3, dp[i][j-1]=dp[3][2]=4 → dp[2][3]<dp[3][2]: take str1[2]='a', i=2
//   str1[1]='b' == str2[2]='b' → match! take 'b', i=1, j=2
//   str1[0]='a' == str2[1]='a' → match! take 'a', i=0, j=1
//   i=0, drain j: take str2[0]='c'
//   ans (before reverse): "cabc" + "c" => reversed: "cabac" ✓
//
// ============================================================
// TIME COMPLEXITY:
//   DP table fill: O(m * n) — two nested loops each running m and n times.
//   Backtracking: O(m + n) — at each step, at least one of i or j decreases.
//   Reverse: O(m + n) — final string has length at most m+n.
//   TOTAL: O(m * n)
//
// SPACE COMPLEXITY:
//   DP table: O(m * n) — we store the full (m+1) x (n+1) table.
//   Answer string: O(m + n) — SCS length is at most m + n.
//   TOTAL: O(m * n)
//   (Can be optimized to O(min(m,n)) for length computation only,
//    but reconstruction requires the full table.)
//
// IS THIS OPTIMAL?
//   - LCS computation inherently requires comparing all pairs of characters,
//     making O(m*n) a known lower bound for general strings.
//   - Our reconstruction is linear in output size.
//   - This is optimal for the general case.
// ============================================================