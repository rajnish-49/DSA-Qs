// @ https://leetcode.com/problems/count-number-of-bad-pairs/description/

class Solution {
public:
    long long countBadPairs(vector<int>& nums) {
        // Frequency map to store the count of (nums[i] - i) for each index i
        unordered_map<long long, long long> m;

        // Variable to store the number of good pairs
        long long goodPairs = 0;

        // Size of the input array
        long long n = nums.size();

        // Iterate through the array
        for (int i = 0; i < n; i++) {
            // Calculate the key for the current index i
            // Key = nums[i] - i
            int key = nums[i] - i;

            // If this key already exists in the map, it means there are previous indices
            // that form good pairs with the current index. Add the frequency of the key
            // to the goodPairs count.
            goodPairs += m[key];

            // Increment the frequency of the current key in the map
            m[key]++;
        }

        // Calculate the total number of pairs (i, j) where i < j
        // Formula: n * (n - 1) / 2
        long long total = (n * (n - 1)) / 2;

        // The number of bad pairs is the total pairs minus the number of good pairs
        return total - goodPairs;
    }
};