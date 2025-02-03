// @ https://leetcode.com/problems/count-partitions-with-even-sum-difference/description/

class Solution {
public:
    int countPartitions(vector<int>& nums) {

        int leftsum = 0;  // Stores the sum of the left subarray
        int rightsum = 0; // Stores the sum of the right subarray
        int totalsum = 0; // Stores the total sum of the array
        int count = 0;    // Stores the count of valid partitions

        // Step 1: Calculate the total sum of the array
        for (int i = 0; i < nums.size(); i++) {
            totalsum += nums[i];
        }

        // Step 2: Iterate through the array to find valid partitions
        for (int j = 0; j < nums.size() - 1; j++) {
            leftsum += nums[j];            // Update left subarray sum
            rightsum = totalsum - leftsum; // Compute right subarray sum

            // Step 3: Check if the difference is even
            if ((leftsum - rightsum) % 2 == 0) {
                count++; // Increment count if valid partition found
            }
        }

        return count; // Return the total count of valid partitions
    }
};
