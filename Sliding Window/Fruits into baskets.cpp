// @https://leetcode.com/problems/fruit-into-baskets/

#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

class Solution
{
public:
    int totalFruit(vector<int> &fruits)
    {
        /* SLIDING WINDOW APPROACH - Variable Size Window with Type Constraint
         * Core Concept: Find the longest contiguous subarray with at most 2 distinct elements
         * Key Insight: This is essentially "Longest Substring with At Most K Distinct Characters" where K=2
         *
         * Problem Translation:
         * - Fruits = characters, Fruit types = distinct characters
         * - 2 baskets = at most 2 distinct fruit types allowed
         * - Goal: Maximum fruits collected = longest valid subarray length
         *
         * Real-world analogy: You walk through an orchard with 2 baskets, each basket can hold
         * only one type of fruit. What's the maximum number of fruits you can collect in one trip?
         */

        unordered_map<int, int> typeCount; // Tracks frequency of each fruit type in current window
        int left = 0;                      // Left boundary of sliding window
        int maxFruits = 0;                 // Maximum fruits collected (longest valid window)
        int n = fruits.size();             // Total fruits available

        /* ALGORITHM STRATEGY - Two Pointer Technique:
         * We maintain a window [left, right] that contains at most 2 distinct fruit types
         *
         * Window States:
         * 1. distinct_types <= 2 → Valid window, can expand
         * 2. distinct_types > 2  → Invalid window, must shrink from left
         *
         * Why this works? We explore all possible valid windows and track the maximum length
         */

        // Right pointer (i) expands the window - exploring all possible ending positions
        for (int right = 0; right < n; right++)
        {

            /* EXPANSION PHASE: Include current fruit in our collection
             * We add the fruit to our current window and update its frequency
             * Map size automatically tracks the number of distinct fruit types
             *
             * Why use map size? It's an elegant way to count distinct elements without
             * manually tracking separate counters for each type
             */
            typeCount[fruits[right]]++;

            /* CONSTRAINT VIOLATION HANDLING: Too many fruit types
             * Why while loop instead of if? Because removing one fruit from left might
             * not be enough to bring distinct types back to 2
             *
             * Example: window has types [A,A,B,B,C], removing one A still leaves us with 3 types
             * We need to keep shrinking until we have exactly 2 types or less
             *
             * CRITICAL INSIGHT: We shrink until the constraint is satisfied, not just once
             */
            while (typeCount.size() > 2)
            {

                /* SHRINKING LOGIC: Remove leftmost fruit from collection
                 * Order of operations is crucial for maintaining correct state:
                 * 1. Decrease frequency count
                 * 2. Remove from map if frequency becomes 0
                 * 3. Move left pointer
                 */
                typeCount[fruits[left]]--;

                /* MAP MAINTENANCE: Why erase when frequency becomes 0?
                 * The map size represents the count of distinct fruit types in our window
                 * If typeCount[fruitType] = 0, that fruit type is no longer in our collection
                 * Keeping it in the map would give wrong distinct count (map.size() would be incorrect)
                 *
                 * This is a common pattern in sliding window problems with distinct element counting
                 */
                if (typeCount[fruits[left]] == 0)
                {
                    typeCount.erase(fruits[left]);
                }

                left++; // Shrink window from left side
            }

            /* OPTIMIZATION TRACKING: Record maximum valid window size
             * After ensuring our window is valid (≤ 2 distinct types), we check if this
             * is the longest valid collection we've seen so far
             *
             * Window length = right - left + 1
             * Why +1? Because indices are 0-based, so length includes both endpoints
             *
             * Example: window from index 2 to 5 has length 5-2+1 = 4 elements
             */
            maxFruits = max(maxFruits, right - left + 1);
        }

        /* ALGORITHM COMPLEXITY ANALYSIS:
         * Time: O(n) - each fruit is added once and removed at most once
         *              even though we have nested loops, the inner while loop's total
         *              iterations across all outer loop iterations is bounded by n
         * Space: O(1) - map stores at most 3 entries (temporarily during constraint violation)
         *
         * KEY INSIGHT: This is NOT O(n²) because each element is processed at most twice
         * (once when added by right pointer, once when removed by left pointer)
         *
         * EDGE CASES HANDLED:
         * - Empty array → maxFruits = 0
         * - Single fruit type → entire array is valid
         * - Array with exactly 2 types → entire array is valid
         * - Array with >2 types → finds optimal subarray
         */
        return maxFruits;
    }
};