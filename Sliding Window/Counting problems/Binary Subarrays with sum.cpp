//@https://leetcode.com/problems/binary-subarrays-with-sum/description/

#include <vector>
#include <unordered_map>
using namespace std;

class Solution {
public:
    int countAtMost(vector<int>& nums, int limit) {
        // No binary subarray can have a sum less than 0.
        // This handles the case goal - 1 becomes -1.
        if (limit < 0) {
            return 0;
        }

        int left = 0;
        int currentSum = 0;
        int subarrayCount = 0;

        // Count all subarrays whose sum is at most 'limit'.
        for (int right = 0; right < nums.size(); right++) {

            // Include nums[right] in the current window.
            currentSum += nums[right];

            // If the sum exceeds the allowed limit, shrink the window
            // until its sum becomes valid again.
            while (currentSum > limit) {
                currentSum -= nums[left];
                left++;
            }

            // The window [left ... right] now has sum <= limit.
            //
            // Since nums contains only 0s and 1s, removing elements
            // from the left cannot increase the sum. Therefore, every
            // subarray ending at right and starting anywhere from
            // left to right is also valid.
            //
            // These subarrays are:
            // [left ... right], [left + 1 ... right], ..., [right ... right]
            subarrayCount += right - left + 1;
        }

        return subarrayCount;
    }

    int numSubarraysWithSum(vector<int>& nums, int goal) {
        // atMost(goal) counts subarrays with sums:
        // 0, 1, 2, ..., goal
        //
        // atMost(goal - 1) counts subarrays with sums:
        // 0, 1, 2, ..., goal - 1
        //
        // Subtracting them leaves only subarrays with sum exactly goal.
        return countAtMost(nums, goal) -
               countAtMost(nums, goal - 1);
    }
};