class Solution {
public:
    int minDepth(TreeNode* root) {
        // Empty tree has depth 0.
        if (root == nullptr)
            return 0;

        // If left subtree doesn't exist, we must go through the right.
        if (root->left == nullptr)
            return 1 + minDepth(root->right);

        // If right subtree doesn't exist, we must go through the left.
        if (root->right == nullptr)
            return 1 + minDepth(root->left);

        // Both subtrees exist, so take the smaller depth.
        int left = minDepth(root->left);
        int right = minDepth(root->right);

        return 1 + min(left, right);
    }
};