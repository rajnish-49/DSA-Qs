/*
Problem: 881. Boats to Save People
Link: https://leetcode.com/problems/boats-to-save-people/

Description:
Each boat can carry at most 2 people, and the total weight of those
people cannot exceed 'limit'.

Return the minimum number of boats required to carry everyone.

Approach: Greedy + Two Pointers

1. Sort the people by weight.
2. Keep one pointer at the lightest person and one at the heaviest.
3. Always handle the heaviest person first.

   - If lightest + heaviest <= limit:
     pair them together and move both pointers.

   - Otherwise:
     the heaviest person must go alone because if even the lightest
     person cannot fit with them, nobody else can.

4. Every iteration uses exactly one boat.

Time Complexity: O(n log n)
Space Complexity: O(1) apart from sorting space.
*/

class Solution {
public:
    int numRescueBoats(vector<int>& people, int limit) {

        sort(people.begin(), people.end());

        int left = 0;
        int right = people.size() - 1;

        int boats = 0;

        while (left <= right) {

            // If the lightest person can fit with the heaviest,
            // pair them together.
            if (people[left] + people[right] <= limit) {
                left++;
                right--;
            }

            // Otherwise, the heaviest person goes alone.
            else {
                right--;
            }

            // In both cases, we used one boat.
            boats++;
        }

        return boats;
    }
};