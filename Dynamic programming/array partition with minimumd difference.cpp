class Solution {
public:
    
    /* ==================================================================================
       CORE PROBLEM: MINIMUM SUBSET SUM DIFFERENCE
       ==================================================================================
       
       Given an array of integers, partition it into two subsets such that the absolute
       difference between their sums is MINIMIZED.
       
       Example: arr = [1, 6, 11, 5]
       - Total sum = 23
       - Optimal partition: {1, 5, 6} and {11}
       - Subset sums: 12 and 11
       - Difference: |12 - 11| = 1 (minimum possible)
       
       ==================================================================================
       FIRST PRINCIPLES INTUITION:
       ==================================================================================
       
       1. KEY INSIGHT: If we can divide elements into two groups with sums S1 and S2,
          then S1 + S2 = totalSum (constant).
          
       2. To minimize |S1 - S2|, we need S1 and S2 to be as close as possible.
          This happens when both are close to totalSum/2.
          
       3. MATHEMATICAL REASONING:
          - Let S1 be the sum of first subset, S2 = totalSum - S1
          - Difference = |S2 - S1| = |totalSum - S1 - S1| = |totalSum - 2*S1|
          - To minimize this, S1 should be as close to totalSum/2 as possible
          
       4. TRANSFORMATION TO SUBSET SUM:
          Instead of exploring all partitions, we ask:
          "What are all possible sums we can achieve using subsets of the array?"
          
          Then for each achievable sum S1 ≤ totalSum/2:
          - S2 = totalSum - S1
          - Calculate difference = |S2 - S1|
          - Track the minimum
          
       5. WHY ONLY CHECK UP TO totalSum/2?
          - If S1 > totalSum/2, then S2 < totalSum/2
          - The difference |S2 - S1| is the same as |S1 - S2|
          - So checking S1 up to half automatically covers all cases without duplication
          
       ==================================================================================
    */
    
    bool solve(int i, int target, vector<int>& arr, 
               vector<vector<int>>& dp) {
        
        /* ------------------------------------------------------------------------------
           RECURSIVE FUNCTION: Can we form a subset with exact sum = target?
           ------------------------------------------------------------------------------
           
           Parameters:
           - i: Current index in array (which element we're deciding on)
           - target: Remaining sum we need to achieve
           - arr: Input array
           - dp: Memoization table
           
           Returns: true if we can form 'target' sum using elements from index i onwards
           
           BASE CASE REASONING:
           - When i == arr.size(), we've considered all elements
           - If target == 0, we successfully formed the exact sum (return true)
           - If target != 0, we failed to form the sum (return false)
        */
        
        if (i == arr.size())
            return (target == 0);  // Success only if we've exhausted the target exactly
        
        /* ------------------------------------------------------------------------------
           MEMOIZATION CHECK:
           - dp[i][target] stores whether we can achieve 'target' from index i onwards
           - -1 means uncomputed, 0 means false, 1 means true
           - Prevents redundant computation of overlapping subproblems
           
           WHY MEMOIZATION HELPS:
           Multiple recursive paths may ask "Can we make sum X from index i?"
           Instead of recomputing, we store and reuse the answer.
        */
        
        if (dp[i][target] != -1)
            return dp[i][target];
        
        /* ------------------------------------------------------------------------------
           DECISION 1: DON'T TAKE current element arr[i]
           - Move to next index without reducing target
           - Recursive call: solve(i+1, target, arr, dp)
        */
        
        bool notTake = solve(i + 1, target, arr, dp);
        
        /* ------------------------------------------------------------------------------
           DECISION 2: TAKE current element arr[i]
           - Only valid if arr[i] <= target (can't take if it exceeds remaining sum)
           - Reduce target by arr[i] and move to next index
           - Recursive call: solve(i+1, target - arr[i], arr, dp)
           
           BOUNDARY CHECK REASONING:
           If arr[i] > target, taking it would make target negative, which is impossible.
           We're looking for exact sum, not "at most" sum.
        */
        
        bool take = false;
        if (arr[i] <= target)
            take = solve(i + 1, target - arr[i], arr, dp);
        
        /* ------------------------------------------------------------------------------
           COMBINE DECISIONS:
           - If EITHER taking OR not taking leads to success, we can form this sum
           - Store result in dp table for future reuse
           - Return the boolean result
        */
        
        return dp[i][target] = (take || notTake);
    }
    
    int minSubsetSumDifference(vector<int>& arr, int n) {
        
        /* ------------------------------------------------------------------------------
           STEP 1: Calculate total sum of all elements
           
           This is our constant: S1 + S2 = totalSum
           Everything else is derived from this constraint.
        */
        
        int totalSum = 0;
        for (int x : arr)
            totalSum += x;
        
        /* ------------------------------------------------------------------------------
           STEP 2: Define search space
           
           half = totalSum / 2
           
           REASONING:
           - We only need to check subset sums from 0 to totalSum/2
           - For any S1 in this range, S2 = totalSum - S1 will be >= S1
           - This avoids checking duplicate pairs (S1, S2) and (S2, S1)
           
           Example: totalSum = 23, half = 11
           - Check S1 = 0 to 11
           - Corresponding S2 = 23 to 12
           - Covers all meaningful partitions without redundancy
        */
        
        int half = totalSum / 2;
        
        /* ------------------------------------------------------------------------------
           STEP 3: Initialize DP table
           
           dp[i][target]:
           - Dimensions: n × (half + 1)
           - dp[i][j] = Can we make sum 'j' using elements from index i onwards?
           - Initialize to -1 (uncomputed state)
           
           SPACE OPTIMIZATION NOTE:
           We could optimize to O(half) space using 1D DP, but 2D is clearer
           for understanding the recursive structure.
        */
        
        vector<vector<int>> dp(n, vector<int>(half + 1, -1));
        
        int minDiff = INT_MAX;  // Track minimum difference found
        
        /* ------------------------------------------------------------------------------
           STEP 4: Try all possible subset sums from 0 to half
           
           CORE ALGORITHM:
           For each potential sum S1 in range [0, half]:
           1. Check if we can actually form a subset with sum = S1
           2. If yes, calculate the corresponding S2 = totalSum - S1
           3. Calculate difference = |S2 - S1|
           4. Update minimum difference
           
           WHY THIS WORKS:
           - We're essentially finding all "reachable" subset sums
           - For each reachable S1, we implicitly partition the array
           - The OTHER subset automatically has sum S2 = totalSum - S1
           - We're guaranteed to find the optimal partition this way
           
           OPTIMIZATION:
           We could break early when minDiff becomes 0 (perfect partition),
           but the loop is already O(half × n) which is acceptable.
        */
        
        for (int s1 = 0; s1 <= half; s1++) {
            
            // Query: Can we form a subset with exactly sum = s1?
            if (solve(0, s1, arr, dp)) {
                
                // Yes! Calculate the complementary subset sum
                int s2 = totalSum - s1;
                
                // Update minimum difference
                // Note: Since s1 <= half and s2 = totalSum - s1, we have s2 >= s1
                // So |s2 - s1| = s2 - s1 (always non-negative)
                minDiff = min(minDiff, abs(s2 - s1));
            }
        }
        
        /* ------------------------------------------------------------------------------
           RETURN: Minimum achievable difference
           
           EDGE CASES HANDLED:
           - s1 = 0 is always achievable (empty subset), giving diff = totalSum
           - s1 = totalSum is achievable (entire array), giving diff = totalSum
           - These are worst cases; any intermediate valid partition is better
        */
        
        return minDiff;
    }
};

/* ====================================================================================
   TIME COMPLEXITY: O(n × totalSum/2)
   - For each of (totalSum/2) possible sums, we run subset sum DP
   - Each DP state (i, target) is computed once due to memoization
   - Total states: n × (totalSum/2)
   
   SPACE COMPLEXITY: O(n × totalSum/2)
   - DP table storage
   - Recursion stack depth: O(n)
   
   ALTERNATIVE APPROACHES:
   1. Brute Force: Try all 2^n partitions → O(2^n) time
   2. Meet-in-the-middle: Divide array in half → O(2^(n/2) × n) time
   3. Bottom-up DP: Iterative subset sum → Same complexity, less stack overhead
   
   THIS APPROACH (Top-down DP with memoization):
   - Clean recursive structure that mirrors the problem's nature
   - Memoization ensures we don't recompute subproblems
   - Optimal for interview/competitive programming scenarios
   ==================================================================================== */




   class Solution {
public:
    int minSubsetSumDifference(vector<int>& arr, int n) {
        
        /* ==================================================================================
           PROBLEM: MINIMUM SUBSET SUM DIFFERENCE (Bottom-Up DP Approach)
           ==================================================================================
           
           Given: Array of integers
           Goal: Partition into TWO subsets such that |sum(S1) - sum(S2)| is MINIMIZED
           
           Example: arr = [1, 6, 11, 5]
           - Total = 23
           - Best partition: {1,5,6} and {11} → sums: 12 and 11 → diff = 1
           
           ==================================================================================
           FUNDAMENTAL INSIGHT: THE MATHEMATICS BEHIND THE SOLUTION
           ==================================================================================
           
           Let S1 = sum of first subset, S2 = sum of second subset
           
           CONSTRAINT: S1 + S2 = totalSum (every element goes to exactly one subset)
           
           OBJECTIVE: Minimize |S1 - S2|
           
           ALGEBRAIC TRANSFORMATION:
           - S2 = totalSum - S1
           - |S1 - S2| = |S1 - (totalSum - S1)| = |2×S1 - totalSum|
           
           KEY REALIZATION:
           To minimize |2×S1 - totalSum|, we need S1 ≈ totalSum/2
           
           So instead of exploring all 2^n partitions, we ask:
           "Which sums between 0 and totalSum/2 can we actually create?"
           
           WHY ONLY UP TO totalSum/2?
           - If S1 = 7 and totalSum = 23, then S2 = 16, diff = 9
           - If S1 = 16 and totalSum = 23, then S2 = 7, diff = 9 (SAME!)
           - Checking both is redundant; we only need S1 ∈ [0, totalSum/2]
           
           ==================================================================================
           APPROACH: BOTTOM-UP DYNAMIC PROGRAMMING
           ==================================================================================
           
           Instead of recursion (top-down), we build the solution from smaller subproblems:
           
           DP STATE DEFINITION:
           dp[i][t] = "Can we form sum 't' using first (i+1) elements (indices 0 to i)?"
           
           TRANSITION:
           For each element arr[i] and target sum t:
           - OPTION 1: Don't include arr[i] → dp[i][t] = dp[i-1][t]
           - OPTION 2: Include arr[i] → dp[i][t] = dp[i-1][t - arr[i]]
           - If EITHER option works, dp[i][t] = true
           
           FINAL ANSWER:
           Check dp[n-1][s1] for all s1 ∈ [0, half]
           For each achievable s1, compute diff = |(totalSum - s1) - s1|
           
           ==================================================================================
        */
        
        /* ------------------------------------------------------------------------------
           STEP 1: Calculate total sum
           
           This defines our constraint space: S1 + S2 = totalSum
           
           EDGE CASE CONSIDERATION:
           - If totalSum is very large, we might have overflow issues
           - For competition: assume totalSum fits in int (typical constraint)
           - For production: consider using long long
        */
        
        int totalSum = 0;
        for (int x : arr)
            totalSum += x;
        
        /* ------------------------------------------------------------------------------
           STEP 2: Define our search boundary
           
           half = totalSum / 2
           
           DEEP REASONING:
           - We're looking for the LARGEST achievable sum ≤ totalSum/2
           - This maximizes S1 while keeping it ≤ half
           - Which minimizes (totalSum - S1) - S1 = totalSum - 2×S1
           
           Example: totalSum = 23, half = 11
           - If we can make S1 = 11, then S2 = 12, diff = 1 ✓
           - If we can only make S1 = 10, then S2 = 13, diff = 3
           - If we can only make S1 = 7, then S2 = 16, diff = 9
           
           The CLOSER S1 gets to half, the SMALLER the difference!
        */
        
        int half = totalSum / 2;
        
        /* ------------------------------------------------------------------------------
           STEP 3: Initialize DP table
           
           dp[i][t] = Can we make sum 't' using elements from index 0 to i?
           
           DIMENSIONS:
           - Rows (i): n elements (0 to n-1)
           - Columns (t): half+1 possible sums (0 to half)
           
           INITIAL STATE:
           - All false (we don't know what's achievable yet)
           
           MEMORY LAYOUT INSIGHT:
           - Space: O(n × totalSum/2)
           - For n=100, totalSum=10000: ~500KB (very manageable)
           - We can optimize to O(totalSum/2) with 1D DP, but 2D is clearer
        */
        
        vector<vector<bool>> dp(n, vector<bool>(half + 1, false));
        
        /* ------------------------------------------------------------------------------
           BASE CASE 1: Sum of 0 is always achievable
           
           REASONING:
           For ANY subset of elements (including empty subset):
           - We can choose to take NOTHING → sum = 0
           - This is true regardless of which elements we've seen
           
           IMPORTANCE:
           - This initializes our DP foundation
           - Without this, we'd have no "true" states to build from
           - It represents the empty subset choice
        */
        
        for (int i = 0; i < n; i++)
            dp[i][0] = true;
        
        /* ------------------------------------------------------------------------------
           BASE CASE 2: First element handling
           
           When we only have arr[0] available:
           - We can form sum = arr[0] (by taking it)
           - We can form sum = 0 (by not taking it - already handled above)
           - We CANNOT form any other sum
           
           BOUNDARY CHECK:
           - Only set dp[0][arr[0]] = true if arr[0] <= half
           - If arr[0] > half, it's outside our search space (we don't care)
           
           Example: arr = [1, 6, 11, 5], half = 11
           - arr[0] = 1, so dp[0][1] = true
           - This means: "Using only first element, we can make sum 1"
        */
        
        if (arr[0] <= half)
            dp[0][arr[0]] = true;
        
        /* ------------------------------------------------------------------------------
           STEP 4: Build DP table bottom-up
           
           ITERATION ORDER REASONING:
           - Outer loop: i from 1 to n-1 (building on previous rows)
           - Inner loop: t from 0 to half (checking all possible target sums)
           
           WHY THIS ORDER?
           - Each dp[i][t] depends only on dp[i-1][...] (previous row)
           - We process rows sequentially to ensure dependencies are computed
           - Within a row, target sums are independent
           
           INTUITION:
           "For each new element we add, update all possible sums we can now create"
        */
        
        for (int i = 1; i < n; i++) {
            for (int t = 0; t <= half; t++) {
                
                /* ------------------------------------------------------------------
                   DECISION 1: Don't take current element arr[i]
                   
                   MEANING:
                   - If we could make sum 't' using elements 0..i-1,
                   - We can still make sum 't' using elements 0..i (just ignore arr[i])
                   
                   EXAMPLE:
                   - dp[2][7] (can we make 7 using first 3 elements?)
                   - If dp[1][7] = true (could make 7 with first 2 elements)
                   - Then dp[2][7] = true (still can, just don't use 3rd element)
                   
                   This preserves all previously achievable sums.
                */
                
                bool notTake = dp[i - 1][t];
                
                /* ------------------------------------------------------------------
                   DECISION 2: Take current element arr[i]
                   
                   CONDITION: arr[i] <= t (must have enough "room" in target)
                   
                   REASONING:
                   - To make sum 't' by INCLUDING arr[i]
                   - We need to have made sum (t - arr[i]) WITHOUT arr[i]
                   - Check: dp[i-1][t - arr[i]]
                   
                   EXAMPLE:
                   - arr = [1, 6, 11, 5], currently at i=1 (arr[1]=6), t=7
                   - To make 7 using elements {1, 6}:
                   - Option: Take 6, and make (7-6)=1 with previous elements
                   - Check: dp[0][1] = true? YES (we set this in base case)
                   - So dp[1][7] = true (we can make 7 = 1 + 6)
                   
                   BOUNDARY PROTECTION:
                   - If arr[i] > t, then (t - arr[i]) < 0 → array out of bounds!
                   - So we only attempt "take" when arr[i] <= t
                */
                
                bool take = false;
                if (arr[i] <= t)
                    take = dp[i - 1][t - arr[i]];
                
                /* ------------------------------------------------------------------
                   COMBINE BOTH DECISIONS
                   
                   LOGICAL OR:
                   - If we can make sum 't' by NOT taking arr[i] → true
                   - OR if we can make sum 't' by TAKING arr[i] → true
                   - Otherwise → false
                   
                   This is the ESSENCE of subset sum DP:
                   "A sum is achievable if it's achievable through ANY valid choice"
                */
                
                dp[i][t] = take || notTake;
            }
        }
        
        /* ------------------------------------------------------------------------------
           STEP 5: Find minimum difference
           
           STRATEGY:
           - Last row dp[n-1] contains all achievable sums using ALL elements
           - For each achievable sum s1 ∈ [0, half]:
           -   Calculate s2 = totalSum - s1 (the OTHER subset's sum)
           -   Calculate diff = |s2 - s1|
           -   Track minimum
           
           WHY SCAN ALL ACHIEVABLE SUMS?
           - The closest achievable sum to half gives minimum difference
           - But we don't know which sums are achievable without checking
           
           OPTIMIZATION OPPORTUNITY:
           - Could scan backward from half to 0 and break on first true
           - But difference might not be monotonic (see example below)
           - So we check all to be safe
           
           EXAMPLE: totalSum = 23, half = 11
           - s1=11: achievable? Check dp[n-1][11]
           -   If true: s2=12, diff=1 (best case!)
           - s1=10: achievable? Check dp[n-1][10]
           -   If true: s2=13, diff=3
           - s1=7: achievable? Check dp[n-1][7]
           -   If true: s2=16, diff=9
           
           We want the s1 CLOSEST to half (but there might be gaps in achievability)
        */
        
        int minDiff = INT_MAX;
        
        for (int s1 = 0; s1 <= half; s1++) {
            
            // Check if sum s1 is achievable using all n elements
            if (dp[n - 1][s1]) {
                
                /* --------------------------------------------------------------
                   Calculate the complementary subset sum
                   
                   LOGIC:
                   - If subset 1 has sum s1
                   - Then subset 2 must have sum (totalSum - s1)
                   - Because every element goes to exactly one subset
                   
                   INVARIANT:
                   Since s1 ≤ half and s2 = totalSum - s1:
                   - s2 ≥ totalSum - half = half (approximately)
                   - So s2 ≥ s1 (always)
                   - Therefore |s2 - s1| = s2 - s1 (no need for abs, but kept for clarity)
                */
                
                int s2 = totalSum - s1;
                
                /* --------------------------------------------------------------
                   Update minimum difference
                   
                   SUBTLE POINT:
                   - We're finding the CLOSEST partition to equal sums
                   - When s1 is closest to half, (s2 - s1) is minimized
                   - This is the mathematical optimum for the partition problem
                */
                
                minDiff = min(minDiff, abs(s2 - s1));
            }
        }
        
        /* ------------------------------------------------------------------------------
           RETURN: Minimum achievable difference
           
           GUARANTEED ANSWER:
           - At minimum, s1=0 is always achievable (empty subset)
           - This gives diff = totalSum (worst case)
           - Any actual partition will be better than or equal to this
           - So minDiff will definitely be updated from INT_MAX
        */
        
        return minDiff;
    }
};

/* ====================================================================================
   COMPLEXITY ANALYSIS
   ====================================================================================
   
   TIME COMPLEXITY: O(n × totalSum)
   - DP table fill: n rows × (totalSum/2) columns = O(n × totalSum/2)
   - Final scan: O(totalSum/2)
   - Total: O(n × totalSum)
   
   SPACE COMPLEXITY: O(n × totalSum)
   - DP table: n × (totalSum/2 + 1) booleans
   - Can be optimized to O(totalSum) using 1D DP (space optimization technique)
   
   ====================================================================================
   COMPARISON: TOP-DOWN vs BOTTOM-UP
   ====================================================================================
   
   TOP-DOWN (Recursion + Memoization):
   ✓ More intuitive (matches problem structure)
   ✓ Only computes needed states
   ✗ Recursion stack overhead
   ✗ Slightly slower due to function calls
   
   BOTTOM-UP (Iterative DP):
   ✓ No recursion overhead
   ✓ Better cache locality
   ✓ Easier to optimize space
   ✗ Computes all states (even unneeded ones)
   ✗ Less intuitive for beginners
   
   ====================================================================================
   SPACE OPTIMIZATION (1D DP)
   ====================================================================================
   
   OBSERVATION: dp[i][t] only depends on dp[i-1][...]
   
   We can use single array and update in-place:
   - Process targets from right to left (half → 0)
   - This prevents overwriting values we still need
   
   Optimized code snippet:
   
   vector<bool> dp(half + 1, false);
   dp[0] = true;
   
   for (int i = 0; i < n; i++) {
       for (int t = half; t >= arr[i]; t--) {
           dp[t] = dp[t] || dp[t - arr[i]];
       }
   }
   
   Space: O(totalSum/2) instead of O(n × totalSum/2)
   
   ====================================================================================
   ALTERNATIVE APPROACHES
   ====================================================================================
   
   1. BRUTE FORCE: Try all 2^n partitions
      - Time: O(2^n × n)
      - Impractical for n > 20
   
   2. MEET IN THE MIDDLE:
      - Split array in half, enumerate all subset sums for each half
      - Time: O(2^(n/2) × n)
      - Space: O(2^(n/2))
      - Good for n ≤ 40 with small element values
   
   3. GREEDY (APPROXIMATION):
      - Sort and assign elements to lighter subset
      - Time: O(n log n)
      - Not guaranteed optimal!
   
   4. THIS DP APPROACH:
      - Exact solution
      - Good for moderate totalSum (≤ 10^5)
      - Pseudo-polynomial time (depends on values, not just n)
   
   ====================================================================================
   WHEN TO USE THIS APPROACH
   ====================================================================================
   
   IDEAL FOR:
   - n ≤ 100, totalSum ≤ 10^5 (typical competitive programming constraints)
   - Need exact optimal solution
   - Values are not too large
   
   AVOID WHEN:
   - totalSum > 10^6 (too much memory/time)
   - n ≤ 40 and totalSum is huge (use meet-in-the-middle)
   - Approximation is acceptable (use greedy)
   
   ==================================================================================== */