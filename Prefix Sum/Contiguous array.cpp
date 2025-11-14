// @https://leetcode.com/problems/contiguous-array/description/

class Solution {
public:
    int findMaxLength(vector<int>& nums) {
        int balance = 0; // Tracks difference between count of 1's and 0's so far
                         // +1 when we see a 1, -1 when we see a 0

        unordered_map<int, int> firstSeenBalance;
        firstSeenBalance[0] = -1; 
        // We store that balance = 0 occurs at index -1
        // This helps handle cases where the subarray starts from index 0 itself
        // Example: [0,1] → balance becomes 0 at i = 1, length = 1 - (-1) = 2

        int maxLen = 0; // Stores the maximum subarray length found so far

        // Iterate through the array
        for (int i = 0; i < nums.size(); i++) {
           
            // Step 1: Update balance based on the current number
            if (nums[i] == 1) {
                balance += 1; // More 1's than before
            } else {
                balance -= 1; // More 0's than before
            }

            // Step 2: Check if this balance was seen before
            if (firstSeenBalance.find(balance) != firstSeenBalance.end()) {
                // If seen before, the subarray between the previous index+1
                // and current index has equal 0's and 1's
                // Because balance hasn't changed, meaning net 0 difference
                int length = i - firstSeenBalance[balance];
                maxLen = max(maxLen, length); // Update max length
            } else {
                // Step 3: First time seeing this balance → store index
                // We store only the first occurrence so that subarrays are longest
                firstSeenBalance[balance] = i;
            }
        }

        return maxLen; // Return the length of the longest valid subarray
    }
};
