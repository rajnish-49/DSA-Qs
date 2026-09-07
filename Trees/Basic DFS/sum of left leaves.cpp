/*
 * LeetCode 404. Sum of Left Leaves
 * https://leetcode.com/problems/sum-of-left-leaves/
 *
 * Problem:
 * Return the sum of all left leaf nodes in a binary tree.
 *
 * A leaf is a node with no left and no right child.
 *
 * Approach:
 * Use recursive DFS.
 *
 * At every node:
 * - Check if its left child exists and is a leaf.
 * - If yes, add the left child's value.
 * - Otherwise, recursively search the left subtree.
 * - Always recursively search the right subtree.
 *
 * Time: O(n)
 * Space: O(h), where h is the height of the tree.
 */

class Solution {
public:
    int sumOfLeftLeaves(TreeNode* root) {
        if (root == nullptr)
            return 0;

        int sum = 0;

        // Check if the left child is a leaf.
        if (root->left != nullptr &&
            root->left->left == nullptr &&
            root->left->right == nullptr) {

            sum += root->left->val;
        }

        // If the left child is not a leaf, search its subtree.
        else {
            sum += sumOfLeftLeaves(root->left);
        }

        // Search the right subtree for left leaves.
        sum += sumOfLeftLeaves(root->right);

        return sum;
    }
};