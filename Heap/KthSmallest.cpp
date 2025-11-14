// @ https://www.geeksforgeeks.org/problems/kth-smallest-element5635/1?itm_source=geeksforgeeks&itm_medium=article&itm_campaign=practice_card



int kthSmallest(int arr[], int l, int r, int k) {
    // Max heap (priority queue) to store the k smallest elements
    priority_queue<int> pq;

    // Step 1: Insert the first k elements into the max heap
    for (int i = 0; i < k; i++) {
        pq.push(arr[i]); // Push first k elements into the heap
    }

    // Step 2: Process the remaining elements
    for (int i = k; i <= r; i++) { // Traverse remaining elements in the array
        if (arr[i] < pq.top()) {   // If current element is smaller than max element in heap
            pq.pop();              // Remove the largest element from the heap
            pq.push(arr[i]);       // Insert the smaller element into the heap
        }
    }

    // Step 3: The root of the heap (pq.top()) is the k-th smallest element
    return pq.top();
}
