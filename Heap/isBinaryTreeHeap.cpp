// @ https://www.geeksforgeeks.org/problems/is-binary-tree-heap/1 

class Solution {
  public:
  
    // Function to check if the given binary tree follows the max-heap property
    bool maxheap(Node* root) {
        // If the node is a leaf node, it satisfies heap property
        if (root->left == NULL && root->right == NULL) {
            return true;
        }

        // If the node has only a left child, check the heap condition
        if (root->right == NULL) {
            return (root->data > root->left->data);
        }

        // If the node has both left and right children
        bool left = maxheap(root->left);  // Check left subtree
        bool right = maxheap(root->right); // Check right subtree
        
        // Return true if both subtrees are heaps and the current node follows the heap property
        return (left && right && (root->data > root->left->data && root->data > root->right->data));
    }
    
    // Function to check if the given binary tree is complete
    bool isCompleteTree(Node* root) {
        queue<Node*> q;
        q.push(root);
        bool flag = false; // Flag to track if we've encountered a NULL node

        while (!q.empty()) {
            Node* temp = q.front();
            q.pop();

            if (temp == NULL) {
                flag = true; // Once we encounter a NULL, all further nodes must also be NULL
            } 
            else {
                if (flag) {
                    return false; // If a non-null node appears after a NULL, it's not a complete tree
                }
                q.push(temp->left);  // Push left child (can be NULL)
                q.push(temp->right); // Push right child (can be NULL)
            }
        }
        return true;
    }
    
    // Function to check if the binary tree is a heap
    bool isHeap(struct Node* tree) {
        // A binary tree is a heap if it is complete and follows the max-heap property
        if (isCompleteTree(tree) && maxheap(tree)) {
            return true;
        }
        return false;
    }
};
