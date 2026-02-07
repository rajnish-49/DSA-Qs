class Solution {
  public:
  
  int n ;
  vector<vector<int>> dp;
  
  /*
   * PROBLEM STATEMENT & CORE CONCEPT:
   * =================================
   * We need to find the COUNT of all possible subsets of arr[] whose sum equals target.
   * 
   * KEY INSIGHT - Why this is a DP problem:
   * Each element presents a BINARY CHOICE: include it in our subset or skip it.
   * For arr = [1, 2, 3] and target = 3, the decision tree looks like:
   *                          []
   *                    /           \
   *              [1]                  []
   *            /     \              /    \
   *        [1,2]     [1]        [2]       []
   *        /  \      /  \       /  \      /  \
   *    [1,2,3][1,2][1,3][1] [2,3][2] [3]  []
   * 
   * This creates overlapping subproblems: we might encounter "remaining target = 2" 
   * multiple times at different array positions, making memoization ideal.
   * 
   * WHY MEMOIZATION WORKS HERE:
   * State = (current index, remaining target)
   * If we've already computed "from index i, how many ways to make target t", 
   * we don't need to recompute it - we can reuse the answer.
   * 
   * DP STATE DEFINITION:
   * dp[idx][target] = number of ways to form 'target' sum using elements from index 'idx' to end
   * 
   * DIMENSION CHOICE RATIONALE:
   * - First dimension (idx): tracks our position in the array (0 to n-1)
   * - Second dimension (target): tracks remaining sum we need to achieve (0 to target)
   * - Why these two? Because these are the ONLY parameters that change in our recursion
   *   and uniquely identify each subproblem
   */
  
  int solve(int idx , vector<int> &arr , int target){
      
      // BASE CASE - We've exhausted all elements
      // =========================================
      // When idx == n, we've made decisions for ALL elements
      // Now we check: did our choices lead to the exact target sum?
      // 
      // WHY target == 0 means success:
      // - We started with original target
      // - Each element we "took" was subtracted from target
      // - If target becomes 0, it means sum of taken elements = original target
      // 
      // Return 1 (found one valid subset) or 0 (this path didn't work)
      if (idx == n) {
            return (target == 0) ? 1 : 0;
        }
      
      // MEMOIZATION CHECK - Avoid redundant computation
      // ================================================
      // If we've already solved this exact subproblem (same idx, same target),
      // return the cached result instead of recomputing the entire subtree.
      // 
      // This is the KEY OPTIMIZATION that reduces time complexity from O(2^n) to O(n * target)
      // because each unique (idx, target) pair is computed only once.
      if(dp[idx][target] != -1)
        return dp[idx][target];
      
      // CHOICE 1: SKIP the current element
      // ===================================
      // We move to next index without taking arr[idx]
      // Target remains unchanged because we didn't add this element to our subset
      // 
      // INTUITION: Maybe this element is too large, or maybe taking it would prevent
      // us from reaching the target. We explore the possibility of ignoring it.
      int skip = solve(idx + 1, arr , target);
      
      // CHOICE 2: TAKE the current element (if valid)
      // ==============================================
      int take = 0 ;
      
      // VALIDITY CHECK: Can we even take this element?
      // We can only take arr[idx] if it doesn't exceed our remaining target
      // 
      // WHY this check matters:
      // - If arr[idx] > target, taking it would make our sum EXCEED the target
      // - We'd end up with negative remaining target, which is impossible to satisfy
      // - Example: if target = 5 and arr[idx] = 7, we cannot take 7
      if(arr[idx] <= target)  
        // Take arr[idx]: reduce target by arr[idx] because we've "used up" that much sum
        // Move to next index because we can't reuse the same element
        // 
        // NEW TARGET = target - arr[idx] represents the "remaining sum we still need"
        // after including this element in our subset
        take = solve(idx + 1, arr , target - arr[idx]);
      
     // COMBINATION PRINCIPLE - The core of counting
     // ============================================
     // We ADD the results because:
     // - 'skip' counts all valid subsets that DON'T include arr[idx]
     // - 'take' counts all valid subsets that DO include arr[idx]
     // - These two sets are MUTUALLY EXCLUSIVE (an element is either in subset or not)
     // - Total count = count(subsets without arr[idx]) + count(subsets with arr[idx])
     // 
     // This is the ADDITION PRINCIPLE from combinatorics:
     // If we can partition outcomes into disjoint cases, total = sum of each case
     // 
     // MEMOIZATION STORE: Cache the result before returning
     // This ensures if we encounter the same (idx, target) again, we return instantly
     return dp[idx][target] = skip + take;
}
      
  
  
    int perfectSum(vector<int>& arr, int target) {
        // INITIALIZATION PHASE
        // ====================
        
        n = arr.size();
        
        // DP Table Setup: 
        // dp[i][j] will store the answer for subproblem "index i, remaining target j"
        // 
        // DIMENSIONS EXPLAINED:
        // - Rows (n): one for each index position in array (0 to n-1)
        // - Columns (target + 1): for each possible remaining sum (0 to target)
        //   We need target+1 because we include 0 as a valid remaining sum
        // 
        // INITIALIZATION VALUE (-1):
        // -1 means "not yet computed" - serves as a sentinel value
        // Any valid answer (count of subsets) will be >= 0, so -1 is distinguishable
        dp.assign(n, vector<int>(target + 1, -1));
        
        // START RECURSION from:
        // - Index 0 (beginning of array)
        // - Full target (haven't reduced it yet)
        // 
        // This kicks off the decision tree exploration for all elements
        return solve(0 , arr , target );
        
    }
};

/*
 * COMPLEXITY ANALYSIS:
 * ====================
 * 
 * TIME COMPLEXITY: O(n * target)
 * - We have n * target unique subproblems (each combination of index and remaining sum)
 * - Each subproblem is solved exactly once due to memoization
 * - Each subproblem does O(1) work (two function calls that hit cache or compute new)
 * 
 * SPACE COMPLEXITY: O(n * target) + O(n)
 * - O(n * target) for the DP table
 * - O(n) for recursion stack depth (in worst case, we go n levels deep)
 * 
 * 
 * EXAMPLE WALKTHROUGH:
 * ====================
 * arr = [2, 3, 5], target = 5
 * 
 * solve(0, target=5):
 *   skip: solve(1, target=5)
 *     skip: solve(2, target=5)
 *       skip: solve(3, target=5) → base case → 0 (target != 0)
 *       take: solve(3, target=0) → base case → 1 ✓ (subset = [5])
 *       Returns: 0 + 1 = 1
 *     take: solve(2, target=2)
 *       skip: solve(3, target=2) → base case → 0
 *       take: can't take (5 > 2)
 *       Returns: 0
 *     Returns: 1 + 0 = 1
 *   take: solve(1, target=3)
 *     skip: solve(2, target=3)
 *       skip: solve(3, target=3) → base case → 0
 *       take: can't take (5 > 3)
 *       Returns: 0
 *     take: solve(2, target=0)
 *       skip: solve(3, target=0) → base case → 1 ✓ (subset = [2, 3])
 *       Returns: 1 (taking 5 when target=0 invalid)
 *     Returns: 0 + 1 = 1
 *   Returns: 1 + 1 = 2 ✓
 * 
 * Valid subsets: [5] and [2, 3] → Count = 2
 */



class Solution {
public:
    
    int perfectSum(vector<int>& arr, int target) {
        
        /*
         * BOTTOM-UP DYNAMIC PROGRAMMING APPROACH (TABULATION)
         * ====================================================
         * 
         * FUNDAMENTAL PROBLEM:
         * Find the count of all subsets of arr[] that sum exactly to target.
         * 
         * WHY BOTTOM-UP vs TOP-DOWN (Memoization)?
         * ==========================================
         * Top-down (recursion + memoization):
         *   - Starts from the problem and breaks down
         *   - Only computes states that are actually needed
         *   - Uses recursion stack (overhead + potential stack overflow)
         * 
         * Bottom-up (tabulation):
         *   - Starts from base cases and builds up
         *   - Computes all possible states systematically
         *   - Iterative (no stack overhead, slightly faster in practice)
         *   - Better cache locality (sequential memory access)
         * 
         * PARADIGM SHIFT: Instead of asking "how many ways from index i?",
         * we ask "how many ways using FIRST i elements?"
         */
        
        int n = arr.size();
        
        /*
         * DP TABLE CONSTRUCTION - The Foundation
         * =======================================
         * 
         * STATE DEFINITION (Critical to understand):
         * dp[i][s] = number of ways to achieve sum 's' using the FIRST 'i' elements of arr[]
         * 
         * INDEXING PHILOSOPHY (Why n+1 and target+1):
         * - dp[0][...] represents using 0 elements (empty array)
         * - dp[1][...] represents using first 1 element (arr[0])
         * - dp[2][...] represents using first 2 elements (arr[0], arr[1])
         * - dp[n][...] represents using all n elements
         * 
         * This is different from memoization where dp[i] meant "from index i onwards"!
         * Here dp[i] means "using elements 0 to i-1 (first i elements)"
         * 
         * DIMENSIONS EXPLAINED:
         * - Rows (n + 1): We need n+1 because we include state "0 elements used"
         *   Range: [0, n] where 0 = no elements, n = all elements
         * 
         * - Columns (target + 1): All possible sums from 0 to target
         *   Range: [0, target] where 0 = empty subset sum, target = goal sum
         * 
         * INITIALIZATION (Why all zeros):
         * We initialize everything to 0 because initially we know of ZERO ways
         * to form any sum with any number of elements. We'll build this up.
         */
        vector<vector<long long>> dp(n + 1, vector<long long>(target + 1, 0));
        
        /*
         * BASE CASE - The Seed of Our Solution
         * =====================================
         * 
         * dp[0][0] = 1 is THE MOST CRITICAL LINE
         * 
         * WHAT IT MEANS:
         * "There is exactly ONE way to make sum 0 using 0 elements"
         * That one way is: the empty subset { }
         * 
         * WHY THIS MATTERS - The Recursive Foundation:
         * Every other cell in our table will ultimately trace back to this.
         * When we take elements and reduce the sum, we eventually reach sum=0,
         * and this base case tells us "yes, that's a valid path, count it as 1".
         * 
         * IMPLICIT BASE CASES (already handled by initialization):
         * - dp[0][s] = 0 for all s > 0
         *   "Zero ways to make any positive sum with zero elements"
         *   You can't make sum=5 if you have no elements!
         * 
         * CONTRAST WITH MEMOIZATION:
         * In recursion, we checked "if(idx == n) return target == 0 ? 1 : 0"
         * Here, dp[0][0] = 1 achieves the same logical outcome through table initialization
         */
        dp[0][0] = 1;
        
        /*
         * MAIN DP ITERATION - Building the Solution Layer by Layer
         * =========================================================
         * 
         * OUTER LOOP (i from 1 to n):
         * Process elements one by one. At iteration i, we're deciding what to do
         * with arr[i-1] (the i-th element, 0-indexed).
         * 
         * WHY i starts at 1:
         * - dp[0][...] is already set (base case for 0 elements)
         * - We're building solutions for 1 element, 2 elements, ..., n elements
         * 
         * INNER LOOP (s from 0 to target):
         * For each possible sum value, compute how many ways to achieve it
         * using the first i elements.
         * 
         * LOGICAL FLOW:
         * At dp[i][s], we're standing at element arr[i-1] and asking:
         * "How many ways can I make sum s using elements arr[0...i-1]?"
         */
        for (int i = 1; i <= n; i++) {
            for (int s = 0; s <= target; s++) {
                
                /*
                 * RECURRENCE RELATION - The Heart of DP
                 * ======================================
                 * 
                 * CHOICE 1: DON'T INCLUDE arr[i-1] in the subset
                 * ------------------------------------------------
                 * dp[i][s] = dp[i - 1][s]
                 * 
                 * INTUITION:
                 * "If I don't take the i-th element (arr[i-1]), then the number of ways
                 * to make sum s using first i elements is the SAME as the number of ways
                 * to make sum s using first (i-1) elements."
                 * 
                 * EXAMPLE:
                 * arr = [2, 3, 5], i=2 (element 3), s=5
                 * If we don't take 3, then ways to make 5 with [2,3] = ways to make 5 with [2]
                 * We're just passing along the previous result.
                 * 
                 * WHY THIS IS ALWAYS VALID:
                 * We ALWAYS have the option to skip an element. Even if taking it
                 * would be beneficial, skipping is still a valid choice to consider.
                 * This captures all subsets that don't include arr[i-1].
                 */
                dp[i][s] = dp[i - 1][s];
                
                /*
                 * CHOICE 2: INCLUDE arr[i-1] in the subset (if possible)
                 * --------------------------------------------------------
                 * 
                 * PRECONDITION CHECK: arr[i - 1] <= s
                 * 
                 * WHY WE NEED THIS CHECK:
                 * If arr[i-1] > s, then including arr[i-1] would make our sum EXCEED s.
                 * We're trying to make sum s, not something larger.
                 * 
                 * EXAMPLE OF INVALID CASE:
                 * arr[i-1] = 7, s = 5
                 * Taking 7 alone already exceeds 5, so this is impossible.
                 */
                if (arr[i - 1] <= s) {
                    
                    /*
                     * THE ADDITION: dp[i][s] += dp[i - 1][s - arr[i - 1]]
                     * 
                     * BREAKTHROUGH INSIGHT - The Magic of DP:
                     * "If I TAKE arr[i-1], then I need the remaining sum to be s - arr[i-1]"
                     * 
                     * DECOMPOSITION LOGIC:
                     * - Current goal: make sum s with elements 0...(i-1)
                     * - If I take arr[i-1], it contributes arr[i-1] to the sum
                     * - Remaining needed: s - arr[i-1]
                     * - Who can provide this remaining sum? Elements 0...(i-2)
                     * - How many ways can they do it? dp[i-1][s - arr[i-1]]
                     * 
                     * CONCRETE EXAMPLE:
                     * arr = [2, 3, 5], i=3 (element 5), s=5
                     * If we take 5:
                     *   - We need remaining sum = 5 - 5 = 0
                     *   - Look at dp[2][0]: ways to make 0 with [2,3]
                     *   - dp[2][0] = 1 (the empty subset)
                     *   - So taking 5 gives us 1 way: {5}
                     * 
                     * arr = [2, 3, 5], i=2 (element 3), s=5
                     * If we take 3:
                     *   - We need remaining sum = 5 - 3 = 2
                     *   - Look at dp[1][2]: ways to make 2 with [2]
                     *   - dp[1][2] = 1 (subset {2})
                     *   - So taking 3 gives us 1 way: {2, 3}
                     * 
                     * WHY += (Addition, not assignment):
                     * We ALREADY counted subsets that don't include arr[i-1] in the first line.
                     * Now we're ADDING the count of subsets that DO include arr[i-1].
                     * Total = (subsets without arr[i-1]) + (subsets with arr[i-1])
                     * 
                     * This is the INCLUSION-EXCLUSION principle in action:
                     * These two groups are mutually exclusive and collectively exhaustive.
                     */
                    dp[i][s] += dp[i - 1][s - arr[i - 1]];
                }
                
                /*
                 * STATE TRANSITION COMPLETE
                 * =========================
                 * At this point, dp[i][s] contains the total count:
                 * - All ways to make sum s WITHOUT using arr[i-1] (from skip)
                 * - All ways to make sum s BY using arr[i-1] (from take)
                 * 
                 * This cell is now complete and can be used by future iterations
                 * to build solutions for larger subproblems.
                 */
            }
        }
        
        /*
         * FINAL ANSWER EXTRACTION
         * ========================
         * 
         * dp[n][target] = our answer
         * 
         * WHAT THIS REPRESENTS:
         * "Number of ways to achieve sum 'target' using ALL n elements (first n elements)"
         * Since we have n elements total, "first n elements" means the entire array.
         * 
         * WHY dp[n][target] specifically:
         * - dp[n][...] means we've considered all elements
         * - dp[...][target] means we've achieved our goal sum
         * - Intersection: all subsets from entire array that sum to target
         * 
         * TRACE BACK TO UNDERSTAND:
         * This final cell was built up from:
         * - dp[n-1][target] (subsets not using last element)
         * - dp[n-1][target - arr[n-1]] (subsets using last element)
         * 
         * And those were built from earlier cells, all the way back to dp[0][0] = 1
         */
        return dp[n][target];
    }
};

/*
 * COMPLETE EXAMPLE WALKTHROUGH
 * ==============================
 * arr = [2, 3], target = 5
 * 
 * INITIAL STATE:
 * dp table (rows = elements considered, columns = sum):
 *        s=0  s=1  s=2  s=3  s=4  s=5
 * i=0     1    0    0    0    0    0   (base: 1 way to make 0 with nothing)
 * i=1     ?    ?    ?    ?    ?    ?   (will consider arr[0]=2)
 * i=2     ?    ?    ?    ?    ?    ?   (will consider arr[1]=3)
 * 
 * 
 * ITERATION 1 (i=1, element arr[0]=2):
 * -----------------------------------------
 * s=0: dp[1][0] = dp[0][0] = 1 (skip 2)
 *      Can't take 2 (2 > 0)
 *      Result: 1 way → {}
 * 
 * s=1: dp[1][1] = dp[0][1] = 0 (skip 2)
 *      Can't take 2 (2 > 1)
 *      Result: 0 ways
 * 
 * s=2: dp[1][2] = dp[0][2] = 0 (skip 2)
 *      Take 2: += dp[0][2-2] = dp[0][0] = 1
 *      Result: 0 + 1 = 1 way → {2}
 * 
 * s=3: dp[1][3] = dp[0][3] = 0 (skip 2)
 *      Take 2: += dp[0][3-2] = dp[0][1] = 0
 *      Result: 0 + 0 = 0 ways
 * 
 * s=4: dp[1][4] = dp[0][4] = 0 (skip 2)
 *      Take 2: += dp[0][4-2] = dp[0][2] = 0
 *      Result: 0 + 0 = 0 ways
 * 
 * s=5: dp[1][5] = dp[0][5] = 0 (skip 2)
 *      Take 2: += dp[0][5-2] = dp[0][3] = 0
 *      Result: 0 + 0 = 0 ways
 * 
 * After i=1:
 *        s=0  s=1  s=2  s=3  s=4  s=5
 * i=0     1    0    0    0    0    0
 * i=1     1    0    1    0    0    0
 * 
 * 
 * ITERATION 2 (i=2, element arr[1]=3):
 * -----------------------------------------
 * s=0: dp[2][0] = dp[1][0] = 1 (skip 3)
 *      Can't take 3 (3 > 0)
 *      Result: 1 way → {}
 * 
 * s=1: dp[2][1] = dp[1][1] = 0 (skip 3)
 *      Can't take 3 (3 > 1)
 *      Result: 0 ways
 * 
 * s=2: dp[2][2] = dp[1][2] = 1 (skip 3)
 *      Can't take 3 (3 > 2)
 *      Result: 1 way → {2}
 * 
 * s=3: dp[2][3] = dp[1][3] = 0 (skip 3)
 *      Take 3: += dp[1][3-3] = dp[1][0] = 1
 *      Result: 0 + 1 = 1 way → {3}
 * 
 * s=4: dp[2][4] = dp[1][4] = 0 (skip 3)
 *      Take 3: += dp[1][4-3] = dp[1][1] = 0
 *      Result: 0 + 0 = 0 ways
 * 
 * s=5: dp[2][5] = dp[1][5] = 0 (skip 3)
 *      Take 3: += dp[1][5-3] = dp[1][2] = 1
 *      Result: 0 + 1 = 1 way → {2, 3}
 * 
 * FINAL TABLE:
 *        s=0  s=1  s=2  s=3  s=4  s=5
 * i=0     1    0    0    0    0    0
 * i=1     1    0    1    0    0    0
 * i=2     1    0    1    1    0    1
 * 
 * ANSWER: dp[2][5] = 1
 * Valid subset: {2, 3}
 * 
 * 
 * COMPLEXITY ANALYSIS
 * ====================
 * 
 * TIME COMPLEXITY: O(n × target)
 * - Two nested loops: outer runs n times, inner runs (target+1) times
 * - Each cell computation is O(1) (just addition/assignment)
 * - Total operations: n × (target + 1) ≈ O(n × target)
 * 
 * SPACE COMPLEXITY: O(n × target)
 * - 2D DP table of size (n+1) × (target+1)
 * - No recursion stack (unlike memoization)
 * - Can be optimized to O(target) using space optimization technique
 *   (keeping only previous row since we only look back one row)
 * 
 * 
 * COMPARISON: TOP-DOWN vs BOTTOM-UP
 * ==================================
 * 
 * Top-Down (Memoization):
 * ✓ More intuitive (follows natural problem breakdown)
 * ✓ Only computes needed states
 * ✗ Recursion overhead
 * ✗ Potential stack overflow for large n
 * 
 * Bottom-Up (Tabulation):
 * ✓ No recursion overhead
 * ✓ Better cache locality (sequential access)
 * ✓ Easier to optimize space
 * ✗ Computes all states (even if not needed)
 * ✗ Less intuitive initially
 * 
 * Both have the same time and space complexity asymptotically!
 */