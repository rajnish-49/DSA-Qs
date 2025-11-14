/*
class Node {
 public:
    int data;
    Node *next;

    Node(int x) {
        data = x;
        next = NULL;
    }
};
*/

class Solution {
  public:
    int lengthOfLoop(Node *head) {
        // Edge case: empty list or single node → no loop possible
        if (!head || !head->next) return 0;

        Node* slow = head;  // moves one step at a time
        Node* fast = head;  // moves two steps at a time

        // Step 1: Detect cycle using Floyd’s cycle-finding algorithm
        while (fast && fast->next) {
            slow = slow->next;           // move 1 step
            fast = fast->next->next;     // move 2 steps

            // Cycle detected when slow and fast meet
            if (slow == fast) {
                // Step 2: Count the length of the cycle
                int length = 1;
                Node* temp = slow->next;

                // Keep moving until we return to the same node
                while (temp != slow) {
                    length++;
                    temp = temp->next;
                }

                return length;  // return cycle length
            }
        }

        // No cycle found
        return 0;
    }
};
