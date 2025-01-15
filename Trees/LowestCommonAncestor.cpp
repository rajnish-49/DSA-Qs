class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        // Base case: if the root is NULL, there is no tree to search, so return NULL.
        if (root == NULL) {
            return NULL;
        }

        // If the current node is equal to either p or q, we return the current node
        // because it could be the lowest common ancestor or part of the path.
        if (root == p || root == q) {
            return root;
        }

        // Recursively search for p and q in the left subtree.
        TreeNode* left = lowestCommonAncestor(root->left, p, q);

        // Recursively search for p and q in the right subtree.
        TreeNode* right = lowestCommonAncestor(root->right, p, q);

        // If both left and right are non-NULL, it means p and q are found in different
        // subtrees of the current node. Hence, the current node is their lowest common ancestor.
        if (left && right) {
            return root;
        }

        // If only one of left or right is non-NULL, return the non-NULL one.
        // This means both p and q are located in the same subtree.
        else if (left != NULL && right == NULL) {
            return left;
        }
        else if (left == NULL && right != NULL) {
            return right;
        }

        // If both left and right are NULL, return NULL as neither p nor q was found
        // in the current subtree.
        else {
            return NULL;
        }
    }
};
