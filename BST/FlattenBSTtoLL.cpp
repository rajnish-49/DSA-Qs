class Solution
{
public:
    // Helper function to perform in-order traversal of the BST
    void inorder(Node* root, vector<int>& ans) {
        // Base case: If the node is null, return
        if (root == NULL) {
            return;
        }

        // Recursive call to traverse the left subtree
        inorder(root->left, ans);

        // Add the current node's data to the vector
        ans.push_back(root->data);

        // Recursive call to traverse the right subtree
        inorder(root->right, ans);
    }

    // Function to flatten the BST into a linked list
    Node* flattenBST(Node* root) {
        vector<int> ans; // Vector to store the in-order traversal of the BST
        
        // Step 1: Perform in-order traversal to get a sorted list of values
        inorder(root, ans);

        Node* head = NULL;   // Pointer to the head of the linked list
        Node* current = NULL; // Pointer to track the last node in the linked list

        // Step 2: Iterate through the sorted values and create the linked list
        for (auto i : ans) {
            Node* newnode = new Node(i); // Create a new node for each value
            newnode->left = NULL;        // Ensure the left pointer is null
            newnode->right = NULL;       // Right pointer will be linked later

            if (head == NULL) {
                // If the linked list is empty, set the new node as the head
                head = newnode;
                current = head; // Initialize the current pointer
            } else {
                // Link the new node to the current node's right
                current->right = newnode;
                current = newnode; // Move the current pointer to the new node
            }
        }

        // Step 3: Return the head of the linked list
        return head;
    }
};
