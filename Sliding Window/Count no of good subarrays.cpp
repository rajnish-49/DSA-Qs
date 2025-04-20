// @https://leetcode.com/problems/count-the-number-of-good-subarrays/?envType=daily-question&envId=2025-04-16

class Solution {
    public:
        long long countGood(vector<int>& nums, int k) {
            int n = nums.size();
    
            // Initialize sliding window pointers
            int left = 0, right = 0;
    
            // Hash map to store frequency of elements in current window
            unordered_map<int, int> mp;
    
            // Number of valid pairs (i < j and nums[i] == nums[j]) in the current window
            long long pairs = 0;
    
            // Final answer: number of good subarrays
            long long ans = 0;
    
            // Iterate over the array using the right pointer
            while (right < n) {
                // When adding nums[right] to the window:
                // it forms 'mp[nums[right]]' new pairs with the existing same elements
                pairs += mp[nums[right]];
    
                // Increment the frequency of the current element
                mp[nums[right]]++;
    
                // While current window has at least k pairs (i.e., it's a good subarray)
                while (pairs >= k) {
                    // All subarrays from current left to right, right+1, ..., n-1 are also good
                    // So we add (n - right) to the answer
                    ans += (n - right);
    
                    // Now shrink the window from the left
                    // Before removing nums[left], reduce its contribution to the pair count
                    mp[nums[left]]--;                 // reduce frequency
                    pairs -= mp[nums[left]];          // subtract the number of remaining identical elements
    
                    left++; // move left pointer to the right
                }
    
                // Expand the window to include next element
                right++;
            }
    
            return ans; // total number of good subarrays
        }
    };
    