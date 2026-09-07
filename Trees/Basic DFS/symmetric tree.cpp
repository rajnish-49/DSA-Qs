/*
 * LeetCode 101. Symmetric Tree
 * https://leetcode.com/problems/symmetric-tree/
 *
 * Problem:
 * Given the root of a binary tree, determine whether the tree is
 * symmetric around its center.
 *
 * Example:
 *
 *          1
 *        /   \
 *       2     2
 *      / \   / \
 *     3   4 4   3
 *
 * This tree is symmetric.
 *
 * Approach:
 * Use recursive DFS and compare two nodes at a time.
 *
 * For a tree to be symmetric:
 * - The left subtree and right subtree must be mirror images.
 * - The left node of one side must match the right node of the other side.
 * - The right node of one side must match the left node of the other side.
 *
 * Base cases:
 * - If both nodes are nullptr, they match.
 * - If only one is nullptr, they don't match.
 * - If their values are different, they don't match.
 *
 * Otherwise:
 * - Compare p->left with q->right.
 * - Compare p->right with q->left.
 *
 * Time: O(n)
 * Space: O(h), where h is the height of the tree due to recursion.
 */

class Solution {
public:

    bool isMirror(TreeNode* p, TreeNode* q) {

        // Both nodes are nullptr, so this part is symmetric.
        if (p == nullptr && q == nullptr)
            return true;

        // Only one node is nullptr, so the structures don't match.
        if (p == nullptr || q == nullptr)
            return false;

        // Corresponding mirror nodes have different values.
        if (p->val != q->val)
            return false;

        // Compare the outer pair and inner pair of the two subtrees.
        return isMirror(p->left, q->right) &&
               isMirror(p->right, q->left);
    }

    bool isSymmetric(TreeNode* root) {

        // An empty tree is symmetric.
        if (root == nullptr)
            return true;

        // Compare the left and right subtrees as mirror images.
        return isMirror(root->left, root->right);
    }
};