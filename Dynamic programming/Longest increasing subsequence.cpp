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

// ==================== APPROACH 2: TABULATION (BOTTOM-UP DP) ====================
//
// PARADIGM SHIFT: Instead of "what can I build going FORWARD from here?"
//                 We ask: "what's the BEST that ENDS right here?"
//
// KEY INSIGHT:
// dp[i] = "length of longest increasing subsequence that ENDS at index i"
//         NOT "starts at i", but "ENDS at i" ← this is CRUCIAL!
//
// WHY THIS WORKS:
// If I know the best subsequences ending at ALL positions before me,
// I can EXTEND any of them (if their last element is smaller than mine)
// Then I take the LONGEST extension possible!
//
// VISUAL EXAMPLE:
// nums = [2, 5, 3, 7]
// Index:  0  1  2  3
//
// At i=0 (value 2): dp[0] = 1 → [2]
// At i=1 (value 5): Can extend [2] → dp[1] = 2 → [2, 5]
// At i=2 (value 3): Can extend [2] → dp[2] = 2 → [2, 3]
// At i=3 (value 7): Can extend [2,5] OR [2,3] → dp[3] = 3 → [2, 5, 7] or [2, 3, 7]
//
// THE ALGORITHM:
// For each position i:
//   1. Look at ALL positions j before i (where j < i)
//   2. If nums[i] > nums[j], we can extend the subsequence ending at j
//   3. dp[i] = max(dp[i], dp[j] + 1) for all valid j
//   4. Track the global maximum across all dp[i]

class Solution
{
public:
    int lengthOfLIS(vector<int> &nums)
    {
        int n = nums.size();
        
        // DP ARRAY INITIALIZATION:
        // dp[i] = "length of longest increasing subsequence ENDING at index i"
        // 
        // Why initialize to 1?
        // Every single element is a valid subsequence of length 1 by itself
        // This is our BASE CASE: worst case scenario, no element before i is smaller
        // So the best we can do is just [nums[i]] alone → length 1
        //
        // Example: nums = [10, 9, 8, 7]
        // Every element is larger than all elements after it
        // So best LIS for each is just itself: dp = [1, 1, 1, 1]
        vector<int> dp(n, 1);
        
        // GLOBAL MAXIMUM TRACKER:
        // The longest LIS might END at ANY position, not necessarily the last index!
        // 
        // Example: nums = [5, 1, 2, 3, 4]
        //   LIS ending at index 0: [5] → length 1
        //   LIS ending at index 4: [1, 2, 3, 4] → length 4
        //   The answer is 4, found at index 4, NOT index 0
        //
        // Another example: nums = [4, 10, 4, 3, 8, 9]
        //   LIS ending at index 5: [4, 8, 9] → length 3
        //   But index 1 has: [4, 10] → length 2
        //   Answer is 3, but we need to track it as we go
        //
        // Initialize to 1 because minimum LIS length is always 1 (any single element)
        int maxi = 1;

        // OUTER LOOP: For each position i, compute "best LIS ending at i"
        // We process left to right because:
        //   - To compute dp[i], we need dp[0], dp[1], ..., dp[i-1] already computed
        //   - This ensures we always have the answers to smaller subproblems
        //   - This is the "bottom-up" nature of tabulation
        for (int i = 0; i < n; i++)
        {
            // INNER LOOP: Look at ALL positions j before i
            // Question we're asking at each j:
            //   "Can I extend the subsequence that ends at j by adding nums[i]?"
            //
            // Why check all j < i?
            // Because we don't know which previous position gives us the LONGEST extension
            // We need to try them ALL and pick the best one
            //
            // Example: nums = [1, 3, 6, 7, 9, 4, 10]
            //                  0  1  2  3  4  5  6
            // At i=6 (value 10):
            //   j=0: 10 > 1 ✓ can extend [1] → length 2
            //   j=1: 10 > 3 ✓ can extend [1,3] → length 3
            //   j=2: 10 > 6 ✓ can extend [1,3,6] → length 4
            //   j=3: 10 > 7 ✓ can extend [1,3,6,7] → length 5
            //   j=4: 10 > 9 ✓ can extend [1,3,6,7,9] → length 6 ← BEST!
            //   j=5: 10 > 4 ✓ can extend [1,3,4] → length 4
            // We pick j=4 because it gives us the longest extension
            for (int j = 0; j < i; j++)
            {
                // CONDITION CHECK: Can we extend the subsequence ending at j?
                // We can ONLY extend if nums[i] > nums[j]
                // This ensures STRICTLY INCREASING property
                //
                // Why strictly greater (>) and not greater-or-equal (>=)?
                // Problem requires STRICTLY INCREASING: [1, 2, 2, 3] can't use both 2's
                //
                // If condition fails (nums[i] <= nums[j]):
                //   - We can't add nums[i] after nums[j] in a valid increasing subsequence
                //   - Skip this j and check the next one
                if (nums[i] > nums[j])
                {
                    // EXTENSION LOGIC:
                    // dp[j] = "best subsequence length ending at j"
                    // If we add nums[i] to that subsequence: new length = dp[j] + 1
                    //   - dp[j]: the length of subsequence ending at j
                    //   - +1: we're adding nums[i] as the next element
                    //
                    // But wait! We might have MULTIPLE valid j's we can extend from
                    // Example: at i=5, both j=2 and j=3 might be valid
                    //   - Extending from j=2 might give us dp[2] + 1 = 4
                    //   - Extending from j=3 might give us dp[3] + 1 = 5
                    // We want the MAXIMUM of all possible extensions!
                    //
                    // That's why we use max(dp[i], dp[j] + 1):
                    //   - dp[i]: best length we've found so far for position i
                    //             (from previous j's we've checked)
                    //   - dp[j] + 1: length if we extend from current j
                    //   - Take the maximum of these two
                    //
                    // This ensures we always keep the LONGEST possible subsequence ending at i
                    dp[i] = max(dp[i], dp[j] + 1);
                    
                    // UPDATE GLOBAL MAXIMUM:
                    // Every time we compute a new dp[i], check if it's the best overall
                    // Why here? Because we just potentially updated dp[i] to a new value
                    // And this new value might be larger than our previous maximum
                    //
                    // We could also do this check outside the inner loop, after we've
                    // finalized dp[i], but doing it here is more efficient (no extra loop)
                    maxi = max(maxi, dp[i]);
                }
                // If nums[i] <= nums[j]: do nothing, try next j
                // dp[i] remains as its current value (initialized to 1, or updated by previous j's)
            }
            // After checking all j < i, dp[i] now holds the maximum LIS length ending at i
        }

        // RETURN THE ANSWER:
        // maxi contains the length of the longest increasing subsequence
        // This is the maximum value across ALL dp[i] values
        //
        // Why not just return dp[n-1]?
        // Because the longest subsequence might not end at the last position!
        // Example: nums = [10, 9, 2, 5, 3, 7, 101, 18]
        //   Longest might be [2, 5, 7, 101] ending at index 6
        //   OR [2, 5, 7, 18] ending at index 7
        //   OR [2, 3, 7, 18] ending at index 7
        //   We need to check ALL ending positions, not just the last one
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