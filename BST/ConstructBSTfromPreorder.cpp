// @ https://leetcode.com/problems/construct-binary-search-tree-from-preorder-traversal/?source=submission-ac

class Solution {
public:
    
     // Helper function to construct a Binary Search Tree (BST) from a preorder traversal.
     
    TreeNode* constructBST(vector<int>& preorder, int& index, int min, int max) {
        // Base case 1: If index is out of bounds, return null
        if (index >= preorder.size()) {
            return nullptr;
        }

        // Base case 2: If the current value is not in the valid range, return null
        if (preorder[index] < min || preorder[index] > max) {
            return nullptr;
        }

        // Create a new node with the current value in the preorder array
        TreeNode* root = new TreeNode(preorder[index]);

        // Increment the index to move to the next element in the preorder array
        index++;

        // Recursively construct the left subtree
        // The valid range for the left subtree is [min, root->val - 1]
        root->left = constructBST(preorder, index, min, root->val - 1);

        // Recursively construct the right subtree
        // The valid range for the right subtree is [root->val + 1, max]
        root->right = constructBST(preorder, index, root->val + 1, max);

        // Return the root of the subtree
        return root;
    }
    
    /**
     * Main function to construct a BST from preorder traversal.
     * @param preorder The array representing the preorder traversal of the BST.
     * @return A pointer to the root of the constructed BST.
     */
    TreeNode* bstFromPreorder(vector<int>& preorder) {
        int index = 0;  // Initialize index to 0, starting from the first element in preorder
        // Call the helper function with the entire valid range for a BST [INT_MIN, INT_MAX]
        return constructBST(preorder, index, INT_MIN, INT_MAX);
    }
};
