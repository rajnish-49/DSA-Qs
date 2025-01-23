// @ https://www.geeksforgeeks.org/problems/merge-two-bst-s/1?itm_source=geeksforgeeks&itm_medium=article&itm_campaign=practice_card

class Solution {
  public:
    // Helper function to perform inorder traversal
    void inorderTraversal(Node* root, vector<int>& result) {
        // If the current node is null, return (base case)
        if (root == NULL) {
            return;
        }
        
        // Traverse the left subtree
        inorderTraversal(root->left, result);

        // Visit the current node (add its value to the result vector)
        result.push_back(root->data);

        // Traverse the right subtree
        inorderTraversal(root->right, result);
    }
    
    // Function to merge two sorted arrays into a single sorted array
    vector<int> mergeSortedArrays(const vector<int>& arr1, const vector<int>& arr2) {
        vector<int> merged; // Vector to store the merged result
        
        int i = 0, j = 0; // Initialize two pointers for arr1 and arr2
        
        // Traverse both arrays until one of them is fully processed
        while (i < arr1.size() && j < arr2.size()) {
            // Compare current elements of arr1 and arr2
            if (arr1[i] < arr2[j]) {
                merged.push_back(arr1[i]); // Add the smaller element to the merged vector
                i++; // Move the pointer in arr1
            } else {
                merged.push_back(arr2[j]); // Add the smaller element to the merged vector
                j++; // Move the pointer in arr2
            }
        }
        
        // Add remaining elements of arr1 (if any)
        while (i < arr1.size()) {
            merged.push_back(arr1[i]);
            i++;
        }
        
        // Add remaining elements of arr2 (if any)
        while (j < arr2.size()) {
            merged.push_back(arr2[j]);
            j++;
        }
        
        return merged; // Return the merged sorted array
    }

    // Main function to merge two BSTs into a sorted list of node values
    vector<int> merge(Node *root1, Node *root2) {
        vector<int> inorder1, inorder2; // Vectors to store the inorder traversals

        // Get the inorder traversal of the first BST
        inorderTraversal(root1, inorder1);

        // Get the inorder traversal of the second BST
        inorderTraversal(root2, inorder2);

        // Merge the two sorted arrays obtained from inorder traversals
        return mergeSortedArrays(inorder1, inorder2);
    }
};
