#include <bits/stdc++.h>
using namespace std;

/*
 * PROBLEM OVERVIEW:
 * A frog wants to jump from stone 0 to stone n-1. At each stone i, it can jump:
 * - To stone (i+1) with cost = |height[i+1] - height[i]|, OR
 * - To stone (i+2) with cost = |height[i+2] - height[i]|
 * Find the minimum total cost to reach the last stone.
 *
 * KEY INSIGHT:
 * This is an optimization DP problem where we need to minimize the total cost.
 * At each stone, we have 2 choices (jump 1 or 2 steps), and we take the minimum.
 *
 * THREE APPROACHES DEMONSTRATED:
 * 1. Memoization (Top-Down): Natural recursive thinking + caching
 * 2. Tabulation (Bottom-Up): Iterative DP with full array
 * 3. Space-Optimized: Same as tabulation but using only O(1) space
 *
 * TIME COMPLEXITY: O(n) for all approaches
 * SPACE COMPLEXITY:
 * - Memoization: O(n) for dp array + O(n) for recursion stack = O(n)
 * - Tabulation: O(n) for dp array
 * - Space-Optimized: O(1) - only two variables!
 */

class Solution
{
public:
    /*--------------------------------------------------
      1. TOP-DOWN DP (MEMOIZATION)

      DEFINITION: f(i) = minimum cost to go from index i to n-1

      WHY TOP-DOWN: We start from the problem we want to solve (index 0)
      and recursively break it down into smaller subproblems, caching results
      to avoid recomputation.

      RECURRENCE: f(i) = min(
          f(i+1) + cost_to_jump_1_step,
          f(i+2) + cost_to_jump_2_steps
      )

      INTUITION: From stone i, we try both possible jumps and take the path
      with minimum total cost.
    --------------------------------------------------*/
    int solveMemo(int i, vector<int> &height, vector<int> &dp)
    {
        int n = height.size();

        // BASE CASE: Already at the destination
        // WHY return 0: No more cost needed when we're at the last stone
        // This is the foundation that stops the recursion
        if (i == n - 1)
            return 0;

        // MEMOIZATION CHECK: Have we already computed this subproblem?
        // WHY: Avoid redundant recursive calls for the same index
        // HOW: dp[i] = -1 means "not computed yet", any other value is cached result
        // This is what makes it DP instead of plain recursion - O(n) instead of O(2^n)
        if (dp[i] != -1)
            return dp[i];

        // CHOICE 1: Jump 1 step forward (i → i+1)
        // COST CALCULATION: Height difference represents the energy/cost needed
        // WHY abs(): Cost is always positive regardless of jumping up or down
        // RECURSIVE CALL: solveMemo(i+1) gives us the minimum cost from the next stone
        // TOTAL COST: current jump cost + future optimal cost
        int one = solveMemo(i + 1, height, dp) + abs(height[i + 1] - height[i]);

        // CHOICE 2: Jump 2 steps forward (i → i+2)
        // Initialize to INT_MAX to handle the case where this jump isn't possible
        int two = INT_MAX;

        // BOUNDARY CHECK: Can we jump 2 steps without going out of bounds?
        // WHY necessary: Prevents array out of bounds and invalid jumps
        if (i + 2 < n)
        {
            two = solveMemo(i + 2, height, dp) + abs(height[i + 2] - height[i]);
        }

        // OPTIMAL SUBSTRUCTURE: Choose the option with minimum cost
        // WHY min(): We want to minimize total cost, so we take the cheaper path
        // MEMOIZATION: Cache this result before returning to avoid recomputation
        dp[i] = min(one, two);
        return dp[i];
    }

    /*--------------------------------------------------
      2. BOTTOM-UP DP (TABULATION)

      DEFINITION: dp[i] = minimum cost to reach index i from index 0

      WHY BOTTOM-UP: We start from the base case (index 0) and iteratively
      build up solutions to larger subproblems until we reach the target (index n-1).

      KEY DIFFERENCE FROM MEMOIZATION:
      - Memoization: "How do I reach n-1 from 0?" (goal → start)
      - Tabulation: "What's the cost to reach each stone from 0?" (start → goal)

      ADVANTAGE: No recursion overhead, easier to optimize space, guaranteed
      to solve all necessary subproblems in the right order.

      RECURRENCE: dp[i] = min(
          dp[i-1] + cost_from_previous_stone,
          dp[i-2] + cost_from_two_stones_back
      )

      INTUITION: To reach stone i, we must have come from either (i-1) or (i-2).
      We choose whichever path gives us minimum total cost.
    --------------------------------------------------*/
    int solveTab(vector<int> &height)
    {
        int n = height.size();
        vector<int> dp(n);

        // BASE CASE: Starting at stone 0 costs nothing (we're already there)
        // WHY 0: No energy/cost spent to be at the starting position
        dp[0] = 0;

        // BASE CASE: To reach stone 1 from stone 0
        // WHY this calculation: Only one way to reach stone 1 - jump from stone 0
        // BOUNDARY CHECK: Ensures we don't access dp[1] if array has only 1 element
        if (n > 1)
            dp[1] = abs(height[1] - height[0]);

        // BUILD UP SOLUTION: Calculate minimum cost to reach each stone
        // WHY start at i=2: Stones 0 and 1 are already initialized (base cases)
        // WHY i < n: Process all stones up to the last one
        // ORDER MATTERS: We process left-to-right because dp[i] depends on dp[i-1] and dp[i-2],
        // which must already be computed. This dependency order is why tabulation works.
        for (int i = 2; i < n; i++)
        {
            // CHOICE 1: Come from stone (i-1) by jumping 1 step
            // TOTAL COST: (cost to reach i-1) + (cost of this jump)
            // CHOICE 2: Come from stone (i-2) by jumping 2 steps
            // TOTAL COST: (cost to reach i-2) + (cost of this jump)
            // WHY we can do this: Both dp[i-1] and dp[i-2] are already computed

            // OPTIMAL SUBSTRUCTURE: Take the path with minimum cost
            // WHY min(): Among all ways to reach stone i, we choose the cheapest
            // This is the essence of optimization DP
            dp[i] = min(
                dp[i - 1] + abs(height[i] - height[i - 1]),
                dp[i - 2] + abs(height[i] - height[i - 2]));
        }

        // ANSWER: Minimum cost to reach the last stone from stone 0
        // WHY dp[n-1]: We've built up the solution stone-by-stone, and dp[n-1]
        // contains the final answer - the minimum cost path from start to end
        return dp[n - 1];
    }

    /*--------------------------------------------------
      3. BOTTOM-UP DP (SPACE OPTIMIZED)

      KEY OPTIMIZATION INSIGHT:
      Notice that in tabulation, dp[i] only depends on dp[i-1] and dp[i-2].
      We never need dp[i-3], dp[i-4], etc. So why store the entire array?

      SPACE OPTIMIZATION: Instead of storing all n values, we only keep track
      of the last 2 values (like a sliding window of size 2).

      WHY THIS WORKS:
      - At step i, we only need: previous value (i-1) and two-steps-back value (i-2)
      - After computing curr (which is dp[i]), we shift: prev2 → prev1, prev1 → curr
      - This "sliding window" gives us O(1) space instead of O(n)

      GENERAL PATTERN: If dp[i] depends on only the last k values,
      you can optimize space from O(n) to O(k).

      TRADE-OFF: Same time complexity O(n), but space reduced from O(n) to O(1)!
      This is the BEST solution for this problem.
    --------------------------------------------------*/
    int solveTabSpaceOpt(vector<int> &height)
    {
        int n = height.size();

        // EDGE CASE: If only 1 stone, we're already there - no cost
        if (n == 1)
            return 0;

        // VARIABLES REPRESENT: The "sliding window" of last 2 DP values
        // prev2 = dp[i-2] = cost to reach stone that's 2 positions back
        // prev1 = dp[i-1] = cost to reach stone that's 1 position back

        // INITIALIZATION: Set up base cases using only 2 variables
        int prev2 = 0;                          // Equivalent to dp[0]: cost to reach stone 0 is 0
        int prev1 = abs(height[1] - height[0]); // Equivalent to dp[1]

        // BUILD UP SOLUTION: Same logic as tabulation, but with only 2 variables
        // WHY this works: At each iteration, we compute curr (which is dp[i]),
        // then "slide the window" by updating prev2 and prev1
        for (int i = 2; i < n; i++)
        {
            // COMPUTE CURRENT: Same recurrence as tabulation
            // curr represents dp[i] - minimum cost to reach current stone i
            int curr = min(
                prev1 + abs(height[i] - height[i - 1]), // From (i-1)
                prev2 + abs(height[i] - height[i - 2])  // From (i-2)
            );

            // SLIDE THE WINDOW: Shift values for next iteration
            // WHY: For next iteration (i+1), current prev1 becomes prev2,
            // and current curr becomes prev1
            // VISUALIZATION: [prev2, prev1, curr] → [prev1, curr, ?]
            prev2 = prev1; // What was (i-1) is now (i-2) relative to next iteration
            prev1 = curr;  // What was (i) is now (i-1) relative to next iteration
        }

        // ANSWER: prev1 holds the final result (equivalent to dp[n-1])
        // WHY prev1: After the loop, prev1 contains the cost to reach the last stone
        return prev1; // dp[n-1]
    }

    /*==================================================
      VARIATION: FROG JUMP WITH K STEPS

      PROBLEM EXTENSION:
      Instead of jumping only 1 or 2 steps, the frog can now jump
      anywhere from 1 to k steps forward. Find minimum cost to reach last stone.

      KEY DIFFERENCE:
      - Original: 2 choices at each stone (jump 1 or 2)
      - K-variation: k choices at each stone (jump 1, 2, 3, ..., k)

      COMPLEXITY IMPACT:
      - Time: O(n) → O(n*k) because we try k options at each of n stones
      - Space: Still O(n) for dp array

      WHY MORE COMPLEX:
      More choices means we need to check more possibilities, but the
      fundamental DP structure remains the same - we still choose the minimum.
    ==================================================*/

    /*--------------------------------------------------
      4. TOP-DOWN DP (MEMOIZATION) - K JUMPS VERSION

      DEFINITION: f(i, k) = minimum cost to go from index i to n-1 with k-jump capability

      RECURRENCE: f(i) = min over all j in [1, k] of:
          f(i+j) + cost_to_jump_j_steps

      WHY LOOP: Instead of checking just 2 options (1 step, 2 steps), we now
      must check k options (1 step, 2 steps, ..., k steps) and take the minimum.

      INTUITION: From stone i, try all possible jump distances up to k,
      recursively compute the cost for each option, and choose the cheapest path.

      TIME: O(n*k) - For each of n stones, we try up to k jumps
      SPACE: O(n) for dp array + O(n) recursion stack
    --------------------------------------------------*/
    int solveMemoK(int i, vector<int> &heights, int k, vector<int> &dp)
    {
        int n = heights.size();

        // BASE CASE: Already at the destination
        // WHY return 0: No more jumps needed when we're at the last stone
        if (i == n - 1)
            return 0;

        // MEMOIZATION CHECK: Have we already solved this subproblem?
        // WHY: Avoid redundant calculations - same optimization as 2-jump version
        // IMPACT: Without this, time would be exponential O(k^n) instead of O(n*k)
        if (dp[i] != -1)
            return dp[i];

        // EXPLORATION: Try all possible jump distances from 1 to k
        // Initialize to INT_MAX because we're looking for minimum
        int ans = INT_MAX;

        // TRY ALL K CHOICES: Iterate through all possible jump lengths
        // WHY j starts at 1: Can't jump 0 steps (stay at same stone)
        // WHY j <= k: k is the maximum allowed jump distance
        for (int j = 1; j <= k; j++)
        {
            // BOUNDARY CHECK: Ensure we don't jump beyond the last stone
            // WHY necessary: i+j must be a valid index (< n)
            if (i + j < n)
            {
                // COST CALCULATION: Current jump cost + future optimal cost from landing stone
                // BREAKDOWN:
                // - abs(heights[i] - heights[i+j]): Cost of jumping from i to i+j
                // - solveMemoK(i+j, ...): Minimum cost to reach n-1 from stone i+j
                // WHY abs(): Height difference is always a positive cost
                int currentCost = abs(heights[i] - heights[i + j]) +
                                  solveMemoK(i + j, heights, k, dp);

                // UPDATE MINIMUM: Keep track of the cheapest option found so far
                // WHY min(): We want the path with minimum total cost among all k choices
                ans = min(ans, currentCost);
            }
        }

        // MEMOIZATION: Cache the result to avoid recomputing this subproblem
        // OPTIMIZATION: This single line converts O(k^n) to O(n*k) complexity
        dp[i] = ans;
        return dp[i];
    }

    /*--------------------------------------------------
      5. BOTTOM-UP DP (TABULATION) - K JUMPS VERSION

      DEFINITION: dp[i] = minimum cost to reach stone i from stone 0 with k-jump capability

      RECURRENCE: dp[i] = min over all j in [1, k] where i-j >= 0 of:
          dp[i-j] + cost_from_stone_(i-j)_to_i

      KEY INSIGHT: To reach stone i, we could have come from any of the previous
      k stones (i-1, i-2, ..., i-k). We check all possibilities and take the minimum.

      WHY TABULATION: Build solution iteratively from left to right, ensuring
      when we compute dp[i], all stones it depends on (i-1, i-2, ..., i-k)
      are already computed.

      ADVANTAGE OVER MEMOIZATION: No recursion overhead, easier to visualize
      the dependency structure.

      TIME: O(n*k) - For each of n stones, check up to k previous stones
      SPACE: O(n) for dp array only (no recursion stack)
    --------------------------------------------------*/
    int solveTabK(vector<int> &heights, int k)
    {
        int n = heights.size();

        // INITIALIZATION: Initialize all positions to "infinity" (INT_MAX)
        // WHY INT_MAX: We're looking for minimum, so initialize with "worst possible" value
        // This ensures any real path cost will be smaller and get selected
        vector<int> dp(n, INT_MAX);

        // BASE CASE: Starting at stone 0 costs nothing
        // WHY 0: We begin at stone 0, so no cost to "reach" it
        dp[0] = 0;

        // BUILD UP SOLUTION: Process each stone from left to right
        // WHY start at 1: Stone 0 is the starting point (already initialized)
        // WHY i < n: Process all stones up to and including the last one
        for (int i = 1; i < n; i++)
        {
            // TRY ALL POSSIBLE PREVIOUS STONES: Check all stones we could have jumped from
            // WHY j from 1 to k: We can jump 1, 2, 3, ..., up to k steps
            // DIRECTION: Looking backward - "which stone did we come from?"
            for (int j = 1; j <= k; j++)
            {
                // BOUNDARY CHECK: Ensure the source stone exists
                // WHY i-j >= 0: Can only come from stones that exist (non-negative index)
                // EXAMPLE: If i=2 and j=5, then i-j=-3 which is invalid
                if (i - j >= 0)
                {
                    // OPTIMAL SUBSTRUCTURE: Update if we found a cheaper path
                    // BREAKDOWN:
                    // - dp[i-j]: Minimum cost to reach stone (i-j)
                    // - abs(heights[i] - heights[i-j]): Cost of jump from (i-j) to i
                    // - Total: Cost to reach (i-j) + cost of this jump
                    // WHY min(): Among all ways to reach stone i (from i-1, i-2, ..., i-k),
                    // keep the cheapest one found so far
                    dp[i] = min(dp[i],
                                dp[i - j] + abs(heights[i] - heights[i - j]));
                }
            }
        }

        // ANSWER: Minimum cost to reach the last stone from stone 0
        // WHY dp[n-1]: After processing all stones, dp[n-1] contains the
        // minimum cost among all possible k-jump paths from start to end
        return dp[n - 1];
    }
};
