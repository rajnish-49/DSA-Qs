/*
 * Problem: Minimum Cost to Connect Ropes
 * Link: https://www.geeksforgeeks.org/problems/minimum-cost-of-ropes-1587115620/1
 *
 * Given n ropes of different lengths, connect them into one rope.
 * The cost of connecting two ropes = sum of their lengths.
 * Find the minimum total cost to connect all ropes.
 *
 * Example:
 *   Input:  ropes = [4, 3, 2, 6]
 *   Output: 29
 *   Explanation: connect 2+3=5 (cost 5), connect 4+5=9 (cost 9),
 *                connect 6+9=15 (cost 15) → total = 5+9+15 = 29
 *
 * ─────────────────────────────────────────────────────────────────────────────
 * APPROACH: Greedy + Min-heap
 *
 * Core insight:
 *   Every time two ropes are merged, the cost of that merge gets added to
 *   the total. But crucially, the merged rope participates in all future
 *   merges too — its length gets added again and again. So a rope that is
 *   merged early contributes to more future costs than one merged late.
 *   To minimize total cost, always merge the two shortest ropes first —
 *   this keeps the intermediate rope lengths as small as possible, reducing
 *   their contribution to future merges.
 *
 * Why a min-heap?
 *   After each merge, the new rope's length may not be the smallest anymore.
 *   A min-heap always gives the two shortest ropes in O(log n) — exactly
 *   what the greedy strategy needs at every step.
 *
 * ─────────────────────────────────────────────────────────────────────────────
 * Complexity:
 *   Time:  O(n log n) — n-1 merges, each with two pops and one push, all O(log n)
 *   Space: O(n)       — heap stores all rope lengths
 */

class Solution {
public:
    int minCost(vector<int>& ropes) {

        // Seed the heap with all rope lengths directly via range constructor —
        // builds the heap in O(n) instead of n individual O(log n) pushes.
        priority_queue<int, vector<int>, greater<int>> minHeap(ropes.begin(), ropes.end());

        int totalCost = 0;

        // Keep merging until one rope remains.
        // Each iteration reduces rope count by 1 (two removed, one added).
        while (minHeap.size() > 1) {

            // Greedily pick the two shortest ropes — they contribute least
            // to future merges if combined now rather than later.
            int first = minHeap.top(); minHeap.pop();
            int second = minHeap.top(); minHeap.pop();

            int cost = first + second;

            // This merge's cost is added to the total.
            // The merged rope re-enters the heap and competes for future merges.
            totalCost += cost;
            minHeap.push(cost);
        }

        return totalCost;
    }
};