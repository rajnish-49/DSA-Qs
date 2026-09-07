/*
 * LeetCode 100. Same Tree
 * https://leetcode.com/problems/same-tree/
 *
 * Problem:
 * Given the roots of two binary trees, determine whether the two trees
 * are identical.
 *
 * Two trees are the same if:
 * 1. They have the same structure.
 * 2. Corresponding nodes have the same values.
 *
 * Approach:
 * Use recursive DFS.
 *
 * For each pair of corresponding nodes:
 * - If both are nullptr, they match.
 * - If only one is nullptr, the structures are different.
 * - If their values are different, the trees are different.
 * - Otherwise, recursively compare their left subtrees and right subtrees.
 *
 * The trees are the same only if both the left and right subtree
 * comparisons return true.
 *
 * Time: O(n)
 * Space: O(h), where h is the height of the tree due to recursion.
 */

class Solution {
public:
    bool isSameTree(TreeNode* p, TreeNode* q) {

        // Both nodes are nullptr, so this part of the trees matches.
        if (p == nullptr && q == nullptr)
            return true;

        // One node is nullptr but the other is not,
        // so the tree structures are different.
        if (p == nullptr || q == nullptr)
            return false;

        // Corresponding nodes have different values.
        if (p->val != q->val)
            return false;

        // Compare the left subtrees and right subtrees.
        return isSameTree(p->left, q->left) &&
               isSameTree(p->right, q->right);
    }
};