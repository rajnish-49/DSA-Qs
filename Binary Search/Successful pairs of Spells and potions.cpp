// @https://leetcode.com/problems/successful-pairs-of-spells-and-potions/

class Solution {
public:
    vector<int> successfulPairs(vector<int>& spells, vector<int>& potions, long long success) {
        
        // Step 1: Sort the potions array to enable binary search
        sort(potions.begin(), potions.end());

        int m = potions.size(); // Number of potions

        vector<int> ans; // To store result for each spell

        // Step 2: For each spell, find how many potions form a successful pair
        for (int i = 0; i < spells.size(); i++) {
            int s = spells[i]; // Current spell strength

            // Step 3: Compute the minimum potion strength required to be successful with this spell
            // We want: spell * potion >= success
            // Rearranged: potion >= ceil(success / spell)
            // To avoid floating-point division, use (success + s - 1) / s
            long minpotion = (success + s - 1) / s;

            // Step 4: Use binary search to find the first potion >= minpotion
            // Since potions is sorted, lower_bound returns iterator to first valid potion
            int idx = lower_bound(potions.begin(), potions.end(), minpotion) - potions.begin();

            // Step 5: Number of successful potions is all potions from idx to end
            int total = m - idx;

            // Step 6: Store the result
            ans.push_back(total);
        }

        // Step 7: Return the answer vector
        return ans;
    }
};
