/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */

class Solution {
public:

    // ---------------------------------------------
    // Step 1: Helper to find the middle of a list
    // ---------------------------------------------
    ListNode* findMiddlePrev(ListNode* head) {
        // If list has 0 or 1 node, return directly (nothing to split)
        if (head == nullptr || head->next == nullptr)
            return head;

        ListNode* slow = head;     // slow moves 1 step
        ListNode* fast = head;     // fast moves 2 steps
        ListNode* prev = nullptr;  // node before slow (so we can cut the list later)

        // Keep moving until fast runs out
        while (fast != nullptr && fast->next != nullptr) {
            prev = slow;            // track node before slow
            slow = slow->next;      // slow moves one step
            fast = fast->next->next;// fast moves two steps
        }

        // Returning "prev" is key: this allows us to cut the list cleanly
        return prev;
    }

    // ------------------------------------------------
    // Step 2: Merge two sorted lists (classic problem)
    // ------------------------------------------------
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        // If one list is empty, the other is already sorted → return it
        if (list1 == nullptr) return list2;
        if (list2 == nullptr) return list1;

        // "merged" will point to the head of merged result
        ListNode* merged = nullptr;

        // Pick the smaller head, link it, and recursively merge the rest
        if (list1->val <= list2->val) {
            merged = list1;  
            // attach the result of merging (list1->next, list2)
            merged->next = mergeTwoLists(list1->next, list2);
        } else {
            merged = list2;
            merged->next = mergeTwoLists(list1, list2->next);
        }

        return merged; // return merged head upward
    }

    // ---------------------------------------------------
    // Step 3: Main sort function (merge sort on list)
    // ---------------------------------------------------
    ListNode* sortList(ListNode* head) {
        // Base case: 0 or 1 node → already sorted
        if (head == nullptr || head->next == nullptr)
            return head;

        // ---- Divide phase ----
        // Find middle of the list using helper
        ListNode* middlePrev = findMiddlePrev(head);
        ListNode* half2 = middlePrev->next; // second half starts here
        middlePrev->next = nullptr;         // CUT the list into two halves

        // ---- Conquer phase ----
        // Sort left and right halves recursively
        ListNode* leftSorted = sortList(head);
        ListNode* rightSorted = sortList(half2);

        // ---- Combine phase ----
        // Merge the two sorted halves back together
        return mergeTwoLists(leftSorted, rightSorted);
    }
};
