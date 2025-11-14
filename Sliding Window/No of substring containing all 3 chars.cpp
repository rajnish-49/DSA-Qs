// @https://leetcode.com/problems/number-of-substrings-containing-all-three-characters/

#include <unordered_map>
#include <string>
using namespace std;

class Solution
{
public:
    int numberOfSubstrings(string s)
    {
        /* SLIDING WINDOW APPROACH - Variable Size Window with Constraint Satisfaction
         * Core Concept: Count ALL substrings that contain at least one 'a', one 'b', and one 'c'
         * Key Insight: If substring [left, right] contains all 3 chars, then ALL extensions [left, x]
         * where x >= right will also contain all 3 chars
         *
         * Problem Pattern: This is a "counting substrings with property" problem
         * We use the monotonic property: valid substrings remain valid when extended to the right
         */

        unordered_map<char, int> freq; // Tracks frequency of characters in current window
        int result = 0;                // Total count of valid substrings
        int left = 0;                  // Left boundary of sliding window
        int n = s.size();              // Length of string for counting extensions

        /* ALGORITHM STRATEGY:
         * Instead of checking every possible substring individually (O(n²)),
         * we use sliding window to find valid starting positions and count extensions
         *
         * Mathematical Foundation:
         * If [left, right] contains all 3 characters, then:
         * - [left, right], [left, right+1], [left, right+2], ..., [left, n-1] are ALL valid
         * - This gives us exactly (n - right) valid substrings starting at 'left'
         */

        // Right pointer (i) expands the window - exploring all possible ending positions
        for (int right = 0; right < s.size(); right++)
        {

            /* EXPANSION PHASE: Include current character in window
             * We add each character and track distinct character count via map size
             * Why map.size()? It automatically gives us count of distinct characters
             * No need to manually track 'a', 'b', 'c' counts separately
             */
            freq[s[right]]++;

            /* VALIDATION & SHRINKING PHASE: Handle windows with all 3 characters
             * Why while loop instead of if? We want to find ALL valid starting positions
             * for the current ending position 'right'
             *
             * CRITICAL INSIGHT: We shrink to find the LEFTMOST starting position where
             * the substring still contains all 3 characters. Every position from 0 to
             * this leftmost position will form a valid substring ending at 'right'
             */
            while (freq.size() == 3)
            {

                /* COUNTING STRATEGY: Mathematical optimization for efficiency
                 * Current insight: [left, right] contains all 3 distinct characters
                 *
                 * ALL possible extensions are valid:
                 * - [left, right]   ← current substring
                 * - [left, right+1] ← extend by 1 character
                 * - [left, right+2] ← extend by 2 characters
                 * - ...
                 * - [left, n-1]     ← extend to end of string
                 *
                 * Why this works? Adding more characters can never make a valid substring invalid
                 * Once we have all 3 chars, extending right maintains the property
                 *
                 * Count = (n-1) - right + 1 = n - right
                 */
                result += (n - right);

                /* SHRINKING LOGIC: Remove leftmost character to find next valid position
                 * We remove characters from left until we lose the "all 3 chars" property
                 * This helps us find all possible starting positions for current 'right'
                 *
                 * ORDER OF OPERATIONS MATTERS:
                 * 1. Decrease frequency first: freq[s[left]]--
                 * 2. Remove if frequency becomes 0: freq.erase(s[left])
                 * 3. Move left pointer: left++
                 *
                 * Why this order? The map size must accurately reflect distinct characters
                 */
                freq[s[left]]--;

                /* CRITICAL: Why erase when frequency becomes 0?
                 * Map size represents count of distinct characters in current window
                 * If freq[char] = 0, that character is no longer in our window
                 * Keeping it would give wrong distinct count (map.size() would be wrong)
                 */
                if (freq[s[left]] == 0)
                {
                    freq.erase(s[left]);
                }

                left++; // Shrink window from left
            }

            /* WINDOW EXPANSION:
             * No explicit right++ needed in for loop
             * Each iteration automatically expands window to include next character
             */
        }

        /* ALGORITHM COMPLEXITY ANALYSIS:
         * Time: O(n) - each character is added once and removed at most once
         * Space: O(1) - map stores at most 3 distinct characters ('a', 'b', 'c')
         *
         * Key optimization insight: We count multiple substrings at once using
         * the mathematical property instead of checking each substring individually
         *
         * EDGE CASES HANDLED:
         * - String with < 3 distinct chars → result = 0 (while loop never executes)
         * - String with exactly 3 distinct chars → counts correctly
         * - String with repeated patterns → handles efficiently via sliding window
         */
        return result;
    }
};