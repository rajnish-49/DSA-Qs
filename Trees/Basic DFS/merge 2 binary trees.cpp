/*
 * LeetCode 617. Merge Two Binary Trees
 * https://leetcode.com/problems/merge-two-binary-trees/
 *
 * Problem:
 * Given the roots of two binary trees, merge them into one tree.
 *
 * If two nodes overlap:
 *     merged value = node1->val + node2->val
 *
 * If only one node exists:
 *     use that existing node.
 *
 * If neither node exists:
 *     return nullptr.
 *
 * Approach:
 * Traverse both trees simultaneously using recursion.
 *
 * At every corresponding position:
 * - Both nodes exist → add their values.
 * - Only one exists → return that node.
 * - Both are nullptr → return nullptr.
 *
 * Time: O(n), where n is the total number of nodes visited.
 * Space: O(h), where h is the height of the trees due to recursion.
 */

class Solution {
public:
    TreeNode* mergeTrees(TreeNode* root1, TreeNode* root2) {

        // Both nodes are missing.
        if (root1 == nullptr && root2 == nullptr)
            return nullptr;

        // Only root2 exists.
        if (root1 == nullptr)
            return root2;

        // Only root1 exists.
        if (root2 == nullptr)
            return root1;

        // Both nodes exist, so add their values.
        root1->val += root2->val;

        // Recursively merge the left subtrees.
        root1->left = mergeTrees(root1->left, root2->left);

        // Recursively merge the right subtrees.
        root1->right = mergeTrees(root1->right, root2->right);

        return root1;
    }
};