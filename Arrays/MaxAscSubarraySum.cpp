// @ https://leetcode.com/problems/maximum-ascending-subarray-sum/

class Solution {
public:
    int maxAscendingSum(vector<int>& nums) {

        // Get the size of the array
        int n = nums.size();

        // If there is only one element, return it as the max sum
        if (n == 1) {
            return nums[0];
        }

        // Initialize variables:
        // currentsum - stores the sum of the current ascending subarray
        // maxsum - stores the maximum sum found so far
        int currentsum = nums[0];
        int maxsum = 0;

        // Iterate through the array starting from the second element
        for (int i = 1; i < n; i++) {

            // If the current element is greater than the previous one, 
            // it means we are still in an ascending subarray
            if (nums[i] > nums[i - 1]) {
                currentsum += nums[i]; // Add to the current sum
            } else {
                // If not ascending, update maxsum and reset currentsum for the new subarray
                maxsum = max(maxsum, currentsum);
                currentsum = nums[i]; // Start new subarray from current element
            }

            // Update maxsum in every iteration to ensure we capture the maximum
            maxsum = max(maxsum, currentsum);
        }

        // Return the maximum sum of any ascending subarray
        return maxsum;
    }
};
