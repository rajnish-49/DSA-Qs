/*
 * PROBLEM OVERVIEW: HOUSE ROBBER II
 *
 * SCENARIO:
 * You are a professional robber planning to rob houses along a street. Each house
 * has a certain amount of money stashed. ALL HOUSES ARE ARRANGED IN A CIRCLE, meaning
 * the first house and the last house are also adjacent to each other.
 *
 * GOAL: Find the maximum amount of money you can rob tonight without alerting police.
 *
 * CONSTRAINT: Cannot rob two adjacent houses (including first and last!)
 *
 * KEY INSIGHT - THE CIRCULAR TWIST:
 * This is an extension of House Robber I, but with houses in a circle:
 * - In House Robber I: Houses are in a line → [1, 2, 3, 4, 5]
 * - In House Robber II: Houses are in a circle → First and Last are neighbors!
 *
 *     Visual Representation:
 *
 *     LINE (Robber I):     1 → 2 → 3 → 4 → 5
 *
 *     CIRCLE (Robber II):      1 ← → 5
 *                             ↙       ↘
 *                            2         4
 *                             ↘       ↙
 *                                3
 *
 * WHY THE CIRCLE MATTERS:
 * If we rob house 0 (first), we CANNOT rob house n-1 (last), and vice versa.
 * This creates a dependency between the first and last elements that doesn't
 * exist in the linear version.
 *
 * BRILLIANT SOLUTION STRATEGY:
 * Since house 0 and house n-1 cannot BOTH be robbed, we can break the circle
 * by considering two separate linear problems:
 *
 * CASE 1: Exclude house 0 → Solve House Robber I on houses [1, 2, 3, ..., n-1]
 *         (We might or might not rob house n-1, but definitely not house 0)
 *
 * CASE 2: Exclude house n-1 → Solve House Robber I on houses [0, 1, 2, ..., n-2]
 *         (We might or might not rob house 0, but definitely not house n-1)
 *
 * FINAL ANSWER: max(Case 1, Case 2)
 *
 * WHY THIS COVERS ALL POSSIBILITIES:
 * - If optimal solution robs house 0: It's covered in Case 2 (house n-1 excluded)
 * - If optimal solution robs house n-1: It's covered in Case 1 (house 0 excluded)
 * - If optimal solution robs neither: It's covered in BOTH cases!
 *
 * REDUCING TO A SOLVED PROBLEM:
 * We've converted the "circular" problem into two "linear" subproblems, each of
 * which is exactly the original House Robber I problem! This is a powerful
 * problem-solving technique: reduce a new problem to a known solved problem.
 *
 * THREE APPROACHES DEMONSTRATED:
 * 1. Memoization (Top-Down): O(n) time, O(n) space (+ recursion stack)
 * 2. Tabulation (Bottom-Up): O(n) time, O(n) space
 * 3. Space-Optimized: O(n) time, O(1) space ✓ BEST
 *
 * Each approach is used to solve House Robber I on a given range, then we
 * apply the two-case strategy described above.
 */

class Solution {
public:
    /*--------------------------------------------------
      1. TOP-DOWN DP (MEMOIZATION) HELPER

      STATE DEFINITION:
      solveMemo(index, end, nums, dp) = Maximum money that can be robbed from
                                         house 'index' to house 'end' (inclusive)

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
      If index > end, return 0 (no houses left in our allowed range)

      WHY 'end' PARAMETER:
      Unlike House Robber I where we always go to n-1, here we need to solve
      the problem on different ranges: [1...n-1] or [0...n-2]. The 'end' parameter
      allows us to specify where to stop.

      TIME: O(n) - Each index computed once due to memoization
      SPACE: O(n) for dp array + O(n) for recursion call stack = O(n)
    --------------------------------------------------*/
    int solveMemo(int index, int end,
                  vector<int>& nums,
                  vector<int>& dp) {

        // BASE CASE: Out of valid range - no more houses to rob
        // WHY > end (not >= n): We're limited to a specific range [start...end]
        // WHY return 0: No houses means zero contribution to total loot
        // IMPORTANCE: Terminates recursion and prevents going into excluded houses
        if (index > end) {
            return 0;
        }

        // MEMOIZATION CHECK: Have we already solved this subproblem?
        // WHY check dp[index] != -1: We initialize dp with -1 to mark "not computed"
        // HOW IT HELPS: If we've already computed the answer for this index, just return it
        // MASSIVE OPTIMIZATION: This single check converts exponential O(2^n) to linear O(n)
        // INTUITION: "If I've already figured out the best strategy from house 'index' onwards,
        // why recompute it? Just reuse the cached answer!"
        if (dp[index] != -1) {
            return dp[index];
        }

        // DECISION 1: ROB the current house
        // IMMEDIATE GAIN: nums[index] money from this house
        // CONSTRAINT CONSEQUENCE: Must skip next house (index+1) due to adjacency rule
        // RECURSIVE STEP: Continue robbing optimally from house index+2 onwards
        // WHY index+2: That's the nearest non-adjacent house we can rob next
        // TOTAL VALUE: Current house money + optimal money from non-adjacent houses
        int pick = nums[index] + solveMemo(index + 2, end, nums, dp);

        // DECISION 2: SKIP the current house (don't rob it)
        // IMMEDIATE GAIN: 0 money from this house
        // BENEFIT: Flexibility to consider the adjacent house (index+1)
        // RECURSIVE STEP: Continue robbing optimally from house index+1 onwards
        // WHY index+1: Since we didn't rob current, no constraint prevents us from trying next
        // TOTAL VALUE: Just the optimal money from remaining houses
        int notPick = solveMemo(index + 1, end, nums, dp);

        // OPTIMAL DECISION: Choose the strategy that maximizes total money
        // WHY max(): We want to maximize our loot, so pick the better option
        // RESULT: dp[index] now stores the optimal solution for "house index to end"
        // MEMOIZATION: Cache this result so we never recompute it
        // CRITICAL: This is where we apply the "optimal substructure" property of DP
        dp[index] = max(pick, notPick);
        return dp[index];
    }

    /*--------------------------------------------------
      2. BOTTOM-UP DP (TABULATION) HELPER

      STATE DEFINITION:
      dp[i] = Maximum money that can be robbed from houses 'start' to 'start+i' (inclusive)
      Note: dp[i] is relative to the 'start' position

      RECURRENCE RELATION:
      dp[i] = max(
          nums[start+i] + dp[i-2],  // ROB house at (start+i), add best from non-adjacent
          dp[i-1]                   // SKIP house at (start+i), take best from previous
      )

      WHY RANGE PARAMETERS [start...end]:
      For House Robber II, we need to solve the subproblem on specific ranges:
      - Range [1...n-1] excludes the first house
      - Range [0...n-2] excludes the last house
      The start and end parameters let us handle these different ranges.

      BASE CASES:
      - dp[0] = nums[start]: Only one house in range, rob it
      - dp[1] = max(nums[start], nums[start+1]): Two houses, rob the richer one

      ITERATION PATTERN:
      For each position i from 2 to len-1 (where len = end - start + 1):
        - Consider robbing it: nums[start+i] + dp[i-2]
        - Consider skipping it: dp[i-1]
        - Take the maximum

      TIME: O(n) - Single pass through all houses in range
      SPACE: O(n) - dp array stores result for each position
    --------------------------------------------------*/
    int solveTabulation(int start, int end,
                        vector<int>& nums) {

        // RANGE SIZE: Number of houses in our subproblem
        // WHY calculate len: Determines size of dp array and loop bounds
        // EXAMPLE: If start=1, end=4, len=4 means houses at indices 1,2,3,4
        int len = end - start + 1;

        // EDGE CASE: Only one house in the range
        // WHY special handling: With 1 house, no adjacency constraint applies
        // DECISION: Just rob it and return its money
        // PREVENTS: Out-of-bounds access when setting dp[1]
        if (len == 1) {
            return nums[start];
        }

        // DP ARRAY: dp[i] = max money from position 0 to i within our range
        // WHY size len: We need to store the solution for each position in range
        // INTERPRETATION: dp[i] answers "What's the best I can do with positions 0...i?"
        // NOTE: dp index is relative (0 to len-1), maps to actual indices (start to end)
        vector<int> dp(len);

        // BASE CASE 1: Only considering first house in range (position 0)
        // WHY nums[start]: With only one house available, the best strategy is to rob it
        // MAPPING: dp[0] corresponds to actual index 'start' in nums
        // FOUNDATION: This is the starting point for building up the solution
        dp[0] = nums[start];

        // BASE CASE 2: Considering first two houses in range (positions 0 and 1)
        // WHY max(): We can rob EITHER position 0 OR position 1, but NOT both
        // REASON: They are adjacent, so robbing both would trigger the alarm
        // MAPPING: dp[1] considers actual indices 'start' and 'start+1' in nums
        // CRITICAL: This properly handles the adjacency constraint for the first two houses
        dp[1] = max(nums[start], nums[start + 1]);

        // BUILD UP SOLUTION: Process each position from 2 to len-1
        // WHY start at 2: Positions 0 and 1 are already handled (base cases)
        // WHY this order: dp[i] depends on dp[i-1] and dp[i-2], which must exist
        // GUARANTEE: By processing left-to-right, all dependencies are satisfied before use
        for (int i = 2; i < len; i++) {

            // OPTION 1: ROB the current house at actual index (start + i)
            // GAIN: nums[start + i] money from this house
            // CONSTRAINT: Can't use dp[i-1] (adjacent position) due to alarm system
            // SOLUTION: Add dp[i-2] which represents the best we could do up to position i-2
            // WHY dp[i-2]: It's the last non-adjacent position, giving us the best valid combination
            int pick = nums[start + i] + dp[i - 2];

            // OPTION 2: SKIP the current house (don't rob it)
            // GAIN: 0 money from this house itself
            // BENEFIT: We can use the best solution that included position i-1
            // WHY dp[i-1]: It represents the optimal strategy for positions 0 to i-1
            int notPick = dp[i - 1];

            // OPTIMAL DECISION: Choose the strategy that maximizes money
            // WHY max(): We want to maximize total loot
            // COMPARISON: "Is it better to rob position i (and add dp[i-2]), or skip it (keep dp[i-1])?"
            // RESULT: dp[i] now contains the optimal solution for positions 0 to i
            dp[i] = max(pick, notPick);
        }

        // FINAL ANSWER: Maximum money from all houses in our range
        // WHY dp[len-1]: After processing all positions, this contains the optimal strategy
        // for the entire range while respecting the adjacency constraint throughout
        return dp[len - 1];
    }

    /*--------------------------------------------------
      3. BOTTOM-UP DP (SPACE OPTIMIZED) HELPER

      KEY OBSERVATION:
      In the tabulation approach, notice that dp[i] only depends on:
      - dp[i-1] (previous position)
      - dp[i-2] (two positions back)

      We NEVER need dp[i-3], dp[i-4], etc. once we've moved past them!

      SPACE OPTIMIZATION IDEA:
      Instead of maintaining an entire array of len elements, we can use just TWO variables:
      - prev1: Represents dp[i-1] (best solution up to previous position)
      - prev2: Represents dp[i-2] (best solution up to two positions back)

      HOW IT WORKS:
      As we iterate through positions, we compute the current best, then "slide the window":
      - prev2 becomes what prev1 was (shift back)
      - prev1 becomes current best (shift back)
      - Ready for next iteration!

      ANALOGY:
      Like a sliding window of size 2 moving through the range, keeping track of only
      the last two values instead of the entire history.

      WHY THIS IS BRILLIANT:
      - Same time complexity: O(n)
      - Space improved: O(n) → O(1)
      - No loss of correctness, just being memory-efficient

      THIS IS THE BEST SOLUTION for this problem in production code.

      TIME: O(n) - Still iterate through all positions once
      SPACE: O(1) - Only two variables regardless of input size!
    --------------------------------------------------*/
    int solveSpaceOptimized(int start, int end,
                            vector<int>& nums) {

        // RANGE SIZE: Number of houses in our subproblem
        int len = end - start + 1;

        // EDGE CASE: Only one house in the range
        // WHY needed: Prevents issues with prev1 initialization
        // SOLUTION: Just rob the single house
        if (len == 1) {
            return nums[start];
        }

        // SPACE-OPTIMIZED VARIABLES: Only track last 2 values instead of full array
        // prev2 = "Best money up to 2 positions back" (equivalent to dp[i-2])
        // prev1 = "Best money up to 1 position back" (equivalent to dp[i-1])

        // INITIALIZATION (Base cases):
        // prev2 represents dp[0]: Only first house in range considered, so rob it
        int prev2 = nums[start];                       // dp[0]

        // prev1 represents dp[1]: First two houses in range considered, rob the richer one
        // WHY max(): Can rob either position 0 or 1, but not both (adjacent)
        int prev1 = max(nums[start], nums[start + 1]); // dp[1]

        // ITERATE through remaining positions (from index 2 to len-1)
        // WHY start at 2: Positions 0 and 1 are already handled by prev2 and prev1
        // PROCESS: For each position, compute best strategy and update our sliding window
        for (int i = 2; i < len; i++) {

            // OPTION 1: ROB current house at actual index (start + i)
            // CALCULATION: House's money + best solution from non-adjacent positions
            // WHY prev2: It holds dp[i-2], the best we could do up to 2 positions ago
            // CONSTRAINT: Can't use prev1 (represents i-1, which is adjacent)
            int pick = nums[start + i] + prev2;

            // OPTION 2: SKIP current house (don't rob it)
            // CALCULATION: Best solution up to previous position (since we're not robbing current)
            // WHY prev1: It holds dp[i-1], the optimal solution for positions 0 to i-1
            int notPick = prev1;

            // COMPUTE CURRENT BEST: Maximum of robbing or skipping current house
            // WHY max(): We want to maximize total money
            // INTERPRETATION: curr is equivalent to dp[i] in tabulation approach
            int curr = max(pick, notPick);

            // SLIDE THE WINDOW: Update variables for next iteration
            // VISUALIZATION:
            // Before: [prev2, prev1, curr]
            // After:  [prev1, curr, ???]  (ready for next iteration)

            // prev2 shifts forward: what was "1 back" becomes "2 back"
            prev2 = prev1;

            // prev1 shifts forward: current solution becomes "1 back" for next iteration
            prev1 = curr;

            // WHY this works: Next iteration will need best of i-1 and i-2, which are
            // now stored in prev1 and prev2 respectively
        }

        // FINAL ANSWER: Maximum money from all houses in our range
        // WHY prev1: After the loop, prev1 holds the best solution for the entire range
        // EQUIVALENT TO: dp[len-1] in the tabulation approach
        return prev1;
    }

    /*--------------------------------------------------
      MAIN FUNCTION — HOUSE ROBBER II

      CORE STRATEGY:
      Break the circular constraint by solving two linear subproblems:
      - Case 1: Exclude first house → Range [1 ... n-1]
      - Case 2: Exclude last house → Range [0 ... n-2]

      WHY THIS WORKS:
      Since first and last houses are adjacent in the circle, we can never
      rob BOTH of them. So at least one of them must be excluded from any
      valid solution. By trying both exclusions, we cover all possibilities.

      IMPLEMENTATION NOTE:
      This solution uses the SPACE-OPTIMIZED version for efficiency.
      You can swap it with solveMemo() or solveTabulation() if needed.

      TIME: O(n) - Two passes through ~n houses each
      SPACE: O(1) - Using space-optimized helper
    --------------------------------------------------*/
    int rob(vector<int>& nums) {

        // GET ARRAY SIZE: Number of houses in the circular arrangement
        int n = nums.size();

        // EDGE CASE: Only one house exists
        // WHY special handling: With 1 house, there's no adjacency issue at all
        // DECISION: Just rob the single house and return its money
        // NOTE: This also prevents issues with empty ranges in the general case
        if (n == 1) {
            return nums[0];
        }

        // CASE 1: EXCLUDE THE FIRST HOUSE (index 0)
        // RANGE: [1 ... n-1] means we consider houses at indices 1, 2, ..., n-1
        // WHY: By excluding house 0, houses 1 and n-1 are no longer "circularly adjacent"
        // EFFECT: The range becomes a linear problem, exactly like House Robber I
        // COVERS: All optimal solutions that DON'T include house 0
        // NOTE: House n-1 MAY or MAY NOT be robbed in this case
        int case1 = solveSpaceOptimized(1, n - 1, nums);

        // CASE 2: EXCLUDE THE LAST HOUSE (index n-1)
        // RANGE: [0 ... n-2] means we consider houses at indices 0, 1, ..., n-2
        // WHY: By excluding house n-1, houses 0 and n-2 are no longer "circularly adjacent"
        // EFFECT: The range becomes a linear problem, exactly like House Robber I
        // COVERS: All optimal solutions that DON'T include house n-1
        // NOTE: House 0 MAY or MAY NOT be robbed in this case
        int case2 = solveSpaceOptimized(0, n - 2, nums);

        // FINAL ANSWER: Take the maximum of both cases
        // WHY max(): The optimal solution must exclude either first or last house (or both)
        // GUARANTEE: We've covered all valid configurations:
        //   - Optimal robs house 0 but not n-1 → Captured in case2
        //   - Optimal robs house n-1 but not 0 → Captured in case1
        //   - Optimal robs neither 0 nor n-1 → Captured in BOTH cases (same result)
        // INTERPRETATION: "The best of 'not robbing first' vs 'not robbing last'"
        return max(case1, case2);
    }
};
