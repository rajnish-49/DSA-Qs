// @https://leetcode.com/problems/integer-replacement/

class Solution {
public:
    int integerReplacement(int n) {
        // Use long long to prevent overflow
        // Why? n can be 2^31 - 1. If we do n + 1 with int, it would overflow.
        long long num = n; 

        // Counter to keep track of the number of operations
        int steps = 0;
        
        // Keep reducing the number until it becomes 1
        while (num != 1) {

            // Case 1: num is even
            if (num % 2 == 0) {
                // Why divide by 2? 
                // Dividing by 2 is the fastest way to reduce an even number.
                // Example: 8 -> 4 -> 2 -> 1 requires only 3 operations.
                num /= 2;
            } 
            // Case 2: num is odd
            else {
                // For odd numbers, we have two choices: increment (num+1) or decrement (num-1)
                // We want to choose the one that leads to **fewer total steps**
                // The intuition: choose the operation that produces a number with more trailing zeros
                // in binary, because more trailing zeros → more consecutive divisions by 2 → faster reduction

                // Special case: num = 3
                // Why handle separately? 
                // 3 -> 2 -> 1 is faster (2 steps) than 3 -> 4 -> 2 -> 1 (3 steps)
                if (num == 3 || ((num >> 1) & 1) == 0) {
                    // Explanation of ((num >> 1) & 1):
                    // 1. Right shift by 1: num >> 1 → looks at the second least significant bit
                    // 2. AND with 1 → isolates that bit
                    // If it is 0, subtracting 1 is better
                    // Why? Because num-1 will then be divisible by 2 (or even by 4), 
                    // allowing more divisions by 2 in subsequent steps
                    num--; // subtract 1
                } else {
                    // Otherwise, adding 1 is better
                    // Why? Because num+1 will often produce a number divisible by 4
                    // Example: 7 -> 8 -> 4 -> 2 -> 1 requires fewer steps than 7 -> 6 -> 3 -> 2 -> 1
                    num++; // add 1
                }
            }

            // Each operation (divide, add, or subtract) counts as one step
            steps++;
        }
        
        // Return the total number of operations needed to reduce num to 1
        return steps;
    }
};
