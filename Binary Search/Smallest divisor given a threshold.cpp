// @https://leetcode.com/problems/find-the-smallest-divisor-given-a-threshold/

class Solution {
public:

    // Helper function to check if we can divide all elements
    // by 'mid' and get a sum of ceiling divisions <= threshold
    bool solve(vector<int>& nums, int mid, int threshold) {
        int current = 0;

        // For each number in nums
        for (auto i : nums) {
            // Add the ceiling of i/mid to current sum
            // Formula (i + mid - 1) / mid is integer ceil division
            current += (i + mid - 1) / mid;
        }

        // Return true if total sum of divisions is <= threshold
        return current <= threshold;
    }

    int smallestDivisor(vector<int>& nums, int threshold) {
        // Initialize left boundary of binary search to 1
        // Because divisor must be positive integer ≥ 1
        int l = 1;

        // Initialize right boundary to maximum element in nums
        // Because no divisor greater than max element will change the result
        int r = *max_element(nums.begin(), nums.end());

        // Binary search loop continues until boundaries meet
        while (l < r) {
            // Find the middle point of current range
            int mid = l + (r - l) / 2;

            // Check if mid divisor can produce a sum <= threshold
            if (solve(nums, mid, threshold)) {
                // If yes, try smaller divisors to find minimal divisor
                r = mid;
            } else {
                // If no, need to increase divisor, so move left boundary up
                l = mid + 1;
            }
        }

        // After the loop ends, l == r and points to the smallest divisor
        return l;
    }
};
