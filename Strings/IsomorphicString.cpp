// @ https://leetcode.com/problems/isomorphic-strings/description/

class Solution {
public:
    bool isIsomorphic(string s, string t) {
        // If the lengths of the strings are not the same, they cannot be isomorphic
        if (s.length() != t.length()) {
            return false;
        }

        // Arrays to store the last seen positions of characters in `s` and `t`
        // Initialize with -1 to indicate that no character has been seen yet
        vector<int> s_map(256, -1); // For string `s`
        vector<int> t_map(256, -1); // For string `t`

        // Iterate through each character of the strings
        for (int i = 0; i < s.length(); ++i) {
            char s_char = s[i]; // Current character in `s`
            char t_char = t[i]; // Current character in `t`

            // Check if the mappings of the current characters in `s` and `t` are consistent
            // If the last seen positions of `s_char` and `t_char` don't match, return false
            if (s_map[s_char] != t_map[t_char]) {
                return false;
            }

            // Update the last seen positions of `s_char` and `t_char` to the current index `i`
            s_map[s_char] = i;
            t_map[t_char] = i;
        }

        // If we traverse the entire strings without mismatches, they are isomorphic
        return true;
    }
};


//