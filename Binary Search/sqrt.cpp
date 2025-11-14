// @https://leetcode.com/problems/sqrtx/description/

class Solution {
public:
    int mySqrt(int x) {
        // Handle the base cases: square root of 0 is 0, and square root of 1 is 1
        if (x == 0 || x == 1) return x;

        // Initialize binary search boundaries
        int l = 0;          // Left bound
        int r = x;          // Right bound (could be sqrt(x), but x works safely)
        int ans = 0;        // Variable to store the floor of sqrt(x)

        // Binary search to find the square root
        while (l <= r) {
            // Compute the middle value to prevent overflow
            int mid = l + (r - l) / 2;

            // Compute square of mid as long long to avoid integer overflow
            long long sq = 1LL * mid * mid;

            // If mid*mid == x, then mid is the perfect square root
            if (sq == x) return mid;

            // If mid*mid < x, mid might be the answer but we need to try higher
            else if (sq < x) {
                ans = mid;       // Store current mid as a possible answer
                l = mid + 1;     // Move search range to right half
            }

            // If mid*mid > x, mid is too large; reduce search space
            else {
                r = mid - 1;     // Move search range to left half
            }
        }

        // When loop ends, 'ans' holds the floor of sqrt(x)
        return ans;
    }
};
