// @https://leetcode.com/problems/remove-nth-node-from-end-of-list/description/

class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        // We use two pointers (fast and slow).
        // Idea: Move 'fast' n steps ahead first.
        // Then move both 'fast' and 'slow' together until 'fast' reaches the end.
        // At that moment, 'slow' will be right before the node we need to delete.

        ListNode* dummy = new ListNode(0); // dummy node to handle edge cases (like removing head)
        dummy->next = head;

        ListNode* fast = dummy;
        ListNode* slow = dummy;

        // Step 1: Move 'fast' pointer n+1 steps ahead
        // Why n+1? Because we want 'slow' to stop exactly at the node before the target node
        for (int i = 0; i <= n; i++) {
            fast = fast->next;
        }

        // Step 2: Move 'fast' and 'slow' together until 'fast' reaches end
        // Why this works: The gap of n ensures that when 'fast' is at end,
        // 'slow' will be right before the nth node from the end
        while (fast != nullptr) {
            fast = fast->next;
            slow = slow->next;
        }

        // Step 3: Skip the target node
        // 'slow->next' is the node we need to remove, so we bypass it
        slow->next = slow->next->next;

        // Step 4: Return updated head (could be different if head was removed)
        return dummy->next;
    }
};


// MORE STRAIGHTFORWARD APPROACH 

class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        // Step 1: Count total nodes in the list
        // Why: If we know the total length, we can figure out the index of the node 
        // to remove when counting from the front.
        int count = 0;
        ListNode* temp = head;
        while (temp != nullptr) {
            count++;
            temp = temp->next;
        }

        // Step 2: Find the position of the node (from the front) that we need to delete
        // Why: The nth node from the end = (count - n)th node from the front (0-based).
        int reach = count - n;

        // Step 3: Special case — if the node to delete is the head itself
        // Why: If reach == 0, it means the first node is the target, 
        // so we just move head forward.
        if (reach == 0) {
            ListNode* newHead = head->next;
            return newHead;
        }

        // Step 4: Traverse to the node just before the one we want to delete
        // Why: To delete a node, we need access to the previous node 
        // (so we can re-link its 'next' pointer).
        ListNode* temp2 = head;
        for (int i = 1; i < reach; i++) {
            temp2 = temp2->next;
        }

        // Step 5: Remove the target node by skipping it
        // Why: This effectively "cuts out" the unwanted node from the chain.
        ListNode* nodeToDelete = temp2->next;  
        temp2->next = temp2->next->next;

        // Step 6: Return updated head
        // Why: In case we removed the first node, the head could have changed.
        return head;
    }
};
