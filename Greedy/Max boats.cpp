// @ https://leetcode.com/problems/boats-to-save-people/

class Solution {
public:
    int numRescueBoats(vector<int>& people, int limit) {
        // STEP 1: Sort people by weight
        // Sorting is necessary because we want to pair the heaviest person
        // with the lightest person possible (greedy approach).
        sort(people.begin(), people.end());

        // STEP 2: Initialize two pointers
        // l → index of the lightest person not yet placed in a boat
        // r → index of the heaviest person not yet placed in a boat
        int l = 0;
        int r = people.size() - 1;

        int ans = 0;  // total number of boats required

        // STEP 3: Process until all people are placed in boats
        while (l <= r) {
            // CASE 1: If the lightest + heaviest can share a boat
            if (people[l] + people[r] <= limit) {
                // Both can go together
                l++;  // move to next lightest
                r--;  // move to next heaviest
            } 
            // CASE 2: Otherwise, the heaviest person must go alone
            else {
                // We cannot pair r with l (or anyone else),
                // so the heaviest person occupies one boat alone.
                r--;  // move to next heaviest
            }
            // In both cases, we used one boat
            ans++;
        }

        // STEP 4: Return total number of boats used
        return ans;
    }
};
