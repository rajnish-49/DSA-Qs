class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        
        // ============================================================================
        // PROBLEM STATEMENT & INTUITION
        // ============================================================================
        // Given an array of coin denominations and a target amount, find the MINIMUM
        // number of coins needed to make that amount. If impossible, return -1.
        // 
        // Example: coins = [1,2,5], amount = 11
        // Answer: 3 coins (5+5+1)
        //
        // KEY INSIGHT: This is a classic UNBOUNDED KNAPSACK problem where we can use
        // each coin unlimited times. We need to explore all possible combinations but
        // only care about the one with minimum coins.
        //
        // WHY DYNAMIC PROGRAMMING?
        // -----------------------
        // Observe that to make amount 'x', we need to:
        // 1. Choose a coin 'c'
        // 2. Find minimum coins for (x - c)
        // 3. Add 1 to that count (for the coin we just used)
        // 
        // This creates OVERLAPPING SUBPROBLEMS - we'll compute "minimum coins for 
        // amount k" many times. DP lets us solve each subproblem once and reuse.
        //
        // RECURRENCE RELATION (the heart of the solution):
        // dp[x] = min(dp[x-c1] + 1, dp[x-c2] + 1, ..., dp[x-cn] + 1)
        // where c1, c2, ..., cn are coins ≤ x
        //
        // In plain English: "To make amount x with minimum coins, try using each 
        // coin that fits, see how many coins were needed for the remaining amount,
        // and pick the option that gives the smallest total."
        // ============================================================================
        
        // INF serves as a sentinel value meaning "impossible to make this amount"
        // We use 1e9 instead of INT_MAX to avoid overflow when we do arithmetic
        // (specifically when we compute "1 + dp[x-c]", we don't want overflow)
        int INF = 1e9;
        
        // ============================================================================
        // DP ARRAY INITIALIZATION
        // ============================================================================
        // dp[i] = minimum number of coins needed to make amount 'i'
        // 
        // Size is (amount+1) because we need indices from 0 to amount (inclusive)
        // 
        // WHY initialize everything to INF?
        // By default, we assume every amount is impossible to make. As we iterate,
        // we'll update the values that ARE actually possible. At the end, if 
        // dp[amount] is still INF, we know it's impossible.
        vector<int> dp(amount + 1, INF);
        
        // BASE CASE: Zero coins needed to make amount 0
        // This is our foundation - every other amount builds from this
        // Mathematically: dp[0] = 0 is the base of our recurrence
        dp[0] = 0;
        
        // ============================================================================
        // BOTTOM-UP DP CONSTRUCTION
        // ============================================================================
        // We build solutions from smallest amounts to largest (hence "bottom-up")
        // This ensures when we compute dp[x], all smaller values dp[x-c] are ready
        //
        // LOOP INVARIANT: After processing amount x, dp[x] contains the minimum
        // number of coins needed to make amount x (or INF if impossible)
        for (int x = 1; x <= amount; x++) {
            
            // For current amount 'x', try using each available coin denomination
            // INTUITION: Imagine you're at amount x and asking "which coin should I
            // use last?" Try all options and pick the best.
            for (int c : coins) {
                
                // FEASIBILITY CHECK #1: Can we even use this coin?
                // If coin value > amount needed, it doesn't fit
                // Example: Can't use a $5 coin to make $3
                if (x - c >= 0 && 
                    // FEASIBILITY CHECK #2: Is the remaining amount achievable?
                    // After using coin c, we need (x-c) more. If that's impossible
                    // (still INF), then this path won't work either.
                    // 
                    // WHY this check matters:
                    // Suppose x=6, c=4, and dp[2]=INF (can't make 2 with given coins)
                    // Then using coin 4 here won't help us make 6, even though 4≤6
                    dp[x - c] != INF) {
                    
                    // ================================================================
                    // THE CORE DP TRANSITION
                    // ================================================================
                    // dp[x] = min(current best, coins needed if we use coin 'c')
                    //
                    // Breaking down "1 + dp[x - c]":
                    // - dp[x - c]: minimum coins needed for the remaining amount
                    // - 1: the coin 'c' we're using right now
                    // - Total: if we use coin c, this is how many coins we'd need
                    //
                    // We take min() because we want the MINIMUM across all choices
                    //
                    // EXAMPLE WALKTHROUGH:
                    // coins = [1,2,5], x = 11
                    // When x=11, we try:
                    //   - Use coin 1: dp[11] = min(dp[11], 1 + dp[10])
                    //   - Use coin 2: dp[11] = min(dp[11], 1 + dp[9])
                    //   - Use coin 5: dp[11] = min(dp[11], 1 + dp[6])
                    // The minimum of these three options becomes dp[11]
                    dp[x] = min(dp[x], 1 + dp[x - c]);
                }
            }
            
            // After trying all coins, dp[x] now holds the minimum coins for amount x
            // (or still INF if no combination of coins can make x)
        }
        
        // ============================================================================
        // FINAL ANSWER EXTRACTION
        // ============================================================================
        // Ternary operator checks if target amount was achievable:
        // - If dp[amount] == INF: we never found a way to make this amount → return -1
        // - Otherwise: dp[amount] contains the minimum coins needed → return it
        //
        // WHY we need this check:
        // Example: coins = [2], amount = 3
        // dp[3] will remain INF because we can't make odd amounts with only even coins
        return (dp[amount] == INF) ? -1 : dp[amount];
    }
};

// ================================================================================
// COMPLEXITY ANALYSIS
// ================================================================================
// Time Complexity: O(amount × coins.length)
// - Outer loop: runs 'amount' times
// - Inner loop: runs 'coins.length' times
// - Each iteration does O(1) work
//
// Space Complexity: O(amount)
// - DP array of size (amount + 1)
//
// ================================================================================
// WHY THIS APPROACH WORKS (Proof Sketch)
// ================================================================================
// 1. OPTIMAL SUBSTRUCTURE: If we use coin c for amount x, the remaining problem
//    (amount x-c) must also be solved optimally for the overall solution to be optimal
//
// 2. OVERLAPPING SUBPROBLEMS: Making amount 11 might require knowing the answer
//    for amounts 10, 9, 6, etc. These same subproblems appear in multiple paths.
//
// 3. CORRECTNESS: By processing amounts 0→amount in order, when we compute dp[x],
//    all values dp[0] through dp[x-1] are already correct. Since our recurrence
//    only depends on smaller amounts, we build the correct answer progressively.
//
// ================================================================================


class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        
        // ============================================================================
        // TOP-DOWN MEMOIZATION APPROACH
        // ============================================================================
        // This is the RECURSIVE solution with MEMOIZATION (also called "top-down DP")
        // 
        // FUNDAMENTAL DIFFERENCE from Bottom-Up:
        // - Bottom-Up: Start from base case (0) and build UP to target
        // - Top-Down: Start from target and break DOWN into subproblems
        //
        // WHEN TO USE WHICH?
        // - Top-Down: More intuitive, easier to write initially, only solves needed
        //   subproblems (useful if not all states are reached)
        // - Bottom-Up: Faster in practice (no recursion overhead), easier to optimize
        //   space, more cache-friendly
        //
        // Both have same time/space complexity for this problem.
        // ============================================================================
        
        // Memoization table: memo[i] = minimum coins needed to make amount 'i'
        // Initialize with -1 to indicate "not yet computed"
        // 
        // WHY -1 as sentinel?
        // - We need to distinguish between:
        //   * "Haven't computed this yet" (-1)
        //   * "Computed and impossible" (INF or very large number)
        //   * "Computed and possible" (actual count)
        // - Since minimum coins is always ≥ 0, -1 safely represents "uncomputed"
        vector<int> memo(amount + 1, -1);
        
        // Kick off the recursive solution starting from the target amount
        int result = solve(coins, amount, memo);
        
        // If result is still our "impossible" sentinel, return -1
        // Otherwise return the computed minimum
        return (result >= 1e9) ? -1 : result;
    }
    
private:
    // ============================================================================
    // RECURSIVE HELPER FUNCTION - The Heart of Top-Down DP
    // ============================================================================
    // PURPOSE: Find minimum coins needed to make 'remaining' amount
    //
    // PARAMETERS:
    // - coins: available denominations (constant reference, shared across calls)
    // - remaining: the current amount we need to make
    // - memo: memoization table to avoid recomputation
    //
    // RETURNS: minimum coins needed for 'remaining' amount, or INF if impossible
    //
    // RECURSION TREE INSIGHT:
    // Without memoization, for amount=11 with coins=[1,2,5], we'd have a tree like:
    //                          solve(11)
    //                    /        |        \
    //              solve(10)  solve(9)  solve(6)
    //              /  |  \     /  |  \    /  |  \
    //            ...  ...  ... ... ... ... ... ... ...
    //
    // Many nodes repeat (e.g., solve(6) appears multiple times in different branches)
    // Memoization ensures we compute each unique amount exactly ONCE.
    // ============================================================================
    int solve(const vector<int>& coins, int remaining, vector<int>& memo) {
        
        // ========================================================================
        // BASE CASE #1: Successfully made exact amount
        // ========================================================================
        // If remaining amount is exactly 0, we've found a valid combination
        // It took 0 MORE coins to make 0 (we're done)
        //
        // This is our SUCCESS termination condition
        if (remaining == 0) {
            return 0;
        }
        
        // ========================================================================
        // BASE CASE #2: Overshot the amount (negative remaining)
        // ========================================================================
        // If remaining is negative, we used a coin that was too large
        // This path is INVALID - return INF to signal impossibility
        //
        // Example: trying to make amount=3 with a coin of value 5
        // leads to remaining = 3-5 = -2 (invalid)
        //
        // This is our FAILURE termination condition
        if (remaining < 0) {
            return 1e9; // INF
        }
        
        // ========================================================================
        // MEMOIZATION CHECK: Have we solved this subproblem before?
        // ========================================================================
        // This is THE KEY OPTIMIZATION that makes this approach efficient
        //
        // If memo[remaining] != -1, it means we've already computed the answer
        // for this exact amount in a previous recursive call. Instead of 
        // recomputing the entire subtree, we instantly return the cached result.
        //
        // IMPACT: Without this, time complexity would be exponential O(2^amount)
        //         With this, time complexity reduces to O(amount × coins.length)
        //
        // Think of memo as a "memory" - once we learn something, we remember it
        if (memo[remaining] != -1) {
            return memo[remaining];
        }
        
        // ========================================================================
        // RECURSIVE CASE: Try all possible coins and find the minimum
        // ========================================================================
        // At this point, we have a subproblem we haven't solved yet
        // We need to compute it from scratch by trying all possibilities
        
        // Initialize to INF (assume impossible until proven otherwise)
        // This serves as our "worst case" - any valid solution will be better
        int minCoins = 1e9;
        
        // ========================================================================
        // THE DECISION TREE: Try using each coin type
        // ========================================================================
        // INTUITION: "I need to make 'remaining' amount. Let me try using each
        // coin type and see which choice leads to the minimum total coins."
        //
        // This is the CHOICE aspect of dynamic programming - at each state,
        // we have multiple choices (which coin to use), and we explore all of them
        for (int coin : coins) {
            
            // ====================================================================
            // RECURSIVE LEAP OF FAITH
            // ====================================================================
            // "If I use this coin, I need to solve a SMALLER problem: 
            // making (remaining - coin) amount"
            //
            // solve(remaining - coin) will tell us the minimum coins needed
            // for the reduced amount. We trust (by induction) that it gives
            // the correct answer for smaller subproblems.
            //
            // BREAKDOWN of "1 + solve(...)":
            // - solve(remaining - coin): min coins for remaining amount after
            //   using this coin
            // - 1: represents the coin we're using RIGHT NOW in this decision
            // - Total: complete cost of choosing this particular coin
            //
            // We recursively solve the smaller problem, and the recursion will
            // eventually hit base cases (remaining == 0 or remaining < 0)
            int coinsNeeded = 1 + solve(coins, remaining - coin, memo);
            
            // ====================================================================
            // OPTIMIZATION: Track the best choice
            // ====================================================================
            // After trying this coin, update our best answer if this path is better
            // 
            // This is the OPTIMIZATION aspect of DP - among all choices, we keep
            // only the one that gives minimum coins
            //
            // The min() aggregates results from different choices:
            // minCoins = min(using coin1, using coin2, using coin3, ...)
            minCoins = min(minCoins, coinsNeeded);
        }
        
        // ========================================================================
        // MEMOIZATION: Cache the result before returning
        // ========================================================================
        // We've now explored all possible ways to make 'remaining' amount
        // and found the minimum. Store this in memo so we never recompute it.
        //
        // CRITICAL: We must cache BEFORE returning, not after, because the
        // return statement exits the function immediately
        //
        // Future calls with the same 'remaining' will hit the memo check above
        // and return instantly instead of redoing all this work
        memo[remaining] = minCoins;
        
        // Return the computed minimum for this subproblem
        // This value will be used by whoever called us (either parent recursive
        // call or the initial coinChange function)
        return minCoins;
    }
};

// ================================================================================
// COMPLEXITY ANALYSIS
// ================================================================================
// Time Complexity: O(amount × coins.length)
// - There are (amount + 1) possible unique subproblems (0 to amount)
// - Each subproblem, when computed for the first time, tries all coins
// - Each coin trial does O(1) work plus a recursive call
// - Due to memoization, each subproblem is solved exactly ONCE
// - Total: O(amount) subproblems × O(coins.length) work per subproblem
//
// Space Complexity: O(amount) for memoization + O(amount) for recursion stack
// - Memo array: O(amount)
// - Recursion call stack: In worst case (e.g., coins=[1], amount=N), we go
//   N levels deep: solve(N) → solve(N-1) → ... → solve(1) → solve(0)
// - Total: O(amount)
//
// ================================================================================
// COMPARISON: TOP-DOWN vs BOTTOM-UP
// ================================================================================
//
// SIMILARITIES:
// ✓ Same time complexity: O(amount × coins.length)
// ✓ Same space complexity: O(amount)
// ✓ Same recurrence relation: dp[x] = min(1 + dp[x-c]) for all coins c
// ✓ Both use memoization/tabulation to avoid redundant computation
//
// DIFFERENCES:
// 
// TOP-DOWN (This Solution):
// ✓ More intuitive: naturally follows the problem structure
// ✓ Easier to write: think recursively, add memoization
// ✓ Only computes needed states: if amount=100 but solution uses only 
//   amounts [100,95,90], we don't compute dp[1], dp[2], etc.
// ✗ Recursion overhead: function call stack has small time/space cost
// ✗ Stack overflow risk: very deep recursion (e.g., amount=10^6 with coins=[1])
//   might exceed stack limit
//
// BOTTOM-UP (Iterative DP):
// ✓ No recursion overhead: pure iteration, faster in practice
// ✓ No stack overflow: uses heap memory (vector), not call stack
// ✓ Better cache locality: sequential array access is cache-friendly
// ✗ Computes all states: always fills entire dp array even if not all needed
// ✗ Less intuitive initially: requires thinking "in what order to fill the table?"
//
// ================================================================================
// DEBUGGING TIP: Trace Example
// ================================================================================
// Let's trace coins=[1,2,5], amount=4 to see memoization in action:
//
// Call Stack:                          Memo State After Each Call:
// solve(4)                             
//   ├─ 1+solve(3)                      memo[3] = 2 (computed)
//   │    ├─ 1+solve(2)                 memo[2] = 1 (computed)
//   │    │    ├─ 1+solve(1)            memo[1] = 1 (computed)
//   │    │    │    ├─ 1+solve(0)=1     
//   │    │    │    └─ returns 1
//   │    │    ├─ 1+solve(0)=1          
//   │    │    └─ returns 1             
//   │    ├─ 1+solve(1)=2 (memo hit!)   memo[1] already = 1, reuse!
//   │    └─ returns 2                  
//   ├─ 1+solve(2)=2 (memo hit!)        memo[2] already = 1, reuse!
//   └─ returns 2                       memo[4] = 2
//
// Notice how solve(1) and solve(2) are called multiple times but only 
// COMPUTED once. Subsequent calls return cached values instantly.
// ================================================================================