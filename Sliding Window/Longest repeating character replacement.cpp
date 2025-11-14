// @https://leetcode.com/problems/longest-repeating-character-replacement/

/*
Algorithm: Sliding Window with Character Frequency Tracking
- Use a sliding window approach with two pointers (i, j)
- Track frequency of each character in current window
- Maintain the maximum frequency character count in the window
- Key insight: If (window_size - max_frequency) > k, then we need more than k replacements
- Shrink window from left when this condition is violated

Time Complexity: O(n) - each character is processed at most twice
Space Complexity: O(1) - map stores at most 26 characters (constant space)
*/

class Solution
{
public:
    int characterReplacement(string s, int k)
    {
        // Hash map to store frequency of each character in current window
        unordered_map<char, int> freqs;

        // Variables for tracking solution
        int res = 0;     // Maximum valid window size found so far
        int i = 0;       // Left pointer (start of sliding window)
        int maxFreq = 0; // Maximum frequency of any character in current window

        // Right pointer j expands the window
        for (int j = 0; j < s.size(); j++)
        {
            // Add current character to window and update its frequency
            freqs[s[j]]++;

            // Update maximum frequency seen so far in current window
            // This represents the most frequent character that we DON'T need to replace
            maxFreq = max(maxFreq, freqs[s[j]]);

            // Check if current window is valid
            // Window size = (j - i + 1)
            // Characters to replace = window_size - maxFreq
            // If characters to replace > k, window is invalid
            while ((j - i + 1) - maxFreq > k)
            {
                // Shrink window from left: remove leftmost character
                freqs[s[i]]--;
                i++;

                // Note: We don't update maxFreq here because:
                // 1. It's complex to recalculate efficiently
                // 2. An overestimated maxFreq only makes the condition more restrictive
                // 3. This doesn't affect correctness, just might shrink window more than needed
            }

            // Update result with current valid window size
            res = max(res, j - i + 1);
        }

        return res;
    }
};