/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val; // Value of the node
 *     TreeNode *left; // Pointer to the left child
 *     TreeNode *right; // Pointer to the right child
 *     
 *     // Default constructor
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     
 *     // Constructor to initialize node with a value
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     
 *     // Constructor to initialize node with value and child pointers
 *     TreeNode(int x, TreeNode *left, TreeNode *right) 
 *         : val(x), left(left), right(right) {}
 * };
 */

class Solution {
public:
    // Helper function to find the kth smallest element
    int solve(TreeNode* root, int &i, int k) {
        // Base case: If the current node is NULL, return -1
        if (root == NULL) {
            return -1;
        }

        // Traverse the left subtree (In-order traversal step 1: L)
        int left = solve(root->left, i, k);
        if (left != -1) { // If the result is already found in the left subtree, return it
            return left;
        }

        // Process the current node (In-order traversal step 2: N)
        i++; // Increment the counter to track the current node's position in sorted order
        if (i == k) { // If the current node is the kth node, return its value
            return root->val;
        }

        // Traverse the right subtree (In-order traversal step 3: R)
        return solve(root->right, i, k);
    }

    // Function to find the kth smallest element in the BST
    int kthSmallest(TreeNode* root, int k) {
        int i = 0; // Initialize a counter to track the position
        return solve(root, i, k); // Call the helper function
    }
};
