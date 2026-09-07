/*
 * LeetCode 113. Path Sum II
 * https://leetcode.com/problems/path-sum-ii/
 *
 * Problem:
 * Given the root of a binary tree and an integer targetSum, return all
 * root-to-leaf paths whose node values add up to targetSum.
 *
 * Approach:
 * Use DFS with backtracking.
 *
 * `temp` represents the path from the root to the current node.
 * `targetSum` represents the sum that is still needed.
 *
 * At every node:
 * - Add the node to the current path.
 * - Subtract its value from the remaining target.
 * - If it is a leaf, check whether the remaining target is 0.
 * - Otherwise, continue searching both subtrees.
 * - Before returning, remove the current node from `temp`.
 *
 * The push/pop operation is backtracking:
 *
 *     push → choose the node and add it to the path
 *     recurse → explore everything below this node
 *     pop → remove the node when going back to its parent
 *
 * Time: O(n * h) in the worst case, excluding the output.
 * Space: O(h) for the recursion stack and current path.
 */

class Solution {
public:

    void solve(TreeNode* root, int targetSum,
               vector<int>& temp,
               vector<vector<int>>& ans) {

        // There is no path to explore from a null node.
        if (root == nullptr)
            return;

        // Include the current node in the path we are building.
        temp.push_back(root->val);

        // The current node contributes to the required path sum.
        targetSum -= root->val;

        // A path is valid only when it ends at a leaf.
        if (root->left == nullptr && root->right == nullptr) {

            // If nothing is left to reach, the current path is valid.
            if (targetSum == 0)
                ans.push_back(temp);
        }
        else {

            // Continue the same path into the left subtree.
            solve(root->left, targetSum, temp, ans);

            // Continue the same path into the right subtree.
            solve(root->right, targetSum, temp, ans);
        }

        // We are returning to the parent.
        // Remove this node so it does not remain in the next path.
        temp.pop_back();
    }

    vector<vector<int>> pathSum(TreeNode* root, int targetSum) {

        vector<vector<int>> ans;
        vector<int> temp;

        // Start DFS with an empty path.
        solve(root, targetSum, temp, ans);

        return ans;
    }
};