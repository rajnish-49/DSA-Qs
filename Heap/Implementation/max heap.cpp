/*
 * MAX HEAP — ARRAY IMPLEMENTATION
 *
 * Max-Heap Property:
 *     Every parent >= its children.
 *
 * Therefore:
 *     heap[0] = maximum element
 *
 * Array ↔ Tree index formulas (0-based):
 *
 *     parent(i) = (i - 1) / 2
 *     left(i)   = 2*i + 1
 *     right(i)  = 2*i + 2
 *
 * Main operations:
 *
 *     insert(x)    → add at end + heapify up
 *     getMax()     → return root
 *     extractMax() → remove root + heapify down
 *
 * Time:
 *     insert      → O(log n)
 *     getMax     → O(1)
 *     extractMax → O(log n)
 */

class MaxHeap {

    // Array stores the heap.
    // The position/index of an element represents its place in the tree.
    vector<int> heap;


    /*
     * HEAPIFY DOWN
     *
     * Used when the element at index i may be SMALLER than its children.
     *
     * Example:
     *
     *          10
     *        /    \
     *       30     20
     *
     * 10 violates the max-heap property.
     *
     * We repeatedly:
     *
     * 1. Find the larger of the two children.
     * 2. If the larger child > current element,
     *    swap them.
     * 3. Continue from the child's new position.
     *
     * Why the LARGER child?
     * Because in a max-heap, the parent must be >= BOTH children.
     * So if we need to move down, we must put the larger child above.
     */
    void heapifyDown(int i) {

        int n = heap.size();

        while (true) {

            // Find the indices of the two children.
            int left = 2 * i + 1;
            int right = 2 * i + 2;

            // Initially assume current element is the largest.
            int largest = i;


            // If left child exists and is larger,
            // make left the current largest.
            if (left < n && heap[left] > heap[largest])
                largest = left;


            // Compare right with whichever is currently largest.
            // This automatically handles:
            //     current > left > right
            //     left > current > right
            //     right > left > current
            // etc.
            if (right < n && heap[right] > heap[largest])
                largest = right;


            // Current element is already >= both children.
            // Heap property is restored.
            if (largest == i)
                break;


            // Larger child moves up, current element moves down.
            swap(heap[i], heap[largest]);

            // Continue fixing the heap from the new position.
            i = largest;
        }
    }


public:

    /*
     * INSERT
     *
     * To insert an element:
     *
     * 1. Put it at the END of the array.
     *    → This keeps the tree complete.
     *
     * 2. The only possible violation is with its PARENT.
     *
     * 3. Keep swapping with the parent while the new element
     *    is larger than the parent.
     *
     * This process is called HEAPIFY UP / BUBBLE UP.
     */
    void insert(int x) {

        // Add the new element at the next available position.
        heap.push_back(x);

        // Start from the newly inserted element.
        int i = heap.size() - 1;


        // Keep moving upward until we reach the root
        // or the parent is already >= current element.
        while (i > 0) {

            // Find parent using the 0-based heap formula.
            int parent = (i - 1) / 2;


            // Parent is already larger/equal.
            // Max-heap property is satisfied.
            if (heap[parent] >= heap[i])
                break;


            // Current element is larger than its parent,
            // so swap them and continue upward.
            swap(heap[parent], heap[i]);

            i = parent;
        }
    }


    /*
     * GET MAX
     *
     * In a max-heap, the maximum element is ALWAYS at the root.
     *
     * Root = index 0
     *
     * Therefore this operation is simply:
     *     return heap[0]
     *
     * Time: O(1)
     */
    int getMax() {

        if (heap.empty())
            throw runtime_error("Heap is empty");

        return heap[0];
    }


    /*
     * EXTRACT MAX
     *
     * Remove and return the maximum element.
     *
     * The maximum is at heap[0].
     *
     * Problem:
     *     If we simply remove index 0, we would create a hole
     *     and shifting the entire array would be expensive.
     *
     * Solution:
     *
     *     1. Save the root.
     *     2. Move the LAST element to the root.
     *     3. Remove the last element.
     *     4. Heapify DOWN from the root.
     *
     * Why does heapify down work?
     *
     * Before removal, everything except the root's new position
     * already satisfies the heap property.
     *
     * So we only need to fix the path from the root downward.
     */
    int extractMax() {

        if (heap.empty())
            throw runtime_error("Heap is empty");


        // Root contains the maximum.
        int ans = heap[0];


        // Move the last element to the root.
        // This fills the hole created by removing the maximum.
        heap[0] = heap.back();


        // Remove the duplicate last element.
        heap.pop_back();


        // If elements are still present, the new root
        // may violate the max-heap property.
        //
        // Push it down until the heap is valid again.
        if (!heap.empty())
            heapifyDown(0);


        return ans;
    }


    /*
     * UTILITY FUNCTIONS
     */
    bool empty() {
        return heap.empty();
    }

    int size() {
        return heap.size();
    }
};