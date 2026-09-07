 /*
  * LeetCode 110 - Balanced Binary Tree
  * https://leetcode.com/problems/balanced-binary-tree/
  *
  * Problem:
  * Determine whether a binary tree is height-balanced.
  *
  * A binary tree is balanced if, for every node, the difference
  * between the heights of its left and right subtrees is at most 1.
  *
  * Approach:
  * Use DFS to calculate the height of each subtree.
  *
  * If any subtree is already unbalanced, return -1.
  * Otherwise, return its height.
  *
  * Returning -1 allows us to detect an unbalanced subtree immediately
  * instead of calculating heights repeatedly.
  *
  * Time Complexity: O(n)
  * Space Complexity: O(h)
  */

 class Solution {
 public:
     int dfs(TreeNode* root) {
         // Empty tree has height 0.
         if (root == nullptr) {
             return 0;
         }

         // Calculate height of the left subtree.
         int left = dfs(root->left);

         // If left subtree is unbalanced, propagate -1 upward.
         if (left == -1) {
             return -1;
         }

         // Calculate height of the right subtree.
         int right = dfs(root->right);

         // If right subtree is unbalanced, propagate -1 upward.
         if (right == -1) {
             return -1;
         }

         // Current node is unbalanced if height difference > 1.
         if (abs(left - right) > 1) {
             return -1;
         }

         // Return height of the current subtree.
         return 1 + max(left, right);
     }

     bool isBalanced(TreeNode* root) {
         // -1 means some subtree is unbalanced.
         return dfs(root) != -1;
     }
 };