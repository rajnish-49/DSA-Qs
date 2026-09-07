/*
 * LeetCode 572. Subtree of Another Tree
 * https://leetcode.com/problems/subtree-of-another-tree/
 *
 * Problem:
 * Given the roots of two binary trees root and subRoot, return true if
 * subRoot is a subtree of root.
 *
 * A subtree must contain a node and all of its descendants.
 *
 * Approach:
 * Use two recursive functions.
 *
 * 1. isSameTree():
 *    Checks whether the two trees are exactly identical.
 *
 * 2. isSubtree():
 *    For every node in root, consider that node as a possible starting
 *    point of subRoot.
 *
 *    At each node:
 *    - First check if the tree starting here is the same as subRoot.
 *    - If not, search in the left subtree.
 *    - If not, search in the right subtree.
 *
 * The important idea:
 *    isSubtree() searches for the starting node.
 *    isSameTree() verifies the entire structure from that node.
 *
 * Time: O(n * m) in the worst case,
 *       where n = number of nodes in root
 *       and m = number of nodes in subRoot.
 *
 * Space: O(h), where h is the height of root due to recursion.
 */

class Solution {
public:

    // Checks whether two trees are exactly the same.
    bool isSameTree(TreeNode* p, TreeNode* q) {

        // Both trees have ended at the same position.
        if (p == nullptr && q == nullptr)
            return true;

        // One tree ended but the other did not.
        if (p == nullptr || q == nullptr)
            return false;

        // Corresponding nodes have different values.
        if (p->val != q->val)
            return false;

        // Both current nodes match, so compare their subtrees.
        return isSameTree(p->left, q->left) &&
               isSameTree(p->right, q->right);
    }

    // Searches for subRoot inside root.
    bool isSubtree(TreeNode* root, TreeNode* subRoot) {

        // We found a matching tree.
        if (isSameTree(root, subRoot))
            return true;

        // If root is nullptr, there is nowhere left to search.
        if (root == nullptr)
            return false;

        // Search in the left or right subtree.
        return isSubtree(root->left, subRoot) ||
               isSubtree(root->right, subRoot);
    }
};