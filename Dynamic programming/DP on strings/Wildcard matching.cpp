/*
================================================================================
PROBLEM: 44. Wildcard Matching (Hard)
================================================================================

WHAT THE PROBLEM ASKS:
    Given a string s (the text) and a pattern p (the rule),
    determine if pattern p can match the ENTIRE string s.

    Think of it like a filter:
        p  ----tries to match---->  s
        p has the RULES ('?', '*', letters)
        s has the ACTUAL TEXT

    Like file search on your computer:
        *.txt   (pattern)  matches   notes.txt  (string)

SPECIAL CHARACTERS:
    '?' → matches EXACTLY ONE character (any character)
    '*' → matches ZERO OR MORE characters (any characters)

    Regular letters → must match EXACTLY the same letter in s

IMPORTANT: The match must cover the ENTIRE string s, not just a part.

CONSTRAINTS:
    0 <= s.length, p.length <= 2000
    s contains only lowercase English letters
    p contains only lowercase English letters, '?' or '*'

    Implication of size 2000:
        O(n^2) solution is acceptable → ~4,000,000 operations
        O(2^n) brute force is NOT acceptable → way too slow

EDGE CASES:
    1. s = "",  p = ""    → true  (both empty, trivially match)
    2. s = "",  p = "*"   → true  (* matches empty sequence)
    3. s = "",  p = "?"   → false (? needs exactly one char, none available)
    4. s = "a", p = ""    → false (pattern exhausted, string still has chars)
    5. s = "a", p = "***" → true  (multiple *s still just match anything)

WHY NAIVE APPROACH FAILS:
    Naive idea: just walk through p and s character by character.
    Problem: when you see '*', you don't know HOW MANY characters it consumes.

    Example:
        s = "abcde"
        p = "a*e"
        Does * eat "bcd"? or "bcde"? (then nothing for 'e')
        You must TRY all possibilities.

    With multiple *s it gets worse:
        s = "aXbYc"
        p = "a*b*c"
        Which * eats what? Exponential combinations to try.

    Naive recursion tries all paths → O(2^n) time → TOO SLOW.
    The fix: many subproblems are REPEATED → cache them (DP).

================================================================================
THINKING FROM FIRST PRINCIPLES
================================================================================

STEP 1: SIMPLIFY THE PROBLEM
    Ask: "What do I do when I look at ONE char of p and ONE char of s?"

    Case 1 — plain letter:
        p[j] == s[i]  → they match, move both pointers forward
        p[j] != s[i]  → no match, FAIL

    Case 2 — '?':
        matches any single char → always move both pointers forward

    Case 3 — '*':
        this is the hard part (see Step 2)

STEP 2: THE KEY INSIGHT FOR '*'
    '*' can match 0, 1, 2, 3, ... characters.
    We don't know upfront how many.

    So we try BOTH options at every '*':

    Option A: '*' matches EMPTY (0 chars)
        → move pattern forward (j+1), string stays (i)
        → "* matched nothing, let's skip it and move on in pattern"

    Option B: '*' eats ONE MORE char from s
        → move string forward (i+1), pattern STAYS at '*' (j)
        → "* ate one char, but * is still there, maybe it eats more"

    Why does j STAY in Option B?
        Because * can eat 0,1,2,3... chars.
        By staying at j, next call will again choose A or B.
        This way * can "eat" as many chars as needed, one at a time.

    If EITHER option leads to a full match → return true.

STEP 3: IDENTIFY REPEATED SUBPROBLEMS
    solve(i, j) = "can p[j...] match s[i...] ?"

    The answer for same (i, j) is ALWAYS the same.
    But naive recursion solves same (i,j) many times.

    Fix: store result of solve(i,j) in a 2D table.
    Never solve same (i,j) twice → polynomial time.

STEP 4: CONVERT TO BOTTOM-UP DP (tabulation)
    Instead of recursion + memo, fill a 2D table iteratively.

    dp[i][j] = "can p[0..j-1] match s[0..i-1] ?"
               (using 1-indexed to handle empty string cleanly)

    Answer we want: dp[n1][n2]
    (can entire pattern match entire string?)

CORE INVARIANT:
    dp[i][j] is true if and only if
    the first j characters of p can fully match the first i characters of s.

================================================================================
WHY THIS ALGORITHM IS CORRECT
================================================================================

BASE CASES:
    dp[0][0] = true
        Empty pattern matches empty string. Trivially true.

    dp[0][j] = dp[0][j-1]  (only when p[j-1] == '*')
        Empty string can only be matched by a sequence of *s.
        Each * matches empty, so we inherit the previous result.
        If any non-* appears, dp[0][j] stays false (default).

    dp[i][0] = false for i > 0
        Non-empty string cannot be matched by empty pattern.
        Already handled by initializing entire table to false.

TRANSITIONS:
    For each cell dp[i][j], we look at p[j-1] (current pattern char):

    Case '*':
        dp[i][j] = dp[i][j-1]   (Option A: * matches empty, skip *)
                 OR
                   dp[i-1][j]   (Option B: * eats s[i-1], stay at *)

        dp[i][j-1]: pattern moved forward (j-1), string same (i) → * skipped
        dp[i-1][j]: string moved forward (i-1), pattern same (j) → * ate one char

    Case '?' or matching letter:
        dp[i][j] = dp[i-1][j-1]
        Both pointers move back by one → both consumed one char each.

    Case mismatch:
        dp[i][j] stays false (default initialization).
        No transition possible.

WHY CORRECTNESS HOLDS:
    Every cell depends only on previously computed cells.
    We fill row by row, left to right → all dependencies are ready.
    The recurrence exactly mirrors the recursive logic,
    which was proven correct by the case analysis above.

================================================================================
IMPLEMENTATION
================================================================================
*/

#include <bits/stdc++.h>
using namespace std;

// ============================================================
// APPROACH 1: BOTTOM-UP DP (Tabulation)
// ============================================================
// Build the answer iteratively by filling a 2D table.
// No recursion stack overhead. Cache-friendly. Preferred in interviews.

class Solution_BottomUp {
public:
    bool isMatch(string s, string p) {

        int n1 = s.size();  // length of string (text)
        int n2 = p.size();  // length of pattern (rule)

        // dp[i][j] = can p[0..j-1] match s[0..i-1] ?
        // Size (n1+1) x (n2+1) to accommodate empty string/pattern at index 0.
        // Default initialized to false: assume no match until proven otherwise.
        vector<vector<bool>> dp(n1 + 1, vector<bool>(n2 + 1, false));

        // BASE CASE 1: empty pattern matches empty string
        // dp[0][0] = true because "" matches ""
        // Without this, no other cell could ever become true.
        dp[0][0] = true;

        // BASE CASE 2: empty string (i=0) with non-empty pattern
        // The only way an empty string can be matched is if
        // ALL pattern characters so far are '*' (each matching empty).
        // dp[0][j] = dp[0][j-1] only if p[j-1] == '*'
        // Example: p="***" can match s="" because each * matches empty.
        // If p[j-1] is a letter or '?', dp[0][j] stays false (can't match empty).
        for (int j = 1; j <= n2; j++) {
            if (p[j - 1] == '*')
                dp[0][j] = dp[0][j - 1];
            // If p[j-1] != '*', dp[0][j] remains false (default).
            // A letter or '?' requires at least one character in s.
        }

        // FILL THE TABLE: for each character of s (i) and p (j)
        // i represents "first i chars of s", j represents "first j chars of p"
        // We use 1-indexed so p[j-1] and s[i-1] are the current characters.
        for (int i = 1; i <= n1; i++) {
            for (int j = 1; j <= n2; j++) {

                if (p[j - 1] == '*') {
                    // STAR CASE: two options, either one working is enough (OR)

                    // Option A: dp[i][j-1]
                    //   '*' matches EMPTY → skip the '*' in pattern
                    //   String pointer stays at i (s unchanged)
                    //   Pattern pointer moves back to j-1 (before this *)
                    //   "Can p[0..j-2] match s[0..i-1]?" (pattern shrinks, string same)

                    // Option B: dp[i-1][j]
                    //   '*' eats ONE char from s → s[i-1] is consumed by *
                    //   String pointer moves back to i-1
                    //   Pattern pointer STAYS at j (still pointing at *)
                    //   "Can p[0..j-1] match s[0..i-2]?" (string shrinks, pattern same)
                    //   j stays because * might eat more chars in future cells.

                    dp[i][j] = dp[i][j - 1] || dp[i - 1][j];
                }
                else if (p[j - 1] == '?' || p[j - 1] == s[i - 1]) {
                    // MATCH CASE: either '?' (matches any single char)
                    //             or exact letter match (p[j-1] == s[i-1])
                    // Both s[i-1] and p[j-1] are consumed.
                    // Result depends on whether the remaining prefix matched:
                    // "Can p[0..j-2] match s[0..i-2]?" (both shrink by one)
                    dp[i][j] = dp[i - 1][j - 1];
                }

                // MISMATCH CASE (implicit):
                // p[j-1] is a letter AND p[j-1] != s[i-1]
                // dp[i][j] stays false (already initialized to false)
                // No transition is possible when letters don't match.
            }
        }

        // dp[n1][n2] = can entire pattern p match entire string s ?
        return dp[n1][n2];
    }
};

// ============================================================
// APPROACH 2: TOP-DOWN DP (Memoization / Recursion + Cache)
// ============================================================
// Natural recursive thinking + cache to avoid recomputation.
// Easier to derive from first principles.
// Same time/space complexity as bottom-up.

class Solution_TopDown {
public:
    // dp[i][j] stores the result of solve(i, j)
    // -1 = not yet computed, 0 = false, 1 = true
    vector<vector<int>> dp;

    // solve(i, j) answers:
    // "Can p[j...] match s[i...]?"
    // i = current position in s (text)
    // j = current position in p (pattern)
    bool solve(int i, int j, string& s, string& p) {

        // BASE CASE 1: both pointers exhausted simultaneously
        // s[i...] is empty AND p[j...] is empty → perfect match
        if (i == s.size() && j == p.size()) return true;

        // BASE CASE 2: pattern exhausted but string still has characters
        // No more rules to apply, but text remains → cannot match
        if (j == p.size()) return false;

        // BASE CASE 3: string exhausted but pattern still has characters
        // The only way to match empty string is if ALL remaining pattern chars are '*'
        // Each '*' would match empty, which is valid.
        // If any non-'*' remains in pattern, it needs a char from s that doesn't exist.
        if (i == s.size()) {
            while (j < p.size()) {
                if (p[j] != '*') return false;
                j++;
            }
            return true;
        }

        // MEMOIZATION CHECK: if this (i,j) was already solved, return cached result
        // Avoids exponential recomputation of same subproblems.
        // This is what transforms O(2^n) to O(n*m).
        if (dp[i][j] != -1) return dp[i][j];

        bool result = false;

        if (p[j] == '*') {
            // STAR CASE:
            // Option A: solve(i, j+1) → '*' matches empty
            //   j moves forward (skip the *), i stays (string unchanged)
            //   "* matched nothing, move past it in pattern"

            // Option B: solve(i+1, j) → '*' eats s[i]
            //   i moves forward (one char consumed by *), j STAYS at *
            //   "* ate one char, but * is still active, might eat more"
            //   j stays so that in the next call, * can again choose A or B.

            // OR because if EITHER way leads to full match → return true
            result = solve(i, j + 1, s, p) || solve(i + 1, j, s, p);
        }
        else if (p[j] == '?' || p[j] == s[i]) {
            // MATCH CASE: '?' matches any single char, or exact letter match
            // Both characters are consumed → both pointers advance
            result = solve(i + 1, j + 1, s, p);
        }

        // MISMATCH CASE (implicit):
        // result stays false if p[j] is a letter that doesn't match s[i]
        // No valid transition exists.

        // Cache and return result
        // dp[i][j] = result ensures we never recompute this (i,j) again
        return dp[i][j] = result;
    }

    bool isMatch(string s, string p) {
        int n1 = s.size();
        int n2 = p.size();

        // Initialize entire dp table to -1 (unvisited)
        // Using n1+1, n2+1 to safely handle boundary indices
        dp.assign(n1 + 1, vector<int>(n2 + 1, -1));

        // Start matching from beginning of both s and p
        return solve(0, 0, s, p);
    }
};

/*
================================================================================
DRY RUN (Bottom-Up DP)
================================================================================

s = "abc",  p = "*c"
n1 = 3,  n2 = 2

Initial table (4 rows × 3 cols), all false:
         ""    *     c
    ""  [ T ][ F ][ F ]
    a   [ F ][ F ][ F ]
    b   [ F ][ F ][ F ]
    c   [ F ][ F ][ F ]

BASE CASE 1: dp[0][0] = true (already set above)

BASE CASE 2: Fill dp[0][j] for j=1,2:
    j=1: p[0]='*' → dp[0][1] = dp[0][0] = true
    j=2: p[1]='c' → not '*' → dp[0][2] stays false

         ""    *     c
    ""  [ T ][ T ][ F ]
    a   [ F ][ F ][ F ]
    b   [ F ][ F ][ F ]
    c   [ F ][ F ][ F ]

FILL TABLE:

i=1 (s[0]='a'), j=1 (p[0]='*'):
    p[j-1]='*' → dp[1][1] = dp[1][0] || dp[0][1]
                           =  false   ||   true    = true

i=1 (s[0]='a'), j=2 (p[1]='c'):
    p[j-1]='c', s[i-1]='a' → mismatch → dp[1][2] = false

i=2 (s[1]='b'), j=1 (p[0]='*'):
    p[j-1]='*' → dp[2][1] = dp[2][0] || dp[1][1]
                           =  false   ||   true    = true

i=2 (s[1]='b'), j=2 (p[1]='c'):
    p[j-1]='c', s[i-1]='b' → mismatch → dp[2][2] = false

i=3 (s[2]='c'), j=1 (p[0]='*'):
    p[j-1]='*' → dp[3][1] = dp[3][0] || dp[2][1]
                           =  false   ||   true    = true

i=3 (s[2]='c'), j=2 (p[1]='c'):
    p[j-1]='c', s[i-1]='c' → MATCH → dp[3][2] = dp[2][1] = true ✅

Final table:
         ""    *     c
    ""  [ T ][ T ][ F ]
    a   [ F ][ T ][ F ]
    b   [ F ][ T ][ F ]
    c   [ F ][ T ][ T ]  ← dp[3][2] = true ✅

return dp[3][2] = TRUE
"*c" successfully matches "abc" ✅
(* ate "ab", then 'c' matched 'c')

================================================================================
COMPLEXITY ANALYSIS
================================================================================

BOTTOM-UP DP:
    Time Complexity: O(n1 × n2)
        We fill a table of size (n1+1) × (n2+1).
        Each cell takes O(1) to compute (constant work per cell).
        Total: O(n1 × n2) where n1 = |s|, n2 = |p|.
        With n1, n2 ≤ 2000 → at most 4,000,000 operations. ✅

    Space Complexity: O(n1 × n2)
        The dp table itself takes (n1+1)(n2+1) space.
        No recursion stack.
        Can be optimized to O(n2) using two rolling rows,
        but full table is clearer and still accepted.

TOP-DOWN MEMOIZATION:
    Time Complexity: O(n1 × n2)
        There are at most (n1+1)(n2+1) unique (i,j) pairs.
        Each is solved exactly once and cached.
        Each solve() call does O(1) work (excluding recursive calls).
        Total unique work: O(n1 × n2). ✅

    Space Complexity: O(n1 × n2)
        dp table: O(n1 × n2)
        Recursion stack depth: O(n1 + n2) in worst case
        (i increases at most n1 times, j increases at most n2 times)
        Total dominated by dp table: O(n1 × n2).

WHY THIS IS OPTIMAL:
    Lower bound argument:
        In the worst case, every cell of the (n1 × n2) table
        carries unique information about whether a prefix of p
        matches a prefix of s. There's no way to avoid considering
        all (n1 × n2) prefix pairs in the worst case.
    Therefore O(n1 × n2) time and space is optimal for this approach.

================================================================================
*/