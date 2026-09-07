/*
 * LeetCode 124 - Binary Tree Maximum Path Sum
 * https://leetcode.com/problems/binary-tree-maximum-path-sum/
 *
 * Problem:
 * Find the maximum sum of any path in a binary tree.
 *
 * A path:
 * - Can start and end at ANY nodes.
 * - Must follow connected parent-child nodes.
 * - Can go through both left and right children of a node.
 *
 * Approach:
 * At every node, calculate two things:
 *
 * 1. Current maximum path through this node:
 *      node + left_gain + right_gain
 *
 *    This path can use BOTH children because it ends at this node.
 *
 * 2. Maximum gain returned to the parent:
 *      node + max(left_gain, right_gain)
 *
 *    A path going to the parent can use only ONE child branch,
 *    because a path cannot split into two branches.
 *
 * Negative subtree contributions are ignored using max(0, gain).
 *
 * Time Complexity: O(n)
 * Space Complexity: O(h)
 *   h = height of the tree due to recursion stack.
 */

class Solution {
public:
    int ans = INT_MIN;

    int dfs(TreeNode* node) {
        // Empty subtree contributes nothing.
        if (node == nullptr) return 0;

        // Get the best contribution from the left subtree.
        // If it is negative, ignore it.
        int left_gain = max(0, dfs(node->left));

        // Get the best contribution from the right subtree.
        // If it is negative, ignore it.
        int right_gain = max(0, dfs(node->right));

        /*
         * A complete path passing through this node can take
         * both the left and right gains:
         *
         * left -> node -> right
         *
         * Update the global answer with this candidate.
         */
        ans = max(ans, node->val + left_gain + right_gain);

        /*
         * Return the best single branch to the parent.
         *
         * We can return only ONE side because the path cannot
         * branch into both children when continuing upward.
         */
        return node->val + max(left_gain, right_gain);
    }

    int maxPathSum(TreeNode* root) {
        // dfs() visits every node and updates ans.
        dfs(root);

        return ans;
    }
};