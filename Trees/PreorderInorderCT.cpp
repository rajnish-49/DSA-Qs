// @ https://leetcode.com/problems/construct-binary-tree-from-preorder-and-inorder-traversal/


class Solution {
public:
  // Function to find the index of the root value in the inorder array
int findInorderIndex(vector<int>& inorder, int inorderStart, int inorderEnd, int rootValue) {
    // Iterate over the inorder array within the specified range to find the index of the root
    for (int i = inorderStart; i <= inorderEnd; ++i) {
        if (inorder[i] == rootValue) {
            return i;  // Return the index of the root value in inorder
        }
    }
    return -1;  // This should never happen if the inputs are valid (the root should always be found)
}

// Helper function to recursively build the binary tree
TreeNode* buildTreeHelper(vector<int>& preorder, vector<int>& inorder, int& preorderIndex, int inorderStart, int inorderEnd) {
    // Base case: if the inorder range is invalid (empty range), return nullptr (no node)
    if (inorderStart > inorderEnd) return nullptr;

    // Step 1: Get the root value from preorder (current value at preorderIndex)
    int rootValue = preorder[preorderIndex++];  // Increment preorderIndex after using it
    TreeNode* root = new TreeNode(rootValue);  // Create the current root node

    // Step 2: Find the index of the root value in the inorder array to separate the left and right subtrees
    int inorderIndex = findInorderIndex(inorder, inorderStart, inorderEnd, rootValue);

    // Step 3: Recursively build the left and right subtrees
    // Left subtree: elements before the root in inorder (range: inorderStart to inorderIndex-1)
    root->left = buildTreeHelper(preorder, inorder, preorderIndex, inorderStart, inorderIndex - 1);

    // Right subtree: elements after the root in inorder (range: inorderIndex+1 to inorderEnd)
    root->right = buildTreeHelper(preorder, inorder, preorderIndex, inorderIndex + 1, inorderEnd);

    // Return the root of the current subtree
    return root;
}

// Main function to build the binary tree from preorder and inorder arrays
TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
    int preorderIndex = 0;  // Start with the first element of preorder
    // Call the helper function to build the tree, passing the initial ranges for inorder
    return buildTreeHelper(preorder, inorder, preorderIndex, 0, inorder.size() - 1);
}
};