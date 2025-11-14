// @ https://leetcode.com/problems/single-element-in-a-sorted-array/description/

class Solution {
public:
    int singleNonDuplicate(vector<int>& nums) {
        int n = nums.size();
        int l = 0;              // Initialize left boundary of binary search
        int h = n - 1;          // Initialize right boundary of binary search

        // Binary search runs as long as l < h
        while (l < h) {
            int mid = l + (h - l) / 2;  // Safe way to compute mid to avoid overflow

            // Check whether mid is even or odd
            if (mid % 2 == 0) {
                // If mid is even and nums[mid] == nums[mid + 1], this pair is valid
                // So, the single element must be in the right half (after mid+1)
                if (nums[mid] == nums[mid + 1]) {
                    l = mid + 2;
                } else {
                    // If nums[mid] != nums[mid + 1], the single element lies on the left side
                    // Could be at mid or before
                    h = mid;
                }
            } else {
                // If mid is odd and nums[mid] == nums[mid - 1], this is also a valid pair
                // So, the single element lies in the right half (after mid)
                if (nums[mid] == nums[mid - 1]) {
                    l = mid + 1;
                } else {
                    // If nums[mid] != nums[mid - 1], then the single element is on the left
                    h = mid - 1;
                }
            }
        }

        // At this point, l == h, which points to the single non-duplicate element
        return nums[h];  // or return nums[l];
    }
};
