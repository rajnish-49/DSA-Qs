class Solution {
public:
    void sortColors(vector<int>& nums) {
        int low = 0;
        int mid = 0;
        int high = nums.size() - 1;

        // At any point, the array is divided into four regions:
        //
        // [0 ... low-1]       -> correctly placed 0s
        // [low ... mid-1]     -> correctly placed 1s
        // [mid ... high]      -> not processed yet
        // [high+1 ... n-1]    -> correctly placed 2s
        //
        // mid always checks the first element of the unprocessed region.

        while (mid <= high) {

            if (nums[mid] == 0) {
                // A 0 belongs on the left.
                // low is the position where the next 0 should be placed.
                swap(nums[mid], nums[low]);

                // The 0 is now correctly placed.
                // The value moved to mid is a processed 1, so both pointers move.
                low++;
                mid++;
            }

            else if (nums[mid] == 1) {
                // A 1 already belongs in the middle region.
                // No swap is required.
                mid++;
            }

            else {
                // nums[mid] is 2, so it belongs on the right.
                // high is the position where the next 2 should be placed.
                swap(nums[mid], nums[high]);
                high--;

                // Do not increment mid here.
                // The value brought from high was unprocessed,
                // so we must check it in the next iteration.
            }
        }
    }
};