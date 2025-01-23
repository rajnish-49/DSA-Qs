/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    // Helper function to perform in-order traversal of the BST
    void inorder(TreeNode* root, vector<int>& ans) {
        // Base case: If the current node is null, return
        if (root == NULL) {
            return;
        }
        
        // Recursive call to the left subtree
        inorder(root->left, ans);
        
        // Add the current node's value to the result vector
        ans.push_back(root->val);
        
        // Recursive call to the right subtree
        inorder(root->right, ans);
    }

    // Function to determine if there exist two numbers in the BST that add up to k
    bool findTarget(TreeNode* root, int k) {
        vector<int> ans; // Vector to store the sorted values of the BST
        
        // Perform in-order traversal to populate the vector with sorted values
        inorder(root, ans);

        // Two-pointer approach to find two numbers that sum up to k
        int i = 0; // Pointer to the start of the vector
        int j = ans.size() - 1; // Pointer to the end of the vector

        // While the two pointers do not cross
        while (i < j) {
            int sum = ans[i] + ans[j]; // Calculate the sum of the two values
            
            // If the sum matches the target, return true
            if (sum == k) {
                return true;
            }
            // If the sum is less than the target, move the left pointer forward
            else if (sum < k) {
                i++;
            }
            // If the sum is greater than the target, move the right pointer backward
            else if (sum > k) {
                j--;
            }
        }

        // If no such pair is found, return false
        return false;
    }
};
