 /*
Problem:
Given an integer array nums and an integer k, return the kth smallest
element in the array.

Note:
The kth smallest element is based on sorted order, not the kth distinct element.

Example:
nums = [3,2,1,5,6,4], k = 2
Answer = 2

Approach:
Use a Max Heap of size k.

Idea:
- Keep the k smallest elements seen so far in the heap.
- Since it is a Max Heap, the largest among these k elements
  stays at the top.
- That largest element is the kth smallest element.

Question Link:
https://www.geeksforgeeks.org/problems/kth-smallest-element5635/1

Time Complexity: O(n log k)
Space Complexity: O(k)
*/

class Solution {
public:
    int kthSmallest(vector<int> &nums, int k) {

        // Max Heap
        // The largest element will always be at the top.
        priority_queue<int> maxHeap;

        for (int num : nums) {

            // Add current element to the heap
            maxHeap.push(num);

            // We only need to keep the k smallest elements.
            // If size becomes greater than k,
            // remove the largest element.
            if (maxHeap.size() > k) {
                maxHeap.pop();
            }
        }

        // Heap now contains the k smallest elements.
        // The largest among them is the kth smallest element.
        return maxHeap.top();
    }
};