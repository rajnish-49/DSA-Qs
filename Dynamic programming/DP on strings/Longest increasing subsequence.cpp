// ==================== PROBLEM: LONGEST INCREASING SUBSEQUENCE (LIS) ====================
// 
// Given: An array of integers nums = [10, 9, 2, 5, 3, 7, 101, 18]
// Find: Length of the longest STRICTLY INCREASING subsequence
// 
// KEY CONCEPTS:
// 1. SUBSEQUENCE vs SUBARRAY:
//    - Subarray: contiguous elements [9, 2, 5] ✓
//    - Subsequence: can skip elements but maintain ORDER [10, 5, 7, 101] ✓
//    - We can pick elements non-contiguously: pick index 0, skip 1, pick 3, skip 4, pick 5...
// 
// 2. STRICTLY INCREASING:
//    - Each element must be GREATER (not equal) than the previous
//    - [1, 2, 2, 3] → can't use both 2's, need [1, 2, 3]
// 
// 3. WE ONLY NEED LENGTH, not the actual subsequence
// 
// EXAMPLE WALKTHROUGH:
// nums = [10, 9, 2, 5, 3, 7, 101, 18]
// Possible subsequences:
// - [2, 5, 7, 101] → length 4
// - [2, 3, 7, 18] → length 4
// - [2, 5, 7, 18] → length 4
// Answer: 4
//
// THE CORE CHALLENGE:
// At each position, we have a CHOICE: include this element or skip it
// If we include it, we must ensure it's greater than the last element we picked
// This creates overlapping subproblems → perfect for Dynamic Programming!

// ==================== APPROACH 1: MEMOIZATION (TOP-DOWN DP) ====================
//
// THINKING PROCESS:
// Imagine you're at index 0, looking at the entire array ahead of you.
// You ask: "What's the longest subsequence I can build from here?"
// To answer this, you need to:
//   1. Decide if you should take the current element
//   2. If you take it, recursively solve "what's the best from the next position?"
//   3. If you skip it, recursively solve "what's the best from the next position?"
//   4. Take the maximum of these two choices
//
// THE KEY INSIGHT:
// You need to remember WHAT WAS THE LAST ELEMENT YOU PICKED
// Why? Because you can only pick current element if it's GREATER than the last one
// So our state needs TWO pieces of information:
//   - Current index (idx): where we are now
//   - Previous index (p): what was the last element we added to our subsequence
//
// RECURRENCE RELATION:
// solve(idx, p) = max(
//     1 + solve(idx+1, idx),  // if we TAKE current element (when valid)
//     solve(idx+1, p)          // if we SKIP current element
// )
//
// BASE CASE: if idx >= n, we've exhausted all elements, return 0

class Solution
{
public:
    int solve(vector<int> &nums, vector<vector<int>> &dp, int idx, int p)
    {
        int n = nums.size();

        // BASE CASE: We've reached beyond the array
        // No more elements to consider, so subsequence length contribution is 0
        if (idx >= n)
        {
            return 0;
        }

        // MEMOIZATION CHECK: Have we already computed this state?
        // State = (idx, p): "LIS starting from idx, with p as last picked element"
        // 
        // Special case: p == -1 means "no previous element yet" (we're starting fresh)
        // We don't memoize p=-1 because:
        //   - It only happens once at the very first call
        //   - Array indices start from 0, so -1 indicates "beginning state"
        //   - If we tried dp[idx][-1], that's invalid array access!
        if (p != -1 && dp[idx][p] != -1)
        {
            return dp[idx][p];  // Already computed, return cached result
        }

        // DECISION BRANCH 1: Can we TAKE the current element?
        // Initialize to 0 (assuming we can't take it by default)
        int take = 0;
        
        // CONDITIONS TO TAKE nums[idx]:
        // Condition 1: p == -1 
        //   → No previous element yet, so this is our FIRST element
        //   → We can always start a subsequence with any element
        //   → Example: starting [10, ...] we can pick 10 as first element
        //
        // Condition 2: nums[idx] > nums[p]
        //   → Current element is STRICTLY GREATER than last picked element
        //   → This maintains the "increasing" property of our subsequence
        //   → Example: if last picked was 5, we can only pick 6, 7, 8... (not 5 or less)
        if (p == -1 || nums[idx] > nums[p])
        {
            // CHOICE: TAKE the current element
            // What happens when we take nums[idx]?
            //   1. Add 1 to our subsequence length (we're including this element)
            //   2. Move to next position: idx + 1 (examine next element)
            //   3. Update "previous" to current: p becomes idx
            //      Why? Because nums[idx] is now the "last element we picked"
            //      Future elements must be compared against nums[idx]
            //
            // Example: nums = [2, 5, 3, 7], we're at idx=1 (value 5), p=0 (value 2)
            //   take = 1 + solve(nums, dp, 2, 1)
            //   We count '5', move to idx=2, and now compare against nums[1]=5
            take = 1 + solve(nums, dp, idx + 1, idx);
        }

        // DECISION BRANCH 2: SKIP the current element
        // Why would we skip? Several reasons:
        //   1. Current element is too small (not greater than previous)
        //   2. Even if valid, maybe skipping gives us a better subsequence later
        //   3. Example: [10, 9, 2, 5] at idx=0 (value 10)
        //      If we take 10, we can only pick 101 later
        //      If we skip 10, we can build [2, 5, 7, 18] which might be longer!
        //
        // What happens when we skip nums[idx]?
        //   1. Don't add to length (no +1)
        //   2. Move to next position: idx + 1 (examine next element)
        //   3. Keep same previous: p stays unchanged
        //      Why? Because we didn't pick this element, the "last picked" is still p
        //
        // Example: nums = [2, 5, 3, 7], we're at idx=2 (value 3), p=1 (value 5)
        //   skip = solve(nums, dp, 3, 1)
        //   We skip '3', move to idx=3, still compare against nums[1]=5
        int skip = solve(nums, dp, idx + 1, p);

        // MEMOIZATION STORAGE: Cache the result for this state
        // Only store when p != -1 because:
        //   - dp array is 0-indexed, can't access dp[idx][-1]
        //   - p=-1 only happens at the initial call, no need to cache it
        if (p != -1)
        {
            dp[idx][p] = max(take, skip);
        }

        // RETURN THE OPTIMAL CHOICE
        // We tried two options: taking current element vs skipping it
        // Return whichever gives us the LONGER subsequence
        // This is the essence of optimization: try all valid choices, pick the best
        return max(take, skip);
    }

    int lengthOfLIS(vector<int> &nums)
    {
        int n = nums.size();
        
        // DP TABLE INITIALIZATION:
        // dp[idx][p] = "maximum LIS length starting from index idx, 
        //               with p as the index of last picked element"
        // 
        // Size explanation:
        //   - First dimension: n+1 to handle idx from 0 to n (base case when idx=n)
        //   - Second dimension: n+1 to handle p from -1 to n-1
        //     We use (n+1) and shift: dp[idx][p+1] to avoid negative indices
        //     But in this implementation, we just don't memoize p=-1
        //
        // Initialization: -1 means "not computed yet"
        // Why -1? Because our answer is always ≥ 0 (lengths are non-negative)
        // So -1 is a clear "unvisited" marker
        vector<vector<int>> dp(n + 1, vector<int>(n + 1, -1));
        
        // INITIAL CALL:
        // Start from index 0 (beginning of array)
        // With p = -1 (no previous element picked yet)
        // This explores all possible subsequences starting from the beginning
        return solve(nums, dp, 0, -1);
    }
};

#include <bits/stdc++.h>
using namespace std;

/*
 * ============================================================
 * LONGEST INCREASING SUBSEQUENCE (LIS) — BOTTOM UP DP
 * ============================================================
 *
 * PROBLEM:
 * Given an array, find the length of the longest subsequence
 * such that all elements are strictly increasing.
 *
 * Note: A subsequence does NOT need to be contiguous.
 * [1, 3, 6, 7] from [1, 3, 5, 6, 4, 7] is valid.
 *
 * ============================================================
 * FIRST PRINCIPLES — HOW TO THINK ABOUT THIS
 * ============================================================
 *
 * The brute force idea is: generate all 2^n subsequences,
 * check which ones are increasing, return the longest.
 * That is exponential and useless for large inputs.
 *
 * The key observation to reach DP:
 *
 *   If I am standing at index i and I want to know the
 *   longest increasing subsequence ENDING at i, I only
 *   need to look at elements to my LEFT that are SMALLER
 *   than nums[i]. Among those, I pick the one that already
 *   has the longest chain built up — and I extend it by 1.
 *
 * This is the entire algorithm. Everything else is just
 * implementation of this idea.
 *
 * WHY DOES THIS WORK?
 *
 * Because the best LIS ending at i depends ONLY on the best
 * LIS ending at some j < i where nums[j] < nums[i].
 * The problem has OPTIMAL SUBSTRUCTURE — the answer to a
 * larger problem is built from answers to smaller problems.
 * And each subproblem (dp[j]) is computed before we need it,
 * because we go left to right. This is what makes it DP.
 *
 * ============================================================
 * INTUITION FOR THE DP DEFINITION
 * ============================================================
 *
 * Define: dp[i] = length of the longest increasing subsequence
 *                 that ENDS at index i (nums[i] is the last element)
 *
 * Why "ending at i" and not "starting at i" or "anywhere up to i"?
 *
 * Because we need a fixed anchor. If we say "longest LIS in
 * the range [0..i]", it becomes hard to extend — we don't know
 * where that LIS ends, so we can't check if nums[i+1] can
 * follow it. By fixing the END point at i, we always know the
 * last element, which lets us check the strictly increasing
 * condition when extending to a future index.
 *
 * ============================================================
 * RECURRENCE (the core of the algorithm)
 * ============================================================
 *
 *   dp[i] = max(dp[j] + 1)
 *           for all j < i where nums[j] < nums[i]
 *
 *   If no such j exists, dp[i] = 1 (just nums[i] alone)
 *
 * Read it as:
 *   "Look at every element to my left that is smaller than me.
 *    Each of them has a chain already built. I can extend the
 *    best of those chains by 1 (by appending myself to it).
 *    That gives me the longest possible LIS ending at i."
 *
 * ============================================================
 * BASE CASE
 * ============================================================
 *
 * dp[i] = 1 for all i.
 *
 * Every single element is a valid increasing subsequence of
 * length 1. This is the minimum possible answer for any index.
 * We never go below 1.
 *
 * ============================================================
 * COMPLEXITY
 * ============================================================
 *
 * Time  : O(n^2) — two nested loops, each running up to n
 * Space : O(n)   — for the dp array
 *
 */

class Solution
{
public:
    int lengthOfLIS(vector<int> &nums)
    {
        int n = nums.size();

        // ----------------------------------------------------------
        // dp[i] = length of longest increasing subsequence ending at i
        //
        // Initialized to 1 because every element alone is a valid
        // subsequence. This also serves as the base case — if no
        // smaller element exists to the left of i, dp[i] stays 1.
        // ----------------------------------------------------------
        vector<int> dp(n, 1);

        // ----------------------------------------------------------
        // maxi tracks the overall answer.
        //
        // CRITICAL: The answer is NOT always dp[n-1].
        // The longest LIS can end at ANY index, not just the last one.
        //
        // Example: nums = [5, 1, 2, 3]
        //   dp = [1, 1, 2, 3]
        //   Longest LIS is [1, 2, 3], ending at index 3. Fine here.
        //
        // Example: nums = [1, 2, 3, 0]
        //   dp = [1, 2, 3, 1]
        //   dp[n-1] = 1, but the real answer is 3 (LIS = [1, 2, 3]).
        //   If we only returned dp[n-1] we would get the wrong answer.
        //
        // So we track the maximum dp[i] seen across all indices.
        // ----------------------------------------------------------
        int maxi = 1;

        // ----------------------------------------------------------
        // OUTER LOOP: iterate over each index i as the "end point"
        //
        // For each i, we compute dp[i] — the best LIS ending at i.
        // We go left to right so that when we process index i,
        // all dp[0], dp[1], ..., dp[i-1] are already finalized.
        // This is what "bottom-up" means — smaller subproblems are
        // solved before the larger ones that depend on them.
        // ----------------------------------------------------------
        for (int i = 0; i < n; i++)
        {
            // ------------------------------------------------------
            // INNER LOOP: check every index j to the left of i
            //
            // We are asking: "which previous element can I extend from?"
            //
            // For each j, if nums[j] < nums[i], it means nums[i] can
            // come AFTER nums[j] in an increasing subsequence.
            // In that case, dp[j] + 1 is a valid candidate for dp[i].
            //
            // We check ALL j < i because we don't know in advance
            // which j will give us the longest chain. We must try
            // every candidate and take the best.
            //
            // WHY ARE dp[j] VALUES ALREADY CORRECT HERE?
            // Because j < i, and we process left to right.
            // By the time we reach index i, all indices before it
            // are completely solved. We are always building on a
            // stable, already-computed foundation.
            // This is the correctness guarantee of bottom-up DP.
            // ------------------------------------------------------
            for (int j = 0; j < i; j++)
            {
                // --------------------------------------------------
                // Can nums[i] extend the subsequence ending at j?
                //
                // Only if nums[i] > nums[j] (strictly increasing).
                // If nums[i] <= nums[j], appending nums[i] after
                // nums[j] would violate the increasing property,
                // so we skip this j entirely.
                // --------------------------------------------------
                if (nums[i] > nums[j])
                {
                    // ----------------------------------------------
                    // EXTEND: dp[j] is the best chain ending at j.
                    // By appending nums[i] to it, we get dp[j] + 1.
                    //
                    // We take max(dp[i], dp[j] + 1) because there
                    // may be multiple valid j's. dp[i] accumulates
                    // the best result seen so far across all j's
                    // checked in this inner loop iteration.
                    //
                    // Example: nums = [1, 3, 2, 5], i = 3 (value 5)
                    //   j=0: nums[0]=1 < 5, dp[0]+1 = 2 → dp[3] = 2
                    //   j=1: nums[1]=3 < 5, dp[1]+1 = 3 → dp[3] = 3
                    //   j=2: nums[2]=2 < 5, dp[2]+1 = 3 → dp[3] = 3
                    //   Best is 3, meaning LIS ending at index 3 is
                    //   either [1,3,5] or [1,2,5], both length 3.
                    // ----------------------------------------------
                    dp[i] = max(dp[i], dp[j] + 1);

                    // ----------------------------------------------
                    // Update global answer whenever dp[i] changes.
                    // The final answer is the max over all dp[i].
                    // ----------------------------------------------
                    maxi = max(maxi, dp[i]);
                }
            }
        }

        // ----------------------------------------------------------
        // Return the length of the longest increasing subsequence
        // found across all possible ending positions.
        // ----------------------------------------------------------
        return maxi;
    }
};

// ==================== COMPLEXITY ANALYSIS ====================
//
// MEMOIZATION APPROACH:
// Time Complexity: O(n²)
//   - We have n possible values for idx (0 to n-1)
//   - We have n possible values for p (-1 to n-1, but effectively n states)
//   - Total states: n × n = n²
//   - Each state is computed once (memoization prevents recomputation)
//   - Work per state: O(1) (just comparing and taking max)
//   - Total: O(n²)
//
// Space Complexity: O(n²) for dp table + O(n) for recursion stack = O(n²)
//
// TABULATION APPROACH:
// Time Complexity: O(n²)
//   - Outer loop: n iterations (for each position i)
//   - Inner loop: up to i iterations (for each j < i)
//   - Total iterations: 0 + 1 + 2 + ... + (n-1) = n(n-1)/2 = O(n²)
//   - Work per iteration: O(1)
//   - Total: O(n²)
//
// Space Complexity: O(n) for dp array (much better than memoization!)
//   - No recursion stack needed
//   - Just one 1D array of size n
//
// ==================== WHEN TO USE WHICH ====================
//
// Use MEMOIZATION when:
//   - Problem naturally expresses as recursion
//   - You might not need all subproblems (sparse state space)
//   - Transition logic is complex
//
// Use TABULATION when:
//   - Clear iterative order exists
//   - You'll need most/all subproblems anyway
//   - Space optimization matters
//   - You want to avoid recursion stack overflow for large inputs
//
// For LIS specifically, TABULATION is usually preferred because:
//   1. Clearer to understand (asking "what ends here?")
//   2. Better space complexity O(n) vs O(n²)
//   3. No recursion overhead
//   4. Easier to optimize further (binary search approach builds on this)