// @https://leetcode.com/problems/search-in-rotated-sorted-array-ii/description/

class Solution {
public:

    int pivot(vector<int>& nums, int l, int r) {
        while(l < r) {
            
            // Remove duplicates from the left
            while(l < r && nums[l] == nums[l+1])
                l++;
            
            // Remove duplicates from the right
            while(r > l && nums[r] == nums[r-1])  // <-- fixed `r < l` to `r > l`
                r--;
            
            /*
                You need to do what I did above because you'll fail in case like
                [1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1]
                2
                Here, the nums[mid] <= nums[r] and
                and we will cut down the right half but our pivot lies there
                So, make it a RULE, whenever there are duplicate elements and you need to do something
                like Binary Search, you need to ignore duplicates like done above
                Similar Qn : "Smallest element in a rotated sorted array with duplicates"
            */

            int mid = l + (r - l) / 2;  // <-- Standard way to avoid overflow

            if(nums[mid] <= nums[r]) { // Sorted part is from mid to right
                r = mid; // possibly my pivot, so include mid in next search window
            } else {
                l = mid + 1; // Pivot must be to the right of mid
            }
        }

        return r; // <-- Final pivot index where the smallest element is
    }

    bool binarysearch(vector<int>& nums, int l, int r, int& target) {
        while(l <= r) {
            int mid = l + (r - l) / 2;

            if(nums[mid] == target)
                return true;

            if(nums[mid] < target)
                l = mid + 1;
            else
                r = mid - 1;
        }

        return false; // <-- Target not found
    }

    bool search(vector<int>& nums, int target) {

        int n = nums.size();

        int p = pivot(nums, 0, n - 1);  // <-- Find the pivot (index of smallest element)

        // First, search in the left part: from start to pivot-1
        if(binarysearch(nums, 0, p - 1, target)){
            return true;
        }

        // If not found, search in the right part: from pivot to end
        return binarysearch(nums, p, n - 1, target);
    }
};
