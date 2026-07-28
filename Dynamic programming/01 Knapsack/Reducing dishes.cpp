/*
 * 1402. Reducing Dishes
 *
 * ─────────────────────────────────────────────────────
 * PROBLEM EXPLANATION
 * ─────────────────────────────────────────────────────
 * You have n dishes, each with a satisfaction value (can be negative).
 * You cook dishes one at a time. The dish cooked at position t gets
 * multiplier t (1-indexed). You can skip any dishes entirely.
 *
 * Like-time coefficient = sum of (position * satisfaction) for chosen dishes.
 *
 * Goal: choose a subset of dishes and arrange them to maximize total
 * like-time coefficient.
 *
 * Important terms:
 *   - "position" here means the order slot (1, 2, 3...) among CHOSEN dishes only.
 *     Skipped dishes don't consume a slot.
 *   - You are not forced to cook any dish. Answer can be 0 (cook nothing).
 *
 * Constraints and implications:
 *   - 1 <= n <= 500, -1000 <= satisfaction[i] <= 1000
 *   - n is small enough for O(n^2) DP.
 *   - Negative satisfaction dishes can still be worth including if they
 *     push a high-value dish to a higher multiplier slot — but only
 *     if the net gain is positive.
 *
 * Edge cases:
 *   - All negative: answer is 0 (cook nothing).
 *   - All positive: cook all in ascending order.
 *
 * ─────────────────────────────────────────────────────
 * THINKING FROM FIRST PRINCIPLES
 * ─────────────────────────────────────────────────────
 * Step 1 — Order of chosen dishes matters.
 *   Among chosen dishes, higher satisfaction should get higher multiplier.
 *   So the optimal arrangement of any chosen subset is always ascending order.
 *   This means we never need to think about permutations — just WHICH dishes
 *   to pick. Sort once, then only decide include/exclude for each dish.
 *
 * Step 2 — After sorting ascending, what's the decision space?
 *   For each dish i (0-indexed), either include it or skip it.
 *   If included, its multiplier = how many dishes have been selected up to
 *   and including it. This is the key dependency: the multiplier of dish i
 *   is not fixed — it depends on how many dishes before it were also selected.
 *
 * Step 3 — Why we need 2D DP.
 *   A 1D state dp[i] = "best value using first i dishes" is insufficient
 *   because it doesn't tell us how many dishes were selected among those i,
 *   which determines the multiplier for dish i if included.
 *   We need to track COUNT of selected dishes explicitly.
 *
 * Step 4 — State definition.
 *   dp[i][k] = maximum like-time coefficient considering the first i dishes
 *              (after sorting), where exactly k dishes have been selected.
 *
 *   If dish i is the k-th selected dish, it sits at position k, so its
 *   contribution is satisfaction[i-1] * k.
 *
 * Step 5 — Transition.
 *   For dish i (1-indexed), having selected k dishes total:
 *     Option A — skip dish i:
 *       dp[i][k] = dp[i-1][k]
 *       (k dishes selected from first i-1 dishes, dish i not added)
 *
 *     Option B — include dish i as the k-th selected dish:
 *       dp[i][k] = dp[i-1][k-1] + satisfaction[i-1] * k
 *       (k-1 dishes selected from first i-1 dishes, dish i becomes k-th)
 *
 *   Take the max of both options.
 *
 * Step 6 — Base case.
 *   dp[0][0] = 0: considering 0 dishes with 0 selected — value is 0.
 *   dp[i][0] = 0 for all i: selecting 0 dishes always gives value 0.
 *   Everything else initialized to INT_MIN (invalid/unreachable state).
 *
 * Step 7 — Answer.
 *   After filling the table, dp[n][k] for any k gives the best value
 *   when exactly k dishes are chosen from all n. Scan all k and take max.
 *   Also consider 0 (cooking nothing is always valid).
 *
 * ─────────────────────────────────────────────────────
 * WHY THIS APPROACH WORKS
 * ─────────────────────────────────────────────────────
 * Sorting ascending is correct because: among any fixed subset, the
 * arrangement that maximizes sum(pos * val) always puts larger values
 * at larger positions. This is provable by exchange argument — swapping
 * any out-of-order pair strictly increases the sum if values differ.
 *
 * The DP is correct because:
 *   - Every possible subset is represented by some (i, k) path through the table.
 *   - The transition covers all cases (skip or include) exhaustively.
 *   - INT_MIN guards prevent using unreachable states (e.g., selecting k dishes
 *     from only i < k candidates).
 *
 * ─────────────────────────────────────────────────────
 */

class Solution {
public:
    int maxSatisfaction(vector<int>& satisfaction) {
        int n = satisfaction.size();

        // Sort ascending so that if we select any subset,
        // dishes naturally appear in the optimal order (low to high).
        // Higher satisfaction dishes will land at higher multiplier positions.
        sort(satisfaction.begin(), satisfaction.end());

        // dp[i][k] = best like-time sum using first i dishes with exactly k selected.
        // Size (n+1) x (n+1): i ranges 0..n, k ranges 0..n.
        // INT_MIN marks states that are impossible/unreachable.
        // We use INT_MIN instead of -infinity to detect invalid transitions —
        // if dp[i-1][k-1] is INT_MIN, it means we can't have selected k-1 dishes
        // from i-1 candidates (e.g., k-1 > i-1), so including dish i as k-th
        // is illegal and must be skipped.
        vector<vector<int>> dp(n + 1, vector<int>(n + 1, INT_MIN));

        // Base: 0 dishes considered, 0 selected — trivially valid, value = 0.
        dp[0][0] = 0;

        for (int i = 1; i <= n; i++) {
            // Selecting 0 dishes from first i dishes is always valid with value 0.
            // This anchors the "skip everything" path for every row.
            dp[i][0] = 0;

            // k cannot exceed i (can't select more dishes than considered so far).
            for (int k = 1; k <= i; k++) {

                // Option A: skip dish i.
                // The count of selected dishes stays k, coming entirely from first i-1 dishes.
                // Inherit whatever was best with k dishes from i-1 candidates.
                dp[i][k] = dp[i - 1][k];

                // Option B: include dish i as the k-th selected dish.
                // We need exactly k-1 dishes selected from the first i-1 dishes (reachable state).
                // Dish i sits at position k, contributing satisfaction[i-1] * k.
                // Guard: dp[i-1][k-1] == INT_MIN means this state was never reachable — skip.
                if (dp[i - 1][k - 1] != INT_MIN) {
                    dp[i][k] = max(
                        dp[i][k],
                        dp[i - 1][k - 1] + satisfaction[i - 1] * k
                        // satisfaction[i-1]: 0-indexed dish value for 1-indexed dish i.
                        // * k: this dish is the k-th selected, so multiplier = k.
                    );
                }
            }
        }

        // Scan all possible counts k of selected dishes.
        // dp[n][k] = best value when exactly k dishes are chosen from all n.
        // Start ans = 0 because cooking nothing (k=0) is always a valid choice.
        int ans = 0;
        for (int k = 0; k <= n; k++) {
            ans = max(ans, dp[n][k]);
        }

        return ans;
    }
};

/*
 * ─────────────────────────────────────────────────────
 * DRY RUN
 * ─────────────────────────────────────────────────────
 * Input: satisfaction = [-1, 0, 5]  (already sorted ascending)
 * n = 3
 *
 * Initial table (INT_MIN shown as X):
 *       k=0  k=1  k=2  k=3
 * i=0 [  0    X    X    X ]
 * i=1 [  X    X    X    X ]
 * i=2 [  X    X    X    X ]
 * i=3 [  X    X    X    X ]
 *
 * i=1, dish = satisfaction[0] = -1:
 *   k=0: dp[1][0] = 0
 *   k=1: skip  = dp[0][1] = X
 *         take  = dp[0][0] + (-1)*1 = 0 + (-1) = -1
 *         dp[1][1] = -1
 *
 *       k=0  k=1  k=2  k=3
 * i=1 [  0   -1    X    X ]
 *
 * i=2, dish = satisfaction[1] = 0:
 *   k=0: dp[2][0] = 0
 *   k=1: skip  = dp[1][1] = -1
 *         take  = dp[1][0] + 0*1 = 0
 *         dp[2][1] = max(-1, 0) = 0
 *   k=2: skip  = dp[1][2] = X
 *         take  = dp[1][1] + 0*2 = -1 + 0 = -1
 *         dp[2][2] = -1
 *
 *       k=0  k=1  k=2  k=3
 * i=2 [  0    0   -1    X ]
 *
 * i=3, dish = satisfaction[2] = 5:
 *   k=0: dp[3][0] = 0
 *   k=1: skip  = dp[2][1] = 0
 *         take  = dp[2][0] + 5*1 = 5
 *         dp[3][1] = 5
 *   k=2: skip  = dp[2][2] = -1
 *         take  = dp[2][1] + 5*2 = 0 + 10 = 10
 *         dp[3][2] = 10
 *   k=3: skip  = dp[2][3] = X
 *         take  = dp[2][2] + 5*3 = -1 + 15 = 14
 *         dp[3][3] = 14
 *
 *       k=0  k=1  k=2  k=3
 * i=3 [  0    5   10   14 ]
 *
 * Answer = max(0, 5, 10, 14) = 14
 * Corresponds to selecting all 3: -1*1 + 0*2 + 5*3 = 14. Correct.
 *
 * ─────────────────────────────────────────────────────
 * COMPLEXITY
 * ─────────────────────────────────────────────────────
 * Time:
 *   - Sorting: O(n log n)
 *   - DP table: outer loop i from 1..n, inner loop k from 1..i
 *     → total iterations = 1 + 2 + ... + n = n(n+1)/2 = O(n^2)
 *   - Final scan: O(n)
 *   Total: O(n^2)
 *
 * Space:
 *   - DP table: (n+1) x (n+1) = O(n^2)
 *   - Can be reduced to O(n) with two 1D arrays (current and previous row)
 *     since dp[i] depends only on dp[i-1].
 *
 * Is this optimal?
 *   For this DP formulation, O(n^2) is optimal — you must consider all
 *   (dish, count) pairs. The greedy suffix approach reduces it to O(n log n)
 *   by exploiting the structural observation that the optimal solution is
 *   always a contiguous suffix of the sorted array, eliminating the k dimension.
 * ─────────────────────────────────────────────────────
 */

 class Solution {
public:
    int n;
    vector<vector<int>> dp;

    int solve(int i, int cnt, vector<int>& satisfaction) {
        if (i == n) return 0;

        if (dp[i][cnt] != -1)
            return dp[i][cnt];

        int skip = solve(i + 1, cnt, satisfaction);

        int take = satisfaction[i] * (cnt + 1) +
                   solve(i + 1, cnt + 1, satisfaction);

        return dp[i][cnt] = max(skip, take);
    }

    int maxSatisfaction(vector<int>& satisfaction) {
        sort(satisfaction.begin(), satisfaction.end());

        n = satisfaction.size();
        dp.assign(n + 1, vector<int>(n + 1, -1));

        return solve(0, 0, satisfaction);
    }
};