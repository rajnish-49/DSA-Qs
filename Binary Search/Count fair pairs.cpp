// @ https://leetcode.com/problems/count-the-number-of-fair-pairs/?envType=daily-question&envId=2025-04-19

class Solution {
    public:
        long long countFairPairs(vector<int>& nums, int lower, int upper) {
    
            // Step 1: Sort the array to apply binary search
            sort(nums.begin(), nums.end());
            int n = nums.size();
    
            long long ans = 0; // Final answer (use long long to avoid overflow)
    
            // Step 2: Fix the first element nums[i] in the pair
            for (int i = 0; i < n; i++) {
    
                // We want to count how many j > i satisfy:
                // lower <= nums[i] + nums[j] <= upper
                // Rearranging: (for fixed nums[i])
                //   lower - nums[i] <= nums[j] <= upper - nums[i]
    
                // ---- FIND LOWER BOUND ----
                // Find the first index where nums[j] >= (lower - nums[i])
                int idx = lower_bound(nums.begin() + i + 1, nums.end(), lower - nums[i]) - nums.begin();
                
                // x = number of elements BEFORE idx in the valid range (i+1 to end)
                // These are the values that are too small to form a valid pair
                int x = idx - i - 1;
    
                // ---- FIND UPPER BOUND ----
                // Find the first index where nums[j] > (upper - nums[i])
                // So all values before this index are <= (upper - nums[i])
                int udx = upper_bound(nums.begin() + i + 1, nums.end(), upper - nums[i]) - nums.begin();
                
                // y = number of elements BEFORE udx in the valid range (i+1 to end)
                int y = udx - i - 1;
    
                // ---- TOTAL VALID j's FOR THIS i ----
                // Total = y - x
                // Because x elements are too small, and y elements are small or just right
                ans += y - x;
            }
    
            return ans;
        }
    };
    