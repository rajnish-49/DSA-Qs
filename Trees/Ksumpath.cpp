// https://www.geeksforgeeks.org/problems/k-sum-paths/1

class Solution {
  private:
    // Helper function to count paths starting from a specific node
    void solve(Node* node, int currentsum, int k, int &count) {
        // Base case: if the current node is NULL, return
        if (node == NULL) {
            return;
        }
        
        // Add the current node's value to the running sum
        currentsum += node->data;

        // Check if the current sum equals the target sum k
        if (currentsum == k) {
            count++; // Increment the count if the condition is met
        }
        
        // Recursively check the left subtree
        solve(node->left, currentsum, k, count);
        
        // Recursively check the right subtree
        solve(node->right, currentsum, k, count);
    }
  
  public:
    // Main function to count paths with sum k in the tree
    int sumK(Node* root, int k) {
        // Base case: if the tree is empty, return 0
        if (root == NULL) {
            return 0;
        }
        
        int count = 0; // Initialize count of paths
        
        // Count paths starting from the current root node
        solve(root, 0, k, count);
        
        // Recursively count paths in the left subtree
        count += sumK(root->left, k);
        
        // Recursively count paths in the right subtree
        count += sumK(root->right, k);
        
        // Return the total count of paths
        return count;
    }
};


class Solution {
  public:
    // Helper function to recursively solve the problem
    void solve(Node* root, int k, int &count, vector<int> path) {
        // Base case: If the current node is NULL, return
        if (root == NULL)
            return;

        // Add the current node's value to the path vector
        path.push_back(root->data);

        // Recur for the left subtree
        solve(root->left, k, count, path);

        // Recur for the right subtree
        solve(root->right, k, count, path);

        // Check for paths ending at the current node that sum to k
        int size = path.size();
        int sum = 0;

        // Traverse the path vector from the current node upwards
        for (int i = size - 1; i >= 0; i--) {
            sum += path[i]; // Calculate the cumulative sum
            if (sum == k)   // If the cumulative sum equals k
                count++;    // Increment the count of valid paths
        }

        // Remove the current node from the path before returning
        path.pop_back();
    }

    // Main function to count all paths with sum k
    int sumK(Node *root, int k) {
        vector<int> path;  // Vector to store the current path
        int count = 0;     // Variable to store the count of valid paths

        // Call the helper function with the root node
        solve(root, k, count, path);

        // Return the total count of valid paths
        return count;
    }
};

/*backward checking ensures all subpaths ending at the current node are efficiently checked for the target sum 
𝑘
k. It avoids redundant calculations and ensures no paths are missed.
*/ 