// @ https://www.geeksforgeeks.org/problems/merge-k-sorted-arrays/1?itm_source=geeksforgeeks&itm_medium=article&itm_campaign=practice_card

class Node {
public:
    int value, arrayIdx, elemIdx;

    // Constructor to initialize node with element value, array index, and element index
    Node(int v, int a, int e) {
        value = v;
        arrayIdx = a;
        elemIdx = e;
    }
};

class Compare {
public:
    // Min Heap: The node with the smallest value should be at the top
    bool operator()(Node a, Node b) {
        return a.value > b.value; // Higher value means lower priority
    }
};

vector<int> mergeKSortedArrays(vector<vector<int>>& arr) {
    
    priority_queue<Node, vector<Node>, Compare> minHeap; // Min heap to store k elements
    vector<int> result; // Final merged sorted array

    // Step 1: Insert the first element of each array into the min heap
    for (int i = 0; i < arr.size(); i++)
        minHeap.push(Node(arr[i][0], i, 0)); // Insert (value, array index, element index)

    // Step 2: Extract minimum element and push the next element from the same array
    while (!minHeap.empty()) {
        Node node = minHeap.top(); // Get the smallest element from heap
        minHeap.pop(); // Remove the extracted element
        result.push_back(node.value); // Store the extracted element in the result array

        // If there is a next element in the same array, insert it into the heap
        if (node.elemIdx + 1 < arr[node.arrayIdx].size())
            minHeap.push(Node(arr[node.arrayIdx][node.elemIdx + 1], node.arrayIdx, node.elemIdx + 1));
    }

    return result; // Return the fully merged sorted array
}

