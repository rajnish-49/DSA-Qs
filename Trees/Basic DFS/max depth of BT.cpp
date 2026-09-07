class Solution {
public:
    int maxDepth(TreeNode* root) {
        // Empty tree has depth 0.
        if (root == nullptr)
            return 0;

        // Find the depth of the left and right subtrees.
        int left = 1 + maxDepth(root->left);
        int right = 1 + maxDepth(root->right);

        // Return the deeper side.
        return max(left, right);
    }
};