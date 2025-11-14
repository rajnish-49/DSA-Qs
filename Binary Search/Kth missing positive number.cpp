// @https://leetcode.com/problems/kth-missing-positive-number/description/

class Solution {
public:
    int findKthPositive(vector<int>& arr, int k) {
        int n = arr.size();

        // Binary search boundaries
        int l = 0;        // left pointer (start of array)
        int r = n - 1;    // right pointer (end of array)

        // Perform binary search to find the smallest index
        // where the count of missing numbers is >= k
        while (l <= r) {
            int mid = l + (r - l) / 2;   // mid index to avoid overflow

            // Calculate how many positive numbers are missing
            // until the element at index mid
            // Formula: arr[mid] - (mid + 1)
            // Explanation:
            // - arr[mid] is the actual value at index mid
            // - (mid + 1) is how many numbers should be present up to this index if no numbers were missing
            // - So difference = missing numbers up to arr[mid]
            int missing_till_mid = arr[mid] - (mid + 1);

            // If missing numbers until mid is less than k,
            // it means kth missing number is **after** mid index,
            // so move left pointer rightwards
            if (missing_till_mid < k) {
                l = mid + 1;
            }
            // Else, kth missing number is **at or before** mid,
            // so move right pointer leftwards to narrow down
            else {
                r = mid - 1;
            }
        }

        // After binary search completes:
        // 'l' is the count of elements from arr before the kth missing number
        // The kth missing number will be at position 'l + k' in the natural number sequence

        // Explanation:
        // - There are 'l' numbers present in arr before the answer
        // - We need to skip 'k' missing numbers
        // - So kth missing number = l + k

        // - We know that position l would have value (l+1) in a perfect sequence
        // - Adding k to that gives us the kth missing number
        // - This can also be understood as: r+1+k-(arr[r]-(r+1)) simplified to l+k

        return l + k;
    }
};
