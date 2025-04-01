// @https://leetcode.com/contest/weekly-contest-443/problems/longest-palindrome-after-substring-concatenation-i/description/?slug=minimum-cost-to-reach-every-position&region=global_v2

class Solution {
    public:
        
        // Function to find the longest palindromic substring in a given string
        int longestPalindromeInString(string &str) {
            int maxLen = 1;  // At minimum, a single character is a palindrome
            int n = str.length();
    
            // Iterate through each character as a possible center of a palindrome
            for (int center = 0; center < n; center++) {
                
                // Case 1: Odd-length palindromes (single character at center)
                int left = center, right = center;
                while (left >= 0 && right < n && str[left] == str[right]) {
                    maxLen = max(maxLen, right - left + 1);  // Update max palindrome length
                    left--;  // Expand to the left
                    right++; // Expand to the right
                }
    
                // Case 2: Even-length palindromes (two same consecutive characters at center)
                left = center, right = center + 1;
                while (left >= 0 && right < n && str[left] == str[right]) {
                    maxLen = max(maxLen, right - left + 1);  // Update max palindrome length
                    left--;  // Expand to the left
                    right++; // Expand to the right
                }
            }
    
            return maxLen; // Return the length of the longest palindrome found
        }
    
        // Function to find the longest palindromic substring after concatenating substrings of s and t
        int longestPalindrome(string s, string t) {
            // First, find the longest palindrome in s and t individually
            int maxLen = max(longestPalindromeInString(s), longestPalindromeInString(t));
    
            // Try all possible prefix-suffix combinations of s and t
            for (int i = 0; i <= s.length(); i++) {  // Iterate through all prefixes of s
                for (int j = 0; j <= t.length(); j++) {  // Iterate through all suffixes of t
                    string combined = s.substr(0, i) + t.substr(j); // Concatenate prefix from s and suffix from t
                    
                    // Check if the combined string forms a longer palindrome
                    if (!combined.empty() && longestPalindromeInString(combined) > maxLen) {
                        maxLen = longestPalindromeInString(combined); // Update max palindrome length
                    }
                }
            }
    
            return maxLen; // Return the maximum palindrome length found
        }
    };
    