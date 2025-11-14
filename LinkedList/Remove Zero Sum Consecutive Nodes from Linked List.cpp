// @https://leetcode.com/problems/remove-zero-sum-consecutive-nodes-from-linked-list/

class Solution {
public:
    ListNode* removeZeroSumSublists(ListNode* head) {
        // Dummy node makes life easier when prefix sum cancels from the start.
        // Example: [1,-1] → dummy ensures prefix 0 exists before we even begin.
        ListNode dummy(0, head);
        unordered_map<int, ListNode*> mp;

        int psum = 0;
        ListNode* temp = &dummy;

        // ---------------- PASS 1 ----------------
        // Build a map: prefix_sum -> *last* node where this prefix occurs.
        // Why last? Because if the prefix repeats, then the sublist in between
        // must sum to 0, and we want to "skip" as far forward as possible.
        while (temp) {
            psum += temp->val;
            mp[psum] = temp; // overwrite ensures we always keep the last occurrence
            temp = temp->next;
        }

        // ---------------- PASS 2 ----------------
        // Rewalk the list, recompute prefix sums, and jump to the
        // farthest node stored for that sum. This effectively removes
        // all zero-sum blocks in between.
        psum = 0;
        temp = &dummy;

        while (temp) {
            psum += temp->val;
            // "Cut out" any zero-sum sequence by skipping directly
            // from the current node to the node after the last one
            // with the same prefix sum.
            temp->next = mp[psum]->next;
            temp = temp->next;
        }

        // Return the cleaned list (dummy->next may no longer equal head
        // if prefix sums at the beginning cancelled out).
        return dummy.next;
    }
};
