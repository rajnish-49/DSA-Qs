class Solution {
    public:

    // Function to heapify a subtree rooted at index i
    void heapify(vector<int> &heap, int n, int i) {
        int largest = i; // Assume root is the largest
        int left = 2 * i + 1;  // Left child index
        int right = 2 * i + 2; // Right child index

        // Check if left child exists and is greater than root
        if (left < n && heap[left] > heap[largest])
            largest = left;

        // Check if right child exists and is greater than largest so far
        if (right < n && heap[right] > heap[largest])
            largest = right;

        // If largest is not root, swap and continue heapifying
        if (largest != i) {
            swap(heap[i], heap[largest]); // Swap values
            heapify(heap, n, largest); // Recursively heapify the affected subtree
        }
    }

    // Function to merge two max heaps into one
    vector<int> mergeHeaps(vector<int> &a, vector<int> &b, int n, int m) {
        // Step 1: Merge both heaps into a single array
        vector<int> merged = a; // Copy elements of first heap
        merged.insert(merged.end(), b.begin(), b.end()); // Append elements of second heap

        int size = merged.size(); // New heap size

        // Step 2: Convert the merged array into a max heap
        // Start heapifying from the last non-leaf node down to the root
        for (int i = size / 2 - 1; i >= 0; i--) {
            heapify(merged, size, i);
        }

        // Step 3: Return the final max heap
        return merged;
    }
};
