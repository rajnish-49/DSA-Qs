// https://leetcode.com/problems/gas-station/description/?envType=problem-list-v2&envId=greedy

class Solution {
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {

        int totalGas = 0; // Total net gas across all stations (to check feasibility)
        int tank = 0;     // Current tank when starting from candidate station
        int start = 0;    // Candidate starting station index

        int n = gas.size();

        for (int i = 0; i < n; ++i) {
            int net = gas[i] - cost[i]; // Net gas gain/loss at station i
            totalGas += net;            // Keep track of total gas vs total cost
            tank += net;                // Track current tank from candidate start

            // --- GREEDY STEP ---
            // If tank goes negative, it means starting from 'start' we can't reach station i+1
            if (tank < 0) {
                // PROOF OF SKIPPING INTERMEDIATE STATIONS:
                // Suppose we try to start at some station between 'start' and 'i'.
                // Let's call it 'mid'. Then, tank from mid to i is:
                // sum_{k=mid}^{i} net[k] = sum_{k=start}^{i} net[k] - sum_{k=start}^{mid-1} net[k]
                // We know sum_{k=start}^{i} net[k] < 0 (we failed at i)
                // Also, sum_{k=start}^{mid-1} net[k] >= 0 (otherwise we would have failed before mid)
                // Subtracting a non-negative number from a negative number is still negative.
                // Therefore, starting at 'mid' will also fail at i.
                // This is why the greedy approach is safe: we can skip all stations between 'start' and 'i'.

                start = i + 1; // Move candidate start to the next station after failure
                tank = 0;      // Reset tank because a new start always begins empty
            }
        }

        // --- FINAL CHECK ---
        // If total gas is enough to cover total cost, the last candidate 'start' is guaranteed to work.
        // Otherwise, return -1 (no solution).
        return (totalGas >= 0) ? start : -1;
    }
};
