/*
Problem: 1029. Two City Scheduling
Link: https://leetcode.com/problems/two-city-scheduling/

Description:
There are 2N people. For each person:
costs[i][0] = cost to send them to city A
costs[i][1] = cost to send them to city B

Exactly N people must go to city A and N people to city B.
We need to minimize the total cost.

Approach: Greedy

Assume initially that everyone is sent to city A.

For each person, if we switch them from A to B, the change in cost is:

    costB - costA

We must move exactly N people to city B.

So we choose the N people with the smallest (costB - costA),
because these are the people for whom moving to B is cheapest
or gives the maximum saving.

Therefore:
1. Sort people by (costB - costA).
2. Send the first N people to city B.
3. Send the remaining N people to city A.

Time Complexity: O(n log n)
Space Complexity: O(1) apart from sorting.
*/

class Solution {
public:
    int twoCitySchedCost(vector<vector<int>>& costs) {

        int n = costs.size() / 2;

        // Sort according to the extra cost of sending a person
        // to city B instead of city A.
        sort(costs.begin(), costs.end(),
             [](const vector<int>& a, const vector<int>& b) {
                 return (a[1] - a[0]) < (b[1] - b[0]);
             });

        int totalCost = 0;

        // First N people have the smallest B - A difference,
        // so they are the best people to send to city B.
        for (int i = 0; i < n; i++) {
            totalCost += costs[i][1];
        }

        // Remaining N people are sent to city A.
        for (int i = n; i < 2 * n; i++) {
            totalCost += costs[i][0];
        }

        return totalCost;
    }
};