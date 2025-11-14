class Solution {
private:
    // Helper function to find the smallest value in a subtree.
    // It always goes to the leftmost node in the tree.
    TreeNode* findMin(TreeNode* root) {
        while (root->left != NULL) {
            root = root->left; // Move to the leftmost node.
        }
        return root;
    }

public:
    TreeNode* deleteNode(TreeNode* root, int key) {
        // Base case: If the tree is empty, return NULL.
        if (root == NULL) {
            return root;
        }

        // If the key to be deleted is smaller than the current node's value,
        // move to the left subtree.
        if (key < root->val) {
            root->left = deleteNode(root->left, key);
        }
        // If the key to be deleted is greater than the current node's value,
        // move to the right subtree.
        else if (key > root->val) {
            root->right = deleteNode(root->right, key);
        }
        // If the current node's value matches the key, this is the node to delete.
        else {
            // Case 1: Node has no left child.
            if (root->left == NULL) {
                TreeNode* temp = root->right; // Store the right child.
                delete(root);                // Free the memory of the current node.
                return temp;                 // Return the right child.
            }
            // Case 2: Node has no right child.
            else if (root->right == NULL) {
                TreeNode* temp = root->left; // Store the left child.
                delete(root);                // Free the memory of the current node.
                return temp;                 // Return the left child.
            }

            // Case 3: Node has two children.
            // Find the smallest node in the right subtree (inorder successor).
            TreeNode* temp = findMin(root->right);

            // Replace the current node's value with the successor's value.
            root->val = temp->val;

            // Delete the inorder successor from the right subtree.
            root->right = deleteNode(root->right, temp->val);
        }

        // Return the updated root after deletion.
        return root;
    }
};