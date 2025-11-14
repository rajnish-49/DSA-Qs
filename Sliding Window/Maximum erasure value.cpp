// @https://leetcode.com/problems/maximum-erasure-value/description/

class Solution {
public:
    int maximumUniqueSubarray(vector<int>& nums) {
        unordered_map<int, int> lastIndex; // Stores last seen index of each element
        int ans = 0;       // Stores maximum sum found so far
        int sum = 0;       // Current window sum
        int j = 0;         // Left pointer of sliding window

        for (int i = 0; i < nums.size(); i++) {
            // If current number is already in the window (duplicate found)
            if (lastIndex.find(nums[i]) != lastIndex.end() && lastIndex[nums[i]] >= j) {
                // Remove elements from the window until we pass the duplicate
                for (int k = j; k <= lastIndex[nums[i]]; k++) {
                    sum -= nums[k]; // Subtract each removed element from the window sum
                }
                // Move left pointer right past the previous occurrence of current element
                j = lastIndex[nums[i]] + 1;
            }

            // Add current element to window
            sum += nums[i];

            // Update last seen index of current element
            lastIndex[nums[i]] = i;

            // Update answer with the best (max) sum so far
            ans = max(ans, sum);
        }

        return ans; // Return the maximum erasure value found
    }
};
