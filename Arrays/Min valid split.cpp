// @ https://leetcode.com/u/rajnishk14102/

class Solution {
    public:
        int minimumIndex(vector<int>& nums) {
            int n = nums.size();
    
            // Step 1: Find the majority element (dominant element) using the Boyer-Moore Voting Algorithm
            int candidate = -1, count = 0;
            for (int num : nums) {
                if (count == 0) {
                    candidate = num; // Set a new candidate
                    count = 1;
                } else if (num == candidate) {
                    count++; // Increase count if the same candidate appears
                } else {
                    count--; // Decrease count for a different number
                }
            }
    
            // Step 2: Count total occurrences of the candidate in the array
            int totalCount = 0;
            for (int num : nums) {
                if (num == candidate) totalCount++;
            }
    
            // Step 3: Iterate through the array to find the minimum index where the split is valid
            int leftCount = 0;  // Count of candidate in the left part
            int leftSize = 0;   // Size of the left part
            for (int i = 0; i < n; i++) {
                if (nums[i] == candidate) leftCount++; // Update left candidate count
                leftSize++; // Increase the size of the left partition
    
                // Calculate values for the right partition
                int rightCount = totalCount - leftCount; // Remaining count in the right partition
                int rightSize = n - leftSize; // Size of the right partition
    
                // Step 4: Check if the candidate is dominant in both partitions
                if (leftCount * 2 > leftSize  && rightCount * 2 > rightSize) {
                    return i; // Return the minimum index where the split is valid
                }
            }
    
            return -1; // No valid split found
        }
    };
    