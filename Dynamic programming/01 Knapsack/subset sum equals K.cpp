/*
═══════════════════════════════════════════════════════════════════════════════
PROBLEM: SUBSET SUM - Can we select elements from array to sum exactly to target?

CORE INSIGHT: This is a DECISION problem with OVERLAPPING SUBPROBLEMS
- At each element, we face a binary choice: include it or exclude it
- These choices create a tree of possibilities that branches exponentially
- BUT many branches lead to the same (index, remaining_target) state
- Dynamic Programming lets us solve each unique state once and reuse the answer

FUNDAMENTAL RECURRENCE RELATION:
    canMakeSum(idx, target) = canMakeSum(idx+1, target)              [skip element]
                               OR
                               canMakeSum(idx+1, target - arr[idx])  [take element, if possible]

WHY THIS WORKS: If we can make 'target' starting from index 'idx', then EITHER:
    1. We don't need arr[idx] (skip works), OR
    2. We use arr[idx] and need to make (target - arr[idx]) from remaining elements
═══════════════════════════════════════════════════════════════════════════════
*/

// ═══════════════════════════════════════════════════════════════════════════
// APPROACH 1: TOP-DOWN MEMOIZATION (Recursion + Cache)
// ═══════════════════════════════════════════════════════════════════════════

vector<vector<int>> dp;  // Cache: dp[idx][target] = answer for subproblem

bool solve(int idx, int target, vector<int> &arr) {
    
    // ═══ BASE CASE 1: Target achieved ═══
    // When remaining target = 0, we've successfully selected elements that sum to original target
    // This is our SUCCESS condition - doesn't matter what index we're at
    if(target == 0)
        return true;

    // ═══ BASE CASE 2: Array exhausted ═══
    // Reached end of array but target > 0 means we FAILED to find a valid subset
    // No more elements to consider, yet we haven't reached our goal
    if(idx == arr.size())
        return false;

    // ═══ MEMOIZATION CHECK: Avoid recomputation ═══
    // If we've already solved this (idx, target) pair, return cached result
    // This is where DP saves us from exponential time complexity
    // Without this, same states would be computed multiple times across different branches
    if(dp[idx][target] != -1)
        return dp[idx][target];

    // ═══ RECURSIVE CHOICE 1: SKIP current element ═══
    // Explore the possibility of NOT including arr[idx] in our subset
    // Move to next index with same target requirement
    bool skip = solve(idx + 1, target, arr);

    // ═══ RECURSIVE CHOICE 2: TAKE current element (if valid) ═══
    // We can only include arr[idx] if it doesn't exceed our remaining target
    // If we take it, reduce target by arr[idx] and move to next element
    bool take = false;
    if(arr[idx] <= target)  // Guard against invalid states (negative target)
        take = solve(idx + 1, target - arr[idx], arr);

    // ═══ COMBINE RESULTS: We succeed if EITHER choice works ═══
    // Store in cache before returning (memoization step)
    // Using OR because we only need ONE valid path to succeed
    return dp[idx][target] = skip || take;
}

bool subsetSumToK(int n, int k, vector<int> &arr) {
    // Initialize memoization table with -1 (uncomputed state marker)
    // Dimensions: n rows (indices) × (k+1) columns (targets from 0 to k)
    dp.assign(n, vector<int>(k + 1, -1));
    
    return solve(0, k, arr);  // Start from index 0 with full target k
}


// ═══════════════════════════════════════════════════════════════════════════
// APPROACH 2: BOTTOM-UP TABULATION (Iterative DP)
// ═══════════════════════════════════════════════════════════════════════════
/*
KEY DIFFERENCE FROM TOP-DOWN:
- Top-down: Start from problem, recurse down to base cases
- Bottom-up: Start from base cases, build up to final answer
- Bottom-up eliminates recursion overhead and is often more space-efficient

TABLE SEMANTICS:
    dp[i][t] = "Can we make sum 't' using first 'i' elements of array?"
    
    Why (n+1) rows? Row 0 represents "using 0 elements" (base case)
    Why (k+1) cols? Column 0 represents target sum of 0 (always achievable)
*/

bool subsetSumToK(int n, int k, vector<int> &arr) {
    // Table stores boolean results (more memory efficient than int)
    // dp[i][t] answers: "Using arr[0...i-1], can we make sum t?"
    vector<vector<bool>> dp(n + 1, vector<bool>(k + 1, false));
    
    // ═══ BASE CASE: Sum of 0 is always achievable ═══
    // With ANY number of elements (0 to n), we can make sum 0 by selecting nothing
    // This is the "empty subset" which has sum = 0
    for(int i = 0; i <= n; i++) {
        dp[i][0] = true;  // Column 0 represents target = 0
    }
    
    // Note: dp[0][t] for t > 0 remains false (can't make positive sum with 0 elements)
    
    // ═══ FILL TABLE: Build solution bottom-up ═══
    for(int i = 1; i <= n; i++) {  // For each element (1-indexed to match table)
        for(int t = 1; t <= k; t++) {  // For each possible target sum
            
            // ═══ CHOICE 1: Don't include arr[i-1] ═══
            // If we could make sum 't' with first (i-1) elements, 
            // we can still make it with 'i' elements by not using the i-th element
            // This copies the result from the row above (previous element set)
            bool skip = dp[i - 1][t];
            
            // ═══ CHOICE 2: Include arr[i-1] if possible ═══
            // Array is 0-indexed but table row 'i' represents first 'i' elements,
            // so current element is arr[i-1]
            bool take = false;
            if(arr[i - 1] <= t) {  // Only valid if element doesn't exceed target
                // If we take arr[i-1], we need to have made (t - arr[i-1]) from previous elements
                // Look up dp[i-1][t - arr[i-1]] to check if that's possible
                take = dp[i - 1][t - arr[i - 1]];
            }
            
            // ═══ COMBINE: Success if EITHER strategy works ═══
            // We only need ONE valid way to achieve the sum
            dp[i][t] = skip || take;
        }
    }
    
    // ═══ FINAL ANSWER: Can we make sum 'k' using all 'n' elements? ═══
    // dp[n][k] represents using first n elements (entire array) to make sum k
    return dp[n][k];
}

/*
═══════════════════════════════════════════════════════════════════════════════
COMPLEXITY ANALYSIS:

TIME: O(n × k)
    - We fill a table of size (n+1) × (k+1)
    - Each cell takes O(1) to compute
    - Both approaches have same time complexity

SPACE:
    - Top-down: O(n × k) for memoization table + O(n) recursion stack
    - Bottom-up: O(n × k) for DP table only (no recursion stack)

WHY DP WORKS HERE:
    1. Optimal Substructure: Solution to problem depends on solutions to subproblems
    2. Overlapping Subproblems: Same (idx, target) states visited multiple times
    3. Without DP: O(2^n) - try all subsets
    4. With DP: O(n × k) - solve each state once
═══════════════════════════════════════════════════════════════════════════════
*/


/*
═══════════════════════════════════════════════════════════════════════════════
SPACE-OPTIMIZED SUBSET SUM: From O(n × k) to O(k)

CRITICAL INSIGHT: We only need the PREVIOUS ROW to compute the CURRENT ROW
    
In 2D DP table:
    dp[i][t] = dp[i-1][t] || dp[i-1][t - arr[i-1]]
               ↑              ↑
           same row     same row above
           position     (shifted left)

OBSERVATION: Row 'i' only depends on row 'i-1', never on rows i-2, i-3, etc.
    → We're wasting space storing all n rows when we only need 2 rows at any time
    → Better yet: Can we use just 1 row and update it in-place?

THE CHALLENGE: In-place updates create a dependency problem
    
If we update left-to-right:
    t=1: dp[1] = dp[1] || dp[1-x]  ← uses OLD dp[1-x] ✓
    t=2: dp[2] = dp[2] || dp[2-x]  ← but dp[2-x] was ALREADY UPDATED ✗
    
    We'd be using NEW values when we need OLD values!

THE SOLUTION: Process RIGHT-TO-LEFT
    
When computing dp[t], we need dp[t-x] which is to the LEFT
    If we go right-to-left, LEFT values are still OLD (not yet updated for current element)
    This preserves the "previous row" semantics while using only 1D array!

═══════════════════════════════════════════════════════════════════════════════
CONCRETE EXAMPLE: arr = [3, 5, 2], k = 7

INITIAL STATE:
    dp = [T, F, F, F, F, F, F, F]
         0  1  2  3  4  5  6  7
    
    dp[0] = true because sum=0 is always achievable (select nothing)
    All other positions false (haven't processed any elements yet)

───────────────────────────────────────────────────────────────────────────────
ITERATION i=0: Processing arr[0] = 3
───────────────────────────────────────────────────────────────────────────────
x = 3
Loop: t = 7 down to 3 (stop before x=3)

t=7: dp[7] = dp[7] || dp[7-3] = F || dp[4] = F || F = F
     (Can't make 7 yet, and can't make 4 either)

t=6: dp[6] = dp[6] || dp[6-3] = F || dp[3] = F || F = F

t=5: dp[5] = dp[5] || dp[5-3] = F || dp[2] = F || F = F

t=4: dp[4] = dp[4] || dp[4-3] = F || dp[1] = F || F = F

t=3: dp[3] = dp[3] || dp[3-3] = F || dp[0] = F || T = T  ← BREAKTHROUGH!
     (We CAN make 3 by taking element 3, since dp[0]=true means "base is achievable")

After element 3:
    dp = [T, F, F, T, F, F, F, F]
         0  1  2  3  4  5  6  7
    
    Interpretation: Using {3}, we can make sums {0, 3}

───────────────────────────────────────────────────────────────────────────────
ITERATION i=1: Processing arr[1] = 5
───────────────────────────────────────────────────────────────────────────────
x = 5
Loop: t = 7 down to 5

t=7: dp[7] = dp[7] || dp[7-5] = F || dp[2] = F || F = F

t=6: dp[6] = dp[6] || dp[6-5] = F || dp[1] = F || F = F

t=5: dp[5] = dp[5] || dp[5-5] = F || dp[0] = F || T = T  ← NEW!
     (Can make 5 by taking element 5, base case dp[0]=true)

After element 5:
    dp = [T, F, F, T, F, T, F, F]
         0  1  2  3  4  5  6  7

WHY didn't dp[8] update? Because t=8 never runs (t starts at k=7)
WHY didn't dp[3+5=8] update? Same reason - we only care about sums ≤ k

    Interpretation: Using {3, 5}, we can make sums {0, 3, 5, 8}
                    But we only track up to k=7, so: {0, 3, 5}

───────────────────────────────────────────────────────────────────────────────
ITERATION i=2: Processing arr[2] = 2
───────────────────────────────────────────────────────────────────────────────
x = 2
Loop: t = 7 down to 2

t=7: dp[7] = dp[7] || dp[7-2] = F || dp[5] = F || T = T  ← FOUND IT!
     (Can make 7 = 5 + 2, and we know dp[5]=true from previous iterations)

t=6: dp[6] = dp[6] || dp[6-2] = F || dp[4] = F || F = F

t=5: dp[5] = dp[5] || dp[5-2] = T || dp[3] = T || T = T
     (Already true, stays true. Could also be made as 3+2)

t=4: dp[4] = dp[4] || dp[4-2] = F || dp[2] = F || F = F

t=3: dp[3] = dp[3] || dp[3-2] = T || dp[1] = T || F = T
     (Already true, stays true)

t=2: dp[2] = dp[2] || dp[2-2] = F || dp[0] = F || T = T  ← NEW!
     (Can make 2 by taking element 2)

FINAL STATE:
    dp = [T, F, T, T, F, T, F, T]
         0  1  2  3  4  5  6  7
    
    Return dp[7] = TRUE ✓
    
    Achievable sums: {0, 2, 3, 5, 7}
    One valid subset for 7: {5, 2}

═══════════════════════════════════════════════════════════════════════════════
*/

bool subsetSumToK(int n, int k, vector<int> &arr) {
    
    // ═══ 1D DP ARRAY: "Can we make sum t using elements seen so far?" ═══
    // dp[t] = true means: from elements processed till now, we can select a subset summing to t
    // Size (k+1) because we need indices 0 to k inclusive
    vector<bool> dp(k + 1, false);
    
    // ═══ BASE CASE: Empty subset has sum 0 ═══
    // Before processing any elements, the only achievable sum is 0 (select nothing)
    // This is the foundation upon which all other sums are built
    dp[0] = true;
    
    // ═══ PROCESS EACH ELEMENT: Build up achievable sums incrementally ═══
    for(int i = 0; i < n; i++) {
        int x = arr[i];  // Current element we're deciding to include or not
        
        // ═══ WHY RIGHT-TO-LEFT? Preserve "previous iteration" semantics ═══
        /*
            For target t, we compute: dp[t] = dp[t] || dp[t-x]
                                              ↑         ↑
                                          don't take   take x
                                          (old value)  (need old value of dp[t-x])
            
            If we went left-to-right:
                - When computing dp[t], dp[t-x] might already be updated
                - We'd be using "current iteration" value instead of "previous iteration"
                - This could make the SAME element contribute MULTIPLE times
                
            Example of what goes wrong with left-to-right:
                arr = [2], k = 4
                Initial: dp = [T, F, F, F, F]
                
                Left-to-right with x=2:
                    t=2: dp[2] = dp[2] || dp[0] = F || T = T  ✓
                    t=4: dp[4] = dp[4] || dp[2] = F || T = T  ✗ WRONG!
                         (We just set dp[2]=true using element 2,
                          now we're using it again to set dp[4]=true,
                          effectively using element 2 TWICE to make 4)
                
            Right-to-left with x=2:
                    t=4: dp[4] = dp[4] || dp[2] = F || F = F  ✓ (correct, can't make 4)
                    t=2: dp[2] = dp[2] || dp[0] = F || T = T  ✓
        */
        
        for(int t = k; t >= x; t--) {
            
            // ═══ RECURRENCE RELATION (compressed from 2D to 1D) ═══
            /*
                2D version was: dp[i][t] = dp[i-1][t] || dp[i-1][t-x]
                                           ↑             ↑
                                    previous row   previous row
                
                1D version is:  dp[t] = dp[t] || dp[t-x]
                                        ↑         ↑
                                   old value  old value (because we go right-to-left)
                
                dp[t]: "Can we make sum t WITHOUT using current element x?"
                       (This is the OLD value, represents previous iteration)
                
                dp[t-x]: "Can we make sum (t-x) from previous elements?"
                         If YES, then adding x gives us sum t
                         (Also OLD value because t-x < t, and we process right-to-left)
            */
            
            dp[t] = dp[t] || dp[t - x];
            //      ↑         ↑
            //   current    if we can make (t-x), then we can make t by adding x
            //   status     
        }
        
        // ═══ WHY LOOP STOPS AT t >= x? ═══
        // For targets smaller than x, we CAN'T use element x (would make sum negative)
        // Also, dp[t-x] would access invalid index when t < x
        // Those smaller targets keep their OLD values (which is correct - element x doesn't help them)
    }
    
    // ═══ FINAL ANSWER: Can we make target sum k? ═══
    // dp[k] now represents: "Using any subset of all n elements, can we make sum k?"
    return dp[k];
}

/*
═══════════════════════════════════════════════════════════════════════════════
COMPLEXITY ANALYSIS:

TIME: O(n × k)
    - Outer loop: n iterations (one per element)
    - Inner loop: at most k iterations per element
    - Same as 2D version

SPACE: O(k)
    - Only one 1D array of size (k+1)
    - HUGE improvement from O(n × k) when k is small and n is large
    
TRADE-OFF:
    ✓ Massively reduced space
    ✗ Can't reconstruct WHICH elements form the subset (lost the path)
    ✗ Slightly harder to understand than 2D version
    
WHEN TO USE THIS:
    - Only need YES/NO answer (not the actual subset)
    - Space is a constraint
    - k is reasonably small (array doesn't explode memory)

KEY TAKEAWAY:
    Right-to-left traversal is the MAGIC that makes 1D DP work
    It ensures we use "previous iteration" values when computing "current iteration"
    This is a common pattern in space-optimized DP problems
═══════════════════════════════════════════════════════════════════════════════
*/