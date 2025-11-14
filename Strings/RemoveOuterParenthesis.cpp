class Solution {
public:
    string removeOuterParentheses(string s) {
        string result;           // To store the final result
        stack<char> stack;       // To track the depth of parentheses

        // Iterate through each character in the input string
        for (char c : s) {
            if (c == '(') {      
                // If stack is not empty, this '(' is not outermost
                if (!stack.empty()) {
                    result += c; // Add to result as it belongs to an inner group
                }
                stack.push(c);   // Push '(' onto the stack to increase depth
            } else if (c == ')') {
                stack.pop();     // Pop a matching '(' from the stack, reducing depth
                // If stack is not empty after popping, this ')' is not outermost
                if (!stack.empty()) {
                    result += c; // Add to result as it belongs to an inner group
                }
            }
        }

        return result;           // Return the string with outer parentheses removed
    }
};
