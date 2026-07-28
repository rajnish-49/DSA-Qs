// @

class Solution {
public:
    long long rob(vector<int>& nums, vector<int>& colors) {
        /*
         * PROBLEM UNDERSTANDING:
         * =====================
         * Classic house robber with a TWIST: We cannot rob two ADJACENT houses 
         * if they have the SAME color code.
         * 
         * Key constraints to understand:
         * 1. Adjacent houses with DIFFERENT colors CAN both be robbed
         * 2. Adjacent houses with SAME color CANNOT both be robbed
         * 3. Non-adjacent houses can ALWAYS be robbed (regardless of color)
         * 
         * This means the "adjacency rule" is CONDITIONAL on color matching.
         * 
         * CORE INSIGHT:
         * =============
         * Traditional house robber: dp[i] = max(skip house i, rob house i + dp[i-2])
         * 
         * Here, when we rob house i, we need to check:
         * - If colors[i] == colors[i-1]: We MUST skip house i-1 (can only add dp[i-2])
         * - If colors[i] != colors[i-1]: We CAN rob house i-1 (can add dp[i-1])
         * 
         * Why this works:
         * - dp[i-1] already contains the maximum money from houses [0...i-1]
         * - If house i has different color than i-1, robbing both is valid
         * - If house i has same color as i-1, we must look back to i-2 to avoid 
         *   the adjacent same-color violation
         * 
         * STATE DEFINITION:
         * =================
         * dp[i] = maximum money that can be robbed from houses [0...i]
         * 
         * The answer naturally accumulates because:
         * - dp[i-1] represents the best solution WITHOUT necessarily robbing house i-1
         * - It could have robbed i-1, or skipped it, or robbed i-2, etc.
         * - When we add nums[i] to dp[i-1] (different colors), we're saying:
         *   "Take the best solution up to i-1, and ADD house i on top of it"
         * - This works because different colors means no constraint is violated
         */
        
        int n = nums.size();
        
        // Edge case: Only one house, rob it
        if (n == 1)
            return nums[0];

        // dp[i] represents maximum money robbed considering houses from index 0 to i
        vector<long long> dp(n + 1, -1);

        // BASE CASES:
        // ===========
        
        // Base case 1: Only house 0 exists, we rob it
        dp[0] = nums[0];
        
        // Base case 2: Two houses exist (indices 0 and 1)
        // This is crucial because it sets up the pattern for the rest
        if (colors[1] == colors[0]) {
            // Same color AND adjacent → CANNOT rob both
            // Choose the house with more money
            dp[1] = max(nums[0], nums[1]);
        } else {
            // Different colors → CAN rob both adjacent houses
            // Take both to maximize profit
            dp[1] = nums[0] + nums[1];
        }

        // RECURRENCE RELATION:
        // ====================
        // For each house i (starting from index 2), we have two choices:
        
        for (int i = 2; i < nums.size(); i++) {
            
            // CHOICE 1: Skip current house i
            // Take whatever maximum we had computed till house i-1
            long long skip = dp[i - 1];
            
            // CHOICE 2: Rob current house i
            long long take = 0;
            
            // Now, the amount we can take depends on the color relationship:
            
            if (colors[i] == colors[i - 1]) {
                // SAME COLOR as previous house:
                // ============================
                // We CANNOT include house i-1 in our solution
                // Why? Because if we rob house i, and i-1 was robbed in dp[i-1],
                // we'd violate the "adjacent same color" rule
                // 
                // Solution: Jump back to dp[i-2]
                // dp[i-2] is the best solution that DEFINITELY doesn't include i-1
                // (or if it does include i-1, it's via a different path, but that's
                // impossible since dp is strictly increasing in valid solutions)
                // 
                // Actually, dp[i-2] represents best solution up to house i-2,
                // which inherently doesn't have house i-1 included in a way that
                // would conflict with us now robbing house i
                take = nums[i] + dp[i - 2];
                
            } else {
                // DIFFERENT COLOR from previous house:
                // ====================================
                // We CAN include the entire optimal solution up to i-1
                // There's no adjacency conflict because colors differ
                // 
                // This is the key insight: dp[i-1] might have robbed i-1, or not
                // But it doesn't matter - since colors differ, we can safely add
                // house i to whatever optimal solution existed at i-1
                take = nums[i] + dp[i - 1];
            }
            
            // Take the maximum of skipping or taking house i
            dp[i] = max(skip, take);
        }

        // The answer is the maximum money we can rob from all houses [0...n-1]
        return dp[n - 1];
        
        /*
         * TIME COMPLEXITY: O(n) - Single pass through all houses
         * SPACE COMPLEXITY: O(n) - DP array of size n
         * 
         * OPTIMIZATION POSSIBILITY:
         * Since we only look back at dp[i-1] and dp[i-2], we could optimize
         * space to O(1) by keeping just two variables instead of the entire array.
         */
    }
};