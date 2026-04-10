/*
 * ============================================================
 * PROBLEM: Mirror Frequency Distance
 * ============================================================
 *
 * PROBLEM STATEMENT (in simple terms):
 * ------------------------------------------------------------
 * Every letter and digit has a "mirror":
 *   - For letters: 'a'<->'z', 'b'<->'y', 'c'<->'x', ..., 'm'<->'n'
 *     (pair up the alphabet from both ends moving inward)
 *   - For digits: '0'<->'9', '1'<->'8', '2'<->'7', '3'<->'6', '4'<->'5'
 *     (same idea, pair up from both ends of 0-9)
 *
 * INPUT:  A string s of lowercase letters and digits.
 * OUTPUT: Sum of |freq(c) - freq(mirror(c))| over all DISTINCT mirror pairs.
 *
 * GOAL (intuitive):
 *   For each mirror pair (c, m), measure how "unbalanced" their
 *   frequencies are in the string. Sum up all these imbalances.
 *
 * ============================================================
 * OBSERVATIONS THAT LEAD TO THE SOLUTION
 * ============================================================
 *
 * 1. The mirror pairs are FIXED and finite:
 *      - Exactly 13 letter pairs  (a-z, b-y, ..., m-n)
 *      - Exactly 5 digit pairs    (0-9, 1-8, 2-7, 3-6, 4-5)
 *      Total: 18 pairs, regardless of what the string contains.
 *
 * 2. A brute force approach might iterate over every character
 *    in the string, look up its mirror, and add the difference.
 *    The problem: if both 'a' and 'z' appear in the string, you
 *    process 'a' (adding |freq(a)-freq(z)|) and then later 'z'
 *    (adding |freq(z)-freq(a)|, the same value). Double counted.
 *
 * 3. The fix is to NOT iterate over characters in the string.
 *    Instead, iterate over the 18 fixed pairs directly. Each
 *    pair is visited exactly once, double counting is impossible
 *    by construction.
 *
 * 4. Since there are only 13 + 5 = 18 pairs, after one O(n)
 *    pass to build frequencies, the rest is O(1). Total: O(n).
 *
 * ============================================================
 * ALGORITHM (step by step):
 * ============================================================
 *
 * Step 1: Count frequencies of all characters in the string.
 *
 * Step 2: Iterate over the 13 letter pairs and 5 digit pairs.
 *         For each pair, add |freq(c) - freq(mirror(c))| to ans.
 *
 * Step 3: Double counting is avoided because we iterate over
 *         pairs, not characters. Each pair is visited once.
 *
 * 
 * APPROACH 1: Uses unordered_map<char,int> for frequency storage.
 *             Iterates over characters 'a'-'z' and '0'-'9' and
 *             uses the condition (ch < mirror) to visit each pair
 *             exactly once.
 *
 * APPROACH 2: Uses fixed-size arrays fL[26] and fD[10] instead
 *             of a hash map. Iterates over numeric indices 0..12
 *             for letters and 0..4 for digits, which directly
 *             covers each pair exactly once without needing a
 *             (ch < mirror) guard condition.
 *
 * Approach 2 is faster in practice because fixed arrays have
 * no hashing overhead and better cache locality.
 */


// ============================================================
// APPROACH 1: Hash Map + Character Iteration
// ============================================================

class Solution {
public:
    int mirrorFrequency(string s) {

        // unordered_map stores frequency of every character seen.
        // Key: the character itself. Value: how many times it appears.
        // We use a map here so any char 'a'-'z' or '0'-'9' can be
        // looked up directly without index arithmetic.
        unordered_map<char, int> mp;

        // --- STEP 1: Count frequencies ---
        // Single pass through the string. If a character hasn't been
        // seen before, the map default-initializes its count to 0
        // and we increment to 1. Subsequent occurrences just increment.
        for (auto c : s) {
            mp[c]++;
        }

        int ans = 0;

        // --- STEP 2: Process letter mirror pairs ---
        // We iterate over every letter 'a' to 'z'.
        // For each ch, its mirror is computed as:
        //   'z' - (ch - 'a')
        //   e.g. ch='a': 'z' - 0 = 'z'
        //        ch='b': 'z' - 1 = 'y'
        //        ch='m': 'z' - 12 = 'n'
        //        ch='z': 'z' - 25 = 'a'
        // The condition (ch < mirror) ensures each pair is counted
        // exactly once. When ch='a', mirror='z', 'a' < 'z' so we
        // process it. When ch='z', mirror='a', 'z' < 'a' is false
        // so we skip it. This is how double counting is avoided
        // in this approach — we only process the "smaller" half.
        for (auto ch = 'a'; ch <= 'z'; ch++) {
            char mirror = 'z' - (ch - 'a');

            // Only process this pair if ch is the "lesser" character.
            // This means we process (a,z), (b,y), ..., (m,n) — exactly
            // 13 pairs — and skip the reverse direction entirely.
            if (ch < mirror) {
                // mp[ch] and mp[mirror] return 0 if the character was
                // never inserted (default behavior of unordered_map),
                // so absent characters are handled automatically.
                ans += abs(mp[ch] - mp[mirror]);
            }
            // When ch == mirror (would mean a character mirrors itself),
            // we skip it too. This doesn't happen with 26 letters since
            // 26 is even, but the guard handles it safely regardless.
        }

        // --- STEP 3: Process digit mirror pairs ---
        // Exact same logic as letters but for '0'-'9'.
        // Mirror formula: '9' - (ch - '0')
        //   e.g. ch='0': '9' - 0 = '9'
        //        ch='1': '9' - 1 = '8'
        //        ch='4': '9' - 4 = '5'
        // Again, (ch < mirror) ensures each of the 5 pairs is
        // processed exactly once.
        for (auto ch = '0'; ch <= '9'; ch++) {
            char mirror = '9' - (ch - '0');

            if (ch < mirror) {
                ans += abs(mp[ch] - mp[mirror]);
            }
        }

        return ans;
    }
};


// ============================================================
// APPROACH 2: Fixed Arrays + Index Iteration
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int mirrorFrequency(string s) {

        // fL[i] stores the frequency of the letter ('a' + i).
        // fD[i] stores the frequency of the digit  ('0' + i).
        // Using fixed arrays instead of a hash map because the
        // character set is fixed (26 letters, 10 digits) — O(1)
        // lookup and no hashing overhead. Better cache locality too.
        int fL[26] = {0};
        int fD[10] = {0};

        // --- STEP 1: Single pass to count frequencies ---
        // We separate letters from digits by checking c >= 'a'.
        // Digits in ASCII are '0'-'9' (48-57), letters 'a'-'z'
        // (97-122), so c >= 'a' reliably identifies a letter.
        for (char c : s) {
            if (c >= 'a') fL[c - 'a']++;  // 'a'->0, 'b'->1, ..., 'z'->25
            else          fD[c - '0']++;  // '0'->0, '1'->1, ..., '9'->9
        }

        int ans = 0;

        // --- STEP 2: Process the 13 letter mirror pairs ---
        // Mirror pair in 0-indexed terms: index i pairs with index (25 - i).
        //   i=0  -> 'a'(0)  <-> 'z'(25)
        //   i=1  -> 'b'(1)  <-> 'y'(24)
        //   ...
        //   i=12 -> 'm'(12) <-> 'n'(13)
        // We stop at i < 13 because at i=13, j=25-13=12, which was
        // already covered when i=12. Unlike Approach 1, there is no
        // need for a (ch < mirror) guard — the loop range itself
        // naturally covers each pair exactly once.
        for (int i = 0; i < 13; i++) {
            int j = 25 - i;  // mirror index in the alphabet
            // |fL[i] - fL[j]| measures how unbalanced this letter
            // pair is. If both have equal frequency, contributes 0.
            // If one is absent, its array slot holds 0 by initialization,
            // so the difference equals the other's full count.
            ans += abs(fL[i] - fL[j]);
        }

        // --- STEP 3: Process the 5 digit mirror pairs ---
        // Mirror pair in 0-indexed terms: index i pairs with index (9 - i).
        //   i=0 -> '0'(0) <-> '9'(9)
        //   i=1 -> '1'(1) <-> '8'(8)
        //   ...
        //   i=4 -> '4'(4) <-> '5'(5)
        // We stop at i < 5 for the same reason as letters:
        // at i=5, j=9-5=4, already processed when i=4.
        for (int i = 0; i < 5; i++) {
            int j = 9 - i;  // mirror index in the digit range
            ans += abs(fD[i] - fD[j]);
        }

        // ans now holds the total imbalance across all 18 mirror pairs.
        return ans;
    }
};