#include <iostream>
#include <stack>
#include <vector>
using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int x) {
        val = x;
        left = nullptr;
        right = nullptr;
    }
};

// Depth-first search (DFS) explores one branch as far as possible before
// moving to the next branch. The recursive versions use the call stack to
// remember where to return; each visits every node once (O(n) time).

// Preorder: Root -> Left -> Right. Useful when the parent must be processed
// before its children (for example, serialising a tree).
void preorderRecursive(TreeNode* root) {
    // The null pointer marks the end of the current branch.
    if (root == nullptr)
        return;

    cout << root->val << " ";
    // Visit the complete left subtree, then the complete right subtree.
    preorderRecursive(root->left);
    preorderRecursive(root->right);
}

// Inorder: Left -> Root -> Right. For a binary search tree this prints keys
// in sorted order.
void inorderRecursive(TreeNode* root) {
    if (root == nullptr)
        return;

    // Delay the root until its left subtree has been visited.
    inorderRecursive(root->left);
    cout << root->val << " ";
    inorderRecursive(root->right);
}

// Postorder: Left -> Right -> Root. Useful when children must be handled
// before their parent (for example, deleting a tree).
void postorderRecursive(TreeNode* root) {
    if (root == nullptr)
        return;

    postorderRecursive(root->left);
    postorderRecursive(root->right);
    cout << root->val << " ";
}


// Iterative DFS replaces recursive calls with an explicit stack. The stack
// contains the next branches to explore, so it avoids call-stack recursion.

// Preorder: Root -> Left -> Right
void preorderIterative(TreeNode* root) {
    if (root == nullptr)
        return;

    stack<TreeNode*> st;
    st.push(root);

    while (!st.empty()) {
        TreeNode* node = st.top();
        st.pop();

        cout << node->val << " ";

        // A stack is LIFO: push right first so left is popped and visited first.
        if (node->right)
            st.push(node->right);

        if (node->left)
            st.push(node->left);
    }
}

// Inorder: Left -> Root -> Right
void inorderIterative(TreeNode* root) {
    stack<TreeNode*> st;
    TreeNode* curr = root;

    while (curr != nullptr || !st.empty()) {

        // Store the path to the next leftmost node before visiting anything.
        while (curr != nullptr) {
            st.push(curr);
            curr = curr->left;
        }

        // The top is the first node whose left subtree is complete.
        curr = st.top();
        st.pop();

        cout << curr->val << " ";

        // Continue with the right subtree; its left path will be pushed next.
        curr = curr->right;
    }
}

// Postorder: Left -> Right -> Root
void postorderIterative(TreeNode* root) {
    if (root == nullptr)
        return;

    stack<TreeNode*> st1, st2;
    st1.push(root);

    // The first stack creates Root -> Right -> Left order and the second
    // reverses it into the required Left -> Right -> Root order.
    while (!st1.empty()) {
        TreeNode* node = st1.top();
        st1.pop();

        st2.push(node);

        if (node->left)
            st1.push(node->left);

        if (node->right)
            st1.push(node->right);
    }

    while (!st2.empty()) {
        cout << st2.top()->val << " ";
        st2.pop();
    }
}


int main() {

    //          1
    //        /   \
    //       2     3
    //      / \   / \
    //     4   5 6   7

    TreeNode* root = new TreeNode(1);

    root->left = new TreeNode(2);
    root->right = new TreeNode(3);

    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);

    root->right->left = new TreeNode(6);
    root->right->right = new TreeNode(7);

    cout << "Recursive Preorder: ";
    preorderRecursive(root);
    cout << "\n";

    cout << "Recursive Inorder: ";
    inorderRecursive(root);
    cout << "\n";

    cout << "Recursive Postorder: ";
    postorderRecursive(root);
    cout << "\n";

    cout << "\nIterative Preorder: ";
    preorderIterative(root);
    cout << "\n";

    cout << "Iterative Inorder: ";
    inorderIterative(root);
    cout << "\n";

    cout << "Iterative Postorder: ";
    postorderIterative(root);
    cout << "\n";

    return 0;
}
