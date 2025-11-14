// @https://leetcode.com/problems/remove-duplicates-from-sorted-array/description/

class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        // Two-pointer approach to remove duplicates in-place from sorted array
        
        // Pointer 'i' tracks the position of the last unique element found
        // It represents the boundary between processed unique elements and the rest
        int i = 0;
        
        // Pointer 'j' explores the array to find the next unique element
        // It always stays ahead of 'i' to scan for new unique values
        int j = 1;
        
        // Continue until we've examined all elements in the array
        while(j < nums.size()){
            
            // Case 1: Current elements are duplicates
            // nums[i] holds the last confirmed unique element
            // nums[j] is a duplicate, so we skip it by advancing j
            if(nums[i] == nums[j]){
                j++; // Move to next element without updating unique boundary
            }
            else{
                // Case 2: Found a new unique element at position j
                
                // First, advance the unique boundary to the next position
                i++;
                
                // Then, place the newly found unique element at the boundary
                // This overwrites any duplicate that might be at position i
                nums[i] = nums[j];
                
                // Note: j will be incremented in the next iteration
                // This maintains the invariant that j > i
            }
        }
        
        // Return the count of unique elements
        // Since i is 0-indexed and points to the last unique element,
        // the total count is i + 1
        return i + 1;
    }
};