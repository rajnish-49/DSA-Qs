class Solution {
public:
    int lengthOfLongestSubstringKDistinct(string s, int k) {
        // Edge case: if k is 0, no characters allowed
        if (k == 0) return 0;

        unordered_map<char, int> freq; // Frequency map of characters in current window
        int l = 0;                     // Left pointer for sliding window
        int maxLen = 0;                 // Stores the maximum length found

        // Expand the window with right pointer
        for (int r = 0; r < s.size(); ++r) {
            // Step 1: Include s[r] in the window
            freq[s[r]]++;

            // Step 2: If we have more than k distinct characters,
            // shrink window from the left until distinct count ≤ k
            while (freq.size() > k) {
                freq[s[l]]--;           // Remove one occurrence of s[l]
                if (freq[s[l]] == 0)    // If count becomes zero, remove from map
                    freq.erase(s[l]);
                l++;                    // Move left pointer to shrink window
            }

            // Step 3: At this point, window [l..r] has at most k distinct characters
            // Check if this window is the largest seen so far
            maxLen = max(maxLen, r - l + 1);
        }

        return maxLen;
    }
};
