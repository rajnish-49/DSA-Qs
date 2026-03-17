class Solution {
public:
    int longestPalindromeSubseq(string s) {

        /*
        ======================== PROBLEM ========================
        Given a string s, find the length of the longest subsequence
        that is a palindrome.

        A subsequence = characters picked from original string in order
        (not necessarily contiguous). A palindrome reads same forwards & backwards.

        Example: s = "bbbab" → answer is 4 ("bbbb" — wait, we pick b,b,b,b skipping 'a')
                 s = "cbbd"  → answer is 2 ("bb")

        ======================== KEY INSIGHT ========================
        A palindrome reads the same forwards and backwards.
        So if a subsequence P is a palindrome, then:
            - P == reverse(P)

        This means: the Longest Palindromic Subsequence of s
                  = the Longest Common Subsequence (LCS) of s and reverse(s)

        WHY? Because any common subsequence of s and reverse(s) must
        appear in both the original order AND reversed order — meaning
        it reads the same both ways → it IS a palindrome.

        And the longest such common subsequence is our answer.

        Example: s = "bbbab"
                 rev = "babbb"
                 LCS("bbbab", "babbb") = "bbbb" → length 4 ✓

        So we've reduced an unfamiliar problem (longest palindromic subsequence)
        to a classic, well-known problem (LCS). This is the core trick.

        ======================== LCS WITH DP ========================
        dp[i][j] = length of LCS of s1[0..i-1] and s2[0..j-1]
                 = LCS considering first i chars of s1, first j chars of s2

        Base case: dp[0][j] = dp[i][0] = 0
        (LCS of empty string with anything = 0)

        Recurrence:
        - If s1[i-1] == s2[j-1]:
              These two characters match! We can extend whatever LCS existed
              before both of these characters (dp[i-1][j-1]) by 1.
              dp[i][j] = 1 + dp[i-1][j-1]

        - If s1[i-1] != s2[j-1]:
              These don't match, so we can't use both simultaneously.
              Best we can do: either skip current char of s1 (dp[i-1][j])
              or skip current char of s2 (dp[i][j-1]), take the better option.
              dp[i][j] = max(dp[i-1][j], dp[i][j-1])

        Final answer: dp[n][n] — LCS of full s1 and full s2

        Time:  O(n²) — fill an n×n table
        Space: O(n²) — the dp table (can be optimized to O(n) but not done here)
        */

        int n = s.size();

        string s1 = s;
        string s2 = s;
        reverse(s2.begin(), s2.end());
        // s1 = original, s2 = reversed
        // Finding LCS(s1, s2) gives us the longest palindromic subsequence

        // dp[i][j] = LCS length for s1[0..i-1] and s2[0..j-1]
        // Size (n+1) x (n+1) so that index 0 represents the empty string base case
        // All initialized to 0, which handles base cases dp[0][j] and dp[i][0]
        vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {

                if (s1[i - 1] == s2[j - 1])
                    // Characters match → include this character in LCS
                    // We inherit the LCS of both strings *before* these indices (diagonal)
                    // and extend it by 1
                    dp[i][j] = 1 + dp[i - 1][j - 1];

                else
                    // Characters don't match → we can't pair them together
                    // Option A: ignore s1[i-1], best LCS using s1[0..i-2] and s2[0..j-1]
                    // Option B: ignore s2[j-1], best LCS using s1[0..i-1] and s2[0..j-2]
                    // Take whichever gives longer LCS
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }

        // dp[n][n] = LCS of entire s1 and s2 = longest palindromic subsequence of s
        return dp[n][n];
    }
};