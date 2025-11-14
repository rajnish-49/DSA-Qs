class Solution {
private:
    // Recursive helper function to solve the problem
    void solve(stack<int>& s, int count, int size) {
        // Base case: If the current count is at the middle element's index
        // This happens when count == size / 2 (middle element)
        if (count == size / 2) {
            s.pop();  // Pop the middle element from the stack
            return;   // Exit recursion after removing the middle element
        }

        int topel = s.top();  // Get the top element of the stack
        s.pop();              // Pop the top element from the stack

        // Recursively call solve on the remaining elements in the stack
        solve(s, count + 1, size);  // Increase count to move towards the middle element

        // After recursion, push the popped element back onto the stack
        // This restores the stack's original order (except for the middle element)
        s.push(topel);
    }

public:
    // Function to delete the middle element of a stack
    void deleteMid(stack<int>& s) {
        int size = s.size();  // Get the initial size of the stack
        solve(s, 0, size);    // Start the recursion with count 0 and initial stack size
    }
};
