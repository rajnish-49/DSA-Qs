// @ https://leetcode.com/problems/powx-n/

class Solution {
    public:
        double myPow(double x, int n) {
            // Base case: If the exponent is 0, any number raised to 0 is 1
            if (n == 0) {
                return 1;
            }
    
            // If n is negative:
            // We use the property: x^(-n) = 1 / (x^n), so we calculate the power for positive n and take the reciprocal
            if (n < 0) {
                // The expression `-(n + 1)` is used to avoid overflow when n is INT_MIN.
                // If n = -2147483648, `-(n + 1)` becomes 2147483647, which is safe for recursion.
                return 1 / myPow(x, -(n + 1)) / x;
            }
    
            // If n is positive and even:
            // If n is even, we can reduce the problem to smaller subproblems:
            // x^n = (x^2)^(n/2), thus we square the base (x) and halve the exponent (n).
            if (n % 2 == 0) {
                // We recursively call myPow with x squared and n halved
                // For example, x^4 = (x^2)^2
                return myPow(x * x, n / 2);
            } 
            else {
                // If n is odd:
                // We reduce the problem by calculating x^(n-1) and multiplying the result by x.
                // For example, x^5 = x * x^4
                return x * myPow(x, n - 1);
            }
        }
    };

// ITERATIVE APPROACH 

class Solution {
    public:
        double myPow(double x, int n) {
            // Special case: If n is 0, then x^0 = 1 for any x
            // This is because any number raised to the power of zero is 1
            if (n == 0) {
                return 1;
            }
    
            // Handle the case when the exponent is negative:
            // If n < 0, we use the property: x^(-n) = 1 / (x^n)
            // We will calculate the power for positive n and return the reciprocal.
            if (n < 0) {
                // To avoid overflow when n = INT_MIN, we handle the special case of INT_MIN.
                // If n = INT_MIN, -(n + 1) will be INT_MAX, which is a safe number to work with.
                return 1 / myPow(x, -(n + 1)) / x;
            }
    
            // If n is positive, we calculate the power iteratively
            double result = 1.0;  // Start with result as 1, since x^0 = 1
            double current_product = x;  // The current base to be multiplied in each iteration
    
            // Loop through each bit of the binary representation of n
            while (n > 0) {
                // If the current bit is 1 (i.e., n is odd), multiply result by current_product
                // This is the same as saying result = result * current_product when n is odd
                if (n % 2 == 1) {
                    result *= current_product;
                }
    
                // Square the current_product to prepare for the next higher power
                // This is similar to exponentiation by squaring:
                // current_product = x^2, x^4, x^8, ...
                current_product *= current_product;
    
                // Shift n to the right by 1 (divide by 2) to process the next bit
                n /= 2;
            }
    
            // Return the final result
            return result;
        }
    };
    
    