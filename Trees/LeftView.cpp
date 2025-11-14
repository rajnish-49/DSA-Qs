class Solution {
private:
    // Recursive function to compute the left view of the binary tree
    void solve(Node* root, vector<int>& ans, int level) {
        // Base case: If the current node is null, return
        if (root == nullptr) 
            return;

        // Check if this is the first node of the current level
        // If yes, add it to the result (ans)
        if (level == ans.size()) {
            ans.push_back(root->data); // Add the node's value to the answer
        }
        // If level == ans.size(), it means that no node from this level has been added to the ans vector yet.

        // Recur for the left subtree first (to prioritize left view nodes)
        solve(root->left, ans, level + 1);

        // Recur for the right subtree next
        solve(root->right, ans, level + 1);
    }

public:
    // Main function to return the left view of the binary tree
    vector<int> leftView(Node* root) {
        vector<int> ans; // Vector to store the left view of the tree
        solve(root, ans, 0); // Start recursion from the root at level 0
        return ans; // Return the result
    }
};
