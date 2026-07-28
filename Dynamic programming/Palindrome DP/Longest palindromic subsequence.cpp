class Solution {
public:
    int t[1001][1001];
    int LPS(string& s, int i, int j) {
        if(i > j)
            return 0;
        if(i == j)
            return 1;
        
        if(t[i][j] != -1)
            return t[i][j];
        if(s[i] == s[j])
            return t[i][j] = 2 + LPS(s, i+1, j-1);
        else
            return t[i][j] = max(LPS(s, i+1, j), LPS(s, i, j-1));
    }
    
    int longestPalindromeSubseq(string s) {
        int m = s.length();
        memset(t, -1, sizeof(t));
        
        return LPS(s, 0, m-1); 
        
    }
};




class Solution {
public:
    int longestPalindromeSubseq(string s) {
        int n = s.length();

        vector<vector<int>> dp(n, vector<int>(n));
    
        for(int i = 0; i < n; i++) {
            dp[i][i] = 1; 
        }

        for(int L = 2; L<=n; L++) {
            for(int i = 0; i < n-L+1; i++) {
                int j = i + L - 1;

                if(s[i] == s[j]) {
                    dp[i][j] = 2 + dp[i+1][j-1];
                } else {
                    dp[i][j] = max(dp[i][j-1], dp[i+1][j]);
                }
            }
        }

        return dp[0][n-1]; 

    }
};