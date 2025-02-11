// @ https://leetcode.com/problems/merge-k-sorted-lists/description/

// Custom comparator class for the priority queue (min-heap)
class Compare {
    public:
        // Defines the comparison for the min-heap
        // This makes sure the smallest element is at the top
        bool operator()(ListNode* a, ListNode* b) {
            return a->val > b->val; // Min-heap: the smallest value has higher priority
        }
    };
    
    class Solution {
    public:
        // Function to merge k sorted linked lists
        ListNode* mergeKLists(vector<ListNode*>& lists) {
            // Min-heap (priority queue) to store the nodes from different linked lists
            priority_queue<ListNode*, vector<ListNode*>, Compare> minHeap;
    
            // Step 1: Push the head (first node) of each linked list into the heap
            for (int i = 0; i < lists.size(); i++) {
                if (lists[i] != NULL) {  // Only push non-null heads
                    minHeap.push(lists[i]);
                }
            }
    
            // Step 2: Initialize pointers for the resulting merged linked list
            ListNode* head = nullptr; // Head of the merged list
            ListNode* tail = nullptr; // Tail to keep track of the last node
    
            // Step 3: Process the heap and build the merged linked list
            while (!minHeap.empty()) {
                // Extract the smallest element from the heap
                ListNode* smallest = minHeap.top();
                minHeap.pop();
    
                // Step 3.1: Attach the smallest node to the merged list
                if (!head) { 
                    // If this is the first node being added, initialize head and tail
                    head = tail = smallest;
                } else {
                    // Otherwise, append to the existing list and update tail
                    tail->next = smallest;
                    tail = tail->next;
                }
    
                // Step 3.2: Push the next node of the extracted node into the heap
                if (smallest->next) {
                    minHeap.push(smallest->next);
                }
            }
    
            // Step 4: Return the head of the merged sorted linked list
            return head;
        }
    };
    