/*
Problem:
Given an integer array nums and an integer k, return the k largest
elements from the array.

Example:
nums = [3, 2, 1, 5, 6, 4], k = 3
Answer = [4, 5, 6]

Approach:
Use a Min Heap of size k.

Idea:
- Keep only the k largest elements in the heap.
- The smallest element among them stays at the top.
- Whenever we get a new element and the heap size becomes
  greater than k, remove the smallest element.

Question Link:
https://www.geeksforgeeks.org/problems/k-largest-elements4206/1

Time Complexity: O(n log k)
Space Complexity: O(k)
*/

class Solution {
public:
    vector<int> kLargest(vector<int>& nums, int k) {

        // Min Heap
        // The smallest element is always at the top.
        priority_queue<int, vector<int>, greater<int>> minHeap;

        for (int num : nums) {

            // Add the current element to the heap
            minHeap.push(num);

            // We only want to keep k elements.
            // If we have more than k elements,
            // remove the smallest one.
            if (minHeap.size() > k) {
                minHeap.pop();
            }
        }

        // The heap now contains the k largest elements.
        vector<int> ans;

        while (!minHeap.empty()) {
            ans.push_back(minHeap.top());
            minHeap.pop();
        }

        return ans;
    }
};