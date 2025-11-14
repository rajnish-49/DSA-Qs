class Solution
{
    public:
    void findPreSuc(Node* root, Node*& pre, Node*& suc, int key) {
    if (root == nullptr) return; // Base case: If the root is null, return

    if (root->key == key) {
        // Find predecessor: rightmost node in the left subtree
        if (root->left) {
            Node* temp = root->left;
            while (temp->right) temp = temp->right;
            pre = temp;
        }

        // Find successor: leftmost node in the right subtree
        if (root->right) {
            Node* temp = root->right;
            while (temp->left) temp = temp->left;
            suc = temp;
        }
        return;
    }

    if (key < root->key) {
        // Update successor to the current node (potential successor)
        suc = root;
        // Recur to the left subtree
        findPreSuc(root->left, pre, suc, key);
    } else {
        // Update predecessor to the current node (potential predecessor)
        pre = root;
        // Recur to the right subtree
        findPreSuc(root->right, pre, suc, key);
    }
}

};