// @https://leetcode.com/problems/lowest-common-ancestor-of-deepest-leaves/description/?envType=daily-question&envId=2025-04-04

#include <unordered_map>
using namespace std;

// Definition for a binary tree node.
// Assume TreeNode is already defined like this:
// struct TreeNode {
//     int val;
//     TreeNode *left;
//     TreeNode *right;
//     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
// };

class Solution {
public:
    unordered_map<int, int> depthMap;  // Stores depth for each node value
    int maxDepth = 0;                  // Keeps track of the maximum depth in the tree

    // Recursive function to compute depth of each node
    void computeDepth(TreeNode* root, int depth) {
        if (!root) return; // Base case: if node is NULL, return

        // Update the maximum depth encountered so far
        maxDepth = max(maxDepth, depth);

        // Store current node's depth in the map
        depthMap[root->val] = depth;

        // Recur for left and right children with increased depth
        computeDepth(root->left, depth + 1);
        computeDepth(root->right, depth + 1);
    }

    // Recursive function to find the Lowest Common Ancestor (LCA) of deepest leaves
    TreeNode* findLCA(TreeNode* root) {
        if (!root) return nullptr;

        // If the current node is one of the deepest nodes, return it
        if (depthMap[root->val] == maxDepth) return root;

        // Recur for left and right subtrees
        TreeNode* left = findLCA(root->left);
        TreeNode* right = findLCA(root->right);

        // If both left and right return non-null, this node is the LCA
        if (left && right) return root;

        // Otherwise, return the non-null child
        return left ? left : right;
    }

    // Main function to find the LCA of deepest leaves
    TreeNode* lcaDeepestLeaves(TreeNode* root) {
        // First compute depth of each node
        computeDepth(root, 0);

        // Then find the LCA of the deepest nodes
        return findLCA(root);
    }
};


class Solution {
public:
    // This is the main function that will be called
    TreeNode* lcaDeepestLeaves(TreeNode* root) {
        // Call the helper function and return the LCA node part of the pair
        return dfs(root).second;
    }

    // Helper function to return a pair:
    //   - first: depth (height) of the deepest leaf in this subtree
    //   - second: the LCA of the deepest leaves in this subtree
    pair<int, TreeNode*> dfs(TreeNode* root) {
        // Base case: If the node is null, it means we are beyond a leaf
        // So depth is 0 and there's no LCA
        if (!root) return {0, nullptr};

        // Recursively go into the left and right subtrees
        pair<int, TreeNode*> left = dfs(root->left);
        pair<int, TreeNode*> right = dfs(root->right);

        // Case 1: The deepest leaves are in both subtrees at the same depth
        // This means the current node is the LCA of the deepest nodes
        if (left.first == right.first) {
            // We add 1 to depth because we're going one level up
            return {left.first + 1, root};
        }

        // Case 2: One side has deeper leaves than the other
        // The LCA must be in the deeper subtree, so we propagate that upward
        if (left.first > right.first) {
            // Left side is deeper, so return its depth + 1 and its LCA
            return {left.first + 1, left.second};
        } else {
            // Right side is deeper, so return its depth + 1 and its LCA
            return {right.first + 1, right.second};
        }
    }
};
