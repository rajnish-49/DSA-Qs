// @https://leetcode.com/problems/count-the-number-of-good-subarrays/?envType=daily-question&envId=2025-04-16

#include <vector>
#include <unordered_map>
using namespace std;

class Solution
{
public:
    long long countGood(vector<int> &nums, int k)
    {
        /* SLIDING WINDOW APPROACH - Variable Size Window + Combinatorics
         * Core Concept: A "good" subarray has at least k pairs of identical elements
         * Key Insight: If window [left, right] is good, then ALL subarrays [left, x] where x >= right are also good
         *
         * Mathematical Foundation:
         * - For n identical elements, we can form C(n,2) = n*(n-1)/2 pairs
         * - When we add an element with frequency f, it forms f new pairs with existing identical elements
         * - When we remove an element with frequency f, it loses f pairs
         */

        int n = nums.size();
        int left = 0;                 // Left boundary of sliding window
        unordered_map<int, int> freq; // Tracks frequency of elements in current window
        long long pairs = 0;          // Current number of identical pairs in window
        long long result = 0;         // Total count of good subarrays

        /* CRITICAL INSIGHT: Why count subarrays instead of just checking conditions?
         * We need to count ALL possible good subarrays, not just find one
         * If [left, right] is good, then [left, right+1], [left, right+2], ... are also good
         * This leads to an efficient counting strategy
         */

        // Right pointer expands the window - exploring all possible ending positions
        for (int right = 0; right < n; right++)
        {

            /* PAIR CALCULATION LOGIC:
             * When adding nums[right] to window:
             * - If freq[nums[right]] = f, adding this element creates f new pairs
             * - Why? The new element pairs with each of the f existing identical elements
             *
             * Example: window = [1,2,2], adding another 2
             * - freq[2] = 2 before adding
             * - New element pairs with both existing 2's → 2 new pairs
             * - Total pairs involving 2's: C(3,2) = 3
             */
            pairs += freq[nums[right]]; // Add new pairs formed by current element
            freq[nums[right]]++;        // Update frequency count

            /* SHRINKING PHASE: Handle windows with too many pairs
             * Why while loop? Because we want to find the LEFTMOST position where pairs >= k
             * Once we find it, ALL positions from current left to n-1 form good subarrays
             *
             * Key Optimization: Instead of checking every possible subarray individually,
             * we use the monotonic property of sliding window
             */
            while (pairs >= k)
            {

                /* COUNTING STRATEGY: Mathematical insight for efficiency
                 * If [left, right] has >= k pairs, then:
                 * - [left, right], [left, right+1], [left, right+2], ..., [left, n-1] are ALL good
                 * The count of integers from a to b, both inclusive, is: b - a + 1
                 * a = right
                 * b = n - 1
                 * count = (n - 1) - right + 1
                 *       = n - right
                 * - That's exactly (n - right) subarrays starting at 'left'
                 *
                 * Why this works? Adding more elements to a good subarray keeps it good
                 * (pairs can only increase or stay same, never decrease when expanding right)
                 */
                result += (n - right);

                /* PAIR REMOVAL LOGIC:
                 * When removing nums[left] from window:
                 * - First decrease its frequency: freq[nums[left]]--
                 * - Then subtract pairs: pairs -= freq[nums[left]]
                 *
                 * WHY THIS ORDER MATTERS:
                 * If freq[nums[left]] was f before removal:
                 * - After freq--, it becomes f-1
                 * - We lose exactly f-1 pairs (the removed element was paired with f-1 others)
                 * - This maintains the correct pair count
                 */
                freq[nums[left]]--;        // Remove element from frequency map
                pairs -= freq[nums[left]]; // Subtract the pairs lost due to removal
                left++;                    // Shrink window from left
            }

            /* WINDOW EXPANSION:
             * No explicit right++ needed in for loop
             * Each iteration automatically expands the window to include next element
             */
        }

        /* ALGORITHM COMPLEXITY ANALYSIS:
         * Time: O(n) - each element is added once and removed at most once
         * Space: O(n) - for the frequency map in worst case (all elements distinct)
         *
         * The key insight that makes this O(n) instead of O(n²):
         * We don't check every subarray individually, but use mathematical properties
         * to count multiple subarrays at once
         */
        return result;
    }
};
