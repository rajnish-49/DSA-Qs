class Solution {
public:
    vector<int> largestDivisibleSubset(vector<int>& nums) {
        /*
        ═══════════════════════════════════════════════════════════════════════════════
        PROBLEM UNDERSTANDING:
        ═══════════════════════════════════════════════════════════════════════════════
        We need to find the largest subset where every pair (i,j) satisfies:
        - Either nums[i] % nums[j] == 0 OR nums[j] % nums[i] == 0
        
        KEY INSIGHT: If we sort the array, and we have a valid subset [a, b, c] where
        a < b < c, then a divides b AND b divides c, which AUTOMATICALLY means a divides c.
        This is the TRANSITIVE PROPERTY of divisibility.
        
        Why does sorting help?
        - After sorting, we only need to check if nums[i] % nums[j] == 0 (larger % smaller)
        - We don't need to check both directions because smaller comes first
        - The transitivity ensures if we have a chain a|b|c, then a|c automatically holds
        
        CORE CONCEPT - Why Dynamic Programming?
        ═══════════════════════════════════════════════════════════════════════════════
        This is similar to "Longest Increasing Subsequence" (LIS) problem, but instead
        of checking if nums[i] > nums[j], we check if nums[i] is divisible by nums[j].
        
        The DP approach works because:
        1. We build solutions incrementally (smaller subsets → larger subsets)
        2. Optimal substructure: If we have the longest divisible subset ending at j,
           and nums[i] % nums[j] == 0, then we can extend that subset to include i
        3. We track the "parent" to reconstruct the actual path later
        
        Think of it as building chains: [2] → [2,4] → [2,4,8] → [2,4,8,16]
        Each element can extend any previous chain where divisibility holds.
        */
        
        // STEP 1: Sort to enable the transitivity property
        // Example: [1,2,4,8] sorted allows us to build chains left-to-right
        // Without sorting, we might have [8,2,4,1] and miss the connections
        sort(nums.begin(), nums.end());
        int n = nums.size();

        // STEP 2: Initialize DP arrays
        // dp[i] represents: "What's the length of the longest divisible subset 
        // that ENDS at index i (i.e., nums[i] is the largest element in that subset)?"
        // Base case: Every single element forms a subset of length 1
        vector<int> dp(n, 1);
        
        // prev[i] stores: "Which index came before i in the optimal subset?"
        // This creates a linked list structure: -1 ← 0 ← 2 ← 5 (example chain)
        // We use this to backtrack and reconstruct the actual subset
        vector<int> prev(n, -1);
        
        // maxi tracks: "At which index does the LONGEST subset end?"
        // This is our starting point for reconstruction
        int maxi = 0;
        
        // STEP 3: Fill the DP table using bottom-up approach
        // For each position i, we look at ALL previous positions j (where j < i)
        for (int i = 1; i < nums.size(); i++) {
            
            // Check every element before i to see if we can extend its subset
            for (int j = 0; j < i; j++) {
                
                /*
                DIVISIBILITY CHECK: nums[i] % nums[j] == 0
                ─────────────────────────────────────────────────────────────────
                Why this works:
                - nums is sorted, so nums[i] >= nums[j] always
                - If nums[i] % nums[j] == 0, then nums[i] is divisible by nums[j]
                - If we already have a valid subset ending at j, say [a, b, nums[j]],
                  then adding nums[i] creates [a, b, nums[j], nums[i]]
                - Due to transitivity: if a|b and b|nums[j] and nums[j]|nums[i],
                  then a|nums[i] automatically (we don't need to check explicitly!)
                
                Example: nums = [1, 2, 4, 8]
                - At i=2 (nums[2]=4), j=1 (nums[1]=2): 4%2==0 ✓
                  We can extend subset ending at 1: [1,2] → [1,2,4]
                - At i=3 (nums[3]=8), j=2 (nums[2]=4): 8%4==0 ✓
                  We can extend subset ending at 2: [1,2,4] → [1,2,4,8]
                  Notice we don't check if 8%2==0 or 8%1==0, it's automatic!
                */
                if (nums[i] % nums[j] == 0 && dp[i] < dp[j] + 1) {
                    
                    // EXTENSION: We found a better (longer) subset ending at i
                    // dp[j] + 1 means: take the subset ending at j (length dp[j])
                    // and add nums[i] to it (hence +1)
                    dp[i] = dp[j] + 1;
                    
                    // BACKTRACKING INFO: Remember that j comes before i in the chain
                    // This creates: ... → j → i (we'll use this to reconstruct)
                    prev[i] = j;
                }
            }
            
            // TRACKING MAXIMUM: Keep track of where the longest subset ends
            // We need this because the longest subset might not end at the last index
            // Example: [1,2,3,4,8] - longest might be [1,2,4,8], ending at index 4, not 3
            if (dp[i] > dp[maxi]) {
                maxi = i;
            }
        }
        
        // STEP 4: Reconstruct the actual subset by following the prev pointers
        /*
        RECONSTRUCTION LOGIC:
        ─────────────────────────────────────────────────────────────────
        We built a "linked list" using the prev array:
        - Start at maxi (where the longest subset ends)
        - Follow prev[i] backwards: maxi → prev[maxi] → prev[prev[maxi]] → ... → -1
        - Each step gives us an element in the subset (in reverse order)
        
        Example: nums = [1,2,4,8], dp = [1,2,3,4], prev = [-1,0,1,2], maxi = 3
        - Start at i=3: res = [8], next i = prev[3] = 2
        - At i=2: res = [8,4], next i = prev[2] = 1  
        - At i=1: res = [8,4,2], next i = prev[1] = 0
        - At i=0: res = [8,4,2,1], next i = prev[0] = -1 (stop)
        - Result: [8,4,2,1] (note: reverse order, but still valid!)
        */
        vector<int> res;
        for (int i = maxi; i >= 0; i = prev[i]) {
            res.push_back(nums[i]);
        }
        
        // Note: We could reverse res here to get ascending order, but the problem
        // doesn't require any specific order, just a valid divisible subset
        return res;
        
        /*
        TIME COMPLEXITY: O(n²) - nested loops over all pairs
        SPACE COMPLEXITY: O(n) - for dp, prev, and result arrays
        
        WHY THIS APPROACH WORKS:
        ═══════════════════════════════════════════════════════════════════════════════
        1. SORTING ensures we can check divisibility in one direction only
        2. TRANSITIVITY means we don't need to verify all pairs in a subset
        3. DP captures the optimal substructure: best subset ending at each position
        4. BACKTRACKING via prev array lets us retrieve the actual elements
        
        This transforms a potentially exponential problem (checking all subsets)
        into a polynomial O(n²) solution by reusing computed results.
        */
    }
};