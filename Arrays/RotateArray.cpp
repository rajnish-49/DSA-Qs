// @ https://leetcode.com/problems/rotate-array/submissions/1390788495/

class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        int n = nums.size();
        k = k % n; // Adjust k to be within the bounds of the array length
        
        // Step 1: Reverse the entire array to bring the last k elements to the front (but in reversed order)
        for (int i = 0; i < n / 2; i++) {
            swap(nums[i], nums[n - 1 - i]);
        }
        
        // Step 2: Reverse the first k elements to correct their order
        for (int i = 0; i < k / 2; i++) {
            swap(nums[i], nums[k - 1 - i]);
        }
        
        // Step 3: Reverse the remaining elements (from index k to end) to correct their order
        for (int i = k; i < (n + k) / 2; i++) {
            swap(nums[i], nums[n - 1 - (i - k)]);
        }
    }
};