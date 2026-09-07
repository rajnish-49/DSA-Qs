/*
 * LeetCode 337 - House Robber III
 * https://leetcode.com/problems/house-robber-iii/
 *
 * Problem:
 * Each node of the binary tree contains some money.
 * If we rob a node, we cannot rob its direct children.
 *
 * Find the maximum amount of money that can be robbed.
 *
 * Approach: Tree DP + DFS
 *
 * For every node, maintain TWO DP states:
 *
 *   dp[0] = maximum money if we SKIP this node
 *   dp[1] = maximum money if we ROB this node
 *
 * We calculate these states using the results from the children.
 *
 * ---------------------------------------------------------
 *
 * Case 1: ROB the current node
 *
 * If we rob the current node, we CANNOT rob either child.
 *
 *     robCurrent = root->val
 *                  + skipLeft
 *                  + skipRight
 *
 * ---------------------------------------------------------
 *
 * Case 2: SKIP the current node
 *
 * If we skip the current node, each child can independently
 * be either robbed or skipped.
 *
 *     skipCurrent = bestLeft + bestRight
 *
 * where:
 *
 *     bestLeft  = max(skipLeft, robLeft)
 *     bestRight = max(skipRight, robRight)
 *
 * ---------------------------------------------------------
 *
 * Finally, return both states to the parent.
 *
 * At the root, there is no parent, so we simply choose the
 * better of robbing or skipping the root.
 *
 * Time Complexity: O(n)
 *   Every node is visited exactly once.
 *
 * Space Complexity: O(h)
 *   h = height of the tree due to recursion.
 */

class Solution {
public:

    /*
     * Returns two DP states for the current subtree:
     *
     * result[0] = maximum money when current node is SKIPPED
     * result[1] = maximum money when current node is ROBBED
     */
    vector<int> dfs(TreeNode* root) {

        // Empty subtree contributes 0 money in either case.
        if (root == nullptr) {
            return {0, 0};
        }

        // First solve both child subtrees.
        // Their DP states will be used to calculate the
        // two states for the current node.
        vector<int> left = dfs(root->left);
        vector<int> right = dfs(root->right);

        /*
         * ROB CURRENT NODE:
         *
         * If we rob the current node, we must skip both children.
         *
         * left[0]  = best money when left child is skipped
         * right[0] = best money when right child is skipped
         */
        int robCurrent =
            root->val + left[0] + right[0];

        /*
         * SKIP CURRENT NODE:
         *
         * Since we are not robbing the current node, each child
         * is free to be either robbed or skipped.
         *
         * Therefore, take the better option from each child.
         */
        int skipCurrent =
            max(left[0], left[1]) +
            max(right[0], right[1]);

        /*
         * Send both possibilities to the parent.
         *
         * [0] -> skip current
         * [1] -> rob current
         */
        return {skipCurrent, robCurrent};
    }

    int rob(TreeNode* root) {

        // Get both possibilities for the root.
        vector<int> result = dfs(root);

        /*
         * The root has no parent restricting our choice.
         * Therefore, simply take whichever is better:
         *
         * result[0] -> skip root
         * result[1] -> rob root
         */
        return max(result[0], result[1]);
    }
};