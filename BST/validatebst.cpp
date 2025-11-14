//@ https://leetcode.com/problems/validate-binary-search-tree/


/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {

public:
    // Helper function to validate if the tree rooted at `root` is a Binary Search Tree (BST).
    // Parameters:
    // - root: Current node to validate.
    // - min: Minimum allowed value for the current subtree.
    // - max: Maximum allowed value for the current subtree.
    bool isBST(TreeNode *root, long long min, long long max) {

        // Base case: An empty tree is a valid BST.
        if (root == NULL) {
            return true;
        }
        
        // Check if the current node's value lies within the valid range [min, max).
        if (root->val > min && root->val < max) {
            // Recursively check the left subtree, with updated max to current node's value.
            bool left = isBST(root->left, min, root->val);
            // Recursively check the right subtree, with updated min to current node's value.
            bool right = isBST(root->right, root->val, max);
            // The current tree is a valid BST only if both subtrees are valid.
            return left && right;
        } else {
            // If the current node's value is out of the allowed range, it's not a BST.
            return false;
        }
    }

    // Public function to check if a binary tree is a valid Binary Search Tree (BST).
    bool isValidBST(TreeNode* root) {
        // Use the helper function with the full range of possible values for a BST.
        return isBST(root, LONG_MIN, LONG_MAX);
    }
};
