/*
 * LeetCode 1022. Sum of Root To Leaf Binary Numbers
 * https://leetcode.com/problems/sum-of-root-to-leaf-binary-numbers/
 *
 * Problem:
 * Each node contains either 0 or 1.
 * Every root-to-leaf path forms a binary number.
 * Return the sum of all root-to-leaf binary numbers.
 *
 * Example:
 *
 *        1
 *       / \
 *      0   1
 *     / \   \
 *    0   1   0
 *
 * Paths:
 * 1 -> 0 -> 0 = 100₂ = 4
 * 1 -> 0 -> 1 = 101₂ = 5
 * 1 -> 1 -> 0 = 110₂ = 6
 *
 * Answer = 4 + 5 + 6 = 15
 *
 * Approach:
 * Traverse the tree using DFS.
 *
 * Instead of storing the complete path in a vector, keep the binary
 * number represented by the path so far in `current`.
 *
 * When we move from one node to its child, we append the child's
 * binary digit using:
 *
 *     current = current * 2 + root->val
 *
 * When we reach a leaf, `current` is the complete binary number
 * represented by that root-to-leaf path, so return it.
 *
 * The answers from the left and right subtrees are then added.
 *
 * Time: O(n)
 * Space: O(h), where h is the height of the tree.
 */

class Solution {
public:
    int solve(TreeNode* root, int current) {

        // No node means there is no root-to-leaf number here.
        if (root == nullptr)
            return 0;

        // Append the current node's binary digit to the number.
        // Example: 10 -> 101
        //          10 * 2 + 1 = 21 in decimal representation,
        //          but here the same operation builds the binary value.
        current = current * 2 + root->val;

        // If this is a leaf, the complete root-to-leaf number is formed.
        if (root->left == nullptr && root->right == nullptr)
            return current;

        // Continue building the number in both possible paths.
        return solve(root->left, current) +
               solve(root->right, current);
    }

    int sumRootToLeaf(TreeNode* root) {
        // Start with an empty binary number.
        return solve(root, 0);
    }
};