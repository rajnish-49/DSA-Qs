class Solution {
public:
    void moveZeroes(vector<int>& nums) {

        // j points to the position where the next non-zero element
        // should be placed.
        int j = 0;

        for (int i = 0; i < nums.size(); i++) {

            // Whenever we find a non-zero element, move it to the
            // earliest available position.
            //
            // Since non-zero elements are processed from left to right,
            // their relative order remains unchanged.
            if (nums[i] != 0) {
                swap(nums[i], nums[j]);
                j++;
            }
        }

        // After all non-zero elements are moved to the front,
        // the remaining positions automatically contain zeroes.
    }
};

//Everything before j is already finalized and contains non-zero elements in the correct relative order.
// j points to the position where the next non-zero should be placed.
//After a zero is encountered, j usually points to the earliest zero.
// i continues searching ahead for the next non-zero.
