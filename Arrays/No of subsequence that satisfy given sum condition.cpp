class Solution {
public:
    // Problem: Count number of subsequences where min + max <= target
    // A subsequence maintains relative order but elements don't need to be contiguous
    int numSubseq(vector<int>& nums, int target) {
        
        int n = nums.size();
        
        // Modulo constant to prevent integer overflow in large calculations
        const int M = 1e9 + 7;
        
        // CRITICAL: Sort the array to use two-pointer technique effectively
        // After sorting, for any subarray [l...r], nums[l] is min and nums[r] is max
        sort(nums.begin(), nums.end());
        
        // Two pointers: left starts from beginning, right from end
        int l = 0;
        int r = n - 1;
        
        int ans = 0; // Final count of valid subsequences
        
        // Precompute powers of 2 to avoid repeated calculations
        // power[i] = 2^i % M
        // This is needed because for a subarray of length k, there are 2^(k-1) subsequences
        // that include the leftmost element (which ensures it's the minimum)
        vector<int> power(n);
        power[0] = 1; // 2^0 = 1
        
        for(int i = 1; i < n; i++){
            power[i] = (power[i-1] * 2) % M; // power[i] = 2^i
        }
        
        int dif = 0; // Variable to store r-l (currently unused but kept for clarity)
        
        // Two-pointer approach to find valid subarrays
        while(l <= r){
            
            // Check if current subarray [l...r] satisfies the condition
            // Since array is sorted: nums[l] = min, nums[r] = max
            if(nums[l] + nums[r] <= target){
                
                dif = r - l; // Length of current valid subarray - 1
                
                // KEY INSIGHT: For subarray [l...r] where min + max <= target,
                // we can form 2^(r-l) subsequences that include nums[l]
                // This is because:
                // - nums[l] must be included (to ensure it's the minimum)
                // - We can choose any subset from the remaining (r-l) elements
                // - Total choices = 2^(r-l)
                ans = (ans + power[r - l]) % M;
                
                // Move left pointer forward to explore more subarrays
                // We've counted all subsequences starting with nums[l]
                l++;
            }
            else{
                // If nums[l] + nums[r] > target, the sum is too large
                // Since nums[l] is the minimum possible, we need to reduce nums[r]
                // Move right pointer backward to get a smaller maximum
                r--;
            }
        }
        
        return ans;
    }
};

/*
ALGORITHM EXPLANATION:
1. Sort array to enable two-pointer technique
2. Use left and right pointers to define current subarray
3. For each valid subarray [l...r] where nums[l] + nums[r] <= target:
   - All subsequences including nums[l] will have nums[l] as minimum
   - Since max in subarray <= target - nums[l], condition is satisfied
   - Count of such subsequences = 2^(r-l)
4. Move pointers strategically to explore all possibilities

TIME COMPLEXITY: O(n log n) - dominated by sorting
SPACE COMPLEXITY: O(n) - for power array

EXAMPLE:
nums = [3,5,6,7], target = 9
After sorting: [3,5,6,7]
- l=0, r=3: 3+7=10 > 9, so r--
- l=0, r=2: 3+6=9 <= 9, add 2^(2-0)=4 subsequences, l++
- l=1, r=2: 5+6=11 > 9, so r--
- l=1, r=1: 5+5=10 > 9, so r--
- l=1, r=0: l > r, stop
Result: 4 subsequences
*/