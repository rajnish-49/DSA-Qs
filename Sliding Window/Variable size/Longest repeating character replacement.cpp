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
class Solution {
public:
    int characterReplacement(string s, int k) {
        vector<int> frequency(26, 0);

        int left = 0;
        int maxFrequency = 0;
        int maxLength = 0;

        // The current window is s[left ... right].
        // We try to make every character in this window equal
        // using at most k replacements.
        for (int right = 0; right < s.size(); right++) {

            // Add the new character entering the window.
            frequency[s[right] - 'A']++;

            // maxFrequency is the highest frequency of any one
            // character seen in the current window.
            //
            // We keep that most frequent character unchanged and
            // replace every other character in the window.
            maxFrequency = max(
                maxFrequency,
                frequency[s[right] - 'A']
            );

            // Number of replacements needed:
            //
            // window size - count of the most frequent character
            //
            // Example: "AABA"
            // window size = 4, maxFrequency = 3
            // replacements needed = 4 - 3 = 1
            while ((right - left + 1) - maxFrequency > k) {

                // More than k replacements are required, so the
                // current window is too large. Remove characters
                // from the left until the window becomes usable again.
                frequency[s[left] - 'A']--;
                left++;
            }

            // The window now needs at most k replacements,
            // so it is a valid candidate for the answer.
            maxLength = max(maxLength, right - left + 1);
        }

        return maxLength;
    }
};