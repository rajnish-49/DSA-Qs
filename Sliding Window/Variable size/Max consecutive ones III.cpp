// @https://leetcode.com/problems/max-consecutive-ones-iii/

/*
Algorithm: Sliding Window (Two Pointers)
- Use a flexible window that can contain at most k zeros
- Expand the window by moving right pointer
- Shrink the window when zeros exceed k by moving left pointer
- Track the maximum window size encountered

Time Complexity: O(n) - each element is visited at most twice
Space Complexity: O(1) - only using constant extra space
*/

class Solution
{
public:
    int longestOnes(vector<int> &nums, int k)
    {
        // Two pointers to define the sliding window boundaries
        int l = 0; // Left pointer (start of window)
        int r = 0; // Right pointer (end of window)

        // Variables to track the solution
        int maxi = 0;  // Maximum window size found so far
        int zeros = 0; // Count of zeros in current window

        // Expand the window by moving right pointer
        while (r < nums.size())
        {
            // If current element is 0, increment zero count
            // This represents "flipping" a zero to one in our window
            if (nums[r] == 0)
            {
                zeros++;
            }

            // Shrink window from left if we have more than k zeros
            // This maintains the constraint of at most k flips
            while (zeros > k)
            {
                // If left element is 0, we're removing a flipped zero
                if (nums[l] == 0)
                {
                    zeros--;
                }
                l++; // Move left boundary inward
            }

            // Update maximum window size
            // Current window size = r - l + 1
            maxi = max(maxi, r - l + 1);

            // Expand window by moving right pointer
            r++;
        }

        return maxi;
    }
};
