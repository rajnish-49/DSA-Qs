/*
 * LOWEST COMMON ANCESTOR (Binary Tree)
 *
 * Problem:
 *   Find the lowest node in the binary tree that has both
 *   p and q in its subtree.
 *
 * Approach:
 *   1. If root is NULL, there is nothing to search.
 *   2. If root is p or q, return root.
 *   3. Recursively search for p and q in the left and right subtrees.
 *   4. If both subtrees return a node, p and q lie on different sides,
 *      so the current root is their Lowest Common Ancestor.
 *   5. If only one subtree returns a node, both p and q are present
 *      in that subtree, so return that result upward.
 *
 * Time Complexity: O(n)
 * Space Complexity: O(h)  // recursion stack
 */

class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root,
                                   TreeNode* p,
                                   TreeNode* q) {

        // Empty subtree: neither p nor q can be found here
        if (root == NULL)
            return NULL;

        // If we reach p or q, return it to the caller.
        // This node may itself be the LCA.
        if (root == p || root == q)
            return root;

        // Look for p or q in the left and right subtrees
        TreeNode* left = lowestCommonAncestor(root->left, p, q);
        TreeNode* right = lowestCommonAncestor(root->right, p, q);

        // One node was found on each side.
        // Therefore, current root is the first common ancestor.
        if (left != NULL && right != NULL)
            return root;

        // If only one side has a result, propagate it upward.
        // If both are NULL, this also returns NULL.
        return left != NULL ? left : right;
    }
};