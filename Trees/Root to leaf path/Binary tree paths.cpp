/*
 * LeetCode 257. Binary Tree Paths
 * https://leetcode.com/problems/binary-tree-paths/
 *
 * Problem:
 * Return all root-to-leaf paths in a binary tree.
 *
 * Example:
 *
 *         1
 *        / \
 *       2   3
 *        \
 *         5
 *
 * Output:
 * ["1->2->5", "1->3"]
 *
 * Approach:
 * Use DFS with backtracking.
 *
 * `temp` stores the nodes in the current root-to-node path.
 *
 * At every node:
 * 1. Add the node to `temp`.
 * 2. If it is a leaf, convert the current path into a string
 *    and store it in `ans`.
 * 3. Otherwise, continue DFS into the left and right children.
 * 4. Remove the current node from `temp` before returning.
 *
 * The pop_back() is important because after finishing one branch,
 * we need to remove that node before exploring another branch.
 *
 * Time: O(n * h) in the worst case because each root-to-leaf path
 *       may require O(h) work to convert into a string.
 *
 * Space: O(h) for the recursion stack and current path,
 *        excluding the returned answer.
 */

class Solution {
public:

    void solve(TreeNode* root,
               vector<int>& temp,
               vector<string>& ans) {

        // No node means there is no path to explore.
        if (root == nullptr)
            return;

        // Add the current node to the path.
        temp.push_back(root->val);

        // A root-to-leaf path is complete when we reach a leaf.
        if (root->left == nullptr && root->right == nullptr) {

            string path = "";

            // Convert the vector path into the required string format.
            // Example: [1, 2, 5] -> "1->2->5"
            for (int i = 0; i < temp.size(); i++) {

                path += to_string(temp[i]);

                // Add "->" between nodes, but not after the last node.
                if (i != temp.size() - 1)
                    path += "->";
            }

            // Store this complete root-to-leaf path.
            ans.push_back(path);
        }
        else {

            // Continue the current path into the left subtree.
            solve(root->left, temp, ans);

            // Continue the current path into the right subtree.
            solve(root->right, temp, ans);
        }

        // We are returning to the parent.
        // Remove the current node so it doesn't remain in the
        // path when we explore another branch.
        temp.pop_back();
    }

    vector<string> binaryTreePaths(TreeNode* root) {

        vector<string> ans;
        vector<int> temp;

        // Start DFS from the root with an empty path.
        solve(root, temp, ans);

        return ans;
    }
};