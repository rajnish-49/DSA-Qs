class Solution {
public:
    // Function to calculate the maximum depth (or height) of a binary tree
    int maxDepth(TreeNode* root) {

        // Base case: If the tree is empty, return 0
        if(root == NULL) {
            return 0;
        }

        // Recursively calculate the depth of the left subtree
        int left = maxDepth(root->left);

        // Recursively calculate the depth of the right subtree
        int right = maxDepth(root->right);

        // Return the maximum of the left and right subtree depths, 
        // adding 1 to account for the current node
        return max(left, right) + 1; 
    }
};
