class Solution {
public:
    int longestSubarray(vector<int>& nums) {
        int left = 0;
        int zeroCount = 0;
        int maxLength = 0;

        for (int right = 0; right < nums.size(); right++) {

            // Allow the current window to contain at most one zero,
            // because that one zero can be deleted later.
            if (nums[right] == 0) {
                zeroCount++;
            }

            // If the window contains more than one zero, it cannot become
            // all 1s after deleting only one element. Shrink it from the left
            // until at most one zero remains.
            while (zeroCount > 1) {
                if (nums[left] == 0) {
                    zeroCount--;
                }
                left++;
            }

            // Current window length is right - left + 1.
            // We must delete exactly one element, so the resulting number
            // of 1s is one less than the window size.
            maxLength = max(maxLength, right - left);
        }

        return maxLength;
    }
};