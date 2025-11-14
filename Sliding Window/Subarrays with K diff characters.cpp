// @

class Solution {
public:

    // Helper function to count subarrays with at most k distinct integers
    int atmostk(vector<int>& nums, int k) {
        unordered_map<int, int> freq;  // Frequency map to store counts of elements in current window
        int l = 0;                     // Left pointer of sliding window
        int count = 0;                 // Total count of valid subarrays
       
        // Iterate over the array with right pointer
        for (int r = 0; r < nums.size(); ++r) {
            // Include nums[r] in the window: increment its frequency
            freq[nums[r]]++;

            // If number of distinct elements in window exceeds k,
            // shrink window from left until it is at most k
            while (freq.size() > k) {
                freq[nums[l]]--;               // Decrease frequency of nums[l]
                if (freq[nums[l]] == 0)       // If frequency becomes zero, remove it from map
                    freq.erase(nums[l]);
                l++;                          // Move left pointer to shrink window
            }

            // At this point, window [l..r] has at most k distinct elements
            // Number of subarrays ending at r with at most k distinct elements:
            // all subarrays starting from any index between l and r (inclusive)
            count += (r - l + 1);
        }

        return count;
    }

    // Main function to return number of subarrays with exactly k distinct integers
    int subarraysWithKDistinct(vector<int>& nums, int k) {
        // Number of subarrays with exactly k distinct =
        // Number of subarrays with at most k distinct -
        // Number of subarrays with at most (k-1) distinct
        return atmostk(nums, k) - atmostk(nums, k - 1);
    }
};
