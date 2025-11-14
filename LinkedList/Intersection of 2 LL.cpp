class Solution {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {

        // Edge case: if either list is empty, they can't intersect
        if (!headA || !headB) return nullptr;

        // Two pointers to traverse both lists
        ListNode *a = headA;
        ListNode *b = headB;

        // Keep moving until they either meet at intersection or both reach null
        while (a != b) {

            // If pointer 'a' reaches the end of list A,
            // redirect it to the head of list B.
            // Why: This way, both pointers will traverse equal total length 
            // (lengthA + lengthB). If intersection exists, they'll meet there.
            if (a == nullptr) {
                a = headB;
            } else {
                a = a->next;
            }

            // Do the same for pointer 'b':
            // If 'b' finishes list B, restart it at head of list A.
            if (b == nullptr) {
                b = headA;
            } else {
                b = b->next;
            }
        }

        // When they meet, either at intersection node or both being nullptr,
        // return that node (nullptr means no intersection).
        return a;
    }
};
