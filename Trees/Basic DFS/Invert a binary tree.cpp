/*
 * LeetCode 226. Invert Binary Tree
 * https://leetcode.com/problems/invert-binary-tree/
 *
 * Problem:
 * Given the root of a binary tree, invert the tree and return its root.
 *
 * Inverting a tree means swapping the left and right child of every node.
 *
 * Approach:
 * Use recursive DFS.
 *
 * At each node:
 * 1. Swap its left and right children.
 * 2. Recursively invert the new left subtree.
 * 3. Recursively invert the new right subtree.
 *
 * Base case:
 * If the node is nullptr, there is nothing to invert.
 *
 * Time: O(n)
 * Space: O(h), where h is the height of the tree due to recursion.
 */

class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {

        // Nothing to invert.
        if (root == nullptr)
            return nullptr;

        // Swap the left and right subtrees.
        swap(root->left, root->right);

        // Recursively invert the left subtree.
        invertTree(root->left);

        // Recursively invert the right subtree.
        invertTree(root->right);

        // Return the root of the inverted tree.
        return root;
    }
};