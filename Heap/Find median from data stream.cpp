/*
 * Problem: Find Median from Data Stream
 * Link: https://leetcode.com/problems/find-median-from-data-stream/
 *
 * Implement a data structure that supports:
 *   - addNum(int num)  : add a number from the data stream
 *   - findMedian()     : return the median of all elements so far
 *
 * Example:
 *   addNum(1), addNum(2) → findMedian() = 1.5
 *   addNum(3)            → findMedian() = 2.0
 *
 * ─────────────────────────────────────────────────────────────────────────────
 * APPROACH: Two heaps — max-heap for left half, min-heap for right half
 *
 * Core insight:
 *   The median only depends on the middle element(s). You don't need the
 *   full stream sorted — you only ever need:
 *     - the largest element of the left half   → maxHeap.top()
 *     - the smallest element of the right half → minHeap.top()
 *   Heaps give you these boundary elements in O(1), with O(log n) insertion.
 *
 * Two invariants maintained at all times:
 *   1. Every element in the left half ≤ every element in the right half
 *   2. |leftSize - rightSize| ≤ 1, with left allowed to have one extra
 *
 * findMedian logic that follows from invariant 2:
 *   - Odd total  → left has one extra → median = maxHeap.top()
 *   - Even total → sizes equal        → median = (maxHeap.top() + minHeap.top()) / 2.0
 *
 * ─────────────────────────────────────────────────────────────────────────────
 * Complexity:
 *   Time:  O(log n) per addNum — two heap operations at most
 *          O(1)     per findMedian — just reading tops
 *   Space: O(n) — all elements stored across both heaps
 */

class MedianFinder {

    // Left half — we only ever need its maximum, so max-heap
    priority_queue<int> maxHeap;

    // Right half — we only ever need its minimum, so min-heap
    priority_queue<int, vector<int>, greater<int>> minHeap;

public:
    MedianFinder() {}

    void addNum(int num) {

        // Step 1: Route num to the correct half.
        // The left boundary is maxHeap.top(). If num fits within the left
        // half, push left. Otherwise it belongs to the right half.
        // Empty check is a guard — no boundary exists yet when left is empty,
        // so the first element always goes left by default.
        if (maxHeap.empty() || num <= maxHeap.top()) {
            maxHeap.push(num);
        } else {
            minHeap.push(num);
        }

        // Step 2: Rebalance if sizes violated.
        // Routing grows one heap by 1 — it may break the size invariant.
        // We fix it by moving the boundary element of the oversized heap
        // to the other. Moving the boundary element is always safe because
        // it is the element closest in value to the other half's boundary.
        if (maxHeap.size() > minHeap.size() + 1) {
            // left has 2 more than right — move left's max to right
            minHeap.push(maxHeap.top());
            maxHeap.pop();
        }
        if (minHeap.size() > maxHeap.size()) {
            // right has more than left — move right's min to left
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }

    double findMedian() {
        // Odd total: left has one extra element — median is left's max
        // Even total: both halves equal size — median is average of both tops
        if (maxHeap.size() > minHeap.size()) return maxHeap.top();
        return (maxHeap.top() + minHeap.top()) / 2.0;
    }
};