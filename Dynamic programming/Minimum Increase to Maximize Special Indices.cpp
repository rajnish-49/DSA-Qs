/*
 * ============================================================
 * PROBLEM: Minimum Increase to Maximize Special Indices
 * ============================================================
 *
 * PROBLEM STATEMENT (in simple terms):
 * -------------------------------------
 * You are given an array nums of length n.
 * An index i is called "special" if:
 *     nums[i] > nums[i-1]  AND  nums[i] > nums[i+1]
 * i.e., it is strictly greater than both its neighbors (a local maximum / peak).
 *
 * Only interior indices (1 to n-2) can ever be special.
 * Indices 0 and n-1 have only one neighbor so they can never qualify.
 *
 * You can perform one operation: pick any index and increase nums[i] by 1.
 * You can do this as many times as you want, on any index.
 *
 * GOAL:
 * 1. Maximize the number of special indices.
 * 2. Among all ways to achieve that maximum, minimize the total operations used.
 * Return the minimum total operations.
 *
 * ============================================================
 * KEY OBSERVATIONS (from first principles)
 * ============================================================
 *
 * Observation 1: Cost to make a single index special in isolation.
 * ----------------------------------------------------------------
 * To make index i special, we need nums[i] > max(nums[i-1], nums[i+1]).
 * We can only raise nums[i], never lower neighbors.
 * So the minimum cost is:
 *     cost[i] = max(0, max(nums[i-1], nums[i+1]) + 1 - nums[i])
 * If nums[i] is already a peak, cost is 0.
 *
 * Observation 2: Two adjacent indices cannot both be special simultaneously.
 * --------------------------------------------------------------------------
 * If i and i+1 are both special:
 *     nums[i] > nums[i+1]  (because i is a peak, greater than right neighbor)
 *     nums[i+1] > nums[i]  (because i+1 is a peak, greater than left neighbor)
 * These two conditions contradict each other.
 * Therefore no two adjacent indices can both be special.
 *
 * Observation 3: This is a classic "no two adjacent" selection problem.
 * ----------------------------------------------------------------------
 * We have interior indices 1..n-2.
 * We want to select a subset with no two adjacent, maximizing count,
 * then minimizing cost. This is exactly the House Robber problem structure,
 * but with a lexicographic (peaks, cost) objective instead of a single value.
 *
 * Observation 4: Why brute force fails.
 * --------------------------------------
 * There are 2^(n-2) subsets of interior indices. Checking all is exponential.
 * We need DP to exploit the overlapping subproblems.
 *
 * ============================================================
 * DP FORMULATION
 * ============================================================
 *
 * State: dp[i] = best (State) achievable considering interior indices 1..i
 *        where State = {peaks: count of special indices, cost: total operations}
 *
 * Transition at index i:
 *   Option 1 - SKIP i: don't make i special.
 *       dp[i] = dp[i-1]   (carry forward best result up to i-1)
 *
 *   Option 2 - TAKE i: make i special.
 *       Must not have taken i-1 (no-adjacent constraint), so come from dp[i-2].
 *       dp[i] = {dp[i-2].peaks + 1, dp[i-2].cost + cost[i]}
 *
 *   dp[i] = better(skip, take)
 *       where "better" means: more peaks wins; tie broken by lower cost.
 *
 * Base cases:
 *   dp[0] = {0, 0}  (index 0 can never be special, no peaks, no cost)
 *   dp[1] = {1, cost[1]}  (index 1 is the first interior index, always take it)
 *
 * Answer: dp[n-2].cost
 *         (best result over all interior indices, return the cost component)
 */


/*
 * ============================================================
 * BOTTOM-UP (TABULATION) VERSION
 * ============================================================
 *
 * CORE INSIGHT: WHY DP?
 * ----------------------
 * We established two facts:
 *   1. cost[i] = operations to make index i a peak in isolation
 *   2. No two adjacent indices can both be peaks (mathematical contradiction)
 *
 * So we need to select a subset of interior indices with no two adjacent,
 * maximizing count first, then minimizing total cost.
 * This is exactly the House Robber problem but instead of maximizing
 * a single value, we optimize a pair (peaks, cost) lexicographically.
 *
 * WHY DO WE NEED A STATE STRUCT INSTEAD OF A SINGLE VALUE?
 * ---------------------------------------------------------
 * In classic House Robber you just maximize one number.
 * Here we have two competing objectives:
 *   - Maximize peaks (primary)
 *   - Minimize cost (secondary, only matters when peaks are equal)
 * Carrying a pair (peaks, cost) through the DP lets us compare
 * two choices correctly — more peaks always wins, cost only
 * breaks ties. If we only tracked cost we might accidentally
 * pick a cheaper solution that has fewer peaks, which is wrong.
 *
 * WHY IS THE TRANSITION dp[i-2] FOR TAKE AND dp[i-1] FOR SKIP?
 * -------------------------------------------------------------
 * If we TAKE index i as a peak, we cannot have taken index i-1
 * (adjacent indices cannot both be peaks). So the last valid
 * state we can build on is dp[i-2] — the best result ending
 * at or before index i-2, which guarantees a gap of at least 1.
 *
 * If we SKIP index i, we simply inherit dp[i-1] unchanged.
 * We are not adding a peak here, just saying "index i contributes nothing,
 * carry forward whatever was best before it."
 *
 * WHY IS dp[1] = {1, cost[1]} AND NOT better(take, skip)?
 * --------------------------------------------------------
 * Index 1 is the very first interior index. There is no index 0
 * that is interior — dp[0] = {0,0} represents "nothing before index 1."
 * Taking index 1 costs cost[1] and gives 1 peak.
 * Skipping index 1 gives {0, 0} — zero peaks.
 * Since maximizing peaks is the primary objective, taking index 1
 * is always better than skipping it (1 peak > 0 peaks regardless of cost).
 * So dp[1] = {1, cost[1]} is always correct as the base case.
 *
 * WHY DOES THE ANSWER LIE AT dp[n-2] AND NOT dp[n-1]?
 * ----------------------------------------------------
 * Interior indices only go from 1 to n-2.
 * Index n-1 is the last element — it has no right neighbor so it
 * can never be a peak. dp[n-2] is the last position we ever fill,
 * and it holds the globally optimal result over all interior indices.
 */

class Solution {
public:
    using ll = long long;

    struct State {
        int peaks;
        ll cost;
    };

    // More peaks always wins because that is the primary objective.
    // Cost only matters when both options produce the same number of peaks —
    // only then do we care about minimizing operations.
    State better(const State& a, const State& b) {
        if (a.peaks != b.peaks) {
            return (a.peaks > b.peaks ? a : b);
        }
        return (a.cost < b.cost ? a : b);
    }

    long long minIncrease(vector<int>& nums) {
        int n = nums.size();

        // WHY PRECOMPUTE COSTS SEPARATELY?
        // Each cost[i] only depends on the original array values of i-1, i, i+1.
        // Importantly, making one index a peak does NOT change the cost of
        // making another index a peak — we only ever raise nums[i] itself,
        // never its neighbors. So costs are independent and safe to precompute.
        // The 1LL* cast is critical: nums values reach 1e9, so
        // max(nums[i-1], nums[i+1]) + 1 can overflow a 32-bit int without it.
        vector<ll> cost(n, 0);
        for (int i = 1; i <= n - 2; i++) {
            cost[i] = max(0LL, 1LL * max(nums[i - 1], nums[i + 1]) + 1 - nums[i]);
        }

        vector<State> dp(n);

        // dp[i] = best answer for candidate peak positions 1..i.
        // Each state stores:
        //   peaks -> the maximum number of peaks we can form in this prefix
        //   cost  -> the minimum total increments needed to achieve that many peaks
        //
        // We only care about indices 1..n-2 because only interior positions
        // can ever become special.

        // Index 0 is never interior — it anchors the DP as an empty state.
        // Think of dp[0] as: "we have considered no valid peak position yet."
        // So the best we can do is 0 peaks with 0 cost.
        // dp[i-2] for i=2 lands here, meaning:
        // "take index 2 as the first peak, with nothing chosen before it."
        dp[0] = {0, 0};

        // First interior index.
        // The two possibilities are:
        //   1. skip index 1 -> {0 peaks, 0 cost}
        //   2. take index 1 -> {1 peak, cost[1]}
        //
        // Since our primary goal is to maximize the number of peaks,
        // taking index 1 always beats skipping it:
        // 1 peak is always better than 0 peaks, no matter what the cost is.
        dp[1] = {1, cost[1]};

        for (int i = 2; i <= n - 2; i++) {

            // SKIP:
            // Do not make index i a peak.
            // Then the best answer for positions 1..i is simply whatever
            // was already best for positions 1..i-1.
            State skip = dp[i - 1];

            // TAKE:
            // Make index i a peak, paying cost[i].
            // Then index i-1 cannot be a peak, because adjacent peaks are impossible.
            // So the best previous state we are allowed to extend is dp[i-2].
            //
            // In other words:
            //   best answer before i that does not conflict with taking i
            //   + one new peak at i
            //   + the increments needed to make i a peak
            State take = {dp[i - 2].peaks + 1, dp[i - 2].cost + cost[i]};

            // Choose the better of the two choices:
            //   skip i
            //   take i
            // "better" means:
            //   1. more peaks is always preferred
            //   2. if peak counts tie, lower cost is preferred
            dp[i] = better(skip, take);
        }

        // After processing all candidate positions 1..n-2,
        // dp[n-2] holds the globally optimal answer.
        return dp[n - 2].cost;
    }
};


/*
 * ============================================================
 * TOP-DOWN (MEMOIZATION) VERSION
 * ============================================================
 *
 * SAME RECURRENCE, OPPOSITE DIRECTION.
 * -------------------------------------
 * Bottom-up builds from index 1 upward, answering:
 *   "what is the best result for indices 1..i?"
 * Top-down starts from n-2 and recurses downward, answering:
 *   "what is the best result for indices 1..i?" on demand.
 * The recurrence and transitions are identical. Only the
 * execution order and caching mechanism differ.
 *
 * WHY USE A SEPARATE vis[] ARRAY INSTEAD OF A SENTINEL IN memo[]?
 * ----------------------------------------------------------------
 * The natural sentinel for "not yet computed" would be {-1, -1} or similar.
 * But {0, 0} is a completely valid and expected State (e.g., for i<=0).
 * There is no State value we can safely reserve as "uncomputed."
 * A separate boolean vis[] removes this ambiguity entirely —
 * vis[i]=false unambiguously means "not yet computed" regardless
 * of what value happens to sit in memo[i].
 *
 * WHY DOES solve(i-2) CORRECTLY HANDLE i=1?
 * ------------------------------------------
 * When i=1, taking it means calling solve(i-2) = solve(-1).
 * The base case i<=0 returns {0,0}, meaning "nothing before index 1,
 * zero peaks, zero cost." This is exactly correct — it represents
 * the empty state before the first interior index.
 * So the base case elegantly handles both i=0 and i=-1 without
 * needing separate checks for the first interior index.
 *
 * WHY IS TOP-DOWN SOMETIMES PREFERABLE HERE?
 * -------------------------------------------
 * In bottom-up we fill every dp[i] from 1 to n-2 unconditionally.
 * In top-down, if the recursion never reaches certain states
 * (e.g., due to early structure in the input), those states are
 * never computed. For this specific problem the savings are minimal
 * since we almost always visit every index, but top-down is often
 * easier to derive directly from the recurrence definition.
 */

class Solution {
public:
    using ll = long long;

    struct State {
        int peaks;
        ll cost;
    };

    vector<ll> incCost;
    vector<State> memo;
    vector<bool> vis;

    State better(const State& a, const State& b) {
        if (a.peaks != b.peaks) {
            return (a.peaks > b.peaks ? a : b);
        }
        return (a.cost < b.cost ? a : b);
    }

    State solve(int i) {
        // i<=0 is the anchor — represents the empty state before
        // any interior index exists. Zero peaks taken, zero cost spent.
        // This correctly handles both the i=0 boundary and the
        // solve(i-2) call when i=1 (which becomes solve(-1)).
        if (i <= 0) return {0, 0};

        if (vis[i]) return memo[i];

        // SKIP: do not make index i a peak.
        // The best result up to i is just the best result up to i-1.
        // We are preserving whatever optimal selection was built before i.
        State skip = solve(i - 1);

        // TAKE: make index i a peak.
        // We jump to i-2 because i-1 might itself be a peak in the
        // optimal solution for solve(i-1). Using i-2 guarantees we
        // never place two peaks on adjacent indices.
        State prev = solve(i - 2);
        State take = {prev.peaks + 1, prev.cost + incCost[i]};

        vis[i] = true;
        return memo[i] = better(skip, take);
    }

    long long minIncrease(vector<int>& nums) {
        int n = nums.size();

        // Costs are independent of each other because we only ever
        // raise nums[i] itself to make it a peak — we never touch
        // its neighbors. So precomputing once from the original
        // array is safe and correct for all DP states.
        incCost.assign(n, 0);
        for (int i = 1; i <= n - 2; i++) {
            incCost[i] = max(0LL, 1LL * max(nums[i - 1], nums[i + 1]) + 1 - nums[i]);
        }

        memo.assign(n, {0, 0});
        vis.assign(n, false);

        // Start from the last interior index n-2 and let the recursion
        // pull in exactly the subproblems it needs going downward.
        // The answer is the cost component of the globally optimal State.
        return solve(n - 2).cost;
    }
};
