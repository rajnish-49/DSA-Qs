/*
 * LeetCode 112. Path Sum
 * https://leetcode.com/problems/path-sum/
 *
 * Problem:
 * Given the root of a binary tree and an integer targetSum, determine
 * whether there exists a root-to-leaf path whose node values add up
 * to targetSum.
 *
 * Approach:
 * Use recursive DFS and keep track of the remaining sum.
 *
 * At each node:
 * - Subtract the current node's value from targetSum.
 * - If the node is a leaf, check whether the remaining sum is 0.
 * - Otherwise, recursively check the left and right subtrees.
 *
 * A valid path must end at a leaf.
 *
 * Time: O(n)
 * Space: O(h), where h is the height of the tree.
 */

class Solution {
public:
    bool hasPathSum(TreeNode* root, int targetSum) {

        // No path exists if the tree is empty.
        if (root == nullptr)
            return false;

        // Subtract the current node from the remaining target.
        targetSum -= root->val;

        // At a leaf, the path is valid only if the target is exactly reached.
        if (root->left == nullptr && root->right == nullptr)
            return targetSum == 0;

        // Check whether either subtree contains a valid path.
        return hasPathSum(root->left, targetSum) ||
               hasPathSum(root->right, targetSum);
    }
};