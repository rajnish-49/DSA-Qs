// @ https://www.geeksforgeeks.org/problems/k-th-largest-sum-contiguous-subarray/1?itm_source=geeksforgeeks&itm_medium=article&itm_campaign=practice_card


class Solution {
  public:
    int kthLargest(vector<int> &arr, int k) {
        // Min Heap to store the k largest sums
        priority_queue<int, vector<int>, greater<int>> minHeap;  

        // Generate all contiguous subarrays and compute their sums
        for (int i = 0; i < arr.size(); i++) {
            int sum = 0; // Initialize sum for subarray starting at index i
            
            for (int j = i; j < arr.size(); j++) {
                sum += arr[j]; // Add current element to sum

                // If heap size is less than k, directly push sum into heap
                if (minHeap.size() < k)
                    minHeap.push(sum);
                else if (sum > minHeap.top()) { 
                    // If heap already has k elements and sum is greater than the smallest sum in heap
                    // Remove the smallest sum (minHeap.top()) and insert the new sum
                    minHeap.pop();
                    minHeap.push(sum);
                }
            }
        }

        // The root of the min heap will be the k-th largest sum
        return minHeap.top();
    }
};
