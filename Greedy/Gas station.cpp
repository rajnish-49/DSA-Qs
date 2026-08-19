/*
Problem: 134. Gas Station
Link: https://leetcode.com/problems/gas-station/

Description:
There are n gas stations arranged in a circle.

gas[i]  = amount of gas available at station i
cost[i] = gas required to travel from station i to station i + 1

Return the starting station index from which we can complete
the entire circular route.

If it is impossible, return -1.

Approach: Greedy

For every station, the useful value is:

    gas[i] - cost[i]

This tells us how much our tank changes after leaving station i.

We maintain:
1. totalTank   -> checks whether completing the whole circuit is possible
2. currentTank -> checks whether the current chosen start can continue
3. start       -> current candidate starting station

Key greedy observation:

If we start from 'start' and currentTank becomes negative at station i,
then starting from 'start' cannot reach station i + 1.

Also, none of the stations between start and i can be valid starts.

Why?
Because while travelling from start to i, the tank was non-negative
before finally failing at i.

Any station in between would start with less accumulated fuel than
we had from 'start', so it would also fail before or at i.

Therefore, we can safely discard all those stations and try:

    start = i + 1

Finally:
- If total gas < total cost, no solution exists.
- Otherwise, the final 'start' is the valid answer.

Time Complexity: O(n)
Space Complexity: O(1)
*/

class Solution {
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {

        int totalTank = 0;
        int currentTank = 0;
        int start = 0;

        for (int i = 0; i < gas.size(); i++) {

            // Net fuel gained/lost after travelling from
            // station i to station i + 1.
            int net = gas[i] - cost[i];

            totalTank += net;
            currentTank += net;

            // Current starting station cannot reach i + 1.
            // So every station from 'start' to i can be discarded
            // as a possible starting point.
            if (currentTank < 0) {
                start = i + 1;
                currentTank = 0;
            }
        }

        // If total gas is less than total travel cost,
        // completing the entire circular route is impossible.
        if (totalTank < 0) {
            return -1;
        }

        return start;
    }
};