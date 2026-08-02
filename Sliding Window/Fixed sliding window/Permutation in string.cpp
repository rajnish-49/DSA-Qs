class Solution {
public:
    bool checkInclusion(string s1, string s2) {
        int n = s2.size();
        int k = s1.size();

        if (k > n) {
            return false;
        }

        vector<int> s1Frequency(26, 0);
        vector<int> windowFrequency(26, 0);

        // Store the required frequency of every character in s1.
        for (char ch : s1) {
            s1Frequency[ch - 'a']++;
        }

        int left = 0;

        for (int right = 0; right < n; right++) {

            // Add the new character entering the current window.
            windowFrequency[s2[right] - 'a']++;

            // Keep the window size at most equal to s1.size().
            if (right - left + 1 > k) {
                windowFrequency[s2[left] - 'a']--;
                left++;
            }

            // A permutation of s1 must have the same length and
            // exactly the same frequency of every character.
            if (right - left + 1 == k &&
                windowFrequency == s1Frequency) {
                return true;
            }
        }

        return false;
    }
};