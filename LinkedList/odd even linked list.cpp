// @https://leetcode.com/problems/odd-even-linked-list/description/


class Solution {
public:
    ListNode* oddEvenList(ListNode* head) {
        // Edge case: if the list has 0 or 1 nodes, no rearrangement is possible or needed.
        // Return head directly because "odd-even grouping" makes no difference.
        if (!head || !head->next) return head;

        // The algorithm partitions the list into two chains:
        //   1. Odd-indexed nodes (1st, 3rd, 5th, … in position order)
        //   2. Even-indexed nodes (2nd, 4th, 6th, … in position order)
        //
        // We then stitch these two chains together: odd list first, then even list.
        //
        // To do this in O(1) space and O(n) time, we maintain two pointers:
        // - `odd` : always points to the last node in the odd chain
        // - `even`: always points to the last node in the even chain
        // We also keep `evenHead` to reconnect later.

        ListNode* odd = head;          // start odd chain at 1st node
        ListNode* even = head->next;   // start even chain at 2nd node
        ListNode* evenHead = even;     // save start of even chain for final stitching

        // Loop until we run out of nodes for the even chain.
        // Why condition `even && even->next`?
        // Because inside the loop we will:
        //   - do `odd->next = even->next` (so even->next must exist)
        //   - then advance even one step (so even must exist)
        // This condition guarantees safety for both operations.
        while (even && even->next) {
            // Step 1: link current odd node to the next odd node.
            //   Before: odd → even → oddNext
            //   After : odd → oddNext (skipping even)
            // This "collects" the next odd node into the odd chain.
            odd->next = even->next;

            // Step 2: move odd pointer forward.
            // Now odd points to the new last node of the odd chain.
            odd = odd->next;

            // Step 3: link current even node to the next even node.
            //   Before: even → oddNext → evenNext
            //   After : even → evenNext (skipping oddNext)
            // This "collects" the next even node into the even chain.
            even->next = odd->next;

            // Step 4: move even pointer forward.
            // Now even points to the new last node of the even chain.
            even = even->next;
        }

        // After the loop:
        // - The odd chain is fully built and ends at the last odd node.
        // - The even chain is fully built and ends at the last even node.
        // - The two chains are currently disjoint.
        //
        // Now we simply attach the even chain to the end of the odd chain.
        odd->next = evenHead;

        // Return the original head, which now represents the start of the rearranged list.
        return head;
    }
};
