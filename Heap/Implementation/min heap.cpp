/*
 * MIN HEAP — ARRAY IMPLEMENTATION
 *
 * Min-Heap Property:
 *     Every parent <= its children.
 *
 * Therefore:
 *     heap[0] = minimum element
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
 *     getMin()     → return root
 *     extractMin() → remove root + heapify down
 *
 * Time:
 *     insert      → O(log n)
 *     getMin      → O(1)
 *     extractMin  → O(log n)
 */

class MinHeap {

    // Array stores the heap.
    // The index of each element represents its position in the tree.
    vector<int> heap;


    /*
     * HEAPIFY DOWN
     *
     * Used when the element at index i may be GREATER than its children.
     *
     * Example:
     *
     *          30
     *        /    \
     *       10     20
     *
     * 30 violates the min-heap property.
     *
     * We repeatedly:
     *
     * 1. Find the SMALLER of the two children.
     * 2. If the smaller child < current element,
     *    swap them.
     * 3. Continue from the child's new position.
     *
     * Why the SMALLER child?
     *
     * In a min-heap, parent must be <= BOTH children.
     * So the smaller child must move up.
     */
    void heapifyDown(int i) {

        int n = heap.size();

        while (true) {

            // Find the indices of the two children.
            int left = 2 * i + 1;
            int right = 2 * i + 2;

            // Initially assume current element is the smallest.
            int smallest = i;


            // If left child exists and is smaller,
            // make left the current smallest.
            if (left < n && heap[left] < heap[smallest])
                smallest = left;


            // Compare right with whichever is currently smallest.
            if (right < n && heap[right] < heap[smallest])
                smallest = right;


            // Current element is already <= both children.
            // Min-heap property is restored.
            if (smallest == i)
                break;


            // Smaller child moves up,
            // current element moves down.
            swap(heap[i], heap[smallest]);

            // Continue fixing the heap from the child's position.
            i = smallest;
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
     *    is smaller than the parent.
     *
     * This is called HEAPIFY UP / BUBBLE UP.
     */
    void insert(int x) {

        // Add the new element at the next available position.
        heap.push_back(x);

        // Start from the newly inserted element.
        int i = heap.size() - 1;


        // Keep moving upward until we reach the root
        // or the parent is already <= current element.
        while (i > 0) {

            // Find parent using the 0-based heap formula.
            int parent = (i - 1) / 2;


            // Parent is already smaller/equal.
            // Min-heap property is satisfied.
            if (heap[parent] <= heap[i])
                break;


            // Current element is smaller than its parent,
            // so swap them and continue upward.
            swap(heap[parent], heap[i]);

            i = parent;
        }
    }


    /*
     * GET MIN
     *
     * In a min-heap, the minimum element is ALWAYS at the root.
     *
     * Root = index 0
     *
     * Therefore:
     *     return heap[0]
     *
     * Time: O(1)
     */
    int getMin() {

        if (heap.empty())
            throw runtime_error("Heap is empty");

        return heap[0];
    }


    /*
     * EXTRACT MIN
     *
     * Remove and return the minimum element.
     *
     * The minimum is at heap[0].
     *
     * Steps:
     *
     * 1. Save the root.
     * 2. Move the LAST element to the root.
     * 3. Remove the last element.
     * 4. Heapify DOWN from the root.
     *
     * Why heapify down?
     *
     * After moving the last element to the root,
     * the new root may be larger than one of its children.
     *
     * We push it downward until the min-heap property is restored.
     */
    int extractMin() {

        if (heap.empty())
            throw runtime_error("Heap is empty");


        // Root contains the minimum.
        int ans = heap[0];


        // Move the last element to the root.
        // This fills the hole created by removing the minimum.
        heap[0] = heap.back();


        // Remove the duplicate last element.
        heap.pop_back();


        // If elements are still present,
        // the new root may violate the min-heap property.
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