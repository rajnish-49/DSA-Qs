/*
 * Problem: K-th Largest Sum Contiguous Subarray
 * Link: https://www.geeksforgeeks.org/problems/k-th-largest-sum-contiguous-subarray/1
 *
 * Given an array of integers, find the k-th largest sum among all
 * contiguous subarrays.
 *
 * Example:
 *   Input:  arr = [3, 2, 1], k = 2
 *   Output: 5
 *   Explanation: All subarray sums = [3, 2, 1, 5, 3, 6]
 *                Sorted descending = [6, 5, 3, 3, 2, 1]
 *                2nd largest = 5
 *
 * ─────────────────────────────────────────────────────────────────────────────
 * APPROACH: Fixed-size min-heap of size k
 *
 * Core insight:
 *   Same pattern as Top K Frequent Elements — maintain a min-heap of size k
 *   where the top is always the smallest among the k largest sums seen so far.
 *   When a new sum arrives, it competes with the current minimum. If it wins,
 *   the minimum is evicted and the new sum takes its place. After processing
 *   all subarray sums, the top of the heap is the k-th largest.
 *
 * Why min-heap and not max-heap?
 *   A max-heap would require generating all subarray sums first, then popping
 *   k times — O(n² + k log n). A min-heap of size k processes each sum in
 *   O(log k) on the fly, no storage of all sums needed.
 *
 * How all subarray sums are generated:
 *   Fix a start index i, then extend end index j from i to n-1, accumulating
 *   the sum incrementally. This avoids recomputing sums from scratch —
 *   sum[i..j] = sum[i..j-1] + arr[j].
 *
 * ─────────────────────────────────────────────────────────────────────────────
 * Complexity:
 *   Time:  O(n² log k) — n² subarray sums, each heap operation is O(log k)
 *   Space: O(k)        — heap holds at most k sums at any time
 */

class Solution {
public:
    int kthLargest(vector<int>& arr, int k) {

        // Min-heap of size k — top is always the weakest (smallest) among
        // the k largest sums seen so far. Any new sum stronger than the top
        // evicts it and takes its place.
        priority_queue<int, vector<int>, greater<int>> minHeap;

        for (int i = 0; i < arr.size(); i++) {
            int sum = 0;

            for (int j = i; j < arr.size(); j++) {
                // Incrementally build sum[i..j] from sum[i..j-1] — avoids
                // restarting the sum computation for every (i, j) pair.
                sum += arr[j];

                if (minHeap.size() < k) {
                    // Heap not full yet — every sum is a candidate, push directly.
                    minHeap.push(sum);
                } else if (sum > minHeap.top()) {
                    // Heap full — current sum is stronger than the weakest
                    // among our k candidates. Evict the weakest, insert the new sum.
                    minHeap.pop();
                    minHeap.push(sum);
                }
                // If sum <= minHeap.top(), it can't be in the top k — discard.
            }
        }

        // The weakest survivor of all eviction rounds = the k-th largest sum.
        return minHeap.top();
    }
};