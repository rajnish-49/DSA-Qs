class Solution {
public:
    int n;
    vector<vector<int>> dp;
    
    /*
     * PROBLEM: Coin Change II - Count UNIQUE COMBINATIONS (not permutations)
     * ========================================================================
     * Given coins = [1,2,5] and amount = 5, we need to count how many DISTINCT WAYS
     * we can make 5 using these coins with unlimited supply.
     * 
     * Valid combinations: {1,1,1,1,1}, {1,1,1,2}, {1,2,2}, {5} = 4 ways
     * 
     * KEY INSIGHT: Why this is NOT a permutation problem
     * ---------------------------------------------------
     * - {1,2,2} and {2,1,2} and {2,2,1} are the SAME combination (just different orders)
     * - We only count {1,2,2} once, not 3 times
     * - This is why we process coins in a FIXED ORDER (index i goes forward, never backward)
     * 
     * CORE STRATEGY: Fix the coin order to avoid duplicate counting
     * ---------------------------------------------------------------
     * By always moving forward in the coins array (i -> i+1), we ensure that:
     * - Once we skip coin[i], we NEVER come back to it for this amount
     * - This automatically prevents counting the same combination multiple times
     * - Example: If we use coin[1] (value=2), then coin[0] (value=1), 
     *   we won't later consider coin[0] then coin[1] as a separate combination
     */
    
    int solve(int i, vector<int>& coins, int amount) {
        /*
         * BASE CASE 1: Perfect Match - We've exactly made the target amount
         * ------------------------------------------------------------------
         * When amount becomes 0, we've found ONE valid combination.
         * This is a LEAF NODE in our decision tree - a successful path.
         * 
         * Why memoize this? Because dp[i][0] will always be 1 for any i.
         * (There's exactly 1 way to make amount=0: use no coins)
         */
        if (amount == 0) 
            return dp[i][amount] = 1;
        
        /*
         * BASE CASE 2: Invalid States - Dead ends in our exploration
         * -----------------------------------------------------------
         * 1. i == n: We've exhausted all coin types without reaching amount=0
         *    - No more coins to try, but amount > 0, so this path fails
         *    - Return 0 (this path contributes 0 combinations)
         * 
         * 2. amount < 0: We've overshot the target (went negative)
         *    - This shouldn't happen with the optimization below, but safety check
         *    - Return 0 (invalid path)
         */
        if (i == n || amount < 0) 
            return 0;
        
        /*
         * MEMOIZATION: Avoid redundant computation
         * -----------------------------------------
         * State: dp[i][amount] = "number of ways to make 'amount' using coins[i..n-1]"
         * 
         * Why is this state sufficient?
         * - 'i' tells us which coins are still available (coins[i] onwards)
         * - 'amount' tells us what we still need to make
         * - Together, they uniquely identify a subproblem
         * 
         * Example overlap: coins=[1,2], amount=5
         * - Path 1: take 1, take 1 -> reach state (0, 3)
         * - Path 2: take 1, take 1, skip 2 -> reach state (1, 3)  [DIFFERENT]
         * But state (0,3) might be computed multiple times from different paths
         */
        if (dp[i][amount] != -1) 
            return dp[i][amount];
        
        /*
         * OPTIMIZATION: Skip coins that are too large
         * --------------------------------------------
         * If coins[i] > amount, taking it will immediately make amount < 0.
         * No point exploring the "take" branch - it's guaranteed to fail.
         * 
         * So we ONLY explore the "skip" option: move to next coin (i+1).
         * 
         * Example: amount=3, coin[i]=5
         * - Taking coin[i] would need amount = 3-5 = -2 (invalid)
         * - We skip to coin[i+1] and continue
         */
        if (coins[i] > amount) 
            return dp[i][amount] = solve(i + 1, coins, amount);
        
        /*
         * DECISION TREE: The heart of the algorithm
         * ==========================================
         * At each state (i, amount), we have TWO choices:
         * 
         * CHOICE 1: TAKE coin[i]
         * -----------------------
         * - Use this coin (reduce amount by coins[i])
         * - STAY at index i (because we can reuse the same coin - unlimited supply!)
         * - Recurse: solve(i, coins, amount - coins[i])
         * 
         * Why stay at index i?
         * - Problem allows unlimited use of each coin
         * - We might want to take coins[i] again (and again) for this amount
         * - Example: coins=[2], amount=6 -> we take 2 three times: {2,2,2}
         * 
         * CHOICE 2: SKIP coin[i]
         * ----------------------
         * - Don't use this coin at all for current amount
         * - Move to next coin (i+1) - we're DONE with coins[i] for this amount
         * - Recurse: solve(i + 1, coins, amount)
         * 
         * Why move to i+1?
         * - We're deciding to NOT use coins[i] anymore
         * - Try making the amount with remaining coins[i+1..n-1]
         * 
         * COMBINATION PRINCIPLE: Add both choices
         * ----------------------------------------
         * Total ways = ways(using coins[i]) + ways(without coins[i])
         * This is the ADDITION PRINCIPLE from combinatorics:
         * - If we partition all solutions into two disjoint sets (with/without coins[i])
         * - Total count = count of set1 + count of set2
         */
        int take = solve(i, coins, amount - coins[i]);
        int skip = solve(i + 1, coins, amount);
        
        return dp[i][amount] = take + skip;
    }
    
    int change(int amount, vector<int>& coins) {
        n = coins.size();
        
        /*
         * INITIALIZATION: Set up memoization table
         * -----------------------------------------
         * dp[i][j] = -1 means "not computed yet"
         * Dimensions: (n+1) × (amount+1)
         * - Row i represents: "using coins from index i onwards"
         * - Column j represents: "target amount j"
         * 
         * Why n+1 rows? 
         * - Index n represents "no coins left" (base case)
         * 
         * Why amount+1 columns?
         * - We need columns for amounts 0 to amount (inclusive)
         */
        dp.assign(n + 1, vector<int>(amount + 1, -1));
        
        /*
         * Start solving from:
         * - Index 0: Consider all coins
         * - Amount: The target amount given
         */
        return solve(0, coins, amount);
    }
};

/*
 * ============================================================================
 * COMPLEXITY ANALYSIS
 * ============================================================================
 * 
 * Time Complexity: O(n × amount)
 * -------------------------------
 * - Total unique states: n × amount
 * - Each state computed once (memoization)
 * - Work per state: O(1) (just two recursive calls)
 * 
 * Space Complexity: O(n × amount)
 * --------------------------------
 * - DP table: O(n × amount)
 * - Recursion stack: O(n + amount) in worst case
 * - Overall: O(n × amount)
 * 
 * ============================================================================
 * EXAMPLE WALKTHROUGH: coins = [1,2,5], amount = 5
 * ============================================================================
 * 
 * solve(0, [1,2,5], 5)  -> "make 5 using coins from index 0 onwards"
 * │
 * ├─ take coins[0]=1: solve(0, [1,2,5], 4)  -> "make 4 using coins from index 0"
 * │  ├─ take 1: solve(0, [1,2,5], 3)
 * │  │  ├─ take 1: solve(0, [1,2,5], 2)
 * │  │  │  ├─ take 1: solve(0, [1,2,5], 1)
 * │  │  │  │  ├─ take 1: solve(0, [1,2,5], 0) -> RETURN 1 ✓ {1,1,1,1,1}
 * │  │  │  │  └─ skip 1: solve(1, [1,2,5], 1)
 * │  │  │  │     └─ skip 2: skip 5: RETURN 0
 * │  │  │  └─ skip 1: solve(1, [1,2,5], 2)
 * │  │  │     ├─ take 2: solve(1, [1,2,5], 0) -> RETURN 1 ✓ {1,1,2}
 * │  │  │     └─ skip 2: solve(2, [1,2,5], 2) -> RETURN 0
 * │  │  └─ skip 1: solve(1, [1,2,5], 3)
 * │  │     ├─ take 2: solve(1, [1,2,5], 1) -> already computed
 * │  │     └─ skip 2: solve(2, [1,2,5], 3) -> RETURN 0
 * │  └─ skip 1: solve(1, [1,2,5], 4)
 * │     ├─ take 2: solve(1, [1,2,5], 2)
 * │     │  ├─ take 2: solve(1, [1,2,5], 0) -> RETURN 1 ✓ {2,2}
 * │     │  └─ skip 2: solve(2, [1,2,5], 2) -> RETURN 0
 * │     └─ skip 2: solve(2, [1,2,5], 4) -> RETURN 0
 * │
 * └─ skip coins[0]=1: solve(1, [1,2,5], 5)
 *    ├─ take 2: solve(1, [1,2,5], 3) -> already computed above
 *    └─ skip 2: solve(2, [1,2,5], 5)
 *       ├─ take 5: solve(2, [1,2,5], 0) -> RETURN 1 ✓ {5}
 *       └─ skip 5: solve(3, [1,2,5], 5) -> RETURN 0 (i==n)
 * 
 * Total: 4 combinations found ✓
 * 
 * ============================================================================
 * ALTERNATIVE APPROACH: BOTTOM-UP DP (Space Optimized)
 * ============================================================================
 * 
 * The above is TOP-DOWN (recursion + memoization). We can also solve BOTTOM-UP:
 */

