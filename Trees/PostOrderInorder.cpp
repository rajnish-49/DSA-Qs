/*
Construction Steps for Postorder + Inorder
Pick the root from the current index of the postorder array (starting from the end).
Find the root's index in the inorder array to divide the tree into left and right subtrees.
Right subtree is built first because in postorder traversal, the right subtree is processed just before the root.
Left subtree is built next.
Construction Steps for Preorder + Inorder
Pick the root from the current index of the preorder array (starting from the beginning).
Find the root's index in the inorder array to divide the tree into left and right subtrees.
Left subtree is built first because in preorder traversal, the left subtree is processed immediately after the root.
Right subtree is built next.*/

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;                // Value of the node
 *     TreeNode *left;         // Pointer to the left child
 *     TreeNode *right;        // Pointer to the right child
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {} // Default constructor
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {} // Constructor with value
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {} // Constructor with value and children
 * };
 */

class Solution {
public:
    // Helper function to find the index of a given value (rootValue) in the inorder array
    int findInorderIndex(vector<int>& inorder, int inorderStart, int inorderEnd, int rootValue) {
        // Iterate over the inorder array within the given range to find the root value
        for (int i = inorderStart; i <= inorderEnd; ++i) {
            if (inorder[i] == rootValue) {
                return i;  // Return the index where the root value is found
            }
        }
        return -1;  // This should not happen with valid inputs
    }

    // Recursive function to construct the binary tree
    TreeNode* buildTreeHelper(vector<int>& postorder, vector<int>& inorder, int &postorderIndex, int inorderStart, int inorderEnd) {
        // Base case: If the range in inorder is invalid, return nullptr (no node)
        if (inorderStart > inorderEnd) return nullptr;

        // Step 1: Get the current root value from the postorder array
        int rootValue = postorder[postorderIndex--]; // Decrement postorderIndex after using it
        TreeNode* root = new TreeNode(rootValue); // Create a new node with the root value

        // Step 2: Find the index of the root value in the inorder array
        int inorderIndex = findInorderIndex(inorder, inorderStart, inorderEnd, rootValue);

        // Step 3: Recursively build the right subtree first, as postorder processes right -> left -> root
        root->right = buildTreeHelper(postorder, inorder, postorderIndex, inorderIndex + 1, inorderEnd);

        // Step 4: Recursively build the left subtree
        root->left = buildTreeHelper(postorder, inorder, postorderIndex, inorderStart, inorderIndex - 1);

        // Return the constructed root node
        return root;
    }

    // Main function to construct the binary tree from inorder and postorder arrays
    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        int n = postorder.size();         // Size of the arrays
        int postorderIndex = n - 1;       // Start with the last element of postorder as the root
        // Call the helper function to construct the tree
        return buildTreeHelper(postorder, inorder, postorderIndex, 0, inorder.size() - 1);
    }
};
