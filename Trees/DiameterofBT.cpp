class Solution {

private: 

    // Helper function to calculate the maximum depth (or height) of a binary tree
    int maxDepth(TreeNode* root) {
        // Base case: If the node is NULL, return 0 as there is no depth
        if(root == NULL) {
            return 0;
        }

        // Recursively calculate the depth of the left subtree
        int left = maxDepth(root->left);

        // Recursively calculate the depth of the right subtree
        int right = maxDepth(root->right);

        // Return the maximum of the left and right subtree depths, adding 1 for the current node
        return max(left, right) + 1; 
    }

public:
    // Function to calculate the diameter of a binary tree
    int diameterOfBinaryTree(TreeNode* root) {
        // Base case: If the tree is empty, the diameter is 0
        if(root == NULL) {
            return 0;
        }

        // Step 1: Recursively calculate the diameter of the left subtree
        int leftsubtree = diameterOfBinaryTree(root->left);

        // Step 2: Recursively calculate the diameter of the right subtree
        int rightsubtree = diameterOfBinaryTree(root->right);

        // Step 3: Calculate the diameter that passes through the root
        // This is the sum of the maximum depths of the left and right subtrees
        int throughroot = maxDepth(root->left) + maxDepth(root->right);


        // Step 4: The final diameter is the maximum of:
        // - The diameter passing through the root
        // - The diameter of the left subtree
        // - The diameter of the right subtree
        return max(throughroot, max(leftsubtree, rightsubtree));
    }
};


// To enhance complexity , remove height function call 
// first box diameter , second box height

pair<int, int> diameterFast(Node* root) {
    // Base case: if the node is NULL, return diameter = 0 and depth = 0
    if (root == NULL) {
        return make_pair(0, 0); // {diameter, depth}
    }

    // Recursive calls for left and right subtrees
    pair<int, int> left = diameterFast(root->left);
    pair<int, int> right = diameterFast(root->right);

    // Calculate the three possible diameters
    int op1 = left.first;                     // Diameter of left subtree
    int op2 = right.first;                    // Diameter of right subtree
    int op3 = left.second + right.second + 1; // Diameter passing through the root

    // Current node's diameter is the maximum of these three
    pair<int, int> ans;
    ans.first = max(op1, max(op2, op3));

    // Current node's depth is 1 + maximum depth of left or right subtree
    ans.second = max(left.second, right.second) + 1;

    return ans;
}

int diameter(Node* root) {
    return diameterFast(root).first; // Return the diameter of the tree
}
