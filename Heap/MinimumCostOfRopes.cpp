int minCost(vector<int> &ropes) {
    // Min-heap (priority queue) to always get the smallest two ropes
    priority_queue<int, vector<int>, greater<int>> minHeap(ropes.begin(), ropes.end()); 

    int totalCost = 0; // To store the total cost of connecting ropes

    // Continue until only one rope remains in the heap
    while (minHeap.size() > 1) {
        // Step 1: Extract the two smallest ropes (min elements)
        int first = minHeap.top();
        minHeap.pop(); // Remove the first smallest element
        int second = minHeap.top();
        minHeap.pop(); // Remove the second smallest element

        // Step 2: Compute the cost of connecting these two ropes
        int cost = first + second;
        totalCost += cost; // Accumulate total cost

        // Step 3: Insert the new merged rope back into the heap
        minHeap.push(cost);
    }

    // The final accumulated cost is the minimum cost to connect all ropes
    return totalCost;
}
