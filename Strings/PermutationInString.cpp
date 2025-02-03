#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool checkInclusion(string s1, string s2) {
    // If s1 is longer than s2, s1 cannot be a permutation of any substring of s2
    if (s1.length() > s2.length()) return false;

    // Arrays to store character counts
    vector<int> count1(26, 0);
    vector<int> count2(26, 0);

    // Fill count1 with the frequency of characters in s1
    for (char c : s1) {
        count1[c - 'a']++;
    }

    // Initialize the first window in s2
    for (int i = 0; i < s1.length(); i++) {
        count2[s2[i] - 'a']++;
    }

    // Function to compare two count vectors
    auto checkEqual = [](const vector<int>& a, const vector<int>& b) {
        for (int i = 0; i < 26; i++) {
            if (a[i] != b[i]) return false;
        }
        return true;
    };

    // Check the initial window
    if (checkEqual(count1, count2)) return true;

    // Slide the window over s2
    for (int i = s1.length(); i < s2.length(); i++) {
        // Add the new character to the window
        count2[s2[i] - 'a']++;
        // Remove the old character from the window
        count2[s2[i - s1.length()] - 'a']--;

        // Check if the current window matches the character count of s1
        if (checkEqual(count1, count2)) return true;
    }

    return false;
}

int main() {
    string s1 = "ab";
    string s2 = "eidbaooo";

    bool result = checkInclusion(s1, s2);
    
    if (result) {
        cout << "s1 is a permutation of a substring of s2." << endl;
    } else {
        cout << "s1 is not a permutation of any substring of s2." << endl;
    }

    return 0;
}
