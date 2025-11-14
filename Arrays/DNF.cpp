class Solution {
public:
    void sortColors(vector<int>& nums) {
        // Initialize three pointers:
        int low = 0;              // Points to the position where the next 0 should go.
        int mid = 0;              // Current element being processed.
        int high = nums.size() - 1; // Points to the position where the next 2 should go.

        // Traverse the array until 'mid' crosses 'high'.
        while (mid <= high) {
            if (nums[mid] == 0) {
                // Case 1: Current element is 0.
                // Swap the current element with the element at 'low'.
                swap(nums[low], nums[mid]);
                low++;  // Move 'low' to the next position.
                mid++;  // Move 'mid' to the next position as 0 is processed.
            } 
            else if (nums[mid] == 1) {
                // Case 2: Current element is 1.
                // No swapping needed as 1 is already in the middle region.
                mid++;  // Just move 'mid' to the next position.
            } 
            else if (nums[mid] == 2) {
                // Case 3: Current element is 2.
                // Swap the current element with the element at 'high'.
                swap(nums[mid], nums[high]);
                high--;  // Move 'high' to the left as 2 is processed.
                // Note: 'mid' is not incremented here because the swapped element at 'mid'
                // could still be 0, 1, or 2 and needs to be processed in the next iteration.
            }
        }
    }
};
