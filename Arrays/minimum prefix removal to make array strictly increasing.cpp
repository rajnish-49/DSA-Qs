// @https://leetcode.com/problems/minimum-prefix-removal-to-make-array-strictly-increasing/

class Solution { 
public: 
    int minimumPrefixLength(vector<int>& nums) { 
        int n = nums.size(); 
        
        // CORE INTUITION (Left to Right):
        // We're looking for the LAST position where strictly increasing breaks.
        // Why? Because if there's a violation at position i (nums[i] <= nums[i-1]),
        // we CANNOT include nums[i-1] in our final strictly increasing sequence.
        // So we must remove everything UP TO AND INCLUDING position i-1,
        // meaning we remove prefix of length i.
        //
        // KEY INSIGHT: We keep updating prefixLength to the RIGHTMOST violation.
        // After the last violation, everything to the right MUST be strictly 
        // increasing (otherwise we would have found another violation).
        //
        // EXAMPLE: [1, -1, 2, 3, 3, 4, 5]
        // Violation at i=1: nums[1]=-1 <= nums[0]=1, prefixLength=1
        // Violation at i=4: nums[4]=3 <= nums[3]=3, prefixLength=4
        // After i=4, no more violations, so [3,4,5] is strictly increasing
        
        int prefixLength = 0; 
        
        // Scan left to right, tracking the LAST position where order breaks
        for (int i = 1; i < n; i++) { 
            // If current element is NOT strictly greater than previous
            if (nums[i] <= nums[i - 1]) { 
                // We found a violation! This means nums[i-1] cannot be part
                // of our strictly increasing suffix. We must remove everything
                // from index 0 to i-1 (inclusive), which is a prefix of length i.
                //
                // We keep UPDATING this because we want the LAST violation.
                // The suffix starting AFTER the last violation is guaranteed
                // to be strictly increasing.
                prefixLength = i; 
            } 
        } 
        
        // prefixLength now holds the position after the LAST violation
        // Everything from prefixLength to end is strictly increasing
        return prefixLength; 
    } 
}; 

class Solution { 
public: 
    int minimumPrefixLength(vector<int>& nums) { 
        int n = nums.size(); 
 
        // CORE INTUITION (Right to Left):
        // Scan backwards to find WHERE the strictly increasing sequence begins.
        // We're looking for the FIRST position (from right) where order breaks.
        //
        // WHY BACKWARDS? We want to find the LONGEST strictly increasing suffix.
        // Starting from the end (which is always valid as a 1-element sequence),
        // we keep extending leftward as long as the strictly increasing property holds.
        //
        // The MOMENT we find a violation (nums[i] >= nums[i+1]), we know:
        // - Everything from i+1 to end is strictly increasing (we verified this)
        // - Position i BREAKS the pattern, so it cannot be included
        // - Therefore, we must remove prefix [0...i], which has length i+1
        //
        // EXAMPLE: [1, -1, 2, 3, 3, 4, 5]
        // Start from right: 5 > 4 ✓, 4 > 3 ✓, 3 >= 3 ✗ STOP!
        // First violation at i=3, so return 3+1=4
        // This means [3,4,5] is our strictly increasing suffix
        //
        // WHY i+1? Because we're removing everything from 0 to i (inclusive)
        // If violation is at index i, prefix length = i+1
        
        // Scan from second-last element backwards to first element
        for (int i = n - 2; i >= 0; i--) { 
            // If current element is NOT strictly less than next element
            if (nums[i] >= nums[i + 1]) { 
                // VIOLATION FOUND! This is the FIRST violation from the right.
                // Everything to the RIGHT of i (from i+1 to end) is strictly increasing.
                // Element at i BREAKS the pattern, so it must be removed.
                //
                // We remove prefix [0, 1, 2, ..., i], which has length i+1
                // This leaves us with suffix [i+1, i+2, ..., n-1] which is
                // guaranteed to be strictly increasing (we verified it going backwards)
                return i + 1; 
            } 
        } 
        
        // If we exit the loop without finding any violation, it means
        // the ENTIRE array is already strictly increasing!
        // No prefix removal needed, so return 0 (empty prefix)
        return 0; 
    } 
};