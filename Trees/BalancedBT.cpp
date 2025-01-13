class Solution {
private:
    /**
     * Helper function to calculate the maximum depth (or height) of a binary tree.
     * 
     * @param root - Pointer to the root of the binary tree
     * @return The maximum depth of the binary tree
     */
    int maxDepth(TreeNode* root) {
        // Base case: If the tree is empty, its depth is 0
        if (root == NULL) {
            return 0;
        }

        // Recursively calculate the depth of the left subtree
        int left = maxDepth(root->left);

        // Recursively calculate the depth of the right subtree
        int right = maxDepth(root->right);

        // Return the greater depth between the left and right subtrees,
        // adding 1 to account for the current node
        return max(left, right) + 1;
    }

public:
    /**
     * Function to check whether a binary tree is height-balanced.
     * 
     * A binary tree is balanced if the height difference between the left and 
     * right subtrees of any node is at most 1, and both subtrees are also balanced.
     * 
     * @param root - Pointer to the root of the binary tree
     * @return true if the tree is balanced, otherwise false
     */
    bool isBalanced(TreeNode* root) {
        // Base case: An empty tree is always balanced
        if (root == NULL) {
            return true;
        }

        // Recursively check if the left subtree is balanced
        bool left = isBalanced(root->left);

        // Recursively check if the right subtree is balanced
        bool right = isBalanced(root->right);

        // Check if the height difference between the left and right subtrees
        // of the current node is at most 1
        bool third = abs(maxDepth(root->left) - maxDepth(root->right)) <= 1;
        // after leaf nodes , it calculates max depth 

        // The current tree is balanced if:
        // 1. The left subtree is balanced
        // 2. The right subtree is balanced
        // 3. The height difference at the current node is at most 1
        if (left && right && third) {
            return true;
        } else {
            return false; // Otherwise, the tree is not balanced
        }
    }
};

// enhance complexity by using single function
pair<bool, int> isBalancedFast(Node* root) {
    // Base case: If the node is NULL, it is balanced, and its height is 0
    if (root == NULL) {
        return make_pair(true, 0); // {isBalanced, height}
    }

    // Recursive calls for left and right subtrees
    pair<bool, int> left = isBalancedFast(root->left);
    pair<bool, int> right = isBalancedFast(root->right);

    // Check if left and right subtrees are balanced
    bool leftAns = left.first;
    bool rightAns = right.first;

    // Calculate the height difference between left and right subtrees
    bool diff = abs(left.second - right.second) <= 1;

    // Current node is balanced if left, right, and diff are all true
    bool isBalanced = leftAns && rightAns && diff;

    // Height of the current node
    int height = max(left.second, right.second) + 1;

    return make_pair(isBalanced, height);
}

bool isBalanced(Node* root) {
    return isBalancedFast(root).first;
}

/*
Step 1: Call isBalanced(Node(1))
Input: The root node (Node 1).
The function isBalanced(Node(1)) calls isBalancedFast(Node(1)).
Step 2: Call isBalancedFast(Node(1))
Input: The root node (Node 1).
First, recursively check the left subtree:
Call isBalancedFast(Node(2)).
Step 3: Call isBalancedFast(Node(2))
Input: Node 2 (left child of Node 1).
First, recursively check the left subtree:
Call isBalancedFast(Node(4)).
Step 4: Call isBalancedFast(Node(4))
Input: Node 4 (left child of Node 2).
Node 4 is a leaf node.
Left child: Call isBalancedFast(NULL).
Right child: Call isBalancedFast(NULL).
Step 5: Call isBalancedFast(NULL) (Left of Node 4)
Input: NULL (left child of Node 4).
Base case:
Return (true, 0) because a NULL node is balanced, and its height is 0.
Step 6: Call isBalancedFast(NULL) (Right of Node 4)
Input: NULL (right child of Node 4).
Base case:
Return (true, 0) because a NULL node is balanced, and its height is 0.
Step 7: Combine Results for Node 4
Left Result: (true, 0) (from Step 5).
Right Result: (true, 0) (from Step 6).
Check Balance:
leftAns = true (left subtree is balanced).
rightAns = true (right subtree is balanced).
diff = abs(0 - 0) <= 1 (height difference is within limits).
isBalanced = leftAns && rightAns && diff = true.
Height:
height = max(0, 0) + 1 = 1.
Return (true, 1) for Node 4.
*/