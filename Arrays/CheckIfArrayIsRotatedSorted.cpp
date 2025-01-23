// @ https://leetcode.com/problems/check-if-array-is-sorted-and-rotated/description/

class Solution {
public:
    // Function to check if the array can be rotated to become non-decreasing
    bool check(vector<int>& nums) {
        int k = 0; // Counter to track the number of "decreasing" points
        int n = nums.size(); // Get the size of the array

        // Loop through the array to count decreasing points
        for (int i = 1; i < n; i++) {
            if (nums[i - 1] > nums[i]) { 
                // Increment counter if current element is less than the previous
                k++;
            }
        }

        // Check if the last element is greater than the first (wrap-around case)
        if (nums[n - 1] > nums[0]) {
            k++;
        }

        // If there is at most one "decreasing" point, the array can be rotated
        return k <= 1;
    }
};
