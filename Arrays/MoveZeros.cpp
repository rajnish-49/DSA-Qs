// @ https://leetcode.com/problems/move-zeroes/description/

class Solution {
    public:
        void moveZeroes(vector<int>& nums) {
            int j = 0;  // Pointer `j` keeps track of the position where the next non-zero element should be placed.
    
            // Iterate through the array
            for (int i = 0; i < nums.size(); i++) {
                // If the current element is non-zero, swap it with the element at index `j`
                if (nums[i] != 0) {
                    swap(nums[i], nums[j]); // Move non-zero element to the leftmost available position
                    j++;  // Increment `j` to point to the next position for a potential non-zero swap
                }
            }
            // At the end of the loop, all non-zero elements are shifted to the left,
            // and zeroes are automatically moved to the right.
        }
    };
    