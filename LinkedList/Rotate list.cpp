// @https://leetcode.com/problems/rotate-list/

class Solution {
public:
    ListNode* rotateRight(ListNode* head, int k) {
        // Edge case: 
        // If the list is empty or has only one node, rotation won't change anything.
        if (!head || !head->next) return head; 

        // ------------------------
        // STEP 1: Find the length of the list and the tail
        // ------------------------
        int length = 0;
        ListNode* temp = head;
        ListNode* tail = nullptr;

        while (temp) {
            length++;                         
            if (temp->next == nullptr) {
                tail = temp;   // keep track of the last node (important for reconnection later)
            } 
            temp = temp->next;
        }
        // At this point:
        // length = number of nodes in the list
        // tail = last node (so we can later connect it back to the head)

        // ------------------------
        // STEP 2: Optimize k
        // ------------------------
        // Rotating 'length' times brings the list back to the original position.
        // So we only need to rotate (k % length) times.
        k = k % length;
        if (k == 0) return head;  // If no effective rotation needed, return as is.

        // ------------------------
        // STEP 3: Identify the new head
        // ------------------------
        // The new head will be at position (length - k) from the start.
        // WHY? Because rotating right by k means:
        // - The last k nodes move to the front
        // - So the "cut" point is right before those k nodes
        //
        // Example: [1,2,3,4,5], k=2
        // length=5, k=2 → cut after (5-2)=3rd node (value 3)
        // So new head will be node with value 4.
        int stepsToNewHead = length - k;

        ListNode* prev = nullptr;   // node just before the new head
        ListNode* curr = head;      // will land on the new head

        for (int i = 0; i < stepsToNewHead; i++) {
            prev = curr;            // move prev forward
            curr = curr->next;      // move curr forward
        }
        // After this loop:
        // prev = node just before new head
        // curr = new head (the node that rotation should start from)

        // ------------------------
        // STEP 4: Break and reconnect
        // ------------------------
        // Currently list = [head ... prev] → [curr ... tail]
        // We want:        [curr ... tail] → [head ... prev]
        //
        // So:
        prev->next = nullptr;   // break the list at prev
        tail->next = head;      // attach old tail to old head

        // ------------------------
        // STEP 5: Return the new head
        // ------------------------
        return curr;   // new head of the rotated list
    }
};
