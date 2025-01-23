// @ https://leetcode.com/problems/balance-a-binary-search-tree/

class Solution {
public:
    // Helper function to perform inorder traversal and store the result in a vector
    void inorder(TreeNode* root, vector<int>& ans) {
        if (root == nullptr) {
            return;  // If the node is null, simply return
        }
        
        // Traverse the left subtree
        inorder(root->left, ans);
        
        // Add the current node's value to the result
        ans.push_back(root->val);
        
        // Traverse the right subtree
        inorder(root->right, ans);
    }

    // Helper function to construct a balanced BST from the sorted array (inorder traversal)
    TreeNode* constructBST(vector<int>& ans, int start, int end) {
        if (start > end) {
            return nullptr;  // If the start index is greater than the end, return null (base case)
        }

        // Find the middle index to keep the tree balanced
        int mid = (start + end) / 2;

        // Create a new node with the middle value
        TreeNode* newroot = new TreeNode(ans[mid]);

        // Recursively construct the left and right subtrees
        newroot->left = constructBST(ans, start, mid - 1);  // Left subtree will be the elements before the middle
        newroot->right = constructBST(ans, mid + 1, end);   // Right subtree will be the elements after the middle

        // Return the newly created node (which becomes the root of this subtree)
        return newroot;
    }

    // Main function to balance the given BST
    TreeNode* balanceBST(TreeNode* root) {
        vector<int> ans;
        
        // Step 1: Perform an inorder traversal of the original tree and store the values in a vector
        inorder(root, ans);

        // Step 2: Use the sorted array of node values to construct a balanced BST
        int j = ans.size() - 1;  // The end index of the vector

        // Step 3: Construct the balanced BST from the sorted array and return the root
        TreeNode* final = constructBST(ans, 0, j);
        return final;
    }
};


// brute force 
class Solution {
public:
    // Step 1: Count the total number of nodes in the tree (needed for recursive construction)
    int countNodes(TreeNode* root) {
        if (root == nullptr) {
            return 0;  // If the node is null, return 0 (no nodes in this subtree)
        }

        // Recursively count the nodes in the left and right subtrees and add 1 for the current node
        return 1 + countNodes(root->left) + countNodes(root->right);
    }

    // Step 2: Recursively build the balanced BST from in-order traversal
    TreeNode* sortedArrayToBST(int start, int end, TreeNode*& root) {
        if (start > end) {
            return nullptr;  // Base case: If the start index exceeds the end, return null (no more nodes to process)
        }

        // Step 3: Recursively build the left subtree (process the left half of the array)
        TreeNode* left = sortedArrayToBST(start, (start + end) / 2 - 1, root);  // Process elements before the middle element

        // Step 4: Assign the root of the tree (the middle element in the current range)
        TreeNode* currentNode = new TreeNode(root->val);  // Create a new node with the current root value
        currentNode->left = left;  // Link the left child to the left subtree we just built

        // Move the original root pointer to the next node in in-order
        root = root->right;  // Move root to the next node in the in-order sequence

        // Step 5: Recursively build the right subtree (process the right half of the array)
        currentNode->right = sortedArrayToBST((start + end) / 2 + 1, end, root);  // Process elements after the middle element

        // Return the root of the subtree being built
        return currentNode;
    }

    // Step 6: Main function to balance the BST
    TreeNode* balanceBST(TreeNode* root) {
        // Step 1: Count the total number of nodes in the tree
        int n = countNodes(root);

        // Step 2: Recurse to build the balanced BST
        return sortedArrayToBST(0, n - 1, root);  // Start from the entire range of the tree
    }
};
