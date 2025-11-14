// @ 

class Solution {
public:
    ListNode* removeElements(ListNode* head, int val) {
        // ---- Step 1: Handle deletions at the head ----
        // If the very first node(s) contain the value we want to delete,
        // simply move 'head' forward until we reach a node with a different value
        // or until the list becomes empty.
        // Example: head = [6,6,1,2], val = 6 → head will move to node '1'.
        while (head != nullptr && head->val == val) {
            head = head->next; 
        }

        // After removing leading nodes, check if the list is now empty.
        // Example: head = [6,6,6], val = 6 → head becomes nullptr here.
        if (head == nullptr) return nullptr;

        // ---- Step 2: Traverse the rest of the list ----
        // At this point, 'head' points to a valid node whose value != val.
        // We'll use a 'temp' pointer to scan through the list.
        ListNode *temp = head;

        // As long as the *next* node exists, check whether it should be deleted.
        // (We look at temp->next instead of temp because deleting the "next"
        // node doesn't break our traversal — we still have 'temp' pointing
        // to a valid node.)
        while (temp->next != nullptr) {
            if (temp->next->val == val) {
                // If the next node’s value matches 'val', we "skip" it
                // by linking current node to next->next.
                // This effectively deletes the next node from the list.
                temp->next = temp->next->next;
            } else {
                // Otherwise, move forward to the next node.
                // Only advance 'temp' when no deletion happens,
                // because after a deletion, temp->next has changed,
                // and we need to recheck it.
                temp = temp->next;
            }
        }

        // ---- Step 3: Return the updated head ----
        // Head might be different than the original (if leading nodes were deleted),
        // but it now points to the correct start of the cleaned list.
        return head;
    }
};
