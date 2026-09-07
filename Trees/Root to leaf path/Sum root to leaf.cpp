/*
 * LeetCode 129. Sum Root to Leaf Numbers
 * https://leetcode.com/problems/sum-root-to-leaf-numbers/
 *
 * Problem:
 * Each root-to-leaf path represents a number.
 *
 * Example:
 *
 *         1
 *        / \
 *       2   3
 *
 * Paths:
 *     1 -> 2 = 12
 *     1 -> 3 = 13
 *
 * Answer = 12 + 13 = 25
 *
 * Approach:
 * Use DFS and build the number while traversing.
 *
 * `current` represents the number formed by the path from the root
 * to the current node.
 *
 * When we move to a child, append its digit to the number:
 *
 *     current = current * 10 + root->val
 *
 * Example:
 *
 *     current = 12
 *     next digit = 3
 *
 *     12 * 10 + 3 = 123
 *
 * When we reach a leaf, `current` is a complete root-to-leaf number,
 * so we add it to the answer.
 *
 * We do NOT need a vector to store the path because the number can
 * be built directly while doing DFS.
 *
 * Time: O(n)
 *      Every node is visited exactly once.
 *
 * Space: O(h)
 *        h = height of the tree because of the recursion stack.
 */

class Solution {
public:

    int solve(TreeNode* root, int current) {

        // There is no number to form from a null node.
        if (root == nullptr)
            return 0;

        /*
         * Append the current node's digit to the number.
         *
         * Example:
         *     current = 12
         *     root->val = 3
         *
         *     12 * 10 + 3 = 123
         */
        current = current * 10 + root->val;

        /*
         * If this is a leaf, the complete root-to-leaf number
         * has been formed.
         *
         * Return this number so that it can be added to the
         * numbers obtained from the other branches.
         */
        if (root->left == nullptr && root->right == nullptr)
            return current;

        /*
         * The current number is passed to both children.
         *
         * Each child extends the same root-to-current path
         * with its own digit.
         */
        int left = solve(root->left, current);
        int right = solve(root->right, current);

        // Add the numbers formed by the left and right paths.
        return left + right;
    }

    int sumNumbers(TreeNode* root) {

        // Start with an empty number.
        return solve(root, 0);
    }
};