// @https://leetcode.com/problems/count-number-of-nice-subarrays/description/

class Solution {
public:
    int numberOfSubarrays(vector<int>& nums, int k) {
       
        // Map to store frequency of "prefix odd counts" seen so far.
        // Key   → number of odd elements counted so far in the prefix
        // Value → how many prefixes had exactly that many odds
        unordered_map<int, int> oddCountFreq;
        
        // We initialize with oddCount = 0 occurring once
        // This represents: "before starting the array, we have seen 0 odd numbers once"
        // This is important because if at some point oddCount == k, 
        // then oddCount - k == 0 will match this entry and count subarrays starting at index 0.
        oddCountFreq[0] = 1;
        
        int oddCount = 0;  // Total odd numbers seen so far in the current prefix
        int result = 0;    // Final answer → total nice subarrays found
        
        // Iterate through each element in nums
        for (int i = 0; i < nums.size(); i++) {

            // Step 1: Count odds in prefix
            // If nums[i] is odd, increment the running count of odd numbers.
            if (nums[i] % 2 == 1) {
                oddCount++;
            }
            
            // Step 2: Figure out how many odds we *should have seen earlier* 
            // to make exactly k odds in the current subarray.
            // Formula: prefixOddCount[j] - prefixOddCount[i-1] == k
            // Rearranged: prefixOddCount[i-1] == prefixOddCount[j] - k
            int remainingCount = oddCount - k;
            
            // Step 3: If such a "remainingCount" has been seen before,
            // then each occurrence represents a valid start point of a subarray
            // that ends here and contains exactly k odds.
            if (oddCountFreq.find(remainingCount) != oddCountFreq.end()) {
                // Add all those valid start positions to result
                result += oddCountFreq[remainingCount];
            }
        
            // Step 4: Record the current oddCount in our map
            // This means "we have now seen this oddCount one more time"
            oddCountFreq[oddCount]++;
        }
        
        return result;
    }
};
