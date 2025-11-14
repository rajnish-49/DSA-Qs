// @https://leetcode.com/problems/reverse-nodes-in-k-group/description/

class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        // If the list is empty OR k=1, there's nothing to reverse.
        // We return early to avoid unnecessary work.
        if (!head || k == 1) return head;

        // A dummy node sits before the head.
        // WHY? Because after reversing the first group, the "real" head
        // may change (e.g., from node 1 → node k). Dummy ensures we always
        // have a stable handle to the start of the list, no special cases.
        ListNode* dummy = new ListNode(0);
        dummy->next = head;

        // prevGroupEnd marks the tail of the already-processed portion.
        // WHY? After reversing a group, we must reconnect its tail
        // to the start of the next group. This pointer helps us "stitch."
        ListNode* prevGroupEnd = dummy;

        // curr is the start of the group we’re about to reverse.
        ListNode* curr = head;

        while (true) {
            // --- Step 1: Check if a full group of size k exists ---
            // We move forward k nodes. If we run out before reaching k,
            // it means fewer than k remain, so we leave them untouched.
            ListNode* kth = curr;
            for (int i = 1; i < k && kth; i++) {
                kth = kth->next;
            }
            if (!kth) break;  // Not enough nodes → stop processing

            // --- Step 2: Save boundary ---
            // nextGroupStart marks the node after the current group.
            // WHY? Because when we reverse this group, the old head becomes
            // the tail, and we need to connect it back to this node.
            ListNode* nextGroupStart = kth->next;

            // --- Step 3: Reverse the group [curr ... kth] ---
            // Classic linked list reversal, but we stop exactly at nextGroupStart
            // instead of nullptr. WHY? Because we only want to reverse k nodes,
            // and everything after must remain in correct order.
            ListNode* prev = nextGroupStart;
            ListNode* node = curr;
            while (node != nextGroupStart) {
                ListNode* tmp = node->next; // temporarily hold "next" node
                node->next = prev;          // flip the arrow
                prev = node;                // advance prev (grows reversed part)
                node = tmp;                 // advance node (shrinks unreversed part)
            }
            // After this loop:
            // - 'prev' points to the new head of this group (kth).
            // - 'curr' points to the old head (now tail).

            // --- Step 4: Stitch reversed group back into the list ---
            // Connect previous group's tail to new head
            prevGroupEnd->next = kth;

            // Move prevGroupEnd to the tail of this group (old head)
            prevGroupEnd = curr;

            // Advance curr to the start of the next group
            curr = nextGroupStart;
        }

        // Return the true head (dummy->next), since dummy was only a helper.
        return dummy->next;
    }
};
