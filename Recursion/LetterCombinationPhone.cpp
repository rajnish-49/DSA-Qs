// @ https://leetcode.com/problems/letter-combinations-of-a-phone-number/description/

class Solution {
    private:
        // Recursive function to generate letter combinations
        void solve(string digits, vector<string>& ans, string output, int index, string keyboard) {
    
            // Base case: If we have processed all digits
            if (index >= digits.length()) {
                // We have formed a complete combination, so add it to the answer
                ans.push_back(output);
                return;
            }
    
            // Get the current digit
            int number = digits[index] - '0'; // Convert char digit to integer
            string value = keyboard[number];     // Get the corresponding letters from keyboard
    
            // Iterate through the letters for the current digit
            for (char ch: value) {
                // Include the current letter in the combination
                output.push_back(ch);
    
                // Recursively generate combinations for the remaining digits
                solve(digits, ans, output, index + 1, keyboard);
    
                // Backtrack: Remove the last added letter to explore other possibilities
                output.pop_back(); 
            }
        }
    
    public:
        vector<string> letterCombinations(string digits) {
    
            vector<string> ans; // Vector to store the resulting combinations
    
            // Handle empty input case
            if (digits.empty()) return ans;
    
            int index = 0;      // Starting index for processing digits
            string output = ""; // String to build the current combination
            string keyboard = {"", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"}; // Keyboard mapping
    
            // Start the recursive process
            solve(digits, ans, output, index, keyboard);
    
            return ans; // Return the vector of letter combinations
        }
    };
