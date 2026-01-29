// @ 

class Solution {
public:
    int N, M, K;
    int MOD = 1e9+7;
    int dp[51][51][101];
    
    /*
     * PROBLEM UNDERSTANDING & CORE CONCEPT:
     * =====================================
     * We need to build an array of length N where each element is between 1 and M.
     * The "search cost" is defined as the number of times we encounter a NEW MAXIMUM
     * as we traverse the array from left to right.
     * 
     * Example: [2, 3, 1, 3, 4] has search cost = 3
     *          - At index 0: max = 2 (first element, count = 1)
     *          - At index 1: max = 3 (new max! count = 2)
     *          - At index 2: 1 ≤ 3 (no new max)
     *          - At index 3: 3 ≤ 3 (no new max)
     *          - At index 4: max = 4 (new max! count = 3)
     * 
     * KEY INSIGHT FOR DP:
     * ==================
     * We use 3 states to track our progress:
     * 1. idx: Current position in array we're building (0 to N-1)
     * 2. searchCost: How many new maximums we've encountered so far (0 to K)
     * 3. maxSoFar: The maximum value we've seen up to position idx (0 to M)
     * 
     * WHY track maxSoFar?
     * - To determine if placing a new number creates a new maximum
     * - If new_number > maxSoFar → increment searchCost
     * - If new_number ≤ maxSoFar → searchCost stays same
     */
    
    int solve(int idx, int searchCost, int maxSoFar) {
        // BASE CASE: We've filled all N positions
        if(idx == N) {
            // Valid array only if we achieved EXACTLY K search cost
            // (not less, not more - exactly K new maximums encountered)
            if(searchCost == K)
                return 1;  // This is one valid way to build the array
            return 0;      // Invalid: didn't meet the required search cost
        }
        
        // MEMOIZATION: Check if we've already computed this state
        // This prevents recalculating the same subproblem multiple times
        if(dp[idx][searchCost][maxSoFar] != -1) {
            return dp[idx][searchCost][maxSoFar];
        }
        
        int result = 0;
        
        /*
         * DECISION MAKING: Try placing each possible value (1 to M) at current index
         * ============================================================================
         * For each position, we have M choices (numbers 1 through M)
         * We need to explore all possibilities and count valid arrays
         */
        for(int i = 1; i <= M; i++) {
            
            /*
             * CRITICAL BRANCHING LOGIC:
             * ========================
             * When we place number 'i' at current position, two scenarios:
             * 
             * SCENARIO 1: i > maxSoFar (We found a NEW MAXIMUM!)
             * - This increases our search cost by 1
             * - Update maxSoFar to i (new maximum value)
             * - Example: maxSoFar=3, placing i=5 → new max, searchCost++
             */
            if(i > maxSoFar) {
                result = (result + solve(idx+1, searchCost+1, i)) % MOD;
            } 
            /*
             * SCENARIO 2: i ≤ maxSoFar (NOT a new maximum)
             * - searchCost remains unchanged
             * - maxSoFar stays the same (we haven't beaten the record)
             * - Example: maxSoFar=5, placing i=3 → no new max
             */
            else {
                result = (result + solve(idx+1, searchCost, maxSoFar)) % MOD;
            }
            
        }
        
        /*
         * MEMOIZATION STORAGE:
         * ===================
         * Store the computed result for this state (idx, searchCost, maxSoFar)
         * This way, if we encounter the same state again in our recursion tree,
         * we can return the answer immediately instead of recomputing
         * 
         * Time Complexity: O(N * K * M * M) → because we have N*K*M states and 
         *                  each state tries M values in the loop
         * Space Complexity: O(N * K * M) for the DP table
         */
        return dp[idx][searchCost][maxSoFar] = result % MOD;
        
    }
    
    int numOfArrays(int n, int m, int k) {
        N = n;
        M = m;
        K = k;
        
        // Initialize DP table with -1 (indicating "not yet computed")
        memset(dp, -1, sizeof(dp));
        
        /*
         * INITIAL CALL:
         * ============
         * Start with:
         * - idx = 0 (beginning of array)
         * - searchCost = 0 (no maximums encountered yet)
         * - maxSoFar = 0 (no elements seen yet, using 0 as sentinel value)
         * 
         * Note: maxSoFar=0 works because all array values are ≥ 1,
         * so any first element will trigger i > maxSoFar condition
         */
        return solve(0, 0, 0);
    }
};

/*
 * VISUALIZATION WITH EXAMPLE:
 * ===========================
 * Let's say N=2, M=3, K=2 (array length 2, values 1-3, need 2 new maxes)
 * 
 * Tree structure (simplified):
 *                    solve(0, 0, 0)
 *                   /      |      \
 *         place 1  /   place 2    \  place 3
 *                 /        |        \
 *       solve(1,1,1)  solve(1,1,2)  solve(1,1,3)
 *         / | \          / | \          / | \
 *    try 1,2,3      try 1,2,3        try 1,2,3
 * 
 * Valid arrays with K=2: [1,2], [1,3], [2,3]
 * - [1,2]: max changes 1→2 (cost=2) ✓
 * - [1,3]: max changes 1→3 (cost=2) ✓
 * - [2,3]: max changes 2→3 (cost=2) ✓
 * Answer: 3
 */

 class Solution {
public:
    int numOfArrays(int n, int m, int k) {
        int MOD = 1e9 + 7;
        
        /*
         * ============================================================================
         * PROBLEM RECAP & APPROACH:
         * ============================================================================
         * We need to count arrays of length n with values 1-m where exactly k 
         * "new maximums" occur as we scan left to right.
         * 
         * FORWARD DP INTUITION:
         * ====================
         * We build the array step by step from left to right.
         * At each step, we track:
         *   1. How many positions we've filled so far (idx)
         *   2. How many new maximums we've encountered (searchCost)
         *   3. What the current maximum value is (maxSoFar)
         * 
         * DP STATE MEANING:
         * ================
         * dp[idx][searchCost][maxSoFar] = 
         *     "Number of ways to build an array where:
         *      - We've filled exactly 'idx' positions (0 to idx-1)
         *      - We've encountered 'searchCost' new maximums so far
         *      - The maximum value among filled positions is 'maxSoFar'"
         * 
         * EXAMPLE:
         * dp[2][2][5] = "Ways to fill first 2 positions with 2 new maxes and max=5"
         *               Possible arrays: [4,5], [3,5], [5,5], [1,5], etc.
         *               (Arrays where first 2 positions have max=5 and 2 new max moments)
         * 
         * INDEX RANGES:
         * ============
         * idx: 0 to n
         *   - idx=0: No positions filled yet (empty array)
         *   - idx=1: First position filled (array[0] is set)
         *   - idx=2: First two positions filled (array[0], array[1] are set)
         *   - idx=n: All n positions filled (complete array)
         * 
         * searchCost: 0 to k
         *   - Counts how many times we've seen a new maximum
         * 
         * maxSoFar: 0 to m
         *   - The largest value we've placed in the array so far
         *   - 0 is sentinel value meaning "no elements placed yet"
         */
        vector<vector<vector<long long>>> dp(n + 1, 
                                              vector<vector<long long>>(k + 1, 
                                                                        vector<long long>(m + 1, 0)));
        
        /*
         * ============================================================================
         * STEP 1: BASE CASE INITIALIZATION
         * ============================================================================
         * 
         * STARTING POINT:
         * ==============
         * dp[0][0][0] = 1
         * 
         * MEANING:
         * "There is exactly 1 way to be in the state where:
         *  - 0 positions are filled (empty array)
         *  - 0 new maximums have been encountered (nothing to encounter yet)
         *  - maxSoFar is 0 (sentinel value, no elements yet)"
         * 
         * This is our initialization - we start with an empty array and build forward.
         * 
         * WHY ONLY THIS ONE STATE?
         * All other states at idx=0 are impossible:
         * - dp[0][1][?] = 0 (can't have 1 new max with empty array)
         * - dp[0][0][5] = 0 (can't have max=5 with empty array)
         * - Only dp[0][0][0] makes sense as starting configuration
         */
        dp[0][0][0] = 1;
        
        /*
         * ============================================================================
         * STEP 2: FILL THE DP TABLE (FORWARD DIRECTION)
         * ============================================================================
         * 
         * LOOP STRUCTURE EXPLANATION:
         * ==========================
         * We iterate idx from 0 to n-1
         * 
         * At each idx:
         *   - We've already filled 'idx' positions (positions 0 to idx-1)
         *   - We're NOW deciding what value to place at position 'idx'
         *   - After placing a value, we transition to state with 'idx+1' positions filled
         * 
         * VISUAL EXAMPLE (n=3):
         * ====================
         * idx=0: Array: [?, ?, ?] → Filling position 0 → Transition to idx=1
         * idx=1: Array: [X, ?, ?] → Filling position 1 → Transition to idx=2
         * idx=2: Array: [X, Y, ?] → Filling position 2 → Transition to idx=3
         * idx=3: Array: [X, Y, Z] → Complete! (loop doesn't enter, we extract answer)
         */
        for(int idx = 0; idx < n; idx++) {
            
            /*
             * ITERATE THROUGH ALL POSSIBLE SEARCH COSTS:
             * ==========================================
             * We consider all possible counts of new maximums we might have 
             * encountered up to this point (0 to k)
             */
            for(int searchCost = 0; searchCost <= k; searchCost++) {
                
                /*
                 * ITERATE THROUGH ALL POSSIBLE CURRENT MAXIMUMS:
                 * ==============================================
                 * We consider all possible maximum values that might exist 
                 * in the array we've built so far (0 to m)
                 * 
                 * Note: maxSoFar=0 only makes sense when idx=0 (empty array)
                 */
                for(int maxSoFar = 0; maxSoFar <= m; maxSoFar++) {
                    
                    /*
                     * OPTIMIZATION: SKIP INVALID STATES
                     * =================================
                     * If dp[idx][searchCost][maxSoFar] = 0, it means there's 
                     * NO WAY to reach this configuration.
                     * 
                     * For example:
                     * - dp[2][5][3] = 0 if we can't have 5 new maxes in just 2 positions
                     * - dp[1][0][0] = 0 because after filling 1 position, we must have
                     *   at least 1 new maximum (the first element itself)
                     * 
                     * Skipping these states saves unnecessary computation.
                     */
                    if(dp[idx][searchCost][maxSoFar] == 0) {
                        continue;  // No ways to be in this state, skip it
                    }
                    
                    /*
                     * CURRENT STATE INFORMATION:
                     * =========================
                     * We have dp[idx][searchCost][maxSoFar] different ways to 
                     * reach this configuration.
                     * 
                     * Example: If dp[2][2][5] = 10, it means there are 10 different
                     * arrays where first 2 positions have 2 new maxes and current max is 5.
                     * 
                     * Now we'll try placing each possible value (1 to m) at position idx
                     * and distribute these 10 ways to the resulting next states.
                     */
                    long long currentWays = dp[idx][searchCost][maxSoFar];
                    
                    /*
                     * ========================================================================
                     * DECISION MAKING: TRY PLACING EACH VALUE FROM 1 TO M
                     * ========================================================================
                     * 
                     * At position idx, we can place ANY value from 1 to m.
                     * For each value, we determine:
                     *   1. Does it create a new maximum?
                     *   2. What state do we transition to?
                     * 
                     * TWO SCENARIOS:
                     * =============
                     * SCENARIO 1: value > maxSoFar → NEW MAXIMUM FOUND
                     * SCENARIO 2: value ≤ maxSoFar → NOT a new maximum
                     */
                    for(int value = 1; value <= m; value++) {
                        
                        /*
                         * ================================================================
                         * SCENARIO 1: value > maxSoFar (CREATES NEW MAXIMUM!)
                         * ================================================================
                         * 
                         * WHAT HAPPENS:
                         * ============
                         * When we place a value GREATER than the current maximum:
                         * - This creates a "new maximum" moment
                         * - searchCost increases by 1
                         * - maxSoFar updates to this new value
                         * 
                         * CONCRETE EXAMPLE:
                         * ================
                         * Current state: dp[2][1][3] = 5
                         *   → "5 ways to have filled 2 positions with 1 new max and max=3"
                         *   → Possible arrays so far: [3,2], [1,3], [2,3], [3,3], [3,1]
                         * 
                         * Now place value=5 at position 2:
                         *   → 5 > 3? YES! This is a NEW MAXIMUM
                         *   → searchCost goes from 1 to 2 (found 2nd new maximum)
                         *   → maxSoFar becomes 5 (new maximum value)
                         *   → Transition TO: dp[3][2][5]
                         *   → Add all 5 ways: dp[3][2][5] += 5
                         * 
                         * Resulting arrays: [3,2,5], [1,3,5], [2,3,5], [3,3,5], [3,1,5]
                         * All have 2 new maximum moments and current max is 5
                         * 
                         * TRANSITION DETAILS:
                         * ==================
                         * FROM: dp[idx][searchCost][maxSoFar]
                         * TO:   dp[idx+1][searchCost+1][value]
                         *       ↑        ↑             ↑
                         *       |        |             |
                         *       |        |             +-- New maximum value
                         *       |        +-- Increment (found new max)
                         *       +-- One more position filled
                         */
                        if(value > maxSoFar) {
                            
                            /*
                             * BOUNDARY CHECK:
                             * ==============
                             * We need searchCost+1 ≤ k
                             * 
                             * Why? Because we can't have MORE than k new maximums.
                             * If we already have k new maximums and try to add another,
                             * the resulting array will have k+1 new maxes, which is invalid.
                             * 
                             * Example: If k=2 and searchCost=2, placing a value > maxSoFar
                             * would give us 3 new maxes, which exceeds our target k=2.
                             */
                            if(searchCost + 1 <= k) {
                                /*
                                 * UPDATE THE NEXT STATE:
                                 * =====================
                                 * Add all the ways from current state to the next state
                                 * 
                                 * We use (a + b) % MOD to prevent overflow
                                 * MOD = 10^9 + 7 (standard modulo for counting problems)
                                 */
                                dp[idx + 1][searchCost + 1][value] = 
                                    (dp[idx + 1][searchCost + 1][value] + currentWays) % MOD;
                            }
                            /*
                             * If searchCost+1 > k, we simply don't make this transition.
                             * These paths are invalid and won't contribute to our answer.
                             */
                        }
                        
                        /*
                         * ================================================================
                         * SCENARIO 2: value ≤ maxSoFar (NOT A NEW MAXIMUM)
                         * ================================================================
                         * 
                         * WHAT HAPPENS:
                         * ============
                         * When we place a value LESS THAN OR EQUAL TO current maximum:
                         * - This does NOT create a new maximum
                         * - searchCost stays the same
                         * - maxSoFar stays the same (we haven't beaten the record)
                         * 
                         * CONCRETE EXAMPLE:
                         * ================
                         * Current state: dp[2][2][7] = 3
                         *   → "3 ways to have filled 2 positions with 2 new maxes and max=7"
                         *   → Possible arrays: [5,7], [2,7], [7,7]
                         * 
                         * Now place value=4 at position 2:
                         *   → 4 ≤ 7? YES, this is NOT a new maximum
                         *   → searchCost stays at 2 (no new maximum found)
                         *   → maxSoFar stays at 7 (still the highest)
                         *   → Transition TO: dp[3][2][7]
                         *   → Add all 3 ways: dp[3][2][7] += 3
                         * 
                         * Resulting arrays: [5,7,4], [2,7,4], [7,7,4]
                         * All still have 2 new maximum moments and current max remains 7
                         * 
                         * ANOTHER EXAMPLE (placing value equal to max):
                         * =============================================
                         * Place value=7 at position 2:
                         *   → 7 ≤ 7? YES, equal is NOT a new maximum
                         *   → searchCost stays at 2
                         *   → maxSoFar stays at 7
                         *   → Same transition: dp[3][2][7] += 3
                         * 
                         * Resulting arrays: [5,7,7], [2,7,7], [7,7,7]
                         * 
                         * KEY INSIGHT:
                         * ===========
                         * When value ≤ maxSoFar, no matter which specific value we choose
                         * (1, 2, 3, ..., maxSoFar), the transition is ALWAYS to the same
                         * next state: dp[idx+1][searchCost][maxSoFar]
                         * 
                         * This is why we see this state getting updated multiple times
                         * in the inner loop - once for each value from 1 to maxSoFar.
                         * 
                         * TRANSITION DETAILS:
                         * ==================
                         * FROM: dp[idx][searchCost][maxSoFar]
                         * TO:   dp[idx+1][searchCost][maxSoFar]
                         *       ↑        ↑          ↑
                         *       |        |          |
                         *       |        |          +-- Stays same (no new max)
                         *       |        +-- Stays same (no new max)
                         *       +-- One more position filled
                         */
                        else {
                            /*
                             * UPDATE THE NEXT STATE:
                             * =====================
                             * Add all the ways from current state to the next state
                             * 
                             * Note: Multiple values (1 to maxSoFar) will all contribute
                             * to this same next state, so dp[idx+1][searchCost][maxSoFar]
                             * accumulates contributions from all these values.
                             */
                            dp[idx + 1][searchCost][maxSoFar] = 
                                (dp[idx + 1][searchCost][maxSoFar] + currentWays) % MOD;
                        }
                    }
                    /*
                     * END OF VALUE LOOP:
                     * =================
                     * We've now tried placing all possible values (1 to m) at position idx
                     * and propagated the ways to appropriate next states.
                     */
                }
            }
        }
        /*
         * END OF DP TABLE FILLING:
         * =======================
         * At this point, dp[n][?][?] contains the number of ways to build
         * complete arrays (all n positions filled) with various search costs
         * and final maximum values.
         */
        
        /*
         * ============================================================================
         * STEP 3: EXTRACT THE ANSWER
         * ============================================================================
         * 
         * WHAT WE WANT:
         * ============
         * Arrays of length n with EXACTLY k new maximums.
         * 
         * WHERE TO LOOK:
         * =============
         * dp[n][k][?] - all states where:
         *   - n positions are filled (complete array)
         *   - Exactly k new maximums were encountered
         *   - Any final maximum value from 1 to m
         * 
         * WHY SUM ACROSS ALL maxSoFar VALUES?
         * ==================================
         * The final maximum can be any value from 1 to m.
         * 
         * Examples (n=3, k=2):
         * - Array [1,2,2]: final max = 2, has k=2 new maxes → counts
         * - Array [1,3,3]: final max = 3, has k=2 new maxes → counts
         * - Array [2,5,4]: final max = 5, has k=2 new maxes → counts
         * 
         * All of these are valid answers with different final maximums.
         * So we sum: dp[n][k][1] + dp[n][k][2] + ... + dp[n][k][m]
         * 
         * NOTE ON maxSoFar=0:
         * ==================
         * We start loop from maxSoFar=1, not 0, because:
         * - A complete non-empty array cannot have maximum value of 0
         * - All array values are between 1 and m
         * - dp[n][k][0] will always be 0 (impossible state)
         * 
         * CONCRETE EXAMPLE:
         * ================
         * If dp[3][2][1] = 0  (no arrays with max=1 and 2 new maxes)
         *    dp[3][2][2] = 5  (5 arrays with max=2 and 2 new maxes)
         *    dp[3][2][3] = 10 (10 arrays with max=3 and 2 new maxes)
         * 
         * Total valid arrays = 0 + 5 + 10 = 15
         */
        long long result = 0;
        
        for(int maxSoFar = 1; maxSoFar <= m; maxSoFar++) {
            /*
             * ACCUMULATE ALL VALID ARRAYS:
             * ===========================
             * Add the count of arrays that:
             * - Have exactly n positions filled
             * - Have exactly k new maximums
             * - Have final maximum value = maxSoFar
             */
            result = (result + dp[n][k][maxSoFar]) % MOD;
        }
        
        /*
         * RETURN THE FINAL ANSWER:
         * =======================
         * result now contains the total count of valid arrays modulo 10^9+7
         */
        return result;
    }
};

/*
 * ============================================================================
 * COMPLEXITY ANALYSIS:
 * ============================================================================
 * 
 * TIME COMPLEXITY: O(N × K × M × M) = O(N × K × M²)
 * =================================================
 * - Outer loop: idx from 0 to n-1 → O(N)
 * - Second loop: searchCost from 0 to k → O(K)
 * - Third loop: maxSoFar from 0 to m → O(M)
 * - Inner loop: value from 1 to m → O(M)
 * - Total: O(N) × O(K) × O(M) × O(M) = O(N × K × M²)
 * 
 * With constraints: N=50, M=100, K=50
 * Worst case: 50 × 50 × 100 × 100 = 25,000,000 operations (acceptable)
 * 
 * SPACE COMPLEXITY: O(N × K × M)
 * ==============================
 * - DP table: dp[N+1][K+1][M+1]
 * - Size: (N+1) × (K+1) × (M+1) = 51 × 51 × 101 ≈ 262,000 long long values
 * - Memory: ~2 MB (well within limits)
 * 
 * ============================================================================
 * EXAMPLE TRACE (N=2, M=3, K=2):
 * ============================================================================
 * 
 * INITIALIZATION:
 * dp[0][0][0] = 1
 * 
 * idx=0: (Filling position 0)
 * -------------------------
 * From dp[0][0][0]=1:
 *   Place 1: 1>0 → dp[1][1][1] += 1 → dp[1][1][1]=1
 *   Place 2: 2>0 → dp[1][1][2] += 1 → dp[1][1][2]=1
 *   Place 3: 3>0 → dp[1][1][3] += 1 → dp[1][1][3]=1
 * 
 * idx=1: (Filling position 1)
 * -------------------------
 * From dp[1][1][1]=1:
 *   Place 1: 1≤1 → dp[2][1][1] += 1 → dp[2][1][1]=1 (array [1,1])
 *   Place 2: 2>1 → dp[2][2][2] += 1 → dp[2][2][2]=1 (array [1,2])
 *   Place 3: 3>1 → dp[2][2][3] += 1 → dp[2][2][3]=1 (array [1,3])
 * 
 * From dp[1][1][2]=1:
 *   Place 1: 1≤2 → dp[2][1][2] += 1 → dp[2][1][2]=1 (array [2,1])
 *   Place 2: 2≤2 → dp[2][1][2] += 1 → dp[2][1][2]=2 (array [2,2])
 *   Place 3: 3>2 → dp[2][2][3] += 1 → dp[2][2][3]=2 (arrays [1,3],[2,3])
 * 
 * From dp[1][1][3]=1:
 *   Place 1,2,3: all ≤3 → dp[2][1][3] += 3 → dp[2][1][3]=3
 * 
 * ANSWER EXTRACTION:
 * result = dp[2][2][1] + dp[2][2][2] + dp[2][2][3]
 *        = 0 + 1 + 2
 *        = 3
 * 
 * Valid arrays: [1,2], [1,3], [2,3] ✓
 */