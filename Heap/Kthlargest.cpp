/*
Problem:
Given an integer array nums and an integer k, return the kth largest
element in the array.

Note:
The kth largest element is based on sorted order, not the kth distinct element.

Example:
nums = [3,2,1,5,6,4], k = 2
Answer = 5

Approach:
Use a Min Heap of size k.

Idea:
- Keep the k largest elements seen so far in the heap.
- Since it is a Min Heap, the smallest among these k elements
  stays at the top.
- That smallest element is the kth largest element.

Question Link:
https://leetcode.com/problems/kth-largest-element-in-an-array/

Time Complexity: O(n log k)
Space Complexity: O(k)
*/

class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {

        // Min Heap
        // The smallest element will always be at the top.
        priority_queue<int, vector<int>, greater<int>> minHeap;

        for (int num : nums) {

            // Add current element to the heap
            minHeap.push(num);

            // We only need to keep the k largest elements.
            // If size becomes greater than k, remove the smallest.
            if (minHeap.size() > k) {
                minHeap.pop();
            }
        }

        // Heap now contains the k largest elements.
        // The smallest among them is the kth largest element.
        return minHeap.top();
    }
};