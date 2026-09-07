#include <iostream>
#include <queue>
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


// Breadth-first search (BFS) visits a binary tree one level at a time.
// A queue preserves the order in which nodes are discovered: children are
// added behind all nodes that still belong to the current level.
// Time: O(n), Space: O(w), where n is the number of nodes and w is the
// maximum width of the tree.
vector<vector<int>> levelOrderIterative(TreeNode* root) {
    vector<vector<int>> result;

    // An empty tree has no levels to return.
    if (root == nullptr)
        return result;

    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        // Capture the queue size before processing. These are exactly the
        // nodes in this level; children pushed below belong to the next one.
        int levelSize = q.size();
        vector<int> level;

        for (int i = 0; i < levelSize; i++) {
            TreeNode* node = q.front();
            q.pop();

            // Visit the node when it is removed from the queue.
            level.push_back(node->val);

            // Enqueue children left-to-right so the level order is stable.
            if (node->left)
                q.push(node->left);
            if (node->right)
                q.push(node->right);
        }

        result.push_back(level);
    }

    return result;
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

    cout << "BFS (level order): ";

    vector<vector<int>> result = levelOrderIterative(root);

    for (auto& level : result) {
        for (int x : level)
            cout << x << " ";
    }

    cout << "\n";

    return 0;
}
