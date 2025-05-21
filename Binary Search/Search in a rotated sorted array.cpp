// @https://leetcode.com/problems/search-in-rotated-sorted-array/description/

class Solution {
public:
    /**
     * Finds the pivot point (index of the smallest element) in a rotated sorted array
     * 
     * @param nums The rotated sorted array
     * @param n Size of the array
     * @return Index of the smallest element (pivot point)
     */
    int findpivot(vector<int> &nums, int n) {
        int l = 0;
        int r = n-1;

        while(l < r) {
            int mid = l + (r-l)/2;  // Calculate mid point safely to avoid overflow

            // If the middle element is greater than the rightmost element,
            // the pivot (smallest element) must be in the right half
            if(nums[mid] > nums[r]) {
                l = mid + 1;
            }
            // Otherwise, the pivot is either at mid or in the left half
            else {
                r = mid;
            }
        }

        // When l == r, we've found the pivot index
        return r;
    }

    /**
     * Standard binary search implementation
     * 
     * @param l Left boundary of the search range
     * @param r Right boundary of the search range
     * @param nums The array to search in
     * @param target The value to search for
     * @return Index of target if found, otherwise -1
     */
    int binarysearch(int l, int r, vector<int>& nums, int target) {
        int idx = -1;  // Default return value when target is not found

        while(l <= r) {
            int mid = l + (r-l)/2;  // Calculate mid point safely to avoid overflow

            if(nums[mid] == target) {  // Target found
                idx = mid;
                break;
            } else if(nums[mid] < target) {  // Target is in right half
                l = mid + 1;
            } else {  // Target is in left half
                r = mid - 1;
            }
        }

        return idx;
    }

    /**
     * Searches for target in a rotated sorted array
     * The algorithm:
     * 1. Find the pivot (where rotation occurs)
     * 2. Perform binary search on the left side of pivot
     * 3. If not found, perform binary search on the right side
     * 
     * @param nums The rotated sorted array
     * @param target The value to search for
     * @return Index of target if found, otherwise -1
     */
    int search(vector<int>& nums, int target) {
        int n = nums.size();
        
        // Find the pivot index (smallest element in the array)
        int pivot_index = findpivot(nums, n);

        // Search in the left part (elements before pivot)
        // This part is sorted in ascending order
        int idx = binarysearch(0, pivot_index - 1, nums, target);
        
        // If target is found in the left part, return its index
        if(idx != -1) {
            return idx;
        }

        // If not found in left part, search in the right part (elements from pivot onwards)
        // This part is also sorted in ascending order
        idx = binarysearch(pivot_index, n-1, nums, target);

        return idx;
    }
};


class Solution {
public:
    int search(vector<int>& nums, int target) {
        int s = 0;                      // Start index
        int e = nums.size() - 1;       // End index

        while (s <= e) {               // Standard binary search loop
            int mid = (s + e) / 2;     // Midpoint

            if (nums[mid] == target)   // If target found
                return mid;

            // Check if the left half is sorted
            if (nums[s] <= nums[mid]) {
                // Target lies in the left sorted half
                if (nums[s] <= target && target <= nums[mid]) {
                    e = mid - 1;
                } else {
                    s = mid + 1;
                }
            }
            // Else, the right half must be sorted
            else {
                // Target lies in the right sorted half
                if (nums[mid] <= target && target <= nums[e]) {
                    s = mid + 1;
                } else {
                    e = mid - 1;
                }
            }
        }

        return -1;  // Target not found
    }
};
