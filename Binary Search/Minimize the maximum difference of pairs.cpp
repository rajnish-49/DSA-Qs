// @https://leetcode.com/problems/minimize-the-maximum-difference-of-pairs/description/

class Solution {
public:
    /*
     * CORE INSIGHT: This function uses GREEDY STRATEGY to check feasibility
     * 
     * WHY GREEDY WORKS: Since array is sorted, if nums[i] and nums[i+1] can form 
     * a valid pair, it's always optimal to pair them rather than pairing nums[i] 
     * with some nums[j] where j > i+1, because:
     * - nums[i+1] - nums[i] <= nums[j] - nums[i] (due to sorted order)
     * - This leaves nums[i+1] available for potentially better future pairings
     * 
     * REVISION TIP: Remember this greedy choice - always pair adjacent valid elements!
     */
    bool possible(vector<int>& nums, int mid, int pairs) {
        int cnt = 0;  // Count of pairs formed so far
        int i = 0;    // Current position in array
        
        // TRAVERSAL PATTERN: Skip by 2 when pair found, by 1 when not found
        while(i < nums.size() - 1) {
            
            // CHECK: Can current and next element form a valid pair?
            if(abs(nums[i] - nums[i+1]) <= mid) {
                cnt++;      // Found a valid pair
                i = i + 2;  // CRITICAL: Skip both elements (they're now used)
                
                // DEBUGGING TIP: If getting wrong answer, check if you're 
                // incrementing i correctly here
            }
            else {
                i++;        // Only current element can't be paired with next,
                           // try next position
            }
        }
        
        // FEASIBILITY CHECK: Can we form at least 'pairs' number of pairs?
        return cnt >= pairs;  // Simplified return statement
    }
    
    /*
     * MAIN ALGORITHM: Binary Search on Answer
     * 
     * KEY INSIGHT: We're not searching for an element IN the array,
     * we're searching for the ANSWER itself (minimum possible maximum difference)
     * 
     * SEARCH SPACE: [0, max_element - min_element]
     * - Lower bound: 0 (if we have duplicate elements)
     * - Upper bound: Maximum possible difference in the array
     * 
     * MONOTONIC PROPERTY: If we can form p pairs with max difference = X,
     * then we can definitely form p pairs with max difference = Y where Y > X
     * This monotonic property enables binary search!
     */
    int minimizeMax(vector<int>& nums, int p) {
        
        // PREPROCESSING: Sort array for greedy strategy to work
        // REVISION NOTE: Always sort when dealing with difference-based problems
        sort(nums.begin(), nums.end());
        
        // SEARCH SPACE DEFINITION
        int lmin = 0;  // Best possible answer (all pairs have 0 difference)
        int rmax = abs(nums.back() - nums.front());  // Worst case scenario
        
        int ans = 0;  // Will store our final answer
        
        // BINARY SEARCH TEMPLATE - searching for minimum valid answer
        while (lmin <= rmax) {
            int mid = lmin + (rmax - lmin) / 2;  // Avoid overflow
            
            // DECISION POINT: Can we achieve our goal with max difference = mid?
            if(possible(nums, mid, p)) {
                // SUCCESS: mid is a valid answer, but can we do better?
                ans = mid;        // Store current valid answer
                rmax = mid - 1;   // Search for smaller valid answer
                                 // (minimize the maximum)
            }
            else {
                // FAILURE: mid is too restrictive, we need larger differences
                lmin = mid + 1;   // Search in right half
            }
        }
        
        return ans;
    }
};

