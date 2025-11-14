// @

class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        // Two-pointer approach to remove elements in-place
        
        int j = 0;  // SLOW/WRITE pointer: tracks position for next valid element
                    // Also represents count of valid elements found so far
        
        int i = 0;  // FAST/READ pointer: scans through entire array
                    // Examines each element one by one
        
        // Single pass through the array
        while (i < nums.size()) {
            
            // Check if current element should be kept (not equal to val)
            if (nums[i] != val) {
                // VALID ELEMENT FOUND:
                // Copy current element to the "write" position
                nums[j] = nums[i];
                
                // Move write pointer forward for next valid element
                j++;
                
                // At this point: j represents number of valid elements processed
                // Valid elements are stored in positions [0, 1, 2, ..., j-1]
            }
            // If nums[i] == val, we skip it (don't copy, don't increment j)
            
            // ALWAYS move read pointer forward to examine next element
            i++;
        }
        
        // j now contains:
        // 1. Count of elements that are NOT equal to val
        // 2. Position where next element would be placed (if we had one)
        // 
        // The first j elements of nums[] contain all valid elements
        // Elements at positions [j, j+1, ..., nums.size()-1] are irrelevant
        
        return j;  // Return count of valid elements
    }
};
