class Solution {
public:
    int countAtMost(vector<int>& nums, int limit) {
        // A subarray cannot contain at most a negative number of odd elements.
        // This handles the case k - 1 becomes negative.
        if (limit < 0) {
            return 0;
        }

        int left = 0;
        int oddCount = 0;
        int subarrayCount = 0;

        // Count all subarrays containing at most 'limit' odd numbers.
        for (int right = 0; right < nums.size(); right++) {

            // An odd number contributes 1 to the odd count.
            // An even number contributes nothing.
            if (nums[right] % 2 != 0) {
                oddCount++;
            }

            // If the window contains too many odd numbers,
            // shrink it from the left until it becomes valid again.
            while (oddCount > limit) {
                if (nums[left] % 2 != 0) {
                    oddCount--;
                }

                left++;
            }

            // The window [left ... right] now contains at most 'limit' odds.
            //
            // Every subarray ending at right and starting anywhere from
            // left to right also contains at most 'limit' odd numbers.
            //
            // These are:
            // [left ... right], [left + 1 ... right], ..., [right ... right]
            subarrayCount += right - left + 1;
        }

        return subarrayCount;
    }

    int numberOfSubarrays(vector<int>& nums, int k) {
        // atMost(k) counts subarrays with:
        // 0, 1, 2, ..., k odd numbers.
        //
        // atMost(k - 1) counts subarrays with:
        // 0, 1, 2, ..., k - 1 odd numbers.
        //
        // Subtracting leaves only subarrays containing exactly k odd numbers.
        return countAtMost(nums, k) -
               countAtMost(nums, k - 1);
    }
};