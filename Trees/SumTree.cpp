// question link - https://practice.geeksforgeeks.org/problems/sum-tree/1?utm_source=gfg&utm_medium=article&utm_campaign=bottom_sticky_on_article


// } Driver Code Ends
/*  Tree node
struct Node
{
    int data;
    Node* left, * right;
}; */

// Should return true if tree is Sum Tree, else false
class Solution {
public:
    pair<bool, int> isSumTreeFast(Node* root) {
        // Base case: If the node is NULL
        if (root == nullptr) {
            return make_pair(true, 0); // (isSumTree, sum)
        }

        // Base case: If the node is a leaf
        if (root->left == nullptr && root->right == nullptr) {
            return make_pair(true, root->data); // Leaf node is a Sum Tree
        }

        // Recursive calls for left and right subtrees
        pair<bool, int> leftAns = isSumTreeFast(root->left);
        pair<bool, int> rightAns = isSumTreeFast(root->right);

        // Check if left and right subtrees are Sum Trees
        bool isLeftSumTree = leftAns.first;
        bool isRightSumTree = rightAns.first;
        
        int leftSum = leftAns.second;
        int rightSum = rightAns.second;

        // Check the condition for the current node
        bool condn = (root->data == leftSum + rightSum);

        // Result for the current node
        pair<bool, int> ans;
        if (isLeftSumTree && isRightSumTree && condn) {
            ans.first = true; // Current node is a Sum Tree
            ans.second = 2 * root->data; // Total sum of the subtree
        } else {
            ans.first = false; // Not a Sum Tree
            ans.second = 0;    // Invalid sum for non-Sum Tree
        }

        return ans;
    }
  
    bool isSumTree(Node* root) {
        // Call the helper function and return the first value (isSumTree)
        return isSumTreeFast(root).first;
    }
};


//{ Driver Code Starts.

