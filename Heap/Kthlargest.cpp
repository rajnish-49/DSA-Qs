// @ https://leetcode.com/problems/kth-largest-element-in-an-array/description/

class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        // Min-heap to store the top 'k' largest elements.
        // Using 'greater<int>' makes it a min-heap, where the smallest element is at the top.
        priority_queue<int, vector<int>, greater<int>> pq; 

        // Step 1: Insert the first 'k' elements into the min-heap
        for (int i = 0; i < k; i++) {
            pq.push(nums[i]); // Push first k elements into the heap
        }

        // Step 2: Process the remaining elements in the array
        for (int i = k; i < nums.size(); i++) { // Traverse elements from index k to end
            if (nums[i] > pq.top()) {  
                // If the current element is greater than the smallest element in the heap,
                // it means it should be included in the top k largest elements.
                pq.pop();  // Remove the smallest element (top of the heap)
                pq.push(nums[i]); // Insert the new element into the heap
            }
        }

        // Step 3: The root of the heap (pq.top()) is the k-th largest element
        return pq.top();
    }
};


