class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int left = 0;
        int currentSum = 0;
        int minLength = nums.size() + 1;

        // The window [left ... right] represents the current subarray.
        // Since all numbers are positive, expanding right always increases
        // the sum, while moving left forward always decreases the sum.
        for (int right = 0; right < nums.size(); right++) {

            // Add the new element entering the window.
            currentSum += nums[right];

            // Once the sum reaches the target, the current window is valid.
            // Keep shrinking it from the left to find the smallest valid
            // window ending at this same right index.
            while (currentSum >= target) {

                // Record the current valid window before removing anything.
                minLength = min(minLength, right - left + 1);

                // Remove the leftmost element and try a smaller window.
                currentSum -= nums[left];
                left++;
            }
        }

        // If minLength was never updated, no subarray reached the target.
        return minLength == nums.size() + 1 ? 0 : minLength;
    }
};