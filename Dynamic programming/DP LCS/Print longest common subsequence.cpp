#include <bits/stdc++.h>
using namespace std;

// Solution class for LCS with reconstruction using tabulation
class Solution {
public:
    // Function to return the LCS string of text1 and text2
    string longestCommonSubsequence(string &text1, string &text2) {
        int n = text1.size();
        int m = text2.size();

        // Create DP table to store lengths of LCS for all substrings
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

        // Fill dp table bottom-up
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (text1[i - 1] == text2[j - 1]) {
                    // Characters match: increase length by 1
                    dp[i][j] = 1 + dp[i - 1][j - 1];
                } else {
                    // Characters don't match: take max of left and top
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }

        // Reconstruct LCS string from dp table
        int i = n, j = m;
        string lcs = "";

        // Traverse dp table from bottom-right to top-left
        while (i > 0 && j > 0) {
            if (text1[i - 1] == text2[j - 1]) {
                // Characters match, add to result and move diagonally
                lcs += text1[i - 1];
                i--;
                j--;
            } else if (dp[i - 1][j] > dp[i][j - 1]) {
                // Move up if top cell has greater value
                i--;
            } else {
                // Move left otherwise
                j--;
            }
        }

        // Reverse string since it was built backwards
        reverse(lcs.begin(), lcs.end());

        return lcs;
    }
};

int main() {
    string s1 = "abcde";
    string s2 = "ace";

    Solution sol;
    cout << "LCS: " << sol.longestCommonSubsequence(s1, s2) << endl;

    return 0;
}
