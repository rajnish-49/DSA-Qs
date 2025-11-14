// @https://www.geeksforgeeks.org/problems/aggressive-cows/1

/*
Problem:
---------
You are given 'n' stalls at different positions along a number line and 'k' cows.
Place the cows in the stalls such that the minimum distance between any two cows is maximized.

Approach:
----------
- First, sort the stall positions to enable sequential greedy placement.
- Use Binary Search on the answer space (i.e., minimum distance between any two cows).
- For a candidate distance 'mid', use a greedy check (`canPlace`) to see if it's possible to place all cows such that each pair is at least 'mid' apart.
- Try to maximize this minimum distance using binary search.
*/

class Solution {
  public:

    // Helper function to check if we can place 'k' cows with at least 'minDist' distance between them
    bool canPlace(vector<int> &stalls, int minDist, int k) {
        int count = 1;              // Place the first cow at the first stall
        int lastPos = stalls[0];    // Track position of the last placed cow

        for (int i = 1; i < stalls.size(); i++) {
            // If current stall is at least 'minDist' away from last placed cow, place next cow
            if (stalls[i] - lastPos >= minDist) {
                count++;              // Place one more cow
                lastPos = stalls[i];  // Update last cow's position
            }

            // If we have placed all k cows successfully, placement is valid
            if (count >= k) return true;
        }

        // Not enough stalls with required separation
        return false;
    }

    // Main function to find the largest minimum distance between any two cows
    int aggressiveCows(vector<int> &stalls, int k) {

        // Step 1: Sort the stalls to facilitate greedy placement
        sort(stalls.begin(), stalls.end());

        // Step 2: Define the search space for minimum distance
        int low = 1;                                  // Minimum possible distance
        int high = stalls.back() - stalls.front();    // Maximum possible distance between farthest stalls
        int ans = 0;                                   // To store the largest valid minimum distance

        // Step 3: Binary Search to find the maximum of all valid minimum distances
        while (low <= high) {
            int mid = low + (high - low) / 2;  // Try middle distance as a candidate

            if (canPlace(stalls, mid, k)) {
                // If valid placement is possible, save the result and search for a larger distance
                ans = mid;
                low = mid + 1;
            } else {
                // If not possible, try smaller distances
                high = mid - 1;
            }
        }

        // Step 4: Return the largest minimum distance that allows valid placement
        return ans;
    }
};
