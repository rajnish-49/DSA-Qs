// @ https://leetcode.com/problems/merge-two-sorted-lists/description/

class Solution {
public:
    /**
     * Merges two sorted linked lists into a single sorted linked list.
     * This function implements a recursive approach.
     *
     * @param list1 Pointer to the head of the first sorted linked list.
     * @param list2 Pointer to the head of the second sorted linked list.
     * @return Pointer to the head of the merged sorted linked list.
     */
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        
        // Base case: If list1 is empty, return list2 (remaining part of the second list)
        if (list1 == nullptr) {
            return list2;
        }

        // Base case: If list2 is empty, return list1 (remaining part of the first list)
        if (list2 == nullptr) {
            return list1;
        }

        // Compare the current nodes of both lists and merge them accordingly
        if (list1->val <= list2->val) {
            // If list1's value is smaller or equal, link it to the result of merging the rest
            list1->next = mergeTwoLists(list1->next, list2);
            return list1; // Return the current node of list1 as it remains in the merged list
        } else {
            // If list2's value is smaller, link it to the result of merging the rest
            list2->next = mergeTwoLists(list1, list2->next);
            return list2; // Return the current node of list2 as it remains in the merged list
        }
    }
};
