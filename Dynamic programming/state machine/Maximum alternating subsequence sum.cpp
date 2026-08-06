/*
 * PROBLEM:
 * Given array nums[], choose any subsequence (preserve relative order,
 * skip any elements). Reindex the chosen subsequence from 0.
 * Compute its alternating sum: add elements at even indices (0,2,4,...),
 * subtract elements at odd indices (1,3,5,...).
 * Return the maximum possible alternating sum over all subsequences.
 *
 * INPUT:  nums[] of positive integers (1 <= nums[i] <= 10^5, length <= 10^5)
 * OUTPUT: single long long — the maximum alternating sum achievable
 *
 * WHY BRUTE FORCE FAILS:
 * There are 2^n possible subsequences. For n=10^5 this is astronomically
 * large. We need to exploit structure to avoid evaluating each one.
 *
 * KEY OBSERVATION:
 * At every index we make a binary choice: take or skip.
 * The sign applied to a taken element depends only on how many elements
 * we've taken before it — specifically, whether that count is even (add)
 * or odd (subtract). We don't need the full history of what we picked;
 * we only need to track: "if I take the next element, will it be added
 * or subtracted?" This is a single boolean — and it only flips when we
 * actually take an element, not when we skip.
 *
 * FROM OBSERVATION TO APPROACH:
 * Since the only state that matters beyond the current index is that one
 * boolean (add-next vs subtract-next), we get a 2-state DP over indices.
 * Total states = n * 2 = O(n), each computed in O(1) → O(n) overall.
 *
 * DP STATE DEFINITION:
 * dp[i][0] = best alternating sum using elements from index 0..i,
 *            where the last element we picked was ADDED (even position)
 *
 * dp[i][1] = best alternating sum using elements from index 0..i,
 *            where the last element we picked was SUBTRACTED (odd position)
 *
 * TRANSITIONS:
 * dp[i][0] = max(dp[i-1][1] + nums[i],  // TAKE: add nums[i]; prior last-pick was subtracted
 *               dp[i-1][0])              // SKIP: last added pick is still somewhere in 0..i-1
 *
 * dp[i][1] = max(dp[i-1][0] - nums[i],  // TAKE: subtract nums[i]; prior last-pick was added
 *               dp[i-1][1])              // SKIP: last subtracted pick is still in 0..i-1
 *
 * BASE CASES:
 * dp[0][0] = nums[0]  → took nums[0], it sits at even index 0, so add it
 * dp[0][1] = 0        → to have a subtracted last-pick at index 0, we'd subtract
 *                        nums[0] as the very first element. Since all values > 0,
 *                        this is never optimal. Best = pick nothing = 0.
 *
 * ANSWER: dp[n-1][0]
 * We want the last picked element to have been added, never subtracted.
 * Since all nums[i] > 0, ending on a subtraction always hurts — we can
 * always improve by dropping that last subtracted element from the subsequence.
 *
 * TOP-DOWN maps this identically: flag=true means "next take will be added",
 * flag=false means "next take will be subtracted". Recurse on index, memoize
 * on (index, flag).
 */


// ============================================================
// APPROACH 1: TOP-DOWN MEMOIZATION
// ============================================================

class Solution1 {
public:
#define ll long long

    // index: which element of nums we are currently deciding on
    // flag:  true  → if we take nums[index], it gets ADDED   (+)
    //        false → if we take nums[index], it gets SUBTRACTED (-)
    //        flag only flips when we actually take; skipping leaves it unchanged
    // dp:    memoization table indexed by [index][flag]
    ll solve(int index, vector<int>& nums, bool flag, vector<array<ll, 2>>& dp) {

        ll n = nums.size();

        // No elements left to decide on; contribute 0 to the sum
        if (index >= n) return 0;

        // Already solved this (index, flag) state — reuse it
        // Without this, overlapping subproblems cause exponential recomputation
        if (dp[index][flag] != -1) return dp[index][flag];

        // OPTION 1 — SKIP nums[index]:
        // We don't place it in the subsequence, so flag doesn't change —
        // the next element we might take still needs the same sign
        ll skip = solve(index + 1, nums, flag, dp);

        // OPTION 2 — TAKE nums[index]:
        // Determine the signed contribution based on current flag
        ll val = nums[index];
        if (flag == false) val = -val;  // odd position → subtract

        // After taking, the next element's sign must flip (!flag)
        ll take = solve(index + 1, nums, !flag, dp) + val;

        // Store the better choice so future calls to this state are O(1)
        return dp[index][flag] = max(skip, take);
    }

    long long maxAlternatingSum(vector<int>& nums) {
        ll n = nums.size();

        // dp[i][0] and dp[i][1] both start as -1 meaning "not yet computed"
        vector<array<ll, 2>> dp(n, {-1, -1});

        // Begin at index 0 with flag=true: the first element we take will be added
        return solve(0, nums, true, dp);
    }
};


// ============================================================
// APPROACH 2: BOTTOM-UP TABULATION
// ============================================================

class Solution2 {
public:
    long long maxAlternatingSum(vector<int>& nums) {
        int n = nums.size();

        // dp[i][0]: best alternating sum over elements 0..i where last picked was ADDED
        // dp[i][1]: best alternating sum over elements 0..i where last picked was SUBTRACTED
        // Using array<long long,2> to avoid map/pair overhead
        vector<array<long long, 2>> dp(n);

        // BASE CASES at index 0 (only nums[0] available):
        // [0]: we took nums[0] and added it — it's the first pick, sits at even index 0
        dp[0][0] = nums[0];
        // [1]: having a subtracted last-pick at index 0 means we subtracted nums[0]
        //      as the very first element. All values > 0, so this is always worse
        //      than picking nothing. "Picked nothing" earns 0, which is our base.
        dp[0][1] = 0;

        for (int i = 1; i < n; i++) {

            // dp[i][0]: best sum where nums[i] or some earlier element is the last ADDED pick
            //
            // TAKE nums[i] and ADD it:
            //   Before this pick, the last pick was subtracted → dp[i-1][1]
            //   Adding nums[i] on top: dp[i-1][1] + nums[i]
            //
            // SKIP nums[i]:
            //   The last added element is unchanged, still somewhere in 0..i-1 → dp[i-1][0]
            dp[i][0] = max(dp[i-1][1] + nums[i],
                           dp[i-1][0]);

            // dp[i][1]: best sum where nums[i] or some earlier element is the last SUBTRACTED pick
            //
            // TAKE nums[i] and SUBTRACT it:
            //   Before this pick, the last pick was added → dp[i-1][0]
            //   Subtracting nums[i]: dp[i-1][0] - nums[i]
            //
            // SKIP nums[i]:
            //   The last subtracted element is unchanged → dp[i-1][1]
            dp[i][1] = max(dp[i-1][0] - nums[i],
                           dp[i-1][1]);
        }

        // dp[n-1][0]: best sum where the final picked element was added.
        // dp[n-1][1] is always <= dp[n-1][0] because all nums[i] > 0 —
        // any subsequence ending in a subtraction can be improved by
        // dropping that last element, which lands us in the [0] state.
        return dp[n-1][0];
    }
};