// @https://leetcode.com/problems/find-peak-element/

class Solution {
public:
    int findPeakElement(vector<int>& nums) {

        int l = 0;                        // Left boundary of search space
        int r = nums.size() - 1;         // Right boundary of search space

        // Binary search loop: stop when l == r, i.e., one element left which is a peak
        while (l < r) {

            int mid = l + (r - l) / 2;   // Calculate middle index to avoid overflow

            // Case 1: If mid is the first element and it's greater than the next
            // Or if mid is the last element and greater than the previous
            // This handles edge cases directly
            if ((mid == 0 && nums[mid] > nums[mid + 1]) || 
                (mid == nums.size() - 1 && nums[mid] > nums[mid - 1])) {
                return mid;
            }

            // Case 2: If nums[mid] is greater than both neighbors, it's a peak
            // (check only if mid is not at boundary)
            else if (mid > 0 && mid < nums.size() - 1 &&
                     nums[mid - 1] < nums[mid] && nums[mid] > nums[mid + 1]) {
                return mid;
            }

            // Case 3: If the element to the right is greater,
            // we are in an increasing slope, so move right
            else if (nums[mid] < nums[mid + 1]) {
                l = mid + 1;
            }

            // Case 4: If the element to the left is greater or equal,
            // move left — peak must be on left side (could include mid)
            else {
                r = mid;
            }
        }

        // When loop ends, l == r, and it will be a peak index
        return l;
    }
};
