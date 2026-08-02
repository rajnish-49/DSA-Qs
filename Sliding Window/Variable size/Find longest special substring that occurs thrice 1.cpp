// @https://leetcode.com/problems/find-longest-special-substring-that-occurs-thrice-i/description/

class Solution {
public:
    int maximumLength(string s) {
        int n = s.size();

        // count[ch][len] stores the total number of occurrences of a
        // special substring made of character ch and having length len.
        //
        // Example:
        // count[0][2] = number of times "aa" occurs in the whole string.
        vector<vector<int>> count(26, vector<int>(n + 1, 0));

        int left = 0;

        // right moves through the string and helps us identify each
        // maximal consecutive block of the same character.
        for (int right = 0; right < n; right++) {

            // The current block ends when:
            // 1. right is the last index, or
            // 2. the next character is different.
            //
            // Current completed block is s[left ... right].
            if (right == n - 1 || s[right] != s[right + 1]) {
                int blockLength = right - left + 1;
                int character = s[left] - 'a';

                // A block of length L contains special substrings
                // of every possible length from 1 to L.
                //
                // Example: block = "aaaa", L = 4
                // len 1: "a"    occurs 4 times
                // len 2: "aa"   occurs 3 times
                // len 3: "aaa"  occurs 2 times
                // len 4: "aaaa" occurs 1 time
                for (int len = 1; len <= blockLength; len++) {
                    int occurrences = blockLength - len + 1;

                    // Add this block's contribution.
                    // Counts from later blocks of the same character
                    // and same length will accumulate here.
                    count[character][len] += occurrences;
                }

                // The next consecutive block starts after right.
                left = right + 1;
            }
        }

        int answer = -1;

        // Check every character and every possible special-substring length.
        // A length is valid when that exact special substring occurs
        // at least three times across all blocks of that character.
        for (int character = 0; character < 26; character++) {
            for (int len = 1; len <= n; len++) {
                if (count[character][len] >= 3) {
                    answer = max(answer, len);
                }
            }
        }

        return answer;
    }
};