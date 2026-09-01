/*
 * Problem: Merge K Sorted Arrays
 *
 * Given k sorted arrays, merge them into one sorted array.
 *
 * Example:
 *   Input:  [[1,4,5], [1,3,4], [2,6]]
 *   Output: [1,1,2,3,4,4,5,6]
 *
 * ─────────────────────────────────────────────────────────────────────────────
 * APPROACH: Min-heap of k frontier elements
 *
 * Core insight:
 *   Identical to Merge K Sorted Lists — the globally smallest element at
 *   any point must be one of the k current frontier elements, one from each
 *   array. Instead of pushing nodes, we push {value, {arrayIndex, elementIndex}}
 *   so that after extracting the min we know exactly which array to advance
 *   and which index to push next.
 *
 * Why store indices instead of just values?
 *   Unlike a linked list where the node itself holds a next pointer, arrays
 *   have no self-referential structure. The only way to know "what comes after
 *   this element" is to remember which array it came from (i) and its position
 *   in that array (j). Then next element is simply mat[i][j+1].
 *
 * ─────────────────────────────────────────────────────────────────────────────
 * Complexity:
 *   Time:  O(N log k) — N total elements, each pushed and popped once;
 *                        each heap operation is O(log k) since heap holds at most k elements
 *   Space: O(k) — heap holds at most one frontier element per array at any time
 */

vector<int> mergeArrays(vector<vector<int>>& mat) {
    int k = mat.size();
    vector<int> res;

    // Min-heap storing {value, {arrayIndex, elementIndex}}.
    // Ordered by value — greater<> on pairs compares first element first,
    // so the smallest value naturally sits at the top.
    // arrayIndex and elementIndex are carried along purely to locate
    // the next element to push after extraction.
    priority_queue<pair<int, pair<int, int>>,
                   vector<pair<int, pair<int, int>>>,
                   greater<pair<int, pair<int, int>>>> minHeap;

    // Step 1: Seed the heap with the first element of each non-empty array.
    // Each array contributes exactly one frontier element at any point in time.
    for (int i = 0; i < k; i++) {
        if (!mat[i].empty()) {
            minHeap.push({mat[i][0], {i, 0}});
        }
    }

    while (!minHeap.empty()) {

        // Step 2: Extract the globally smallest frontier element.
        auto top = minHeap.top();
        minHeap.pop();
        int val = top.first;
        int i = top.second.first;   // which array this came from
        int j = top.second.second;  // its position in that array

        res.push_back(val);

        // Step 3: Advance this array's frontier by one.
        // If a next element exists in the same array, push it —
        // it is now this array's new candidate for the globally smallest position.
        if (j + 1 < mat[i].size()) {
            minHeap.push({mat[i][j + 1], {i, j + 1}});
        }
    }

    return res;
}