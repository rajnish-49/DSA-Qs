// @ https://leetcode.com/problems/minimum-operations-to-exceed-threshold-value-ii/

class Solution {
    public:
        int minOperations(vector<int>& nums, int k) {
            // Min-heap (priority queue) to always fetch the two smallest elements efficiently
            priority_queue<long, vector<long>, greater<long>> pq;
    
            // Insert all elements from the nums array into the min-heap
            for (auto i : nums) {
                pq.push(i);
            }
    
            // Count the number of operations performed
            long count = 0;
    
            // Continue processing until the smallest element in the heap is at least 'k'
            while (pq.top() < k) {
                // Extract the two smallest elements
                long n1 = pq.top();
                pq.pop();
                long n2 = pq.top();
                pq.pop();
    
                // Compute the new number using the formula: (2 * min + max)
                long new_num = (2 * n1) + n2;
    
                // Insert the new number back into the min-heap
                pq.push(new_num);
    
                // Increment the operation count
                count++;
            }
    
            // Return the minimum number of operations required
            return count;
        }
    };
    