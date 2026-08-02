class Solution {
public:
    string minWindow(string s, string t) {
        if (t.size() > s.size()) {
            return "";
        }

        vector<int> required(128, 0);
        vector<int> window(128, 0);

        // Store how many times each character is required from t.
        // Example: t = "AABC"
        // required['A'] = 2, required['B'] = 1, required['C'] = 1
        for (char ch : t) {
            required[ch]++;
        }

        // Number of distinct characters whose frequencies must be satisfied.
        int requiredCharacters = 0;

        for (int i = 0; i < 128; i++) {
            if (required[i] > 0) {
                requiredCharacters++;
            }
        }

        int formedCharacters = 0;
        int left = 0;

        int minLength = INT_MAX;
        int answerStart = 0;

        // Expand the window by moving right.
        for (int right = 0; right < s.size(); right++) {
            char currentChar = s[right];

            window[currentChar]++;

            // A required character becomes satisfied only when its
            // frequency reaches exactly the amount needed in t.
            //
            // Extra copies do not increase formedCharacters again.
            if (required[currentChar] > 0 &&
                window[currentChar] == required[currentChar]) {
                formedCharacters++;
            }

            // If every required character has enough occurrences,
            // the current window contains all characters of t.
            //
            // Now shrink from the left to find the smallest valid
            // window ending at the current right index.
            while (formedCharacters == requiredCharacters) {
                int currentLength = right - left + 1;

                if (currentLength < minLength) {
                    minLength = currentLength;
                    answerStart = left;
                }

                char leftChar = s[left];

                // Before removing leftChar, check whether it is currently
                // present exactly as many times as required.
                //
                // Removing it will then make this character insufficient,
                // causing the window to become invalid.
                if (required[leftChar] > 0 &&
                    window[leftChar] == required[leftChar]) {
                    formedCharacters--;
                }

                window[leftChar]--;
                left++;
            }
        }

        // No valid window was ever found.
        if (minLength == INT_MAX) {
            return "";
        }

        return s.substr(answerStart, minLength);
    }
};