problem link: https://leetcode.com/problems/binary-tree-inorder-traversal/  


class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> ans;  // Result vector to store the in-order traversal
        stack<TreeNode*> stk;  // Stack to simulate recursion for traversal
        TreeNode* current = root;  // Start traversal from the root node

        // Traverse the tree iteratively until all nodes are processed
        while (current != nullptr || !stk.empty()) {
            // Step 1: Move to the leftmost node
            while (current != nullptr) {
                // Push the current node onto the stack for later processing
                stk.push(current);
                // Move to the left child of the current node
                current = current->left;
            }

            // Step 2: Process the node at the top of the stack
            current = stk.top();  // Retrieve the top node from the stack
            stk.pop();  // Remove the node from the stack
            ans.push_back(current->val);  // Add the node's value to the result

            // Step 3: Move to the right child of the current node
            current = current->right;
        }

        return ans;  // Return the result of the in-order traversal
    }
};
