// https://leetcode.com/problems/minimum-size-subarray-sum/description/?envType=problem-list-v2&envId=prefix-sum
class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {

        int n = nums.size();

        // If there's only one element, the minimum subarray length is either 1 or 0 
        // (depending on whether it meets the target)
        if (n == 1) {
            return (nums[0] >= target) ? 1 : 0;
        }

        // Initialize variables:
        int minlength = INT_MAX; // Stores the minimum length of a valid subarray
        int sum = 0; // Running sum of the current subarray
        int left = 0; // Left pointer for the sliding window

        // Iterate through the array with the right pointer
        for (int right = 0; right < n; right++) {
            sum += nums[right]; // Expand the window by adding the current element

            // Shrink the window from the left while the sum is >= target
            while (sum >= target) {
                // Update minlength to store the smallest subarray found
                minlength = min(minlength, right - left + 1);

                // Remove the leftmost element and move the left pointer forward
                sum -= nums[left];
                left++;
            }
        }

        // If minlength is still INT_MAX, it means no valid subarray was found
        return (minlength == INT_MAX) ? 0 : minlength;
    }
};
