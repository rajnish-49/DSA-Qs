// @https://leetcode.com/problems/linked-list-cycle-ii/description/

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode *detectCycle(ListNode *head) {
        // Edge case: if list is empty or has only one node, 
        // there can never be a cycle.
        if (!head || !head->next) return nullptr;

        // Initialize two pointers: slow (moves 1 step), fast (moves 2 steps).
        // This is the core of Floyd's cycle detection algorithm.
        ListNode* slow = head;
        ListNode* fast = head;

        // -------- Phase 1: Detect if a cycle exists --------
        while (fast && fast->next) {
            slow = slow->next;          // move slow by 1
            fast = fast->next->next;    // move fast by 2

            if (slow == fast) {
                // ✅ A meeting point is found → cycle confirmed.
                // Why? Because if there's a cycle, fast will eventually
                // "lap" slow and they will meet inside the cycle.

                // -------- Phase 2: Find the cycle start --------
                // Reset one pointer back to head.
                ListNode* ptr1 = head;
                // Keep the other pointer at the meeting point.
                ListNode* ptr2 = slow;

                // Now move both pointers one step at a time.
                // Intuition: From the proof, distance(head → cycleStart) = distance(meeting → cycleStart).
                // Therefore, when they both move together, they will meet
                // exactly at the cycle start node.
                while (ptr1 != ptr2) {
                    ptr1 = ptr1->next;
                    ptr2 = ptr2->next;
                }

                // Both pointers meet at the cycle start.
                return ptr1;
            }
        }

        // If we exit the loop, it means fast reached null → no cycle exists.
        return nullptr;
    }
};


