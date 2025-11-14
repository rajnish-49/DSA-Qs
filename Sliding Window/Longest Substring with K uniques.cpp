// @https://www.geeksforgeeks.org/problems/longest-k-unique-characters-substring0853/1

#include <unordered_map>
#include <algorithm>
using namespace std;

class Solution
{
public:
    int longestKSubstr(string &s, int k)
    {
        /* SLIDING WINDOW APPROACH - Variable Size Window
         * Core Concept: Maintain a window [left, right] that contains AT MOST k distinct characters
         * Key Insight: We only care about substrings with EXACTLY k distinct characters
         *
         * Window States:
         * 1. distinct_chars < k  → Expand window (add more characters)
         * 2. distinct_chars == k → Valid window, record length
         * 3. distinct_chars > k  → Invalid window, shrink from left
         */

        unordered_map<char, int> freq; // Tracks frequency of characters in current window
        int left = 0;                  // Left boundary of sliding window
        int maxLen = -1;               // Initialize to -1 (indicates no valid substring found)

        /* CRITICAL: Why initialize maxLen = -1?
         * If no substring with exactly k distinct characters exists, we must return -1
         * Starting with 0 would give wrong answer for impossible cases
         */

        // Right pointer expands the window - this is the core of sliding window technique
        for (int right = 0; right < s.size(); right++)
        {

            /* EXPAND PHASE: Include current character in window
             * Why we expand first? We're exploring all possible substrings ending at 'right'
             * Each iteration, we fix the right boundary and find optimal left boundary
             */
            freq[s[right]]++;

            /* SHRINK PHASE: Handle overflow condition
             * Why while loop instead of if? Because removing one character from left
             * might not be enough to bring distinct count back to k
             *
             * Example: window = "abcd", k=2
             * After removing 'a', we still have "bcd" with 3 distinct chars
             * We need to keep shrinking until we have ≤ k distinct chars
             */
            while (freq.size() > k)
            {
                freq[s[left]]--;

                /* IMPORTANT: Why erase when frequency becomes 0?
                 * Map size represents distinct character count
                 * If freq[char] = 0, that character is no longer in our window
                 * Keeping it would give wrong distinct count
                 */
                if (freq[s[left]] == 0)
                {
                    freq.erase(s[left]);
                }
                left++; // Shrink window from left
            }

            /* VALIDATION PHASE: Check if current window is valid
             * CRUCIAL INSIGHT: We only update answer when we have EXACTLY k distinct characters
             *
             * Why not update when freq.size() <= k?
             * Because we want longest substring with EXACTLY k uniques, not AT MOST k
             *
             * Window states after shrinking:
             * - freq.size() < k: Not enough distinct characters yet
             * - freq.size() == k: Perfect! This is what we want
             * - freq.size() > k: Impossible after while loop above
             */
            if (freq.size() == k)
            {
                maxLen = max(maxLen, right - left + 1);
            }
        }

        /* EDGE CASE HANDLING:
         * If maxLen is still -1, it means no valid substring exists
         * This happens when total distinct characters in string < k
         *
         * Example: s = "aa", k = 3
         * No matter how we form substrings, we can't get 3 distinct characters
         */
        return maxLen;
    }
};