class Solution {
public:
    int minInsertions(string s) {
        /*
         * PROBLEM INTUITION:
         * We need to find the minimum number of character insertions to make s a palindrome.
         * 
         * KEY INSIGHT: Instead of thinking "what do I insert?", think about
         * "what's already in place and doesn't need changing?"
         * 
         * The characters that are ALREADY in a palindromic arrangement don't need
         * any insertions. We only need to insert characters for the parts that
         * aren't palindromic yet.
         * 
         * So the answer = n - (length of longest palindromic subsequence in s)
         * 
         * WHY? If the longest palindromic subsequence (LPS) has length k,
         * those k characters are already "mirrored" correctly. The remaining
         * (n - k) characters each need exactly 1 insertion (their mirror copy)
         * to complete the palindrome.
         * 
         * Example: "mbadm" -> LPS is "mam" or "mdm" (length 3)
         * So we need 5 - 3 = 2 insertions. ✓
         * 
         * HOW TO FIND LPS:
         * LPS(s) = LCS(s, reverse(s))
         * 
         * WHY does LCS with reverse give LPS?
         * A palindrome reads the same forwards and backwards.
         * So a palindromic subsequence of s must appear as a subsequence
         * in s reading left-to-right AND also appear reading right-to-left.
         * That's exactly a common subsequence between s and its reverse!
         * 
         * Example: s = "mbadm", reverse = "mdabm"
         * LCS = "mam" or "mdm" (length 3) → LPS length = 3
         */

        string s2 = s;
        reverse(s2.begin(), s2.end());  // s2 is the reversed string
        int n = s.size();

        /*
         * CLASSIC LCS DP:
         * dp[i][j] = length of LCS of s[0..i-1] and s2[0..j-1]
         * 
         * Base case: dp[0][j] = dp[i][0] = 0 (LCS with empty string is 0)
         * 
         * Transition:
         * - If s[i-1] == s2[j-1]: this character can extend the LCS from
         *   the state before both characters → dp[i][j] = 1 + dp[i-1][j-1]
         * 
         * - If s[i-1] != s2[j-1]: we can't use both characters together,
         *   so we take the best by skipping one of them:
         *   dp[i][j] = max(dp[i-1][j], dp[i][j-1])
         *   (skip current char of s, OR skip current char of s2)
         */
        vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (s[i-1] == s2[j - 1])
                    dp[i][j] = 1 + dp[i-1][j-1];
                else
                    dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
            }
        }

        /*
         * dp[n][n] = LCS(s, reverse(s)) = LPS(s)
         * 
         * Answer = n - LPS length
         * = number of characters NOT part of the longest palindromic subsequence
         * = each of these needs exactly one mirror insertion
         */
        return n - dp[n][n];
    }
};
```

**Core mental model for revision:**

The whole solution rests on one elegant equivalence chain:
```
min insertions to make palindrome
    = n - longest palindromic subsequence (LPS)
    = n - LCS(s, reverse(s))