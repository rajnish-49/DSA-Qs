// @ https://leetcode.com/problems/longest-substring-without-repeating-characters/description/

class Solution {
    public:
        int lengthOfLongestSubstring(string s) {
    
            // Declare a set to store unique characters in the current window (substring)
            set<char> st;
    
            // Variable to store the maximum length found so far
            int count = 0;
    
            // Two pointers to represent the sliding window
            int left = 0;   // Start of the window
            int right = 0;  // End of the window
    
            // Loop until the right pointer reaches the end of the string
            while (right < s.size()) {
    
                // If the character at the 'right' pointer is already in the set,
                // it means we have a repeating character in the current window
                if (st.find(s[right]) != st.end()) {
    
                    // Remove the character at 'left' pointer from the set
                    // and move the 'left' pointer one step ahead
                    // This helps in removing the repeating character and
                    // shrinking the window from the left side
                    st.erase(s[left]);
                    left++;
                }
                else {
                    // If the character is not in the set, insert it
                    st.insert(s[right]);
    
                    // Update the maximum count if current window size is greater
                    count = max(count, right - left + 1);
    
                    // Expand the window by moving the 'right' pointer to the next character
                    right++;
                }
            }
    
            // Return the length of the longest substring without repeating characters
            return count;
        }
    };
    
// ANOTHER APPROACH 

int lengthOfLongestSubstring(string s) {
    // Stores the last index where each character was seen
    unordered_map<char, int> mp;

    int maxLen = 0; // To store the length of the longest valid substring
    int left = 0;   // Left end of the current window

    // Expand the window by moving 'right' pointer
    for (int right = 0; right < s.length(); right++) {
        char ch = s[right];

        // Check if the current character was seen before AND
        // its last seen index is within the current window
        if (mp.find(ch) != mp.end() && mp[ch] >= left) {
            // If duplicate found inside the window, move the left boundary
            // to one position after the last occurrence of the current character
            left = mp[ch] + 1;
        }

        // Update the last seen index of the current character
        mp[ch] = right;

        // Calculate current window size and update max length if larger
        int windowLength = right - left + 1;
        maxLen = max(maxLen, windowLength);
    }

    return maxLen;
}
