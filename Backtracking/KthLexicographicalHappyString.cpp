// @ https://leetcode.com/problems/the-k-th-lexicographical-string-of-all-happy-strings-of-length-n/description/

class Solution {
    public:
        // Recursive function to generate happy strings of length 'n'.
        void solve(int n, string& temp, vector<string>& ans) {
            // Base case: If the current string 'temp' has reached the desired length 'n',
            // it's a valid happy string, so add it to the 'ans' vector.
            if (temp.length() == n) {
                ans.push_back(temp);
                return;
            }
    
            // Iterate through the characters 'a', 'b', and 'c'.
            for (auto c : string("abc")) {
                // Check if the current character 'c' is the same as the last character in 'temp'.
                // If it is, skip it because consecutive characters must be different in a happy string.
                if (!temp.empty() && temp.back() == c) {
                    continue;
                }
    
                // If the current character 'c' is valid, append it to 'temp'.
                temp.push_back(c);
    
                // Recursively call 'solve' to generate the rest of the string.
                solve(n, temp, ans);
    
                // Backtrack: Remove the last character from 'temp' to explore other possibilities.
                temp.pop_back();
            }
        }
    
        // Function to find the k-th happy string of length 'n'.
        string getHappyString(int n, int k) {
            // Initialize an empty string 'temp' to store the current string being generated.
            string temp = "";
    
            // Initialize a vector 'ans' to store all generated happy strings.
            vector<string> ans;
    
            // Call the 'solve' function to generate all happy strings of length 'n'.
            solve(n, temp, ans);
    
            // Check if 'k' is greater than the total number of generated happy strings.
            // If it is, it means the k-th happy string doesn't exist, so return an empty string.
            if (k > ans.size()) {
                return "";
            }
    
            // Return the k-th happy string (remember that vectors are 0-indexed, so we use k-1).
            return ans[k - 1];
        }
    };