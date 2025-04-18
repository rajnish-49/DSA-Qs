// Helper function to insert an element at the bottom of the stack
void insertAtBottom(stack<int> &St, int elem)
{
    // Base case: if the stack is empty, directly insert the element
    if (St.empty())
    {
        St.push(elem); // Push elem at the bottom (or if stack was empty)
        return;
    }

    // Recursive case: store the current top element and remove it from the stack
    int topElem = St.top();
    St.pop();

    // Recursively call insertAtBottom until we reach the empty stack
    insertAtBottom(St, elem);

    // After placing elem at the bottom, push the stored elements back on top
    St.push(topElem);
}

class Solution
{
public:
    // Function to reverse the stack using recursion
    void Reverse(stack<int> &St)
    {
        // Base case: If the stack is empty, there's nothing to reverse
        if (St.empty())
        {
            return;
        }

        // Step 1: Pop the top element of the stack and hold it
        int topElem = St.top();
        St.pop();

        // Step 2: Recursively reverse the rest of the stack
        Reverse(St);

        // Step 3: Insert the held element at the bottom of the reversed stack
        insertAtBottom(St, topElem);
    }
};