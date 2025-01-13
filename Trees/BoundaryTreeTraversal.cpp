// prefer the left child over the right child when traversing. Do not include leaf nodes in this section.
//Leaf Nodes: All leaf nodes, in left-to-right order, that are not part of the left or right boundary.
// Reverse Right Boundary: This includes all the nodes on the path from the rightmost leaf node to the root, traversed in reverse order. You must prefer the right child over the left child when traversing. Do not include the root in this section if it was already included in the left boundary.

// ALGO 

/*
Initialize:

Create three lists to store the left boundary, leaf nodes, and right boundary.
Left Boundary:

Start from the root and traverse down the left subtree.
For each node, add it to the left boundary list if it is not a leaf node.
Move to the left child if it exists; otherwise, move to the right child.
Stop when a leaf node is encountered.
Leaf Nodes:

Perform an in-order traversal (or any traversal) of the tree.
For each node, if it is a leaf node (i.e., both left and right children are null), add it to the leaf nodes list.
Right Boundary:

Start from the root and traverse down the right subtree.
For each node, add it to a temporary list if it is not a leaf node.
Move to the right child if it exists; otherwise, move to the left child.
Stop when a leaf node is encountered.
Reverse the temporary list and append it to the right boundary list.
Combine Results:

Append the left boundary list.
Append the leaf nodes list.
Append the reversed right boundary list.
*/

class Solution {
  
private:
    void traverseLeft(Node* root, vector<int> &ans) {
        if ((root == NULL) || (root->left == NULL && root->right == NULL))
            return;

        ans.push_back(root->data);
        if (root->left)
            traverseLeft(root->left, ans);
        else
            traverseLeft(root->right, ans);
    }

    void traverseLeaf(Node* root, vector<int> &ans) {
        if (root == NULL)
            return;

        if (root->left == NULL && root->right == NULL) {
            ans.push_back(root->data);
            return;
        }

        traverseLeaf(root->left, ans);
        traverseLeaf(root->right, ans);
    }

    void traverseRight(Node* root, vector<int> &ans) {
        if ((root == NULL) || (root->left == NULL && root->right == NULL))
            return;

        if (root->right)
            traverseRight(root->right, ans);
        else
            traverseRight(root->left, ans);

        ans.push_back(root->data);
    }

public:
    vector<int> boundaryTraversal(Node* root) {
        vector<int> ans;
        if (root == NULL)
            return ans;

        ans.push_back(root->data);

        traverseLeft(root->left, ans);
        traverseLeaf(root->left, ans);
        traverseLeaf(root->right, ans);
        traverseRight(root->right, ans);

        return ans;
    }
};
