// @ https://www.geeksforgeeks.org/problems/largest-bst/1?itm_source=geeksforgeeks&itm_medium=article&itm_campaign=practice_card

/* info : -

int maxi 
int mini 
bool bst 
size 

*/ 

// Define a class to store information about each subtree
class info {
public:
    int maxi;   // Maximum value in the subtree
    int mini;   // Minimum value in the subtree
    bool isBST; // Boolean to indicate if the subtree is a BST
    int size;   // Size of the subtree (number of nodes)
};

// Helper function to recursively determine the largest BST
info solve(Node* root, int& ans) {
    // Base case: if the node is NULL, it represents an empty subtree
    if (root == NULL) {
        return {INT_MIN, INT_MAX, true, 0};
        // INT_MIN and INT_MAX are used for comparison in parent nodes
        // An empty subtree is always considered a valid BST
        // Size of an empty subtree is 0
    }

    // Recursively solve for the left and right subtrees
    info left = solve(root->left, ans);
    info right = solve(root->right, ans);

    // Initialize the current node's info object
    info currentnode;

    // Update the size of the current subtree
    currentnode.size = left.size + right.size + 1;

    // Update the maximum and minimum values in the current subtree
    currentnode.maxi = max(root->data, right.maxi); // Max value comes from right subtree or the root
    currentnode.mini = min(root->data, left.mini); // Min value comes from left subtree or the root

    // Check if the current subtree satisfies BST conditions
    if (left.isBST && right.isBST && 
        (root->data > left.maxi && root->data < right.mini)) {
        currentnode.isBST = true; // Current subtree is a BST
    } else {
        currentnode.isBST = false; // Current subtree is not a BST
    }

    // If the current subtree is a BST, update the maximum BST size
    if (currentnode.isBST) {
        ans = max(ans, currentnode.size);
    }

    // Return the current node's information to the parent node
    return currentnode;
}

// Solution class to implement the function for finding the largest BST
class Solution {
public:
    int largestBst(Node* root) {
        int maxsize = 0; // Variable to store the size of the largest BST
        info temp = solve(root, maxsize); // Call the recursive helper function
        return maxsize; // Return the size of the largest BST found
    }
};
