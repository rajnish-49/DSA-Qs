class Solution {
public:
    vector<int> largestValues(TreeNode* root) {
        vector<int> result;

        if (root == nullptr)
            return result;

        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {

            // Number of nodes in the current level.
            int levelSize = q.size();

            // Initialize maximum with the smallest possible integer.
            int maximum = INT_MIN;

            // Process all nodes of the current level.
            for (int i = 0; i < levelSize; i++) {

                TreeNode* node = q.front();
                q.pop();

                maximum = max(maximum, node->val);

                // Add children for the next level.
                if (node->left)
                    q.push(node->left);

                if (node->right)
                    q.push(node->right);
            }

            // Store the largest value of the current level.
            result.push_back(maximum);
        }

        return result;
    }
};