/*
 * LeetCode 437. Path Sum III
 * https://leetcode.com/problems/path-sum-iii/
 *
 * Problem:
 * Count the number of paths whose node values add up to targetSum.
 *
 * A path:
 * - Can start at ANY node.
 * - Can end at ANY node.
 * - Must move downward from parent to child.
 *
 * Approach:
 * We use two recursive functions because there are two separate jobs:
 *
 * 1. solve()
 *    Assume the current node is the START of the path.
 *    It explores every possible downward path from that node
 *    and counts the ones whose sum equals targetSum.
 *
 * 2. pathSum()
 *    Makes every node in the tree a possible STARTING point.
 *    It calls solve() for the current node, then recursively
 *    tries starting from nodes in the left and right subtrees.
 *
 * In short:
 *
 *    pathSum() → decides WHERE a path starts.
 *    solve()   → explores WHERE that path can go.
 *
 * Time: O(n²) in the worst case.
 * Space: O(h), where h is the height of the tree.
 */

class Solution {
public:

    /*
     * Count valid paths that START at the current node.
     *
     * We do NOT start a new path inside this function.
     * We simply continue the current path downward.
     */
    int solve(TreeNode* root, long long targetSum) {

        // No node means there is no path to count.
        if (root == nullptr)
            return 0;

        // Include the current node in the path.
        targetSum -= root->val;

        int count = 0;

        // If the remaining sum is 0, the path from the
        // chosen starting node to the current node is valid.
        if (targetSum == 0)
            count++;

        // Continue the same path into the left subtree.
        count += solve(root->left, targetSum);

        // Continue the same path into the right subtree.
        count += solve(root->right, targetSum);

        return count;
    }

    /*
     * Try every node as a possible starting point.
     */
    int pathSum(TreeNode* root, int targetSum) {

        // No nodes means there are no paths.
        if (root == nullptr)
            return 0;

        /*
         * First, count all valid paths that START at this node.
         *
         * solve() will explore every downward path from here.
         */
        int count = solve(root, targetSum);

        /*
         * Now forget the current starting point.
         * Recursively visit the left subtree and allow one of its
         * nodes to become a completely NEW starting point.
         */
        count += pathSum(root->left, targetSum);

        /*
         * Do the same for the right subtree.
         */
        count += pathSum(root->right, targetSum);

        return count;
    }
};