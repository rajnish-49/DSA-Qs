
class Solution {
public:
    bool hasCycle(ListNode *head) {

        // Two pointers: one moves slow (1 step), the other moves fast (2 steps).
        // Intuition: If there's a cycle, the fast pointer will eventually "lap" the slow pointer.
        // If there's no cycle, fast will reach the end (nullptr).

        ListNode *fast = head;
        ListNode *slow = head; 

        // Traverse while there are nodes to move forward
        // (fast must not be null, and fast->next must also not be null 
        // since fast moves 2 steps at a time).
        while (fast != NULL && fast->next != NULL) {
            
            // Move slow by 1 step
            slow = slow->next;

            // Move fast by 2 steps
            fast = fast->next->next;

            // If slow and fast meet, that means fast has looped around
            // and caught up to slow inside a cycle → cycle detected.
            if (fast == slow) {
                return true;
            }
        }

        // If we exit the loop, it means fast reached the end of the list,
        // so there's no cycle.
        return false;
    }
};
