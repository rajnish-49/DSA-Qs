// @

class Solution {
public:
    vector<int> transformArray(vector<int>& nums) {
        int j = 0;  
        // j marks the "boundary" of the zero-zone.
        // All indices < j will be filled with 0's as we process the array.
        // Whenever we see a 0, we expand this zone by swapping that 0 into position j.

        for (int i = 0; i < nums.size(); i++) {
            // Step 1: Map the current number to binary form (0 if even, 1 if odd).
            nums[i] = (nums[i] % 2 == 0 ? 0 : 1);

            // Step 2: If it's a 0 → this belongs at the front.
            if (nums[i] == 0) {
                // Swap ensures this 0 is placed at the earliest "empty slot" in the zero-zone.
                // If i == j, it's just a self-swap (no effect).
                swap(nums[i], nums[j]);

                // Move the zero-zone boundary forward.
                j++;
            }
            // If it's a 1 → do nothing, because 1s naturally belong to the right
            // and will fall into place as we keep pulling 0s forward.
        }

        // At the end of this pass:
        // - All 0's are compacted at the front [0...j-1].
        // - All 1's are compacted after that [j...n-1].
        // Which is exactly the "sorted" array we need.
        return nums;
    }
};
