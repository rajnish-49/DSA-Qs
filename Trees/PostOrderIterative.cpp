// Problem Link: https://leetcode.com/problems/binary-tree-postorder-traversal/
// Approach: Iterative Postorder Traversal

class Solution {
public:
    vector<int> postorderTraversal(TreeNode* root) {

        // Vector to store the result of the postorder traversal
        vector<int> ans;

        // Two stacks to simulate postorder traversal using iterative approach
        stack<TreeNode*> st1; // First stack to process nodes
        stack<TreeNode*> st2; // Second stack to store nodes in postorder sequence

        // Push the root node into the first stack
        st1.push(root);

        // Process nodes while the first stack is not empty
        while (!st1.empty()) {

            // Retrieve the node at the top of the first stack
            TreeNode* current = st1.top();
            st1.pop();

            // If the node is not null, process it
            if (current != NULL) {
                // Push the current node onto the second stack
                st2.push(current);

                // Push the left child of the current node onto the first stack
                st1.push(current->left);

                // Push the right child of the current node onto the first stack
                st1.push(current->right);
            }
        }

        // The second stack now contains nodes in the order of root-right-left.
        // Pop nodes from the second stack to get postorder (left-right-root) sequence
        while (!st2.empty()) {
            ans.push_back(st2.top()->val); // Add the value of the top node to the result
            st2.pop(); // Remove the top node from the stack
        }

        // Return the postorder traversal result
        return ans;
    }
};
