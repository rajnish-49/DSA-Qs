/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;                 // Value of the node
 *     TreeNode *left;          // Pointer to the left child
 *     TreeNode *right;         // Pointer to the right child
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}  // Constructor to initialize node
 * };
 */

class Solution {
public:
    /**
     * Function to find the Lowest Common Ancestor (LCA) of two nodes in a Binary Search Tree (BST).
     *
     * @param root: Pointer to the root of the BST.
     * @param p: Pointer to the first node.
     * @param q: Pointer to the second node.
     * @return: Pointer to the LCA node.
     */
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        // Base case: If the root is NULL, or if root matches either p or q,
        // it means we have found the LCA, or we've reached the end of the tree.
        if (!root || root == p || root == q) return root;

        // If both p and q are greater than the current root's value,
        // the LCA must lie in the right subtree because all greater values
        // in a BST are located to the right.
        if (root->val < p->val && root->val < q->val) {
            return lowestCommonAncestor(root->right, p, q);
        }

        // If both p and q are smaller than the current root's value,
        // the LCA must lie in the left subtree because all smaller values
        // in a BST are located to the left.
        if (root->val > p->val && root->val > q->val) {
            return lowestCommonAncestor(root->left, p, q);
        }

        // If the above conditions are false, it means one of p or q lies in
        // the left subtree and the other lies in the right subtree. Therefore,
        // the current root is the LCA.
        return root;
    }
};
