// @https://leetcode.com/problems/maximum-value-at-a-given-index-in-a-bounded-array/description/

#define ll long long

class Solution {
public:
    int maxValue(int n, int index, int maxSum) {
        // We try candidate values for the peak at 'index' from maxSum down to 1.
        // The goal is to find the maximum possible peak value i such that
        // the sum of the array with that peak does not exceed maxSum.

        for (int i = maxSum; i >= 1; i--) {
            ll total = i;  // Start sum with the peak element value 'i' at position 'index'

            int leftLen = index;          // Number of elements to the left of 'index'
            int rightLen = n - index - 1; // Number of elements to the right of 'index'

            // Calculate sum of left side elements when peak is i
            if (leftLen > 0) {
                int decCount = i - 1; // Maximum decreasing values from peak on one side (since values decrease by 1 each step)

                if (leftLen <= decCount) {
                    // If left side length is less than or equal to decCount,
                    // then left side elements form a strictly decreasing sequence starting from i-1 down to (i - leftLen).
                    int first = i - 1;            // First element on left side (just before peak)
                    int last = i - leftLen;       // Last element on left side
                    // Sum of arithmetic progression (first + last) * number_of_terms / 2
                    total += 1LL * (first + last) * leftLen / 2;
                } else {
                    // If left side length is greater than decCount,
                    // first decCount elements form decreasing sequence from i-1 to 1,
                    // the remaining elements (leftLen - decCount) will be 1 each (minimum value).
                    total += 1LL * decCount * (decCount + 1) / 2; // sum of 1 to decCount (decreasing sequence)
                    total += 1LL * (leftLen - decCount);          // remaining elements are all 1s
                }
            }

            // Calculate sum of right side elements similarly
            if (rightLen > 0) {
                int decCount = i - 1; // maximum decreasing sequence length on right side

                if (rightLen <= decCount) {
                    // Right side elements form a decreasing sequence starting from i-1 down to (i - rightLen)
                    int first = i - 1;            // first element on right side
                    int last = i - rightLen;      // last element on right side
                    total += 1LL * (first + last) * rightLen / 2;
                } else {
                    // Right side length is longer than decCount
                    // decCount elements form decreasing sequence from i-1 to 1,
                    // remaining elements are 1s
                    total += 1LL * decCount * (decCount + 1) / 2; // sum of decreasing sequence
                    total += 1LL * (rightLen - decCount);         // remaining elements are 1s
                }
            }

            // If total sum with peak 'i' and sides does not exceed maxSum,
            // then this is the maximum valid peak value, so return it
            if (total <= maxSum)
                return i;
        }

        // If no value satisfies the condition (should not happen for min value 1),
        // return 1 as the minimum possible value for the peak
        return 1;
    }
};
