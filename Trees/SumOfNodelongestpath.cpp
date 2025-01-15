// question link - https://www.geeksforgeeks.org/problems/sum-of-the-longest-bloodline-of-a-tree/1?itm_source=geeksforgeeks&itm_medium=article&itm_campaign=practice_card

pair<int, int> findLongestPathSum(TreeNode* root) {
    if (!root) {
        // Base case: No path from a NULL node
        return {0, 0}; // {path_length, path_sum}
    }

    // Recursive calls for left and right subtrees
    pair<int, int> left = findLongestPathSum(root->left);
    pair<int, int> right = findLongestPathSum(root->right);

    // Compare path lengths
    if (left.first > right.first) {
        // Left path is longer
        return {left.first + 1, left.second + root->val};
    } else if (right.first > left.first) {
        // Right path is longer
        return {right.first + 1, right.second + root->val};
    } else {
        // Path lengths are equal, choose the one with the larger sum
        return {left.first + 1, max(left.second, right.second) + root->val};
    }
}

// Main function to find the sum of nodes on the longest path
int sumOnLongestPath(TreeNode* root) {
    return findLongestPathSum(root).second;
}

class Solution {
  private:
  
  // Helper function to recursively traverse the tree and calculate the maximum sum of the longest path
  void solve(Node* root, int sum, int& maxSum, int len, int& maxLen) {
    // Base case: If the current node is NULL, evaluate the current path
    if (root == NULL) {
        // If the current path length is greater than the max path length so far, update maxLen and maxSum
        if (len > maxLen) {
            maxLen = len;        // Update max path length
            maxSum = sum;        // Update max sum for the new longest path
        } 
        // If the current path length equals the max path length, check which path has a greater sum
        else if (len == maxLen) {
            maxSum = max(maxSum, sum);  // Update max sum if the current sum is greater
        }
        return; // End the current recursive call
    }

    // Recursive call to explore the left child of the current node
    // Pass updated sum and length of the path to the left subtree
    solve(root->left, sum + root->data, maxSum, len + 1, maxLen);

    // Recursive call to explore the right child of the current node
    // Pass updated sum and length of the path to the right subtree
    solve(root->right, sum + root->data, maxSum, len + 1, maxLen);
}

  public:
    // Main function to find the maximum sum of the longest root-to-leaf path
    int sumOfLongRootToLeafPath(Node *root) {
        // Initialize variables to track the length and sum of paths
        int len = 0;        // Length of the current path
        int maxLen = 0;     // Maximum length of any path found so far

        int sum = 0;        // Sum of the current path
        int maxSum = INT_MIN; // Maximum sum for the longest path, initialized to the smallest integer

        // Start the recursive traversal of the tree
        solve(root, sum, maxSum, len, maxLen);

        // Return the maximum sum for the longest root-to-leaf path
        return maxSum;
    }
};
