class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        vector<int> answer;

        int n = s.size();
        int k = p.size();

        if (k > n) {
            return answer;
        }

        unordered_map<char, int> pFrequency;
        unordered_map<char, int> windowFrequency;

        // Build the exact character-frequency requirement of p.
        // Example: p = "aab"
        // pFrequency = { 'a': 2, 'b': 1 }
        for (char ch : p) {
            pFrequency[ch]++;
        }

        int left = 0;

        // right expands the current window one character at a time.
        for (int right = 0; right < n; right++) {

            // Include s[right] in the current window.
            windowFrequency[s[right]]++;

            // The required window size is fixed at p.size().
            // If the window becomes larger, remove its leftmost character
            // so that [left ... right] again contains exactly k characters.
            if (right - left + 1 > k) {
                char leavingChar = s[left];

                windowFrequency[leavingChar]--;

                // A character with frequency 0 is no longer present
                // in the current window. Erase it so map equality works
                // correctly without extra zero-frequency keys.
                if (windowFrequency[leavingChar] == 0) {
                    windowFrequency.erase(leavingChar);
                }

                left++;
            }

            // An anagram must:
            // 1. have the same length as p, and
            // 2. contain every character with exactly the same frequency.
            //
            // If both maps are equal, s[left ... right] is an anagram of p.
            if (right - left + 1 == k &&
                windowFrequency == pFrequency) {
                answer.push_back(left);
            }
        }

        return answer;
    }
};