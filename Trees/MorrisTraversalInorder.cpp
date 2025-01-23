class Solution {
public:
    vector<int> inOrder(Node* root) {
        vector<int> result;  // Vector to store the in-order traversal
        Node* current = root;

        while (current != NULL) {
            // If there is no left child, visit the node
            if (current->left == NULL) {
                result.push_back(current->data);  // Visit the current node

                // Move to the right child
                current = current->right;
            } else {
                // Find the inorder predecessor of current
                Node* pre = current->left;
                while (pre->right != NULL && pre->right != current) {
                    pre = pre->right;
                }

                // If the right child of the predecessor is NULL
                if (pre->right == NULL) {
                    // Create a temporary link from predecessor to current
                    pre->right = current;

                    // Move current to its left child
                    current = current->left;
                } else {
                    // Restore the tree by setting pre->right to NULL
                    pre->right = NULL;

                    // Visit the current node
                    result.push_back(current->data);  // Visit the current node

                    // Move to the right child
                    current = current->right;
                }
            }
        }

        return result;  // Return the in-order traversal result
    }
};