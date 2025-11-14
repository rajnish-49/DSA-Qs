// @ https://leetcode.com/problems/longest-substring-with-at-least-k-repeating-characters/

class Solution {
    public:
        int longestSubstring(string s, int k) {
            // Base case: If the string is shorter than k, 
            // then no substring of it can satisfy the condition
            if (s.length() < k) {
                return 0;
            }
    
            // Frequency array to store count of each character (a to z)
            vector<int> freq(26, 0);
            for (char ch : s) {
                freq[ch - 'a']++;  // increment the count for character ch
            }
    
            // We now find a character that occurs more than 0 times but less than k times.
            // This character will be our "split point" because any substring containing it
            // cannot be valid (as it violates the frequency condition).
            char splitChar = 0; // Initialize with null (0) to detect if we don't need to split
            for (int i = 0; i < 26; i++) {
                if (freq[i] > 0 && freq[i] < k) {
                    // Found a character that doesn't satisfy the condition
                    splitChar = 'a' + i;
                    break;
                }
            }
    
            // If no invalid character found, it means entire string is valid
            if (splitChar == 0) {
                return s.length();
            }
    
            // Now we split the string at each occurrence of the invalid character (splitChar)
            // because no substring containing splitChar can be valid
            vector<string> parts; // store valid parts of the string after splitting
            string temp = "";     // temporary string to build substrings
    
            for (char ch : s) {
                if (ch == splitChar) {
                    // Time to split. Push the temp string if it's not empty
                    if (!temp.empty()) {
                        parts.push_back(temp);
                        temp = ""; // reset for the next part
                    }
                } else {
                    temp += ch; // build the current substring
                }
            }
    
            // Push the last part if there is any remaining
            if (!temp.empty()) {
                parts.push_back(temp);
            }
    
            // Recursively apply the same logic to all parts and take the maximum
            int maxLen = 0;
            for (string part : parts) {
                // Each part is processed recursively to find the longest valid substring
                maxLen = max(maxLen, longestSubstring(part, k));
            }
    
            return maxLen; // return the length of the longest valid substring found
        }
    };
    