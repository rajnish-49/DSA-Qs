/*
Problem: Cinema Seat Allocation

There are n rows in a cinema, each having 10 seats numbered 1 to 10.
Some seats are already reserved.

A family of 4 must sit together in one of these blocks:
    [2,3,4,5]
    [4,5,6,7]
    [6,7,8,9]

Find the maximum number of families that can be seated.

Key observation:
For a row, only seats 2 to 9 matter. Seats 1 and 10 can never
belong to a valid 4-seat family block.

Problem Link:
https://leetcode.com/problems/cinema-seat-allocation/
*/

class Solution {
public:
    int maxNumberOfFamilies(int n, vector<vector<int>>& reservedSeats) {

        // Keep only the rows that have reserved seats.
        // For each such row, store which seat numbers are reserved.
        map<int, set<int>> reserved;

        for(auto &seat : reservedSeats) {
            reserved[seat[0]].insert(seat[1]);
        }

        // Any row with no reservations can always fit 2 families:
        // one in [2-5] and another in [6-9].
        int ans = (n - reserved.size()) * 2;

        for(auto &[row, seats] : reserved) {

            // Check whether the left block [2-5] is completely free.
            bool left = !seats.count(2) && !seats.count(3) &&
                        !seats.count(4) && !seats.count(5);

            // Check whether the right block [6-9] is completely free.
            bool right = !seats.count(6) && !seats.count(7) &&
                         !seats.count(8) && !seats.count(9);

            if(left && right) {
                // Both non-overlapping blocks are available,
                // so this row can accommodate 2 families.
                ans += 2;
            }

            else if(left || right) {
                // One of the two outer blocks is available,
                // so we can accommodate 1 family.
                ans += 1;
            }

            else {
                // Neither outer block is available.
                // The only remaining possible block is the middle [4-7].
                //
                // It can be used only if seats 4,5,6,7 are all free.
                bool middle = !seats.count(4) &&
                              !seats.count(5) &&
                              !seats.count(6) &&
                              !seats.count(7);

                if(middle)
                    ans += 1;
            }
        }

        return ans;
    }
};