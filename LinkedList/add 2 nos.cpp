class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {

        // dummy node acts as a stable anchor for the result list
        // → avoids special handling for the first node
        ListNode* dummy = new ListNode(0);

        // tail always tracks the *last* node in the result
        // → lets us append in O(1) without traversing each time
        ListNode* tail = dummy;

        // carry propagates the overflow when two digits sum ≥ 10
        // → mimics the "carry over" step of manual addition
        int carry = 0;

        // keep looping as long as:
        //   - l1 still has digits, OR
        //   - l2 still has digits, OR
        //   - there’s a leftover carry
        // ensures unequal length lists and final carry are both handled
        while (l1 != nullptr || l2 != nullptr || carry != 0) {

            // fetch digits from both lists
            // if one list is exhausted, treat its digit as 0
            // → this makes the addition logic uniform and symmetric
            int x = (l1 != nullptr) ? l1->val : 0;
            int y = (l2 != nullptr) ? l2->val : 0;

            // perform column addition for this digit + carry
            int sum = x + y + carry;

            // new carry is the overflow (≥ 10 → carry 1, else 0)
            carry = sum / 10;

            // actual digit to store in this position is the remainder
            // → sum % 10 gives the correct "ones place"
            tail->next = new ListNode(sum % 10);

            // move tail forward so it always points to the last node
            tail = tail->next;

            // advance both input lists if possible
            // → naturally handles uneven lengths since null is skipped
            if (l1) l1 = l1->next;
            if (l2) l2 = l2->next;
        }

        // final step: skip the dummy and return the true head
        ListNode* head = dummy->next;

        // free the dummy to avoid memory leak (the rest of the list remains intact)
        delete dummy;

        return head;
    }
};
