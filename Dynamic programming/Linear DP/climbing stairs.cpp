/*
 * PROBLEM OVERVIEW:
 * You are climbing a staircase with n steps. Each time you can climb either 1 or 2 steps.
 * Find the number of distinct ways to reach the top.
 *
 * KEY INSIGHT:
 * This is a classic Fibonacci-based DP problem. To reach step i, you can either:
 * - Come from step (i-1) by taking 1 step, OR
 * - Come from step (i-2) by taking 2 steps
 * Therefore: ways[i] = ways[i-1] + ways[i-2]
 *
 * WHY THIS WORKS:
 * The principle of counting: Total ways to reach step i equals the sum of:
 * - All paths that reach (i-1) and then take 1 step
 * - All paths that reach (i-2) and then take 2 steps
 * These two sets are mutually exclusive (no overlap), so we add them.
 *
 * PATTERN RECOGNITION:
 * This generates the Fibonacci sequence:
 * n=1: 1 way  (1)
 * n=2: 2 ways (1+1, 2)
 * n=3: 3 ways (1+1+1, 1+2, 2+1)
 * n=4: 5 ways (1+1+1+1, 1+1+2, 1+2+1, 2+1+1, 2+2)
 * Each value is the sum of the previous two.
 *
 * TIME COMPLEXITY: O(n) - Single pass through all steps
 * SPACE COMPLEXITY: O(n) - DP array of size n+1
 * NOTE: Can be optimized to O(1) space by keeping only last 2 values
 */

class Solution
{
public:
    int climbStairs(int n)
    {
        // BASE CASE: For 0 or 1 steps, there's exactly 1 way
        // WHY: n=0 is treated as "already there" (1 way: do nothing)
        //      n=1 has only 1 way: take 1 step
        // This base case ensures our recurrence relation works correctly
        if (n <= 1)
            return 1;

        // DP array where dp[i] = number of distinct ways to reach step i
        // WHY use array: We need to build up from smaller subproblems to larger ones
        // HOW: Each position stores the solution to the subproblem "ways to reach step i"
        vector<int> dp(n + 1);

        // INITIALIZATION: Set base cases
        // dp[0] = 1: Convention that there's 1 way to stay at ground (important for math to work)
        // dp[1] = 1: Only 1 way to reach first step (take 1 step from ground)
        // WHY these values: They serve as the foundation for our recurrence relation
        // Without dp[0]=1, our formula dp[2] = dp[1] + dp[0] wouldn't give the correct answer
        dp[0] = 1;
        dp[1] = 1;

        // BUILD UP SOLUTION: Calculate ways to reach each step from 2 to n
        // WHY start at 2: Steps 0 and 1 are already initialized (base cases)
        // WHY this order: Each step depends only on previous steps, so we build bottom-up
        // This ensures when computing dp[i], both dp[i-1] and dp[i-2] are already calculated
        for (int i = 2; i <= n; i++)
        {
            // RECURRENCE RELATION: ways[i] = ways[i-1] + ways[i-2]
            // INTUITION: To reach step i, you must have been at either step (i-1) or (i-2)
            // - If at step (i-1): take 1 step → contributes dp[i-1] ways
            // - If at step (i-2): take 2 steps → contributes dp[i-2] ways
            // WHY addition: These are disjoint sets (you can't be at both i-1 and i-2 simultaneously)
            // so by the addition principle of combinatorics, we sum them
            dp[i] = dp[i - 1] + dp[i - 2];
        }

        // Return the number of ways to reach the nth step
        // This represents all possible distinct sequences of 1-step and 2-step moves
        // that sum to exactly n
        return dp[n];
    }
};