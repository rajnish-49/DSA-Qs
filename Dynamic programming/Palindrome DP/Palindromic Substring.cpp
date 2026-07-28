class Solution {
public:
    int countSubstrings(string s) {
        int n = s.size();

        vector<vector<bool>> t(n, vector<bool>(n, false));
        int count = 0;

        for (int L = 1; L <= n; L++) {

            
            for (int i = 0; i + L - 1 < n; i++) {
                int j = i + L - 1;

                if (i == j) {
                    
                    t[i][j] = true;
                }
                else if (j == i + 1) {
            
                    t[i][j] = (s[i] == s[j]);
                }
                else {
                    
                    t[i][j] = (s[i] == s[j]) && t[i+1][j-1];
                }

                if (t[i][j]) count++;
            }
        }

        return count;
    }
};