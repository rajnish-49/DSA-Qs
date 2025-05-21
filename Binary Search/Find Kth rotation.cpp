// @ https://www.geeksforgeeks.org/problems/rotation4723/1

class Solution {
  public:
    int findKRotation(vector<int> &arr) {
        // This function returns the index of the minimum element in a rotated sorted array
        // which corresponds to the number of times the array has been rotated
        
        // Initialize pointers for binary search
        int l = 0;                // Left pointer starts at the beginning
        int r = arr.size() - 1;   // Right pointer starts at the end
        
        // Binary search continues until the pointers meet
        while(l < r) {
            // Compute middle index (avoids integer overflow)
            int mid = l + (r - l) / 2;
            
            // Key comparison: if mid element is greater than rightmost element,
            // it means the pivot (minimum element) must be in the right half
            if(arr[mid] > arr[r]) {
                // Current mid is part of the first (larger) sorted subarray
                // So the pivot must be to the right of mid
                l = mid + 1;
            }
            else {
                // If arr[mid] <= arr[r], then:
                // 1. mid might be the pivot itself, or
                // 2. pivot is to the left of mid
                // We can't exclude mid yet, so we set right pointer to mid
                r = mid;
            }
        }
        
        // When l == r, we've found the minimum element (the pivot)
        // This index equals the number of rotations performed on the original sorted array
        return l;
    }
};