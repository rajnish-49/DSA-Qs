/*
 * PROBLEM OVERVIEW: HOUSE ROBBER
 *
 * SCENARIO:
 * You are a professional robber planning to rob houses along a street. Each house
 * has a certain amount of money stashed. The security system is such that if two
 * adjacent houses are broken into on the same night, the police will be alerted.
 *
 * GOAL: Find the maximum amount of money you can rob tonight without alerting police.
 *
 * CONSTRAINT: Cannot rob two adjacent houses
 *
 * KEY INSIGHT:
 * This is a classic "decision DP" problem where at each position you have two choices:
 * 1. ROB this house: Get its money, but skip the next house (can't rob adjacent)
 * 2. SKIP this house: Get 0 money from it, but you can consider the next house
 *
 * The optimal answer is the maximum of these two choices at each step.
 *
 * WHY DP WORKS HERE:
 * - OVERLAPPING SUBPROBLEMS: The decision at house i depends on optimal solutions
 *   for houses i+1, i+2, etc. Same subproblems are solved multiple times.
 * - OPTIMAL SUBSTRUCTURE: The optimal solution for n houses can be built from
 *   optimal solutions of smaller subproblems (n-1 houses, n-2 houses, etc.)
 *
 * THREE APPROACHES DEMONSTRATED:
 * 1. Memoization (Top-Down): O(n) time, O(n) space (+ recursion stack)
 * 2. Tabulation (Bottom-Up): O(n) time, O(n) space
 * 3. Space-Optimized: O(n) time, O(1) space ✓ BEST
 *
 * REAL-WORLD ANALOGY:
 * Think of it like scheduling tasks where consecutive tasks cannot both be done.
 * You want to maximize profit by choosing the best non-consecutive subset.
 */

/*--------------------------------------------------
  1. TOP-DOWN DP (MEMOIZATION)

  STATE DEFINITION:
  robMemoHelper(index, nums, dp) = Maximum money that can be robbed from house
                                    'index' to the last house (n-1)

  RECURRENCE RELATION:
  dp[index] = max(
      nums[index] + dp[index+2],  // ROB current, skip next, continue from index+2
      dp[index+1]                 // SKIP current, try next house
  )

  WHY THIS RECURRENCE:
  - If we rob house 'index', we get nums[index] money BUT we must skip index+1
    (adjacency constraint), so we can only rob from index+2 onwards
  - If we skip house 'index', we get 0 from it, but we're free to consider index+1
  - We take the maximum of these two strategies

  BASE CASE:
  If index >= n, return 0 (no houses left to rob)

  WHY MEMOIZATION:
  Without memoization, the recursion tree would explore the same house indices
  multiple times, leading to O(2^n) time complexity. With memoization, each
  house index is computed exactly once, reducing it to O(n).

  EXAMPLE TRACE (nums = [2, 7, 9, 3, 1]):
  rob(0) = max(2 + rob(2), rob(1))
         = max(2 + 12, 10) = 12
  rob(2) = max(9 + rob(4), rob(3))
         = max(9 + 1, 3) = 10
  ...and so on

  TIME: O(n) - Each index computed once due to memoization
  SPACE: O(n) for dp array + O(n) for recursion call stack = O(n)
--------------------------------------------------*/
int robMemoHelper(int index, vector<int> &nums, vector<int> &dp)
{

    int n = nums.size();

    // BASE CASE: Out of bounds - no more houses to rob
    // WHY >= n: If current index is at or beyond array size, there's nothing to rob
    // WHY return 0: Contributes zero money to the total
    // IMPORTANCE: This terminates the recursion and prevents array out-of-bounds errors
    if (index >= n)
    {
        return 0;
    }

    // MEMOIZATION CHECK: Have we already solved this subproblem?
    // WHY check dp[index] != -1: We initialize dp with -1 to mark "not computed"
    // HOW IT HELPS: If we've already computed the answer for this index, just return it
    // MASSIVE OPTIMIZATION: This single check converts exponential O(2^n) to linear O(n)
    // INTUITION: "If I've already figured out the best strategy from house 'index' onwards,
    // why recompute it? Just reuse the cached answer!"
    if (dp[index] != -1)
    {
        return dp[index];
    }

    // DECISION 1: ROB the current house
    // IMMEDIATE GAIN: nums[index] money from this house
    // CONSTRAINT CONSEQUENCE: Must skip next house (index+1) due to adjacency rule
    // RECURSIVE STEP: Continue robbing optimally from house index+2 onwards
    // WHY index+2: That's the nearest non-adjacent house we can rob next
    // TOTAL VALUE: Current house money + optimal money from non-adjacent houses
    int pickCurrent = nums[index] + robMemoHelper(index + 2, nums, dp);

    // DECISION 2: SKIP the current house (don't rob it)
    // IMMEDIATE GAIN: 0 money from this house
    // BENEFIT: Flexibility to consider the adjacent house (index+1)
    // RECURSIVE STEP: Continue robbing optimally from house index+1 onwards
    // WHY index+1: Since we didn't rob current, no constraint prevents us from trying next
    // TOTAL VALUE: Just the optimal money from remaining houses
    int skipCurrent = robMemoHelper(index + 1, nums, dp);

    // OPTIMAL DECISION: Choose the strategy that maximizes total money
    // WHY max(): We want to maximize our loot, so pick the better option
    // RESULT: dp[index] now stores the optimal solution for "house index to end"
    // MEMOIZATION: Cache this result so we never recompute it
    // CRITICAL: This is where we apply the "optimal substructure" property of DP
    dp[index] = max(pickCurrent, skipCurrent);

    return dp[index];
}

/*--------------------------------------------------
  2. BOTTOM-UP DP (TABULATION)

  STATE DEFINITION:
  dp[i] = Maximum money that can be robbed from houses 0 to i (inclusive)

  RECURRENCE RELATION:
  dp[i] = max(
      nums[i] + dp[i-2],  // ROB house i, add best from non-adjacent (i-2)
      dp[i-1]             // SKIP house i, take best from previous house
  )

  KEY DIFFERENCE FROM MEMOIZATION:
  - Memoization asks: "From house i to end, what's the max?" (forward-looking)
  - Tabulation asks: "From start to house i, what's the max?" (backward-looking)

  WHY BOTTOM-UP:
  We start from the smallest subproblem (house 0, then houses 0-1) and build up
  to the full problem (houses 0 to n-1). This ensures when we compute dp[i],
  all dependencies (dp[i-1], dp[i-2]) are already computed.

  BASE CASES:
  - dp[0] = nums[0]: Only one house, rob it
  - dp[1] = max(nums[0], nums[1]): Two houses, rob the richer one (can't rob both)

  ITERATION PATTERN:
  For each house i from 2 to n-1:
    - Consider robbing it: nums[i] + dp[i-2]
    - Consider skipping it: dp[i-1]
    - Take the maximum

  EXAMPLE (nums = [2, 7, 9, 3, 1]):
  dp[0] = 2
  dp[1] = max(2, 7) = 7
  dp[2] = max(9+2, 7) = 11
  dp[3] = max(3+7, 11) = 11
  dp[4] = max(1+11, 11) = 12  ← Answer

  ADVANTAGE OVER MEMOIZATION:
  - No recursion overhead (stack frames)
  - Easier to visualize the computation flow
  - Slightly better constant factors in practice

  TIME: O(n) - Single pass through all houses
  SPACE: O(n) - dp array stores result for each house
--------------------------------------------------*/
int robTabulation(vector<int> &nums)
{

    int n = nums.size();

    // EDGE CASE: Only one house exists
    // WHY special handling: With 1 house, no adjacency constraint applies
    // DECISION: Just rob it and return its money
    // PREVENTS: Out-of-bounds access when setting dp[1]
    if (n == 1)
    {
        return nums[0];
    }

    // DP ARRAY: dp[i] = max money from houses 0 to i
    // WHY size n: We need to store the solution for each house from 0 to n-1
    // INTERPRETATION: dp[i] answers "What's the best I can do with houses 0...i?"
    vector<int> dp(n);

    // BASE CASE 1: Only considering house 0
    // WHY nums[0]: With only one house available, the best strategy is to rob it
    // FOUNDATION: This is the starting point for building up the solution
    dp[0] = nums[0];

    // BASE CASE 2: Considering houses 0 and 1
    // WHY max(nums[0], nums[1]): We can rob EITHER house 0 OR house 1, but NOT both
    // REASON: They are adjacent, so robbing both would trigger the alarm
    // DECISION: Pick whichever house has more money
    // CRITICAL: This properly handles the adjacency constraint for the first two houses
    dp[1] = max(nums[0], nums[1]);

    // BUILD UP SOLUTION: Process each house from index 2 to n-1
    // WHY start at 2: Houses 0 and 1 are already handled (base cases)
    // WHY this order: dp[i] depends on dp[i-1] and dp[i-2], which must exist
    // GUARANTEE: By processing left-to-right, all dependencies are satisfied before use
    for (int i = 2; i < n; i++)
    {

        // OPTION 1: ROB the current house i
        // GAIN: nums[i] money from house i
        // CONSTRAINT: Can't use dp[i-1] (adjacent house) due to alarm system
        // SOLUTION: Add dp[i-2] which represents the best we could do up to house i-2
        // WHY dp[i-2]: It's the last non-adjacent house, giving us the best valid combination
        // INTERPRETATION: "If I rob house i, the best I could have done before was at i-2"
        int pickCurrent = nums[i] + dp[i - 2];

        // OPTION 2: SKIP the current house i (don't rob it)
        // GAIN: 0 money from house i itself
        // BENEFIT: We can use the best solution that included house i-1
        // WHY dp[i-1]: It represents the optimal strategy for houses 0 to i-1
        // INTERPRETATION: "If I skip house i, my total is whatever I optimally got up to i-1"
        int skipCurrent = dp[i - 1];

        // OPTIMAL DECISION: Choose the strategy that maximizes money
        // WHY max(): We want to maximize our total loot
        // COMPARISON: "Is it better to rob house i (and add dp[i-2]), or skip it (and keep dp[i-1])?"
        // RESULT: dp[i] now contains the optimal solution for houses 0 to i
        // PRINCIPLE: This embodies the "optimal substructure" - optimal solution for i
        // is built from optimal solutions of smaller subproblems (i-1 and i-2)
        dp[i] = max(pickCurrent, skipCurrent);
    }

    // FINAL ANSWER: Maximum money from all houses (0 to n-1)
    // WHY dp[n-1]: After processing all houses, dp[n-1] contains the optimal strategy
    // for the entire street while respecting the adjacency constraint throughout
    // INTERPRETATION: "The best I can do considering all houses from start to end"
    return dp[n - 1];
}

/*--------------------------------------------------
  3. BOTTOM-UP DP (SPACE OPTIMIZED)

  KEY OBSERVATION:
  In the tabulation approach, notice that dp[i] only depends on:
  - dp[i-1] (previous house)
  - dp[i-2] (two houses back)

  We NEVER need dp[i-3], dp[i-4], etc. once we've moved past them!

  SPACE OPTIMIZATION IDEA:
  Instead of maintaining an entire array of n elements, we can use just TWO variables:
  - prev1: Represents dp[i-1] (best solution up to previous house)
  - prev2: Represents dp[i-2] (best solution up to two houses back)

  HOW IT WORKS:
  As we iterate through houses, we compute the current best, then "slide the window":
  - prev2 becomes what prev1 was (shift back)
  - prev1 becomes current best (shift back)
  - Ready for next iteration!

  ANALOGY:
  Like a sliding window of size 2 moving through the array, keeping track of only
  the last two values instead of the entire history.

  WHY THIS IS BRILLIANT:
  - Same time complexity: O(n)
  - Space improved: O(n) → O(1)
  - No loss of correctness, just being memory-efficient

  TRADE-OFF:
  We can't reconstruct which houses we robbed (no full dp array), but we only
  need the maximum money value, so this is perfect!

  THIS IS THE BEST SOLUTION for this problem in production code.

  TIME: O(n) - Still iterate through all houses once
  SPACE: O(1) - Only two variables regardless of input size!
--------------------------------------------------*/
int robSpaceOptimized(vector<int> &nums)
{

    int n = nums.size();

    // EDGE CASE: Only one house exists
    // WHY needed: Prevents issues with prev1 initialization
    // SOLUTION: Just rob the single house
    if (n == 1)
    {
        return nums[0];
    }

    // SPACE-OPTIMIZED VARIABLES: Only track last 2 values instead of full array
    // prev2 = "Best money up to 2 houses back" (equivalent to dp[i-2])
    // prev1 = "Best money up to 1 house back" (equivalent to dp[i-1])

    // INITIALIZATION (Base cases):
    // prev2 represents dp[0]: Only house 0 considered, so rob it
    int prev2 = nums[0]; // dp[0]

    // prev1 represents dp[1]: Houses 0 and 1 considered, rob the richer one
    // WHY max(): Can rob either house 0 or 1, but not both (adjacent)
    int prev1 = max(nums[0], nums[1]); // dp[1]

    // ITERATE through remaining houses (from index 2 to n-1)
    // WHY start at 2: Houses 0 and 1 are already handled by prev2 and prev1
    // PROCESS: For each house, compute best strategy and update our sliding window
    for (int i = 2; i < n; i++)
    {

        // OPTION 1: ROB current house i
        // CALCULATION: House i's money + best solution from non-adjacent houses
        // WHY prev2: It holds dp[i-2], the best we could do up to 2 houses ago
        // CONSTRAINT: Can't use prev1 (represents i-1, which is adjacent)
        int pickCurrent = nums[i] + prev2;

        // OPTION 2: SKIP current house i
        // CALCULATION: Best solution up to previous house (since we're not robbing current)
        // WHY prev1: It holds dp[i-1], the optimal solution for houses 0 to i-1
        int skipCurrent = prev1;

        // COMPUTE CURRENT BEST: Maximum of robbing or skipping current house
        // WHY max(): We want to maximize total money
        // INTERPRETATION: currentBest is equivalent to dp[i] in tabulation approach
        int currentBest = max(pickCurrent, skipCurrent);

        // SLIDE THE WINDOW: Update variables for next iteration
        // VISUALIZATION:
        // Before: [prev2, prev1, currentBest]
        // After:  [prev1, currentBest, ???]  (ready for next iteration)

        // prev2 shifts forward: what was "1 back" becomes "2 back"
        prev2 = prev1;

        // prev1 shifts forward: current solution becomes "1 back" for next iteration
        prev1 = currentBest;

        // WHY this works: Next iteration will need best of i-1 and i-2, which are
        // now stored in prev1 and prev2 respectively
    }

    // FINAL ANSWER: Maximum money from all houses
    // WHY prev1: After the loop, prev1 holds the best solution for all n houses
    // EQUIVALENT TO: dp[n-1] in the tabulation approach
    // INTERPRETATION: "The optimal strategy for the entire street"
    return prev1;
}
