 https://leetcode.com/problems/binary-tree-preorder-traversal/

class Solution {
public:
    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> ans;  // Vector to store the result of the preorder traversal
        stack<TreeNode*> st;  // Stack to simulate recursive calls for traversal
        st.push(root);  // Push the root node onto the stack to start the traversal

        // Continue processing until the stack is empty (no nodes left to visit)
        while (!st.empty()) {
            TreeNode* current = st.top();  // Get the node at the top of the stack
            st.pop();  // Pop the node from the stack as it will be processed

            // Check if the current node is valid (not null)
            if (current != NULL) {
                ans.push_back(current->val);  // Process the current node (add its value to the result)

                // Preorder traversal requires visiting the left subtree before the right.
                // Push the right child first, so it is processed **after** the left child.
                st.push(current->right);  // Push the right child onto the stack

                // Push the left child next so it is processed **before** the right child.
                st.push(current->left);  // Push the left child onto the stack
            }
        }

        // Return the final result after all nodes have been processed
        return ans;
    }
};
