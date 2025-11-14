/*
PROBLEM: Reverse Linked List II
Given a linked list and positions left and right (1-indexed), 
reverse the nodes from position left to position right.

APPROACH: Three-Phase Solution
1. LOCATE: Find the segment boundaries (before, start, end, after)
2. REVERSE: Reverse only the segment between left and right
3. RECONNECT: Link the reversed segment back to the main list

KEY INSIGHT: We need 4 critical pointers:
- preslow: Node just BEFORE the reversal segment (for reconnection)
- slow: FIRST node of the reversal segment (becomes tail after reversal)
- fast: LAST node of the reversal segment (becomes head after reversal)
- postfast: Node just AFTER the reversal segment (for reconnection)

EXAMPLE: 1->2->3->4->5, left=2, right=4
Before: 1 -> 2 -> 3 -> 4 -> 5
After:  1 -> 4 -> 3 -> 2 -> 5
        ^    ^---------^    ^
     preslow  reversed    postfast

TIME COMPLEXITY: O(n) - single pass to locate + single pass to reverse
SPACE COMPLEXITY: O(1) - only using pointers
*/

class Solution {
public:
    ListNode* reverseBetween(ListNode* head, int left, int right) {

        // ✅ Edge cases: No reversal needed
        if (!head || !head->next || left == right) return head;

        // 🎯 PHASE 1: LOCATE the segment boundaries
        int slowcnt = 1;    // Counter to track slow pointer position
        int fastcnt = 1;    // Counter to track fast pointer position

        ListNode* tempfast = head;  // Unused variable - could be removed
        
        // 🔑 Dummy node: Critical for handling edge case when left=1
        // Without dummy, we'd lose reference to reconnect at the start
        ListNode dummy(0, head);
        ListNode* preslow = &dummy;  // Points to node BEFORE reversal start
        ListNode* slow = head;       // Will point to reversal start (position left)
        ListNode* fast = head;       // Will point to reversal end (position right)

        // 📍 Find the START of reversal segment (position left)
        // preslow stops at node just before position left
        while (slowcnt != left) {
            slow = slow->next;
            slowcnt++;
            preslow = preslow->next;  // Keep preslow one step behind slow
        }

        // 📍 Find the END of reversal segment (position right)
        while (fastcnt != right) {
            fast = fast->next;
            fastcnt++;
        }

        // 📍 Store the node AFTER the reversal segment
        // This is where the reversed segment will reconnect
        ListNode* postfast = fast->next;

        // 🎯 PHASE 2: REVERSE the segment from slow to fast
        ListNode* forward = NULL;   // Temporary pointer to store next node

        // 🧠 CLEVER INSIGHT: Set prev = postfast initially
        // This automatically connects the last node of reversal to postfast
        ListNode* tail = slow;          // tail will become the last node after reversal
        ListNode* prev = postfast;      // Start with the connection point
        ListNode* curr = slow;          // Start reversing from the beginning

        // Standard reversal loop, but with a twist:
        // We stop at postfast (not nullptr) to reverse only the segment
        while (curr != postfast) {
            forward = curr->next;       // Store next before breaking the link
            curr->next = prev;          // Reverse the link
            prev = curr;                // Move prev forward
            curr = forward;             // Move curr forward
        }
        // After loop: prev points to the new head of reversed segment (original fast)

        // 🎯 PHASE 3: RECONNECT the reversed segment to main list
        preslow->next = prev;           // Connect node before segment to new head
        tail->next = postfast;          // Connect tail of reversed segment to after segment

        // Return dummy.next to handle the case where head itself changed
        return dummy.next;
    }
};

/*
STEP-BY-STEP WALKTHROUGH: [1,2,3,4,5], left=2, right=4

INITIAL STATE:
dummy -> 1 -> 2 -> 3 -> 4 -> 5
preslow=dummy, slow=1, fast=1

AFTER LOCATING:
preslow=1, slow=2, fast=4, postfast=5
1 -> 2 -> 3 -> 4 -> 5
^    ^---------^    ^
preslow  segment  postfast

REVERSAL PROCESS:
Initial: prev=5, curr=2
Step 1: 2->5, prev=2, curr=3
Step 2: 3->2, prev=3, curr=4  
Step 3: 4->3, prev=4, curr=5 (stop here as curr==postfast)

AFTER REVERSAL:
5 <- 2 <- 3 <- 4   5
     ^         ^    ^
   tail      prev  postfast

RECONNECTION:
preslow->next = prev  →  1 -> 4
tail->next = postfast →  2 -> 5

FINAL RESULT: 1 -> 4 -> 3 -> 2 -> 5

CRITICAL INSIGHTS:
1. Dummy node handles edge case when reversing from position 1
2. Setting prev=postfast initially eliminates need for separate reconnection
3. The original 'slow' becomes the tail of reversed segment
4. The original 'fast' becomes the head of reversed segment
5. We reverse pointers one by one while maintaining connection to rest of list