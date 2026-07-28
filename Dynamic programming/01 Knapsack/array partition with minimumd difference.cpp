/*
 * ============================================================
 * PROBLEM: Minimum Subset Sum Difference
 * ============================================================
 *
 * PROBLEM STATEMENT (Plain English):
 * -----------------------------------
 * You are given an array of non-negative integers.
 * Your task is to split this array into two non-empty subsets (S1 and S2)
 * such that EVERY element belongs to exactly one of the two subsets.
 * You want to minimize the absolute difference |sum(S1) - sum(S2)|.
 *
 * INPUT:
 *   - arr: a vector of non-negative integers
 *   - n: size of the array
 *
 * OUTPUT:
 *   - An integer: the minimum possible |sum(S1) - sum(S2)| over all
 *     valid ways to partition the array into two subsets.
 *
 * EXAMPLE:
 *   arr = [1, 6, 11, 5]
 *   One possible split: S1 = {1, 5, 6} → sum = 12,  S2 = {11} → sum = 11
 *   Difference = |12 - 11| = 1  ← this is the minimum possible
 *
 * ============================================================
 * FIRST PRINCIPLES THINKING — How Do We Approach This?
 * ============================================================
 *
 * KEY OBSERVATION 1:
 *   Let totalSum = sum of all elements.
 *   If we assign one subset a sum of s1, the other automatically gets
 *   s2 = totalSum - s1.
 *   The difference = |s2 - s1| = |totalSum - 2*s1|.
 *   To MINIMIZE this, we want s1 to be as close to totalSum/2 as possible.
 *
 * KEY OBSERVATION 2:
 *   So the problem reduces to:
 *   "Can we pick a subset of arr with sum exactly equal to s1?"
 *   for every s1 from 0 to totalSum/2.
 *   This is the classic 0/1 Subset Sum problem.
 *
 * WHY BRUTE FORCE FAILS:
 *   There are 2^n ways to partition n elements. For n=30, that's ~1 billion.
 *   We need something better.
 *
 * OPTIMAL IDEA — DP on Subset Sum:
 *   Use DP to compute, for each target sum t (0 to totalSum/2),
 *   whether it's achievable using a subset of arr.
 *   Then scan all achievable s1 values and track the minimum |totalSum - 2*s1|.
 *
 * DP STATE DEFINITION:
 *   dp[i][t] = true  if using elements arr[0..i], we can form a subset
 *                    with sum exactly t.
 *            = false otherwise.
 *
 * TRANSITION:
 *   For each element arr[i], we have two choices:
 *     - Don't take arr[i]: dp[i][t] = dp[i-1][t]
 *     - Take arr[i] (only if arr[i] <= t): dp[i][t] = dp[i-1][t - arr[i]]
 *   dp[i][t] = (take || notTake)
 *
 * BASE CASES:
 *   - dp[i][0] = true for all i: sum = 0 is always achievable (empty subset).
 *   - dp[0][arr[0]] = true (if arr[0] <= half): first element alone forms
 *                     a subset with sum arr[0].
 *
 * We only need sums up to totalSum/2 because:
 *   If s1 > totalSum/2, then s2 < totalSum/2, and we'd have already
 *   considered the same partition from the other direction.
 *
 * ============================================================
 */


/* ============================================================
 * APPROACH 1: Top-Down DP with Memoization (Recursive)
 * ============================================================
 *
 * This is the recursive + memo version of the subset sum DP.
 * We try to answer: "Can we achieve sum = target using arr[i..n-1]?"
 * and cache results to avoid recomputation.
 * ============================================================
 */
class Solution
{
public:
    /*
     * solve(i, target, arr, dp):
     *   Returns true if we can pick a subset from arr[i..n-1] whose sum = target.
     *
     *   i      : current index we're deciding on (take or skip)
     *   target : remaining sum we still need to achieve
     *   arr    : the input array
     *   dp     : memoization table; dp[i][target] stores the answer once computed
     */
    bool solve(int i, int target, vector<int> &arr,
               vector<vector<int>> &dp)
    {
        // BASE CASE: We've considered all elements.
        // If the remaining target is 0, we successfully formed a subset with
        // the desired sum → return true. Otherwise false.
        if (i == arr.size())
            return (target == 0);

        // MEMOIZATION CHECK:
        // If we've already computed this (i, target) subproblem, return cached result.
        // This prevents exponential recomputation of overlapping subproblems.
        if (dp[i][target] != -1)
            return dp[i][target];

        // CHOICE 1 — DON'T TAKE arr[i]:
        // Move to the next element without reducing the target.
        // We're checking: can we still achieve 'target' without using arr[i]?
        bool notTake = solve(i + 1, target, arr, dp);

        // CHOICE 2 — TAKE arr[i]:
        // We can only take arr[i] if it doesn't exceed the remaining target
        // (since all values are non-negative, taking a value larger than target
        // would make the sum exceed what we need — impossible to correct later).
        bool take = false;
        if (arr[i] <= target)
            take = solve(i + 1, target - arr[i], arr, dp);

        // STORE AND RETURN:
        // If either choice leads to a valid partition, this subproblem is solvable.
        // Cache it so future calls with same (i, target) return instantly.
        return dp[i][target] = (take || notTake);
    }

    int minSubsetSumDifference(vector<int> &arr, int n)
    {
        // Compute totalSum. This anchors everything:
        // If one subset has sum s1, the other has sum (totalSum - s1).
        int totalSum = 0;
        for (int x : arr)
            totalSum += x;

        // We only need to check s1 values up to totalSum/2.
        // Beyond that, s1 and s2 simply swap roles — we'd be double-counting.
        int half = totalSum / 2;

        // dp[i][t]: -1 = unvisited, 0 = false, 1 = true
        // Dimension: n rows (one per element), half+1 columns (sums 0..half)
        vector<vector<int>> dp(n, vector<int>(half + 1, -1));

        int minDiff = INT_MAX;

        // Try every possible value of s1 from 0 to half.
        // For each s1, check if it's achievable as a subset sum.
        // If yes, compute the difference and track the minimum.
        for (int s1 = 0; s1 <= half; s1++)
        {
            if (solve(0, s1, arr, dp))
            {
                // s2 is determined automatically once s1 is fixed
                int s2 = totalSum - s1;

                // |s2 - s1| = |totalSum - 2*s1|
                // Since s1 <= half <= totalSum/2, s2 >= s1, so s2 - s1 >= 0.
                minDiff = min(minDiff, abs(s2 - s1));
            }
        }

        return minDiff;
    }
};


/* ============================================================
 * APPROACH 2: Bottom-Up Tabulation (Iterative DP)
 * ============================================================
 *
 * Same DP logic as above, but built iteratively from smaller subproblems
 * up to larger ones. No recursion stack overhead.
 *
 * dp[i][t] = true if we can form a subset with sum exactly t
 *            using elements from arr[0..i].
 * ============================================================
 */
class Solution
{
public:
    int minSubsetSumDifference(vector<int> &arr, int n)
    {
        // Compute total sum of all elements.
        // This is needed to derive s2 from s1 (s2 = totalSum - s1),
        // and to cap our target range at totalSum/2.
        int totalSum = 0;
        for (int x : arr)
            totalSum += x;

        // We only need to check subset sums up to totalSum/2.
        // If s1 <= half, then s2 = totalSum - s1 >= s1, so |s2 - s1| = s2 - s1.
        // Checking beyond half would repeat the same (mirrored) partitions.
        int half = totalSum / 2;

        // DP TABLE:
        // dp[i][t] = true means: using arr[0..i], we can pick a subset summing to t.
        // Size: n rows × (half+1) columns.
        // Initialize all to false; we'll fill in base cases next.
        vector<vector<bool>> dp(n, vector<bool>(half + 1, false));

        // BASE CASE 1:
        // For every row (every element index i), sum = 0 is always achievable
        // by picking the empty subset. So dp[i][0] = true for all i.
        for (int i = 0; i < n; i++)
            dp[i][0] = true;

        // BASE CASE 2:
        // With only the first element (i = 0), we can achieve sum = arr[0]
        // by taking it — but only if arr[0] fits within our target range (≤ half).
        if (arr[0] <= half)
            dp[0][arr[0]] = true;

        // FILL THE TABLE:
        // For each subsequent element (i from 1 to n-1), and each target sum t,
        // decide: can we achieve sum t using elements arr[0..i]?
        for (int i = 1; i < n; i++)
        {
            for (int t = 0; t <= half; t++)
            {
                // OPTION A — SKIP arr[i]:
                // If we could already achieve sum t using arr[0..i-1],
                // we can still achieve it by ignoring arr[i].
                bool notTake = dp[i - 1][t];

                // OPTION B — TAKE arr[i]:
                // Only valid if arr[i] <= t (otherwise sum would overshoot t).
                // If we take arr[i], we need to have achieved (t - arr[i])
                // using elements arr[0..i-1].
                bool take = false;
                if (arr[i] <= t)
                    take = dp[i - 1][t - arr[i]];

                // dp[i][t] is true if at least one of the two choices works.
                dp[i][t] = take || notTake;
            }
        }

        // FIND MINIMUM DIFFERENCE:
        // After filling the table, dp[n-1][s1] tells us whether sum s1 is
        // achievable using all n elements.
        // For every achievable s1 (0 to half), compute the resulting difference
        // and keep track of the minimum.
        int minDiff = INT_MAX;

        for (int s1 = 0; s1 <= half; s1++)
        {
            if (dp[n - 1][s1])
            {
                // The other subset automatically has sum s2 = totalSum - s1.
                int s2 = totalSum - s1;

                // Since s1 ≤ half ≤ totalSum/2, we have s2 ≥ s1,
                // so the difference is always s2 - s1 = totalSum - 2*s1.
                // We take abs() for safety with integer division edge cases.
                minDiff = min(minDiff, abs(s2 - s1));
            }
        }

        return minDiff;
    }
};

/*
 * ============================================================
 * COMPLEXITY ANALYSIS
 * ============================================================
 *
 * Let S = totalSum, n = number of elements.
 *
 * TIME COMPLEXITY:  O(n * S/2) = O(n * S)
 *   We fill an n × (S/2 + 1) DP table, each cell in O(1).
 *
 * SPACE COMPLEXITY: O(n * S)
 *   The 2D DP table.
 *   (Can be reduced to O(S) using a 1D rolling array, since row i only
 *    depends on row i-1. That optimization isn't applied here.)
 *
 * ============================================================
 * QUICK REVISION SUMMARY
 * ============================================================
 *
 * 1. The difference |s1 - s2| = |totalSum - 2*s1|.
 *    Minimize by finding s1 ≤ totalSum/2 that is achievable AND closest to half.
 *
 * 2. "Is s1 achievable?" → classic 0/1 Subset Sum DP.
 *    dp[i][t] = can we pick from arr[0..i] to sum to exactly t?
 *
 * 3. Transition: dp[i][t] = dp[i-1][t]  (skip)
 *                          || dp[i-1][t - arr[i]]  (take, if arr[i] <= t)
 *
 * 4. Base cases: dp[i][0] = true always; dp[0][arr[0]] = true if arr[0] <= half.
 *
 * 5. Answer: scan s1 from 0 to half; find minimum abs(totalSum - 2*s1)
 *    among all s1 where dp[n-1][s1] = true.
 * ============================================================
 */