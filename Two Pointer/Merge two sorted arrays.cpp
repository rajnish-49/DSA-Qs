// @https://leetcode.com/problems/merge-sorted-array/description/

void merge(vector<int> &nums1, int m, vector<int> &nums2, int n)
{
    // STRATEGY: Merge from the end to avoid overwriting elements in nums1
    // This avoids the need for extra space or shifting elements

    // Initialize three pointers:
    int p1 = m - 1;    // Pointer to the last valid element in nums1
    int p2 = n - 1;    // Pointer to the last element in nums2
    int p = m + n - 1; // Pointer to the last position in the final merged array (in nums1)

    // Merge nums1 and nums2 by working backwards
    // Compare elements from both arrays and place the larger one at the end
    while (p1 >= 0 && p2 >= 0)
    {
        if (nums1[p1] > nums2[p2])
        {
            nums1[p] = nums1[p1]; // Place nums1's larger element at current merge position
            p1--;                 // Move to previous element in nums1
        }
        else
        {
            nums1[p] = nums2[p2]; // Place nums2's larger (or equal) element at current merge position
            p2--;                 // Move to previous element in nums2
        }
        p--; // Move merge position pointer one step back
    }

    // If nums2 still has elements left, copy them to the beginning of nums1
    // Note: If nums1 has elements left, they're already in their correct positions
    while (p2 >= 0)
    {
        nums1[p] = nums2[p2]; // Copy remaining elements from nums2 to nums1
        p2--;
        p--;
    }

    // The merge is complete with all elements in sorted order within nums1
}
