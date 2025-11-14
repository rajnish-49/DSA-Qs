// @ https://www.geeksforgeeks.org/problems/is-binary-tree-heap/1

class Solution {
public:
    // Function to check if the tree satisfies the max heap property.
    // A max heap requires every parent node's value to be greater than
    // its children's values.
    bool maxheap(Node* root) {
        // Base Case 1: If the node is a leaf (both children are NULL),
        // it trivially satisfies the max heap property.
        if (root->left == NULL && root->right == NULL) {
            return true;
        }
        
        // Base Case 2: If the node has only a left child (i.e., right is NULL),
        // then just check if the current node's data is greater than the left child's data.
        if (root->right == NULL) {
            return (root->data > root->left->data);
        }
        
        // Recursive Case: The node has both left and right children.
        else {
            // Recursively check if the left subtree satisfies the max heap property.
            bool left = maxheap(root->left);
            // Recursively check if the right subtree satisfies the max heap property.
            bool right = maxheap(root->right);
            
            // Return true only if both the left and right subtrees are valid max heaps,
            // AND the current node's data is greater than both its left and right child's data.
            return (left && right && 
                    (root->data > root->left->data && root->data > root->right->data));
        }
    }
    
    // Function to check if the binary tree is complete.
    // A complete binary tree means that every level, except possibly the last,
    // is completely filled, and all nodes are as far left as possible.
    bool isCompleteTree(Node* root) {
        // Use a queue to perform level-order traversal (BFS).
        queue<Node*> q;
        q.push(root);
        
        // 'flag' will be set to true once a NULL is encountered.
        // After a NULL is seen, no non-null node should appear.
        bool flag = false;

        // Traverse the tree using BFS.
        while (!q.empty()) {
            Node* temp = q.front();
            q.pop();

            // If the current node is NULL, set the flag to true.
            if (temp == NULL) {
                flag = true; // Indicates that we have reached the end of a level or a missing child.
            } 
            else {
                // If a non-null node is encountered after a NULL, the tree is not complete.
                if (flag) {
                    return false;
                }
                // Push the left child (even if it's NULL) to maintain the proper order.
                q.push(temp->left);
                // Push the right child (even if it's NULL) as well.
                q.push(temp->right);
            }
        }
        // If the loop completes without finding a non-null node after a NULL,
        // then the tree is complete.
        return true;
    }
    
    // Function to check if a given binary tree is a valid max heap.
    // A valid max heap must satisfy two properties:
    // 1. It must be a complete binary tree.
    // 2. It must satisfy the max heap property (every parent's value is greater than its children).
    bool isHeap(struct Node* tree) {
        // Check both properties: completeness and max heap ordering.
        if (isCompleteTree(tree) && maxheap(tree)) {
            return true;
        }
        else {
            return false;
        }
    }
};