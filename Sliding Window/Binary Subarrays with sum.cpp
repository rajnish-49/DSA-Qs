//@https://leetcode.com/problems/binary-subarrays-with-sum/description/

#include <vector>
#include <unordered_map>
using namespace std;

/*
🧠 CORE PROBLEM INSIGHT:
Writing code to find exactly k ones in a subarray is tricky.
It requires checking every subarray, counting how many 1s it has → very slow (O(n²)).

✅ THE MATHEMATICAL TRICK TO SIMPLIFY:
Count how many subarrays have "at most k ones", and
Count how many subarrays have "at most k-1 ones"

Then subtract: exactly(k) = atMost(k) - atMost(k - 1)

🎯 INTUITIVE ANALOGY:
Imagine you have a bag of mixed candies:
- Red candies = subarrays with exactly k ones
- Blue candies = subarrays with < k ones

If you count:
- Total candies with ≤ k ones = Red + Blue candies
- Total candies with ≤ k-1 ones = Blue candies only

Then: Red candies = (Red + Blue) - Blue = atMost(k) - atMost(k-1)

This transforms a complex "exactly k" problem into two simple "at most k" problems!
*/

class Solution
{
public:
    /*
    🔥 SLIDING WINDOW FOR "AT MOST K" PATTERN:

    KEY INSIGHT: For every right pointer position, ALL subarrays ending at that position
    and starting from ANY valid left position are counted.

    WHY count = r - l + 1?
    If we have a valid window [l...r], then ALL these subarrays are valid:
    - [r, r] (length 1)
    - [r-1, r] (length 2)
    - [r-2, r] (length 3)
    - ...
    - [l, r] (length r-l+1)

    Total count = r - l + 1 subarrays ending at position r.

    🎯 CRITICAL UNDERSTANDING:
    We're not just counting windows, we're counting ALL valid subarrays!
    Each expansion of right pointer potentially adds multiple subarrays.
    */
    int subarraycount(vector<int> &nums, int goal)
    {
        // Edge case: negative goal is impossible with binary array (0s and 1s only)
        if (goal < 0)
            return 0;

        int l = 0, r = 0; // Sliding window boundaries
        int count = 0;    // Total count of valid subarrays
        int sum = 0;      // Current window sum

        while (r < nums.size())
        {
            // Expand window: include nums[r] in current window
            sum += nums[r];

            // Contract window: shrink from left while sum exceeds goal
            // This maintains the "at most goal" constraint
            while (sum > goal)
            {
                sum -= nums[l];
                l++;
            }

            // 🚀 THE MAGIC LINE: Add ALL subarrays ending at position r
            // Every subarray from [l, r], [l+1, r], [l+2, r], ..., [r, r] is valid
            count += r - l + 1;

            r++; // Move to next position
        }

        return count;
    }

    /*
    🎯 MAIN SOLUTION: Transform "exactly goal" into difference of two "at most" problems

    MATHEMATICAL PROOF:
    - subarraysWithSum(goal) = all subarrays with sum exactly equal to goal
    - subarraycount(goal) = all subarrays with sum ≤ goal
    - subarraycount(goal-1) = all subarrays with sum ≤ goal-1

    Therefore: subarraysWithSum(goal) = subarraycount(goal) - subarraycount(goal-1)

    EXAMPLE with goal=2, array=[1,0,1,0,1]:
    - atMost(2) counts: subarrays with sum 0, 1, or 2
    - atMost(1) counts: subarrays with sum 0 or 1
    - Difference gives: subarrays with sum exactly 2

    This elegant transformation converts a complex exact-match problem
    into two simple sliding window problems! 🔥
    */
    int numSubarraysWithSum(vector<int> &nums, int goal)
    {
        return subarraycount(nums, goal) - subarraycount(nums, goal - 1);
    }
};

/*
🌟 ALTERNATIVE APPROACH: PREFIX SUM + HASHING
Time: O(n), Space: O(n)

The prefix sum approach uses the mathematical relationship:
prefixSum[j] - prefixSum[i-1] == goal
=> prefixSum[i-1] == prefixSum[j] - goal

CORE INSIGHT: If we've seen prefix sum = (currentSum - goal) before,
that's a valid starting point for a subarray ending at current position with sum = goal.

This is the same principle used in "Subarray Sum Equals K" problems.
Both approaches are valid - sliding window saves space, prefix sum is more intuitive.
*/

/*
🧠 PREFIX SUM APPROACH - DETAILED EXPLANATION:

MATHEMATICAL FOUNDATION:
For any subarray from index i to j, the sum is:
sum(i, j) = prefixSum[j] - prefixSum[i-1]

If we want sum(i, j) = goal, then:
prefixSum[j] - prefixSum[i-1] = goal
=> prefixSum[i-1] = prefixSum[j] - goal

🎯 KEY INSIGHT:
As we iterate through the array calculating prefix sums, for each position j:
- We ask: "How many previous positions had prefixSum = (currentPrefixSum - goal)?"
- Each such position represents a valid starting point for a subarray ending at j with sum = goal

🔥 WHY USE HASHMAP?
- We store frequency of each prefix sum seen so far
- When we find prefixSum[j], we look up how many times (prefixSum[j] - goal) occurred before
- This gives us the count of valid subarrays ending at position j

STEP-BY-STEP EXAMPLE:
Array: [1, 0, 1, 0, 1], goal = 2

Position 0: nums[0] = 1
- prefixSum = 1
- remainingSum = prefixSum - goal = 1 - 2 = -1
- Looking for: remainingSum = -1 (not found)
- Store: map[1] = 1
- Count = 0

Position 1: nums[1] = 0
- prefixSum = 1
- remainingSum = prefixSum - goal = 1 - 2 = -1
- Looking for: remainingSum = -1 (not found)
- map[1] already exists, increment: map[1] = 2
- Count = 0

Position 2: nums[2] = 1
- prefixSum = 2
- remainingSum = prefixSum - goal = 2 - 2 = 0
- Looking for: remainingSum = 0 (found 1 time - the implicit prefix sum before start)
- Store: map[2] = 1
- Count = 1 (subarray [1,0,1] from index 0-2)

Position 3: nums[3] = 0
- prefixSum = 2
- remainingSum = prefixSum - goal = 2 - 2 = 0
- Looking for: remainingSum = 0 (found 1 time)
- map[2] already exists, increment: map[2] = 2
- Count = 1 + 1 = 2

Position 4: nums[4] = 1
- prefixSum = 3
- remainingSum = prefixSum - goal = 3 - 2 = 1
- Looking for: remainingSum = 1 (found 2 times!)
- Count = 2 + 2 = 4

Final answer: 4 subarrays with sum = 2
*/

class SolutionPrefixSum
{
public:
    int numSubarraysWithSum(vector<int> &nums, int goal)
    {
        // Hash map to store frequency of each prefix sum seen so far
        unordered_map<int, int> prefixSumCount;

        // Initialize: there's one way to achieve sum 0 (empty prefix)
        // This handles cases where subarray starts from index 0
        prefixSumCount[0] = 1;

        int prefixSum = 0; // Running prefix sum
        int count = 0;     // Total count of valid subarrays

        for (int i = 0; i < nums.size(); i++)
        {
            // Update prefix sum by including current element
            prefixSum += nums[i];

            // Calculate what prefix sum we need to find for valid subarrays
            // remainingSum = prefixSum - goal
            // This represents the prefix sum that, when subtracted from current prefixSum, gives us goal
            int remainingSum = prefixSum - goal;

            // 🔥 CORE LOGIC: Check if remainingSum exists in our hashmap
            // If it does, it means there are subarrays ending at position i with sum = goal
            if (prefixSumCount.find(remainingSum) != prefixSumCount.end())
            {
                count += prefixSumCount[remainingSum];
            }

            // Store/update frequency of current prefix sum for future lookups
            prefixSumCount[prefixSum]++;
        }

        return count;
    }
};

/*
🚀 COMPARISON: SLIDING WINDOW vs PREFIX SUM

SLIDING WINDOW APPROACH:
✅ Space: O(1) - constant extra space
✅ Intuitive for "at most K" problems
✅ Great for problems requiring window constraints
❌ Requires transformation: exactly(K) = atMost(K) - atMost(K-1)
❌ Less intuitive for direct "exactly K" problems

PREFIX SUM + HASHING APPROACH:
✅ Direct solution for "exactly K" problems
✅ More intuitive mathematical relationship
✅ Generalizes well to arrays with negative numbers
❌ Space: O(n) - extra space for hashmap
❌ Slightly more complex edge case handling

WHEN TO USE WHICH?
- Binary arrays + "exactly K": Either approach works well
- Arrays with negatives + "exactly K": Prefix sum is better
- "At most K" problems: Sliding window is natural
- Space-constrained environments: Sliding window

Both are O(n) time complexity - choose based on context and preference!
*/
