class Solution {
public:
    string reverseVowels(string s) {
        // Initialize two pointers: i at start, j at end
        int i = 0, j = s.size() - 1;
        
        // String containing all vowels (both lowercase and uppercase)
        // We'll use this to check if a character is a vowel
        string vowels = "aeiouAEIOU";
        
        // Continue until pointers meet or cross
        while (i < j) {
            
            // STEP 1: Move left pointer (i) forward until we find a vowel
            // vowels.find(s[i]) returns string::npos if s[i] is NOT found in vowels
            // So this loop continues while s[i] is NOT a vowel AND i < j
            while (i < j && vowels.find(s[i]) == string::npos) {
                i++;  // Skip non-vowel characters from left
            }
            
            // STEP 2: Move right pointer (j) backward until we find a vowel
            // Similar logic: continue while s[j] is NOT a vowel AND i < j
            while (i < j && vowels.find(s[j]) == string::npos) {
                j--;  // Skip non-vowel characters from right
            }
            
            // STEP 3: If both pointers are still valid and pointing to vowels
            // At this point: s[i] is a vowel, s[j] is a vowel, and i < j
            if (i < j) {
                swap(s[i], s[j]);  // Swap the two vowels
                i++;               // Move left pointer forward
                j--;               // Move right pointer backward
            }
            
            // Loop continues with next iteration
            // This process repeats until all vowels are reversed
        }
        
        return s;  // Return the modified string
    }
};

/*
ALGORITHM EXPLANATION:
1. Use two-pointer technique with i starting from left, j from right
2. Find vowels from both ends simultaneously
3. When both pointers point to vowels, swap them
4. Continue until pointers meet in the middle

TIME COMPLEXITY: O(n) - each character is visited at most twice
SPACE COMPLEXITY: O(1) - only using a few variables (vowels string is constant)

EXAMPLE WALKTHROUGH:
Input: "hello"
- Initial: i=0(h), j=4(o)
- i moves to 1(e) since 'h' is not a vowel
- Both 'e' and 'o' are vowels, so swap → "holle"
- i=2, j=3, but s[2]='l' and s[3]='l' are not vowels
- Pointers move and meet, algorithm ends
- Result: "holle"
*/