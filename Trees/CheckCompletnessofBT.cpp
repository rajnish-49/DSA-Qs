// @ https://leetcode.com/problems/check-completeness-of-a-binary-tree/description/

class Solution {
public:
    bool isCompleteTree(TreeNode* root) {
        // Queue for level-order traversal (BFS)
        queue<TreeNode*> q;
        q.push(root);

        bool flag = false; // Flag to indicate if a NULL node has been encountered

        while (!q.empty()) {
            TreeNode* temp = q.front(); // Get the front node
            q.pop(); // Remove the node from the queue

            // If the node is NULL, mark that a NULL node has been seen
            if (temp == NULL) {
                flag = true; // Once a NULL node appears, all further nodes must also be NULL
            } 
            else {
                // If we have already seen a NULL node before and now encounter a non-null node, it's not a complete tree
                if (flag) {
                    return false; // A non-null node appearing after NULL means tree is not complete
                }

                // Push left and right children into the queue
                // We push NULL values too, to properly check completeness
                q.push(temp->left); 
                q.push(temp->right);
            }
        }
        return true; // If no rule is violated, the tree is complete
    }
};