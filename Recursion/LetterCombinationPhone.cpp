// @ https://leetcode.com/problems/letter-combinations-of-a-phone-number/description/

class Solution {
private:
    // Helper function to solve the problem recursively
    // Parameters:
    // - digits: The input string containing digits
    // - ans: Reference to a vector that stores all possible combinations
    // - output: Current combination being generated
    // - index: Current index in the digits string
    // - keyboard: Array mapping digits to their corresponding characters
    void solve(string digits, vector<string>& ans, string output, int index, string keyboard[]) {
        // Base case: If the index reaches the end of the digits string,
        // push the current combination (output) to the result vector and return.
        if (index >= digits.length()) {
            ans.push_back(output);
            return;
        }

        // Get the current digit and map it to the corresponding string using the keyboard array
        int number = digits[index] - '0'; // Convert character digit to integer
        string value = keyboard[number]; // Characters corresponding to the digit

        // Iterate over each character in the string mapped from the current digit
        for (char ch : value) {
            // Add the character to the current output combination
            output.push_back(ch);

            // Recursively call the solve function for the next digit
            solve(digits, ans, output, index + 1, keyboard);

            // Backtrack by removing the last character to try other possibilities
            output.pop_back();
        }
    }

public:
    // Main function to generate all possible letter combinations
    vector<string> letterCombinations(string digits) {
        vector<string> ans; // To store the result combinations

        // Edge case: If the input string is empty, return an empty result
        if (digits.empty()) return ans;

        // Initialize variables
        int index = 0; // Start from the first digit
        string output; // Temporary string to build each combination
        string keyboard[10] = {
            "", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"
        }; // Mapping of digits to characters

        // Start the recursive function
        solve(digits, ans, output, index, keyboard);

        // Return the result vector
        return ans;
    }
};
