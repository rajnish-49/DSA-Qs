// @ https://leetcode.com/problems/remove-element/

#include <vector>

using namespace std;

class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        // 'writer' tracks the exact index where the next valid element should go.
        // It also acts as the counter for the new length of the array.
        int writer = 0;
        
        // 'reader' scans through every element in the array from start to finish.
        for (int reader = 0; reader < nums.size(); reader++) {
            
            // If the current element is NOT the target value, we want to keep it.
            if (nums[reader] != val) {
                
                // Copy the valid element into the safe 'writer' position.
                nums[writer] = nums[reader];
                
                // Advance the writer forward to prepare for the next valid element.
                writer++;
            }
            // If nums[reader] == val, we do nothing. The reader just moves on, 
            // effectively leaving the garbage value behind to be overwritten later.
        }
        
        // The final value of 'writer' is exactly the count of valid elements.
        return writer;
    }
};