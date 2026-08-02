// @https://leetcode.com/problems/subarrays-with-k-different-integers/description/

class Solution {
public:
    long long countAtMostKDistinct(vector<int>& nums, int k) {
        if (k < 0) {
            return 0;
        }

        unordered_map<int, int> frequency;

        int left = 0;
        long long count = 0;

        for (int right = 0; right < nums.size(); right++) {

            // Include nums[right] in the current window.
            frequency[nums[right]]++;

            // Keep shrinking until the window has at most k distinct values.
            // frequency.size() gives the current number of distinct values
            // because zero-frequency entries are erased.
            while (frequency.size() > k) {
                int leavingValue = nums[left];

                frequency[leavingValue]--;

                // Once its frequency becomes zero, this value is no longer
                // present in the window, so the distinct count decreases.
                if (frequency[leavingValue] == 0) {
                    frequency.erase(leavingValue);
                }

                left++;
            }

            // Now [left ... right] has at most k distinct values.
            //
            // Therefore, every subarray ending at right and starting from
            // left, left + 1, ..., right is also valid.
            //
            // Number of such subarrays:
            // right - left + 1
            count += right - left + 1;
        }

        return count;
    }

    int subarraysWithKDistinct(vector<int>& nums, int k) {

        // Subarrays with exactly k distinct values:
        //
        // atMost(k) contains subarrays with:
        // 1, 2, ..., k distinct values.
        //
        // atMost(k - 1) contains subarrays with:
        // 1, 2, ..., k - 1 distinct values.
        //
        // Subtracting removes all smaller cases and leaves only
        // subarrays with exactly k distinct values.
        return countAtMostKDistinct(nums, k)
             - countAtMostKDistinct(nums, k - 1);
    }
};