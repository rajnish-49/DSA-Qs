/*
 * ============================================================
 * PROBLEM: Jump Game VII (LeetCode 1871)
 * ============================================================
 *
 * WHAT THE PROBLEM IS ASKING:
 * ----------------------------
 * You stand at index 0 of a binary string s (guaranteed '0').
 * From index i, you may jump to index j if:
 *   1. i + minJump <= j <= min(i + maxJump, n-1)   (jump window)
 *   2. s[j] == '0'                                  (must land on '0')
 *
 * Return true if you can reach index n-1, false otherwise.
 *
 * IMPORTANT TERMS:
 * ----------------
 * - "Jump window of i": the range of indices [i+minJump, i+maxJump]
 *   that index i can reach in one jump.
 * - dp[i]: true if index i is reachable from index 0 following the rules.
 * - "possible": a running count of how many reachable indices currently
 *   have i in their jump window — i.e., how many valid predecessors
 *   can reach i. Acts as a sliding window sum over dp[].
 *
 * CONSTRAINTS AND IMPLICATIONS:
 * ------------------------------
 * - s.length up to 10^5  → O(n) solution required; O(n^2) is too slow.
 * - s[0] == '0'          → starting point is always valid.
 * - s[i] ∈ {'0','1'}     → only '0' cells are landable.
 * - 1 <= minJump <= maxJump < n → window is always non-empty and
 *                                 never reaches out of bounds on the right.
 *
 * EDGE CASES:
 * -----------
 * - s.length == 2, s = "00", minJump=1, maxJump=1 → true (direct jump)
 * - s[n-1] == '1' → always false (can't land there)
 * - All '1's except index 0 → false
 *
 * WHY NAIVE APPROACH FAILS:
 * --------------------------
 * Naive: for every index i where dp[i]=true, iterate over its entire
 * jump window [i+minJump, i+maxJump] and mark those '0' cells reachable.
 * This is O(n * (maxJump - minJump + 1)) which is O(n^2) in the worst
 * case (e.g. minJump=1, maxJump=n). For n=10^5 that's 10^10 — too slow.
 *
 * ============================================================
 * THINKING FROM FIRST PRINCIPLES
 * ============================================================
 *
 * STEP 1 — Frame it as reachability DP:
 *   dp[i] = "can I reach index i from index 0?"
 *   dp[0] = true (given).
 *   dp[i] = true  iff  s[i]=='0'  AND  exists some j in [i-maxJump, i-minJump]
 *                                        where dp[j] == true.
 *
 *   Read the window backwards: "who can reach me?" — any reachable index j
 *   that is between i-maxJump and i-minJump (inclusive) could have jumped
 *   to i.
 *
 * STEP 2 — The bottleneck is the range query:
 *   For each i, we need: "is there any true value in dp[i-maxJump .. i-minJump]?"
 *   Scanning that range naively is O(window size) per index → O(n^2) total.
 *
 * STEP 3 — Key insight: sliding window COUNT replaces range scan:
 *   Instead of scanning the window for each i, maintain a running variable
 *   `possible` = number of TRUE entries currently inside the window
 *                [i-maxJump .. i-minJump].
 *
 *   As i advances by 1:
 *   - The window's RIGHT edge moves: i-minJump increases by 1.
 *     The NEW element entering the window's right is index (i-minJump).
 *     If dp[i-minJump] is true → possible++.
 *   - The window's LEFT edge moves: i-maxJump increases by 1.
 *     The element LEAVING the window on the left was (i-maxJump-1).
 *     If dp[i-maxJump-1] was true → possible--.
 *
 *   After this update, possible > 0 means "at least one reachable
 *   predecessor exists in the window" → dp[i] can be true (if s[i]=='0').
 *
 *   This converts O(window) per step → O(1) per step → O(n) total.
 *
 * WHY THE SLIDING WINDOW IS VALID HERE (not just a trick):
 *   The window [i-maxJump, i-minJump] shifts by exactly 1 for each
 *   unit increase in i. Each element enters the window exactly once
 *   (on the right) and leaves exactly once (on the left). So the
 *   incremental +/- bookkeeping is perfectly accurate at every step.
 *
 * ============================================================
 * WHY THIS ALGORITHM IS CORRECT AND OPTIMAL
 * ============================================================
 *
 * CORRECTNESS:
 *   dp[i] = true  iff  s[i]=='0' AND possible > 0 at step i.
 *   `possible` exactly counts true entries in [i-maxJump, i-minJump],
 *   which is exactly the set of predecessors that can jump to i.
 *   So dp[i] is set true iff i is genuinely reachable. No false
 *   positives (possible=0 means truly no valid predecessor), no false
 *   negatives (any valid true predecessor increments possible).
 *
 * OPTIMALITY:
 *   We must inspect each index at least once → Ω(n) lower bound.
 *   This algorithm does O(1) work per index → O(n) total. Optimal.
 *
 * ============================================================
 * IMPLEMENTATION
 * ============================================================
 */

class Solution {
public:
    bool canReach(string s, int minJump, int maxJump) {

        int n = s.size();

        /*
         * dp[i] = true means index i is reachable from index 0.
         * We initialize all to false and set dp[0]=true because
         * the problem guarantees s[0]=='0' and we start there.
         */
        vector<bool> dp(n, false);
        dp[0] = true;

        /*
         * `possible` = number of dp[j]=true entries currently in
         * the window [i-maxJump, i-minJump] as we process index i.
         *
         * Initialized to 0 before the loop. The loop begins at i=1,
         * and the window for i=1 is [1-maxJump, 1-minJump], which
         * for minJump >= 1 has its right edge at 1-minJump <= 0.
         * The first meaningful entry (dp[0]) enters at i=minJump.
         */
        int possible = 0;

        for (int i = 1; i < n; i++) {

            /*
             * RIGHT EDGE ENTERS THE WINDOW:
             * When processing index i, the rightmost predecessor that
             * could jump to i is at i-minJump (since j + minJump <= i
             * means j <= i-minJump). So index (i-minJump) just became
             * a valid predecessor as we moved from i-1 to i.
             *
             * We add dp[i-minJump] to `possible` if it's in bounds.
             * This is the new index sliding INTO the window on the right.
             *
             * WHY i-minJump and not i-minJump+1?
             * At step i-1, the right edge was (i-1)-minJump = i-minJump-1.
             * Now at step i, it's i-minJump. So exactly one new index
             * enters: i-minJump.
             */
            if (i - minJump >= 0 && dp[i - minJump]) {
                possible++;
            }

            /*
             * LEFT EDGE LEAVES THE WINDOW:
             * The leftmost predecessor that could jump to i is i-maxJump
             * (since j + maxJump >= i means j >= i-maxJump).
             * The index that just slid OUT of the window (it was valid
             * for i-1 but not for i) is (i-1)-maxJump = i-maxJump-1.
             *
             * If dp[i-maxJump-1] was true, we must decrement `possible`
             * because that predecessor is no longer in range for index i.
             *
             * WHY subtract at i-maxJump-1 and not i-maxJump?
             * At step i, the left edge of the window is i-maxJump.
             * At step i-1, it was i-1-maxJump = i-maxJump-1.
             * So the index that just LEFT is i-maxJump-1.
             */
            if (i - maxJump - 1 >= 0 && dp[i - maxJump - 1]) {
                possible--;
            }

            /*
             * REACHABILITY DECISION:
             * Index i is reachable iff:
             *   (a) s[i] == '0': we are allowed to land here
             *   (b) possible > 0: at least one reachable predecessor
             *       exists in the window [i-maxJump, i-minJump]
             *
             * Note: we check s[i]=='0' AFTER updating `possible`
             * because `possible` must be accurate regardless — even
             * if s[i]=='1' (we skip marking dp[i]=true, but the
             * window bookkeeping still happens, keeping future
             * iterations correct).
             *
             * If s[i]=='1', dp[i] stays false (can't land here).
             * We do NOT short-circuit the window updates for '1'
             * cells — that would corrupt `possible` for future steps.
             */
            if (s[i] == '0' && possible > 0) {
                dp[i] = true;
            }
        }

        /*
         * The answer is simply whether the last index is reachable.
         */
        return dp[n - 1];
    }
};

/*
 * ============================================================
 * DRY RUN
 * ============================================================
 *
 * s = "011010", minJump=2, maxJump=3
 * n=6, dp = [T, F, F, F, F, F], possible=0
 *
 * i=1: right edge: 1-2=-1 → skip. Left edge: 1-3-1=-3 → skip.
 *      s[1]='1' → dp[1] stays F.   dp=[T,F,F,F,F,F]  possible=0
 *
 * i=2: right: 2-2=0, dp[0]=T → possible++ → possible=1
 *      left:  2-3-1=-2 → skip.
 *      s[2]='1' → dp[2] stays F.   dp=[T,F,F,F,F,F]  possible=1
 *
 * i=3: right: 3-2=1, dp[1]=F → no change.  possible=1
 *      left:  3-3-1=-1 → skip.
 *      s[3]='0' AND possible=1>0 → dp[3]=T  dp=[T,F,F,T,F,F]
 *
 * i=4: right: 4-2=2, dp[2]=F → no change.  possible=1
 *      left:  4-3-1=0, dp[0]=T → possible-- → possible=0
 *      s[4]='1' → dp[4] stays F.   dp=[T,F,F,T,F,F]  possible=0
 *
 * i=5: right: 5-2=3, dp[3]=T → possible++ → possible=1
 *      left:  5-3-1=1, dp[1]=F → no change.  possible=1
 *      s[5]='0' AND possible=1>0 → dp[5]=T  dp=[T,F,F,T,F,T]
 *
 * return dp[5] = true  ✓
 *
 * Reachability chain: 0 → 3 → 5
 *
 * ============================================================
 * COMPLEXITY ANALYSIS
 * ============================================================
 *
 * TIME COMPLEXITY: O(n)
 * ----------------------
 * - The main loop runs exactly n-1 iterations.
 * - Each iteration does O(1) work: two boundary checks and
 *   at most two increments/decrements to `possible`.
 * - No inner loop, no range scan.
 * - Total: O(n).
 *
 * SPACE COMPLEXITY: O(n)
 * -----------------------
 * - dp[] array of size n: O(n).
 * - `possible` is a single integer: O(1).
 * - No recursion stack.
 * - Total: O(n).
 *   (Could be reduced to O(1) extra space if we only need the
 *   final boolean, but storing dp[] is necessary here because
 *   past dp values are queried by the sliding window.)
 *
 * WHY THIS IS OPTIMAL:
 * ---------------------
 * Reading the input string alone takes O(n), so no algorithm
 * can do better than O(n). This solution matches that lower
 * bound. The sliding window converts what would be an O(n^2)
 * range-query DP into an O(n) one by amortizing the window
 * update cost: each index enters and leaves `possible` exactly
 * once across the entire run → O(n) total updates.
 */