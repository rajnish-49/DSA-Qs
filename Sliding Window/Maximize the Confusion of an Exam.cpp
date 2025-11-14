// @https://leetcode.com/problems/maximize-the-confusion-of-an-exam/

// BINARY SEACRH SOLUTION 

#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

class Solution {
public:

    /**
     * Helper function: isValid
     *
     * Purpose:
     *   For a given window size = `length`, check if there exists ANY window
     *   in the string that can be made all 'T' or all 'F' using at most `k` flips.
     *
     * Idea:
     *   - We'll slide a window of size `length` across the string.
     *   - For each window, count how many 'T's and 'F's it has.
     *   - To make all 'T's: we need to flip all 'F's.
     *   - To make all 'F's: we need to flip all 'T's.
     *   - So if min(countT, countF) <= k → we can flip and make this window valid.
     *
     * This is a classical sliding window frequency problem.
     */
    bool isValid(const string& answerKey, int k, int length) {
        int countT = 0, countF = 0;

        for (int i = 0; i < answerKey.size(); ++i) {
            // Add current character to the window count
            if (answerKey[i] == 'T') countT++;
            else countF++;

            // If window size exceeds the target length, shrink from left
            if (i >= length) {
                if (answerKey[i - length] == 'T') countT--;
                else countF--;
            }

            // Once we have a full window of size = length
            if (i >= length - 1) {
                // Check if we can make all same using ≤ k flips
                if (min(countT, countF) <= k) return true;
            }
        }

        // No such window found
        return false;
    }

    /**
     * Main function: maxConsecutiveAnswers
     *
     * Problem:
     *   Maximize the length of a contiguous block of same characters (either all 'T' or all 'F')
     *   by flipping at most k characters.
     *
     * Key Insight:
     *   - Instead of solving the problem directly, we use binary search to guess the answer.
     *   - For each guess `mid`, we ask: "Is it possible to create a block of size `mid` with at most k flips?"
     *   - If yes, we try a larger value.
     *   - If no, we try a smaller value.
     *
     * Why Binary Search Works:
     *   - The problem is monotonic:
     *       - If size L is valid → all sizes < L are also valid.
     *       - If size L is invalid → all sizes > L are also invalid.
     *   - This monotonicity is perfect for binary search.
     */
    int maxConsecutiveAnswers(string answerKey, int k) {
        int left = 1;                          // Minimum possible block size
        int right = answerKey.size();          // Maximum block size (whole string)
        int result = 0;                        // Store the best valid block size found

        while (left <= right) {
            int mid = (left + right) / 2;      // Try this as candidate block length

            if (isValid(answerKey, k, mid)) {
                // If block of size mid is valid, try finding a larger one
                result = mid;
                left = mid + 1;
            } else {
                // If block of size mid is invalid, reduce the size
                right = mid - 1;
            }
        }

        return result;
    }
};

// pure sliding window solution

class Solution {
public:
    int maxConsecutiveAnswers(string answerKey, int k) {

        int n = answerKey.size();

        unordered_map<char, int> mp;  // Tracks frequency of 'T' and 'F' in the current window
        int res = 0;                  // Final result: maximum length of valid window
        int maxi = 0;                 // Max count of a single character ('T' or 'F') in current window
        int j = 0;                    // Left pointer of the sliding window

        // i = right pointer of the sliding window
        for (int i = 0; i < n; i++) {
            mp[answerKey[i]]++;              // Include the new character at right in window count

            maxi = max(maxi, mp[answerKey[i]]);  // Update the max freq of any char in window

            /*
              Window size = (i - j + 1)
              To make all characters in the window the same, we need to flip the other chars.

              So, flips_needed = window_size - maxi

              If flips_needed > k ⇒ too many flips, window is invalid, so shrink from the left
            */
            while ((i - j + 1) - maxi > k) {
                mp[answerKey[j]]--;  // Remove leftmost char from window count
                j++;                 // Shrink window from the left
            }

            /*
              Now, window [j...i] is valid ⇒ (i - j + 1) is a candidate answer
              Update result if this window is the largest so far
            */
            res = max(res, i - j + 1);
        }

        return res;
    }
};

