// Helper class to store information about a subtree
class info {
public:
    int maxi;   // Maximum value in the subtree
    int mini;   // Minimum value in the subtree
    bool isBST; // Boolean flag to indicate whether the subtree is a BST
    int sum;    // Sum of all node values in the subtree
};

// Helper function to solve the problem recursively
info solve(TreeNode* root, int& ans) {
    // Base case: If the current node is NULL (empty subtree)
    if (root == NULL) {
        return {INT_MIN, INT_MAX, true, 0}; 
        // INT_MIN: Represents no maximum value in an empty subtree
        // INT_MAX: Represents no minimum value in an empty subtree
        // An empty subtree is always a valid BST
        // Sum of an empty subtree is 0
    }

    // Recursively calculate information for the left and right subtrees
    info left = solve(root->left, ans);   // Information for the left subtree
    info right = solve(root->right, ans); // Information for the right subtree

    // Initialize the current node's information
    info currentnode;

    // Calculate the sum of the current subtree
    // Sum is the sum of left subtree + right subtree + current node's value
    currentnode.sum = left.sum + right.sum + root->val;

    // Update the maximum value in the current subtree
    // It's the larger of the current node's value and the maximum value in the right subtree
    currentnode.maxi = max(root->val, right.maxi);

    // Update the minimum value in the current subtree
    // It's the smaller of the current node's value and the minimum value in the left subtree
    currentnode.mini = min(root->val, left.mini);

    // Check if the current subtree is a BST
    // Conditions for a BST:
    // 1. Left subtree is a BST
    // 2. Right subtree is a BST
    // 3. The current node's value is greater than the maximum value in the left subtree
    // 4. The current node's value is less than the minimum value in the right subtree
    if (left.isBST && right.isBST && 
        (root->val > left.maxi && root->val < right.mini)) {
        currentnode.isBST = true; // The current subtree is a BST
    } else {
        currentnode.isBST = false; // The current subtree is not a BST
    }

    // If the current subtree is a BST, update the maximum sum
    if (currentnode.isBST) {
        ans = max(ans, currentnode.sum); 
        // Update the global maximum sum with the larger of:
        // 1. Current global maximum sum
        // 2. Sum of the current BST
    }

    // Return the information about the current subtree to its parent
    return currentnode;
}

// Main class to solve the problem
class Solution {
public:
    int maxSumBST(TreeNode* root) {
        int maxsum = 0; // Variable to store the maximum sum of any BST found in the tree
        info temp = solve(root, maxsum); // Call the helper function starting from the root
        return maxsum; // Return the maximum sum of any BST
    }
};