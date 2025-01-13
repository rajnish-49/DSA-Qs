class Solution {
public:
    /**
     * Function to determine if the binary tree has a root-to-leaf path
     * such that adding up all the values along the path equals the targetSum.
     * 
     * @param root - Pointer to the root of the binary tree
     * @param targetSum - The sum we want to check for a root-to-leaf path
     * @return true if such a path exists, otherwise false
     */
    bool hasPathSum(TreeNode* root, int targetSum) {
        // Base case: if the current node is null, there is no path
        if (root == NULL) {
            return false;
        }

        // Check if the current node is a leaf node
        if (root->left == NULL && root->right == NULL) {
            // If the leaf node's value equals the targetSum, return true
            if (root->val == targetSum) {
                return true;
            } else {
                return false; // Otherwise, return false
            }
        }

        // Calculate the remaining sum after including the current node's value
        int remaining = targetSum - root->val;

        // Recursively check the left and right subtrees for the remaining sum
        bool leftst = hasPathSum(root->left, remaining);
        bool rightst = hasPathSum(root->right, remaining);

        // If either the left or right subtree has a valid path, return true
        return leftst || rightst;
    }
};