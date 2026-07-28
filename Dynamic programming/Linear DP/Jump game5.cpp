/*
 * ============================================================
 * PROBLEM: Jump Game V (LeetCode 1340)
 * ============================================================
 *
 * WHAT THE PROBLEM IS ASKING:
 * ----------------------------
 * You have an array `arr` of integers and a jump limit `d`.
 * From index i, you may jump to index j if ALL of the following hold:
 *   1. |i - j| <= d          (within jump distance)
 *   2. arr[i] > arr[j]       (destination must be strictly shorter)
 *   3. arr[i] > arr[k] for every k strictly between i and j
 *                            (no bar in between can be >= arr[i] — it
 *                             acts as a wall that blocks the jump)
 *
 * You can start at ANY index. Return the maximum number of indices
 * you can visit in a single traversal (including the starting index).
 *
 * IMPORTANT TERMS:
 * ----------------
 * - "Blocked jump": if any intermediate index k has arr[k] >= arr[i],
 *   you cannot jump past it, even if the destination is valid.
 * - "Reachable chain": a sequence of indices i0 -> i1 -> i2 -> ...
 *   where each hop obeys the jump rules above.
 * - dp[i]: the maximum number of indices visitable starting from i.
 *
 * CONSTRAINTS AND IMPLICATIONS:
 * ------------------------------
 * - 1 <= arr.length <= 1000      → O(n * d) per node is fine (n^2 worst case)
 * - 1 <= arr[i] <= 10^5          → values are positive; equal heights block
 * - 1 <= d <= arr.length         → d can be as large as n
 *
 * EDGE CASES:
 * -----------
 * - All equal heights (e.g., [3,3,3,3]): no jump is ever valid → answer = 1
 * - Strictly decreasing array with d=1: every step is valid → answer = n
 * - Single element: answer = 1 trivially
 *
 * WHY NAIVE / GREEDY FAILS:
 * --------------------------
 * Greedy ("always jump to the lowest reachable neighbor") fails because
 * the locally cheapest jump might trap you in a dead end, while a slightly
 * higher jump could unlock a much longer chain.
 *
 * Plain BFS/DFS without memoization is O(n!) in the worst case because
 * from each of the n starting points you might re-explore the same
 * sub-chains exponentially many times.
 *
 * ============================================================
 * THINKING FROM FIRST PRINCIPLES
 * ============================================================
 *
 * STEP 1 — Model it as a graph problem:
 * Each index is a node. Draw a directed edge i → j whenever
 * the jump from i to j is legal. The answer is the length of
 * the longest path in this DAG.
 *
 * STEP 2 — Why is this graph a DAG (no cycles)?
 * Every legal jump goes from a TALLER bar to a SHORTER bar
 * (arr[i] > arr[j] is mandatory). So height strictly decreases
 * along every path. You can never return to a previously visited
 * height, which means cycles are impossible. This is the key
 * structural insight that makes DP valid here.
 *
 * STEP 3 — Optimal substructure on a DAG:
 * Because the graph is a DAG, the longest path from i depends
 * only on the longest paths from i's direct neighbors. Formally:
 *
 *   dp[i] = 1 + max(dp[j]) over all j reachable from i
 *          (or just 1 if i has no outgoing edges)
 *
 * This recurrence has no circular dependency: dp[i] only uses
 * dp[j] where arr[j] < arr[i], so we always recurse toward
 * smaller values — the recursion is guaranteed to terminate.
 *
 * STEP 4 — Memoization makes this efficient:
 * Without it, shared suffixes of chains are recomputed from each
 * of the n starting points → exponential blowup.
 * With memoization, each dp[i] is computed exactly once →
 * the total work is O(n * d).
 *
 * STATE DEFINITION:
 *   dp[i] = max indices visitable starting from index i
 *
 * TRANSITION:
 *   For each valid neighbor j of i:
 *     dp[i] = max(dp[i], 1 + dp[j])
 *
 * BASE CASE:
 *   If i has no valid outgoing edges → dp[i] = 1
 *   (you can always count the starting index itself)
 *
 * ============================================================
 * WHY THIS ALGORITHM IS CORRECT AND OPTIMAL
 * ============================================================
 *
 * CORRECTNESS:
 *   - The DAG property guarantees the recurrence is well-founded.
 *   - Memoization ensures every sub-problem is solved completely
 *     before its result is reused.
 *   - Trying all n starting points ensures we don't miss a chain
 *     that only becomes optimal when started from a tall peak.
 *
 * OPTIMALITY:
 *   - We cannot do better than O(n * d) because in the worst case
 *     every node has d outgoing edges and n nodes must be visited.
 *   - The memoization table eliminates all redundant recomputation,
 *     so no work is wasted.
 *
 * ============================================================
 * IMPLEMENTATION
 * ============================================================
 */

class Solution {
public:
    /*
     * dp[i] = memoized answer for index i.
     * Initialized to -1 to mean "not yet computed".
     * Once computed, it stores the true longest chain from i
     * so any future call to dfs(i) returns instantly.
     */
    vector<int> dp;

    /*
     * n = size of arr, stored as a member to avoid passing it
     * through every recursive call.
     */
    int n;

    /*
     * dfs(i, arr, d):
     *   Returns dp[i] — the maximum number of indices reachable
     *   starting from index i, following all jump rules.
     *
     *   The function explores all valid left-neighbors and all
     *   valid right-neighbors of i, picks the best, memoizes,
     *   and returns.
     */
    int dfs(int i, vector<int>& arr, int d) {

        /*
         * MEMOIZATION CHECK:
         * If dp[i] was already computed in a previous call
         * (from a different starting index that happened to
         * reach i as a sub-problem), return it immediately.
         * This is the core reason the algorithm is efficient:
         * each index is fully solved exactly once.
         */
        if (dp[i] != -1)
            return dp[i];

        /*
         * BASE VALUE = 1:
         * Index i itself is always counted, even if no jump is
         * possible from here. So we start ans at 1 and try to
         * improve it by finding valid neighbors.
         */
        int ans = 1;

        /*
         * LEFT SCAN — explore indices to the LEFT of i.
         *
         * Why iterate from i-1 down to i-d (not the other way)?
         * The "blocking" rule says: if any bar between i and j
         * is >= arr[i], we cannot jump past it. Iterating
         * outward from i means we naturally encounter blocking
         * bars before we encounter farther candidates.
         * The moment we see arr[j] >= arr[i], every index
         * further left is also unreachable (they'd require
         * passing through this blocker), so we `break` — not
         * `continue`. This is critical; using `continue` here
         * would incorrectly allow jumps past blockers.
         */
        for (int j = i - 1; j >= max(0, i - d); j--) {

            /*
             * BLOCKER CHECK:
             * arr[j] >= arr[i] means j is at least as tall as i.
             * Two sub-cases:
             *   (a) arr[j] == arr[i]: equal height — jump NOT
             *       allowed (rule requires strict >), AND this
             *       bar blocks everything further left.
             *   (b) arr[j] >  arr[i]: taller bar — jump not
             *       allowed, AND definitely blocks further left.
             * In both cases `break` is correct.
             */
            if (arr[j] >= arr[i])
                break;

            /*
             * VALID JUMP TO j:
             * arr[j] < arr[i] and no blocker was between i and j,
             * so this jump is legal. Recursively find the longest
             * chain starting from j and extend our current chain
             * by 1 (for index i itself).
             */
            ans = max(ans, 1 + dfs(j, arr, d));
        }

        /*
         * RIGHT SCAN — mirror of the left scan, exploring indices
         * to the RIGHT of i, again outward from i.
         * Same blocking logic applies symmetrically.
         */
        for (int j = i + 1; j <= min(n - 1, i + d); j++) {

            /*
             * Same blocker check for the right direction.
             * Once we see a bar >= arr[i], no jump can cross it.
             */
            if (arr[j] >= arr[i])
                break;

            ans = max(ans, 1 + dfs(j, arr, d));
        }

        /*
         * MEMOIZE AND RETURN:
         * Store the computed answer in dp[i] before returning.
         * Future calls with the same i skip all the above work.
         * The assignment `dp[i] = ans` happens here, not
         * earlier, because `ans` is only finalized after both
         * loops finish.
         */
        return dp[i] = ans;
    }

    int maxJumps(vector<int>& arr, int d) {

        n = arr.size();

        /*
         * Initialize every dp entry to -1 (sentinel for "unsolved").
         * We cannot use 0 as sentinel because 0 is never a valid
         * answer (every index counts at least itself → dp[i] >= 1).
         */
        dp.assign(n, -1);

        /*
         * WHY TRY ALL STARTING POINTS?
         * The longest chain might begin at any index — there is no
         * greedy way to identify the best start without checking all.
         * Crucially, the memoization table is SHARED across all
         * starting-point calls: if dfs(5) was already solved while
         * processing start=8, calling dfs(5) for start=5 is O(1).
         * So the overhead of trying all n starts is just n lookups
         * for the already-solved sub-problems.
         */
        int res = 1;

        for (int i = 0; i < n; i++) {
            res = max(res, dfs(i, arr, d));
        }

        return res;
    }
};

/*
 * ============================================================
 * DRY RUN
 * ============================================================
 *
 * arr = [3, 1, 2], d = 2
 * Indices:  0   1   2
 *
 * Call dfs(0): arr[0]=3
 *   Left scan: j from -1 → nothing (max(0, 0-2) = 0, loop doesn't execute)
 *   Right scan:
 *     j=1: arr[1]=1 < 3 → valid. Call dfs(1):
 *       dfs(1): arr[1]=1
 *         Left:  j=0: arr[0]=3 >= 1 → BREAK (blocked)
 *         Right: j=2: arr[2]=2 >= 1 → BREAK (blocked)
 *         dp[1] = 1. Return 1.
 *     ans = max(1, 1+1) = 2
 *     j=2: arr[2]=2 < 3 → valid. Call dfs(2):
 *       dfs(2): arr[2]=2
 *         Left:  j=1: arr[1]=1 < 2 → valid. dfs(1) = 1 (memoized!)
 *                ans = max(1, 1+1) = 2
 *                j=0: arr[0]=3 >= 2 → BREAK
 *         Right: j=3: out of bounds → loop doesn't run
 *         dp[2] = 2. Return 2.
 *     ans = max(2, 1+2) = 3
 *   dp[0] = 3. Return 3.
 *
 * Call dfs(1): already memoized → return 1
 * Call dfs(2): already memoized → return 2
 *
 * res = max(3, 1, 2) = 3  ✓
 * (Chain: 0 → 2 → 1  i.e. arr values 3 → 2 → 1, all strictly decreasing)
 *
 * ============================================================
 * COMPLEXITY ANALYSIS
 * ============================================================
 *
 * TIME COMPLEXITY: O(n * d)
 * --------------------------
 * - There are n distinct states (one per index).
 * - Each state is computed exactly once (memoization).
 * - Computing one state requires scanning at most d neighbors
 *   to the left and d to the right → O(d) work per state.
 * - Total: O(n * d).
 * - Worst case when d = n: O(n^2), which for n=1000 is 10^6 — fine.
 *
 * SPACE COMPLEXITY: O(n)
 * -----------------------
 * - dp[] array: O(n)
 * - Recursion call stack: in the worst case (strictly decreasing
 *   array, d=1), the recursion depth is n → O(n) stack frames.
 * - Total auxiliary space: O(n).
 *
 * WHY THIS IS OPTIMAL:
 * ---------------------
 * Any algorithm must, at minimum, determine dp[i] for every index i
 * (otherwise it might miss the globally optimal starting point).
 * Computing dp[i] inherently requires inspecting i's neighbors.
 * So Ω(n) is a lower bound; O(n * d) is essentially tight since
 * each edge (i, j) in the jump graph must be inspected at least once.
 * The memoization ensures no edge is inspected more than once.
 */