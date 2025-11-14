// @https://leetcode.com/problems/minimum-number-of-days-to-make-m-bouquets/description/

class Solution {
public:
    // Helper function to check if we can make 'm' bouquets by 'day'
    // Each bouquet requires 'k' adjacent bloomed flowers.
    bool canMakeBouquets(vector<int>& bloomDay, int day, int m, int k) {
        int bouquets = 0;  // Count of bouquets made so far
        int flowers = 0;   // Count of consecutive bloomed flowers

        // Iterate through each flower in the garden
        for (int i = 0; i < bloomDay.size(); i++) {
            // If the flower has bloomed by 'day', increment consecutive count
            if (bloomDay[i] <= day) {
                flowers++;
                // Once we have enough adjacent flowers for one bouquet
                if (flowers == k) {
                    bouquets++;      // One bouquet is made
                    flowers = 0;    // Reset consecutive count for next bouquet
                }
            } else {
                // Flower not bloomed by 'day', reset consecutive count
                flowers = 0;
            }

            // If we already made enough bouquets, return true early for efficiency
            if (bouquets >= m) return true;
        }

        // Return true if we can make at least 'm' bouquets, else false
        return bouquets >= m;
    }

    // Main function to find the minimum number of days to make 'm' bouquets
    int minDays(vector<int>& bloomDay, int m, int k) {
        int n = bloomDay.size();

        // Quick check: If total flowers needed exceed available flowers, return -1 immediately
        // Because m bouquets * k flowers each = total flowers required
        if ((long long)m * k > n) return -1;

        // Initialize binary search boundaries
        // The earliest possible day is the minimum bloom day
        int low = *min_element(bloomDay.begin(), bloomDay.end());
        // The latest possible day is the maximum bloom day
        int high = *max_element(bloomDay.begin(), bloomDay.end());
        int result = -1;  // To store the minimum valid day found

        // Binary search to find the smallest day that allows making m bouquets
        while (low <= high) {
            int mid = low + (high - low) / 2;  // Middle day to test

            // Check if we can make bouquets by day = mid
            if (canMakeBouquets(bloomDay, mid, m, k)) {
                result = mid;      // mid is a valid day
                high = mid - 1;   // Try to find an even smaller valid day (search left)
            } else {
                low = mid + 1;    // mid not valid, need more days (search right)
            }
        }

        // Return the minimum day found or -1 if impossible
        return result;
    }
};
