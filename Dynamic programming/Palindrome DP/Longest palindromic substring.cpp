class Solution {
public:

    vector<vector<int>> dp;

    bool solve ( string &s , int i , int j){
        if( i >= j){
            return 1;
        }

        if(dp[i][j] != -1){
            return dp[i][j];
        }
        if(s[i] == s[j]){
            return dp[i][j] = solve(s , i+1 , j-1);
        }
        else{
            return dp[i][j] = 0;
        }
    }



    string longestPalindrome(string s) {


        int n = s.length();

        dp.assign(n , vector<int>(n , -1));

        int maxlen = 0;
        int start = 0;

        for (int i = 0 ; i < n ; i++){
            for(int j = i ; j < n ; j++){

                if(solve(s , i , j ) == true ){
                    if(j-i+1 > maxlen){
                        maxlen = j-i+1;
                        start = i;
                    }
                }

            }
        }

        return s.substr( start , maxlen);
        
    }
};




class Solution {
public:
    string longestPalindrome(string s) {
        int n = s.size();

        vector<vector<bool>> dp(n, vector<bool>(n, false));

        int start = 0;
        int maxLen = 1;

        // L = length of substring
        for (int L = 1; L <= n; L++) {

            for (int i = 0; i + L - 1 < n; i++) {
                int j = i + L - 1;

                if (i == j) {
                    dp[i][j] = true;
                }
                else if (j == i + 1) {
                    dp[i][j] = (s[i] == s[j]);
                }
                else {
                    dp[i][j] = (s[i] == s[j]) && dp[i+1][j-1];
                }

                // update answer
                if (dp[i][j] && L > maxLen) {
                    maxLen = L;
                    start = i;
                }
            }
        }

        return s.substr(start, maxLen);
    }
};