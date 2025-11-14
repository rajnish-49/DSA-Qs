// @https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/description/

ListNode* deleteDuplicates(ListNode* head) {
    // Create a dummy node that points to head.
    // This helps handle edge cases cleanly (like when head itself is a duplicate).
    ListNode dummy(0, head);

    // 'prev' will always point to the *last confirmed unique node*
    // i.e., the last node that we decided is not part of a duplicate block.
    ListNode* prev = &dummy;

    // 'temp' is the main scanning pointer that moves through the list.
    ListNode* temp = head;

    // Traverse the entire linked list
    while (temp) {
        // Case 1: Current node 'temp' has duplicates ahead
        // Check if next node exists AND has the same value
        if (temp->next && temp->val == temp->next->val) {
            // Keep moving 'temp' forward until the value changes
            // This skips the entire block of duplicates
            while (temp->next && temp->val == temp->next->val) {
                temp = temp->next;
            }
            // At this point, 'temp' is at the *last node* of the duplicate block.
            // Skip the entire duplicate block by linking prev->next
            // to the node after the duplicate block (temp->next).
            prev->next = temp->next;
        } else {
            // Case 2: Current node 'temp' has no duplicates
            // So it's safe to include it in the final list.
            // Move 'prev' forward to point at this node.
            prev = temp;
        }

        // Always move 'temp' forward to continue scanning
        temp = temp->next;
    }

    // Return the new head (dummy.next), which skips any duplicates at the start.
    return dummy.next;
}


class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if (!head) return nullptr;

        // We create a dummy node before head.
        // The reason: if the very first node(s) form a duplicate block,
        // we need a stable anchor to reconnect the list after removing them.
        ListNode dummy(0, head);

        // preSlow always points to the node just before the current candidate block.
        // This allows us to "cut out" duplicates cleanly by reconnecting around them.
        ListNode* preSlow = &dummy;

        // slow marks the start of a block of equal values we are currently examining.
        ListNode* slow = head;

        // fast scans ahead to see how far this block of equal values extends.
        ListNode* fast = head->next;

        while (slow) {
            bool duplicate = false;

            // Move fast forward as long as it matches slow's value.
            // By the end, fast will either be nullptr or the first node with a different value.
            // This way, the entire duplicate block is skipped in one go.
            while (fast && fast->val == slow->val) {
                duplicate = true;
                fast = fast->next;
            }

            if (duplicate) {
                // If duplicates were found, the entire block starting at slow
                // must be discarded. We do this by linking preSlow directly to fast,
                // which jumps over all those duplicate nodes.
                preSlow->next = fast;
            } else {
                // If no duplicates were found, slow is a unique node and should remain.
                // So we advance preSlow to point at slow,
                // extending our "clean" portion of the list.
                preSlow = slow;
            }

            // Advance to the next block: slow takes fast's position.
            // fast is then moved one step ahead to prepare for the next scan.
            slow = fast;
            if (fast) fast = fast->next;
        }

        // dummy.next is the new head of the cleaned list,
        // since dummy was pointing at the original head.
        return dummy.next;
    }
};
