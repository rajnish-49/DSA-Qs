/*
 * ============================================================
 * LC 1335 - Minimum Difficulty of a Job Schedule
 * ============================================================
 *
 * PROBLEM EXPLANATION
 * -------------------
 * You have n jobs that must be done in order (job i must finish before job i+1).
 * You must schedule them across exactly d days.
 * Each day must have at least one job.
 * The difficulty of a day = max difficulty among all jobs done that day.
 * The difficulty of a schedule = sum of daily difficulties.
 * Goal: minimize the total schedule difficulty.
 *
 * IMPORTANT TERMS
 * ---------------
 * - jobDifficulty[i] : difficulty of the ith job (0-indexed)
 * - Day difficulty   : max of jobDifficulty values assigned to that day
 * - Schedule difficulty : sum of all day difficulties
 *
 * CONSTRAINTS AND IMPLICATIONS
 * ----------------------------
 * - Jobs are ordered: you can only assign a contiguous prefix to day 1,
 *   next contiguous chunk to day 2, and so on. No skipping or reordering.
 * - Each day needs at least 1 job, so you need n >= d. If n < d, return -1.
 * - The problem reduces to: where do you place d-1 cuts in the array
 *   to split it into d contiguous groups, minimizing the sum of
 *   maximums of each group.
 *
 * EDGE CASES
 * ----------
 * - n < d  : impossible, return -1
 * - n == d : one job per day, answer = sum of all values
 * - d == 1 : entire array in one day, answer = max of entire array
 *
 * ============================================================
 * THINKING FROM FIRST PRINCIPLES
 * ============================================================
 *
 * STEP 1 — Understand the structure of a valid schedule
 * ------------------------------------------------------
 * Since jobs must be done in order and each day is contiguous,
 * any valid schedule is just a way to draw d-1 cuts in the array.
 *
 * Example: [6,5,4,3,2,1], d=3
 * One possible schedule: [6,5] | [4,3] | [2,1]
 * Cost = max(6,5) + max(4,3) + max(2,1) = 6 + 4 + 2 = 12
 *
 * STEP 2 — Why brute force is too slow
 * --------------------------------------
 * Trying all ways to place d-1 cuts in n-1 positions = C(n-1, d-1).
 * For large n and d this explodes. We need to reuse computation.
 *
 * STEP 3 — Key observation: only ONE cut matters at each step
 * ------------------------------------------------------------
 * Suppose you want the best schedule for some chunk of jobs using some days.
 * You don't need to know exactly how all previous days were split.
 * You only need to know:
 *   - Where did the current day start / end?
 *   - What was the best cost for the remaining subproblem?
 *
 * This is optimal substructure. It can be viewed in two directions:
 *   - Suffix view (memoization): fix where today ENDS, recurse on the rest
 *   - Prefix view (tabulation) : fix where today STARTS, use precomputed prefix
 *
 * Both views produce the same recurrence, just indexed differently.
 *
 * ============================================================
 * THREE APPROACHES — OVERVIEW
 * ============================================================
 *
 * APPROACH 1 — Memoization (Top-Down)
 *   State : solve(idx, days) = min cost to schedule jobs[idx..n-1] in `days` days
 *   Direction: idx moves forward, days counts down
 *   Natural for recursion — think "what's left to do"
 *
 * APPROACH 2 — Bottom-Up Tabulation (Suffix DP)
 *   State : dp[i][day] = min cost to schedule jobs[i..n-1] in `day` days
 *   Same meaning as memoization, just filled iteratively
 *   Fill order: day from 1 up to d, i from right to left
 *
 * APPROACH 3 — Bottom-Up Tabulation (Prefix DP)
 *   State : dp[i][day] = min cost to schedule first i jobs in `day` days
 *   Flipped perspective — think "how much have we scheduled so far"
 *   Fill order: day from 1 up to d, i from left to right
 *   Base case is cleaner: dp[0][0] = 0
 *
 * ============================================================
 */

#include <vector>
#include <algorithm>
#include <climits>
using namespace std;


/* ============================================================
 * APPROACH 1 — MEMOIZATION (TOP-DOWN DP)
 * ============================================================
 *
 * STATE DEFINITION
 * ----------------
 * solve(idx, days) = minimum difficulty to schedule jobs[idx..n-1] in exactly `days` days
 *
 * Meaning: "I have already handled jobs before idx. Now I need to schedule
 * the remaining jobs (from idx onward) across `days` more days."
 *
 * TRANSITION
 * ----------
 * For the current day, pick a contiguous chunk starting at idx.
 * Let `end` be the last job index of today.
 * Today's cost = max(job[idx..end]).
 * Remaining cost = solve(end+1, days-1).
 *
 * We try all valid `end` from idx to n-days (leaving at least 1 job per remaining day).
 * Take the minimum over all choices.
 *
 * BASE CASE
 * ---------
 * days == 1: only one day left, must take all remaining jobs.
 * Cost = max(job[idx..n-1]).
 *
 * VALID RANGE FOR `end`
 * ----------------------
 * end goes from idx to n-days because:
 *   - end >= idx     : today needs at least 1 job
 *   - end <= n-days  : remaining days-1 days each need at least 1 job,
 *                      so at least days-1 jobs must remain after end
 *                      => end+1 <= n-(days-1) => end <= n-days
 *
 * WHY MEMOIZATION WORKS HERE
 * --------------------------
 * The same (idx, days) pair is reached via many different paths
 * (different ways to split earlier jobs). Without memoization,
 * we recompute the same subproblem exponentially many times.
 * dp[idx][days] caches the answer after first computation.
 *
 * ============================================================
 */

class SolutionMemo {
public:
    vector<int> job;
    int n;
    vector<vector<int>> dp;

    int solve(int idx, int days) {
        // Base case: only 1 day left, must take all remaining jobs.
        // No more decisions to make — cost is max of jobs[idx..n-1].
        if (days == 1) {
            int mx = 0;
            for (int i = idx; i < n; i++)
                mx = max(mx, job[i]);
            return mx;
        }

        // Return cached result if already computed.
        // Avoids recomputing the same (idx, days) subproblem.
        if (dp[idx][days] != -1)
            return dp[idx][days];

        int ans = INT_MAX;
        int mx = 0;

        // Try every possible ending index for today's jobs.
        // end is the last job index assigned to today (0-indexed).
        // end <= n-days ensures at least 1 job per remaining day after today.
        for (int end = idx; end <= n - days; end++) {

            // Extend today's window to include job[end].
            // mx = max(job[idx..end]) = today's difficulty so far.
            // Growing rightward so max is maintained incrementally in O(1).
            mx = max(mx, job[end]);

            // Total cost: today's difficulty + best cost for remaining subproblem.
            // solve(end+1, days-1) handles jobs[end+1..n-1] in days-1 days.
            int cost = mx + solve(end + 1, days - 1);

            ans = min(ans, cost);
        }

        // Cache and return.
        return dp[idx][days] = ans;
    }

    int minDifficulty(vector<int>& jobDifficulty, int d) {
        n = jobDifficulty.size();

        if (n < d) return -1;

        job = jobDifficulty;

        // dp[i][day] = min cost to schedule jobs[i..n-1] in `day` days.
        // -1 means not yet computed.
        dp.assign(n, vector<int>(d + 1, -1));

        return solve(0, d);
    }
};


/* ============================================================
 * APPROACH 2 — BOTTOM-UP TABULATION (SUFFIX DP)
 * ============================================================
 *
 * STATE DEFINITION
 * ----------------
 * dp[i][day] = minimum difficulty to schedule jobs[i..n-1] in exactly `day` days
 *
 * Same meaning as memoization. "Starting from job i, using day days."
 *
 * TRANSITION
 * ----------
 * For dp[i][day], fix where today ends (index `end`).
 * Today covers jobs[i..end], cost = max(job[i..end]).
 * Remaining: dp[end+1][day-1] covers jobs[end+1..n-1] in day-1 days.
 *
 * dp[i][day] = min over all valid end of (max(job[i..end]) + dp[end+1][day-1])
 *
 * Valid range for end: i to n-day (same reasoning as memoization).
 *
 * BASE CASE
 * ---------
 * dp[i][1] = max(job[i..n-1]) for all i.
 * Only 1 day left, must take everything from i to n-1.
 *
 * FILL ORDER
 * ----------
 * dp[i][day] depends on dp[end+1][day-1] — previous day column.
 * So fill day=1 first, then day=2, ..., up to day=d.
 * For each day, i goes from right to left (n-day down to 0)
 * because larger i (fewer remaining jobs) are simpler subproblems.
 * Within each i, end grows rightward so curMax extends in O(1).
 *
 * ANSWER
 * ------
 * dp[0][d] = min cost to schedule all n jobs in d days.
 *
 * ============================================================
 */

class SolutionSuffixDP {
public:
    int minDifficulty(vector<int>& jobDifficulty, int d) {
        int n = jobDifficulty.size();

        if (n < d) return -1;

        // dp[i][day] = min cost to schedule jobs[i..n-1] in `day` days.
        // Size (n+1) x (d+1). dp[n][0]=0 is the base anchor (0 jobs, 0 days).
        // Initialize to INT_MAX — no valid schedule found yet.
        vector<vector<int>> dp(n + 1, vector<int>(d + 1, INT_MAX));

        // Base anchor: 0 jobs remaining in 0 days costs 0.
        // When day=1 and end=n-1, dp[end+1][day-1]=dp[n][0]=0 correctly gives
        // dp[i][1] = max(job[i..n-1]) + 0 = max of remaining jobs. Correct.
        dp[n][0] = 0;

        // Fill base case day=1 explicitly.
        // dp[i][1] = max(job[i..n-1]) — only 1 day, take all remaining jobs.
        // Fill right to left, maintaining running max.
        {
            int mx = 0;
            for (int i = n - 1; i >= 0; i--) {
                mx = max(mx, jobDifficulty[i]);
                dp[i][1] = mx;
            }
        }

        // Fill day=2 through day=d.
        for (int day = 2; day <= d; day++) {

            // i = starting index of today's remaining jobs.
            // i can go from 0 to n-day (need at least `day` jobs from i onward).
            for (int i = n - day; i >= 0; i--) {

                int curMax = 0;

                // end = last index of today's jobs.
                // end starts at i and goes up to n-day.
                // Growing rightward: curMax = max(job[i..end]) maintained in O(1).
                for (int end = i; end <= n - day; end++) {

                    // Extend today's window rightward.
                    curMax = max(curMax, jobDifficulty[end]);

                    // Guard: skip if dp[end+1][day-1] has no valid schedule.
                    if (dp[end + 1][day - 1] != INT_MAX)
                        dp[i][day] = min(dp[i][day], curMax + dp[end + 1][day - 1]);
                }
            }
        }

        return dp[0][d] == INT_MAX ? -1 : dp[0][d];
    }
};


/* ============================================================
 * APPROACH 3 — BOTTOM-UP TABULATION (PREFIX DP)
 * ============================================================
 *
 * STATE DEFINITION
 * ----------------
 * dp[i][day] = minimum difficulty to schedule the first i jobs in `day` days
 *
 * "First i jobs" = jobs at indices 0, 1, ..., i-1.
 * Think in counts, not indices. dp[6][3] = 6 jobs, 3 days.
 *
 * This is the FLIPPED version of the suffix state.
 * Instead of "jobs remaining", we think "jobs done so far".
 * Cleaner for bottom-up because the base case dp[0][0]=0 is a natural empty anchor.
 *
 * TRANSITION
 * ----------
 * For dp[i][day], ask: where did the LAST day start?
 * Let j = starting INDEX of the last day (0-indexed into jobDifficulty).
 * Last day covers jobs[j..i-1], cost = max(job[j..i-1]).
 * Previous days handled first j jobs in day-1 days => dp[j][day-1].
 *
 * dp[i][day] = min over all valid j of (dp[j][day-1] + max(job[j..i-1]))
 *
 * Valid range for j:
 *   - j >= day-1 : first day-1 days need at least day-1 jobs total
 *   - j <= i-1   : last day needs at least 1 job (window job[j..i-1] non-empty)
 *
 * BASE CASE
 * ---------
 * dp[0][0] = 0 : zero jobs in zero days costs nothing.
 * This is the only seed. All of day=1 flows from this anchor.
 *
 * FILL ORDER
 * ----------
 * dp[i][day] depends on dp[j][day-1] — previous day column.
 * Fill day=1 first (seeded by dp[0][0]), then day=2, ..., up to day=d.
 * For each day, i goes from day to n (i < day is invalid).
 * Innermost loop: j from i-1 DOWN to day-1.
 *   Going downward: as j decreases, today's window job[j..i-1] grows leftward.
 *   We extend curMax = max(curMax, job[j]) in O(1) each step.
 *   If j went upward, today's window would shrink from the left —
 *   max cannot be maintained incrementally when shrinking.
 *
 * ANSWER
 * ------
 * dp[n][d] = min cost to schedule all n jobs in d days.
 *
 * ============================================================
 */

class SolutionPrefixDP {
public:
    int minDifficulty(vector<int>& jobDifficulty, int d) {
        int n = jobDifficulty.size();

        // If fewer jobs than days, impossible to assign at least 1 job per day.
        if (n < d) return -1;

        // dp[i][day] = min difficulty to schedule first i jobs in `day` days.
        // Size (n+1) x (d+1): i ranges 0..n, day ranges 0..d.
        // Initialize to 1e9 (infinity) — no valid schedule found yet.
        vector<vector<int>> dp(n + 1, vector<int>(d + 1, 1e9));

        // Base case: 0 jobs in 0 days costs 0.
        // This is the empty-prefix anchor. When day=1, j can reach 0,
        // giving dp[i][1] = dp[0][0] + max(job[0..i-1]) = max(job[0..i-1]).
        // That is correct — first i jobs in 1 day = max of all of them.
        dp[0][0] = 0;

        // Outer loop: fill one full day-column at a time.
        // dp[i][day] reads from dp[j][day-1], so previous column must be ready.
        for (int day = 1; day <= d; day++) {

            // i = total jobs scheduled so far (count, not index).
            // Minimum valid i is `day`: each of the `day` days needs at least 1 job.
            // dp[i][day] where i < day is impossible — not enough jobs for all days.
            for (int i = day; i <= n; i++) {

                // curMax tracks max(jobDifficulty[j..i-1]) as j decreases.
                // j is the starting index (0-indexed) of today's jobs.
                // As j decreases by 1, today's window grows leftward by 1 job.
                // We include job[j] into curMax each step in O(1).
                int curMax = 0;

                // j = starting index of the last day's jobs (0-indexed).
                // j goes from i-1 DOWN to day-1:
                //   - j <= i-1   : today needs at least 1 job (non-empty window)
                //   - j >= day-1 : first day-1 days need at least day-1 jobs
                // Going downward so curMax can be extended incrementally.
                for (int j = i - 1; j >= day - 1; j--) {

                    // Grow today's window leftward to include job at index j.
                    // After this: curMax = max(jobDifficulty[j..i-1]) = today's cost.
                    curMax = max(curMax, jobDifficulty[j]);

                    // Transition:
                    // dp[j][day-1] = best cost for first j jobs in day-1 days (precomputed)
                    // curMax       = cost of today = max(job[j..i-1])
                    // Sum = total cost when last day starts at index j.
                    // Take minimum over all valid j.
                    dp[i][day] = min(dp[i][day], dp[j][day - 1] + curMax);
                }
            }
        }

        // dp[n][d] = min cost to schedule all n jobs in exactly d days.
        return dp[n][d];
    }
};


/*
 * ============================================================
 * DRY RUN — ALL THREE APPROACHES
 * ============================================================
 * jobDifficulty = [6, 5, 4, 3, 2, 1], d = 2, n = 6
 * Expected output: 7
 *
 * ---- APPROACH 1: MEMOIZATION ----
 * solve(0, 2):
 *   end=0: mx=6, cost = 6 + solve(1,1) = 6 + max(5,4,3,2,1) = 6+5 = 11
 *   end=1: mx=6, cost = 6 + solve(2,1) = 6 + max(4,3,2,1)   = 6+4 = 10
 *   end=2: mx=6, cost = 6 + solve(3,1) = 6 + max(3,2,1)     = 6+3 = 9
 *   end=3: mx=6, cost = 6 + solve(4,1) = 6 + max(2,1)       = 6+2 = 8
 *   end=4: mx=6, cost = 6 + solve(5,1) = 6 + max(1)         = 6+1 = 7 ✓
 *   ans = 7
 *
 * ---- APPROACH 2: SUFFIX DP ----
 * Base day=1 (right to left):
 *   dp[5][1]=1, dp[4][1]=2, dp[3][1]=3, dp[2][1]=4, dp[1][1]=5, dp[0][1]=6
 *
 * day=2:
 *   i=4: end=4, curMax=2, dp[4][2]=2+dp[5][1]=3
 *   i=3: end=3, curMax=3, dp[3][2]=3+dp[4][1]=5
 *         end=4, curMax=3, dp[3][2]=min(5,3+dp[5][1])=min(5,4)=4
 *   i=2: end=2, curMax=4, dp[2][2]=4+dp[3][1]=7
 *         end=3, curMax=4, dp[2][2]=min(7,4+dp[4][1])=min(7,6)=6
 *         end=4, curMax=4, dp[2][2]=min(6,4+dp[5][1])=min(6,5)=5
 *   i=1: best at end=4: curMax=5, dp[1][2]=5+dp[5][1]=5+1=6
 *   i=0: best at end=4: curMax=6, dp[0][2]=6+dp[5][1]=6+1=7 ✓
 *
 * ---- APPROACH 3: PREFIX DP ----
 * Base: dp[0][0]=0, all else = INF
 *
 * day=1:
 *   i=1: j=0, curMax=job[0]=6, dp[1][1]=0+6=6
 *   i=2: j=1, curMax=5, dp[2][1]=INF; j=0, curMax=6, dp[2][1]=0+6=6
 *   dp[3][1]=dp[4][1]=dp[5][1]=dp[6][1]=6 (job[0]=6 always dominates)
 *
 * day=2:
 *   i=2: j=1, curMax=5, dp[2][2]=dp[1][1]+5=11
 *   i=6: j=5, curMax=1, dp[6][2]=dp[5][1]+1=6+1=7
 *         j=4, curMax=2, dp[6][2]=min(7,dp[4][1]+2)=min(7,8)=7
 *         j=3, curMax=3, dp[6][2]=min(7,dp[3][1]+3)=min(7,9)=7
 *         ... minimum stays 7
 *
 * Answer: dp[6][2] = 7 ✓
 *
 * ============================================================
 * COMPLEXITY ANALYSIS
 * ============================================================
 *
 * APPROACH 1 — MEMOIZATION
 * Time : O(n^2 * d)
 *   - Total unique states: n * d
 *   - Each state tries O(n) values of `end`
 *   - curMax maintained in O(1) per step
 *   - Total: O(n^2 * d)
 * Space: O(n * d) memo table + O(n + d) recursion stack
 *
 * APPROACH 2 — SUFFIX DP
 * Time : O(n^2 * d)
 *   - d outer iterations, n middle iterations, n inner iterations
 *   - curMax maintained in O(1) per inner step
 * Space: O(n * d) for dp table, no recursion stack
 *
 * APPROACH 3 — PREFIX DP
 * Time : O(n^2 * d)
 *   - Same loop structure as suffix DP
 *   - curMax maintained in O(1) per inner step
 * Space: O(n * d) for dp table, no recursion stack
 *   - Reducible to O(n) by keeping only two columns (day-1 and day)
 *
 * IS THIS OPTIMAL?
 * ----------------
 * Yes for this DP formulation. Any solution must consider all possible
 * placements of the last cut for each subproblem — O(n) work per cell,
 * n*d cells => O(n^2 * d) is the lower bound. The curMax trick ensures
 * we hit this bound exactly without a naive O(n) range-max recompute.
 *
 * WHICH APPROACH TO PREFER?
 * --------------------------
 * - Memoization : most natural to derive, easiest to reason about correctness
 * - Suffix DP   : direct iterative translation of memoization, same indexing
 * - Prefix DP   : cleanest base case (dp[0][0]=0), most common in interviews
 *                 for this class of "partition array into k groups" problems
 * ============================================================
 */