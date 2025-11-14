// @

Node* reverseList(Node* head) {
    // Base case 1: If the list is empty (head == nullptr), just return nullptr.
    // Base case 2: If there is only one node (head->next == nullptr), 
    // then that single node is already "reversed", so return it as is.
    if (head == nullptr || head->next == nullptr)
        return head;

    // Recursively reverse the rest of the list from 'head->next' onwards.
    // Think: this call will eventually hit the last node and start unwinding the stack.
    // revHead will always point to the *new head* of the reversed list (last node in original list).
    Node* revHead = reverseList(head->next);

    // At this point, recursion has reversed the sublist [head->next ... end].
    // Example trace:
    // Suppose list = 1 -> 2 -> 3 -> 4
    // When recursion at node 3, revHead = 4 (head of reversed sublist [4]).
    // Now we need to attach "3" correctly at the end of that reversed sublist.

    // head->next is pointing to the first node of the reversed sublist (e.g., 4 when at 3).
    // head->next->next = head basically says:
    // "append the current node (head) at the end of the already reversed part".
    head->next->next = head;

    // Now, head is supposed to become the *last* node of the reversed sublist,
    // so its 'next' should point to nullptr (otherwise it would form a cycle).
    head->next = nullptr;

    // revHead (which always points to the head of the reversed list) doesn't change,
    // so we just return it back up the recursion.
    return revHead;
}
