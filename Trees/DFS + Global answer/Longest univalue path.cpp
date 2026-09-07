 /*
  * LeetCode 687 - Longest Univalue Path
  * https://leetcode.com/problems/longest-univalue-path/
  *
  * Problem:
  * Find the length of the longest path where every node has
  * the same value.
  *
  * The answer is measured in EDGES, not nodes.
  *
  * Approach:
  * For every node, find the longest same-value path extending
  * from its left and right child.
  *
  * leftPath  = same-value path that can be extended through the left child
  * rightPath = same-value path that can be extended through the right child
  *
  * A complete path through the current node can use BOTH sides:
  *
  *      leftPath → current → rightPath
  *
  * Therefore:
  *      ans = leftPath + rightPath
  *
  * But when returning to the parent, we can only continue through
  * ONE side. A path cannot branch.
  *
  * Therefore:
  *      return max(leftPath, rightPath)
  *
  * Time Complexity: O(n)
  * Space Complexity: O(h)
  *   h = height of the tree due to recursion stack.
  */

 class Solution {
 public:
     int ans = 0;

     int dfs(TreeNode* root) {
         // Empty subtree has no univalue path.
         if (root == nullptr) {
             return 0;
         }

         // Get the longest extendable path from both children.
         int left = dfs(root->left);
         int right = dfs(root->right);

         // Initially, neither side can be connected to the current node.
         int leftPath = 0;
         int rightPath = 0;

         // We can extend the left path only if the left child
         // has the same value as the current node.
         if (root->left != nullptr &&
             root->left->val == root->val) {
             leftPath = left + 1;
         }

         // Similarly, extend the right path only when the values match.
         if (root->right != nullptr &&
             root->right->val == root->val) {
             rightPath = right + 1;
         }

         /*
          * A complete path passing through the current node
          * can use BOTH sides.
          *
          * Example:
          *
          *        5
          *       / \
          *      5   5
          *
          * leftPath = 1
          * rightPath = 1
          *
          * Complete path = 1 + 1 = 2 edges.
          */
         ans = max(ans, leftPath + rightPath);

         /*
          * Return only ONE side to the parent.
          *
          * The parent can extend the path through the current node,
          * but a path cannot branch into both children.
          *
          * So return whichever side is longer.
          */
         return max(leftPath, rightPath);
     }

     int longestUnivaluePath(TreeNode* root) {
         // dfs() visits every node and updates the global answer.
         dfs(root);

         return ans;
     }
 };