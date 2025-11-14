// @https://leetcode.com/problems/minimum-window-substring/description/

class Solution {
public:
    string minWindow(string s, string t) {

        // If t is longer than s, it's impossible to find a valid window
        if (t.size() > s.size())
            return "";

        // Frequency map of characters in t (required counts)
        unordered_map<char, int> ft;
        for (char c : t)
            ft[c]++;

        // Frequency map for current sliding window in s
        unordered_map<char, int> window;

        int current = 0;               // Number of unique chars in the window that meet their required count
        int required = ft.size();      // Total number of unique chars needed

        // Two pointers for the sliding window
        int l = 0, r = 0;

        // Track the smallest window found
        int minl = INT_MAX;            // Length of smallest valid window
        int start = 0;                 // Starting index of that window

        // Expand the window by moving 'r' to the right
        for (r = 0; r < s.size(); r++) {
            char ch = s[r];            // Current character at right pointer
            window[ch]++;              // Add it to our window frequency map

            // If the character is required (exists in t)
            // AND we have now exactly the number of that char we need
            if (ft.count(ch) && window[ch] == ft[ch]) {
                current++;             // We've satisfied one more required character
            }

            // Try shrinking the window from the left while it's valid
            while (current == required) {

                // Update minimum window if current one is smaller
                if (r - l + 1 < minl) {
                    minl = r - l + 1;  // Update smallest length
                    start = l;         // Update starting position
                }

                // Character at the left pointer (we're about to remove it)
                char leftmost = s[l];
                window[leftmost]--;    // Decrease its count in the window

                // If this char is required and we're now below its needed count
                if (ft.count(leftmost) && window[leftmost] < ft[leftmost]) {
                    current--;         // We've broken the requirement for one character
                }

                l++;                   // Shrink the window from the left
            }
        }

        // If we never found a valid window, return ""
        // Otherwise, return the substring starting at 'start' with length 'minl'
        return (minl == INT_MAX) ? "" : s.substr(start, minl);
    }
};
