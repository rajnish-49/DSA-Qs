// @https://leetcode.com/problems/maximum-value-at-a-given-index-in-a-bounded-array/description/

#define ll long long

class Solution {
public:
    // This function calculates the total sum of the array
    // assuming that nums[index] = mid, and values on both sides decrease by 1
    // (but not going below 1). This helps us check if placing mid at index
    // stays within the allowed maxSum.
    long long totalSum(int mid, int index, int size) {
        int leftLen = index;              // Elements to the left of index
        int rightLen = size - index - 1;  // Elements to the right of index

        // ----------------------
        // LEFT SIDE CONSTRUCTION
        // ----------------------

        long long left = 0;

        // If we can decrease from (mid-1) for all leftLen positions
        if (leftLen < mid) {
            // We're constructing a perfect decreasing sequence:
            // (mid-1), (mid-2), ..., down to (mid - leftLen)
            // This avoids going below 1
            int first = mid - 1;
            int last = mid - leftLen;
            left = 1LL * (first + last) * leftLen / 2;
        } else {
            // If leftLen is too long to decrease fully without going below 1
            // We decrease down to 1 (mid-1 steps), and then fill remaining positions with 1
            int full = mid - 1;                 // Fully decreasing part
            int ones = leftLen - full;          // Positions that must be filled with 1
            // Sum = decreasing sequence from (mid-1) to 1 + 'ones' number of 1s
            left = 1LL * full * (full + 1) / 2 + ones;
        }

        // -----------------------
        // RIGHT SIDE CONSTRUCTION
        // -----------------------

        long long right = 0;

        // Same logic as the left side
        if (rightLen < mid) {
            int first = mid - 1;
            int last = mid - rightLen;
            right = 1LL * (first + last) * rightLen / 2;
        } else {
            int full = mid - 1;
            int ones = rightLen - full;
            right = 1LL * full * (full + 1) / 2 + ones;
        }

        // Add the middle element (nums[index] = mid) to the total sum
        return left + right + mid;
    }

    // The main function: use binary search to find the maximum value
    // that can be placed at nums[index] such that total sum ≤ maxSum
    int maxValue(int n, int index, int maxSum) {
        int low = 1, high = maxSum, ans = 1;

        // Binary search over possible values of nums[index]
        // We want to find the highest value for which totalSum ≤ maxSum
        while (low <= high) {
            int mid = (low + high) / 2;

            // totalSum checks if we can build a valid array with nums[index] = mid
            if (totalSum(mid, index, n) <= maxSum) {
                ans = mid;        // It's a valid answer, try to find a higher one
                low = mid + 1;
            } else {
                high = mid - 1;   // Too big, try a smaller mid
            }
        }

        // Final answer is the highest valid value at nums[index]
        return ans;
    }
};
