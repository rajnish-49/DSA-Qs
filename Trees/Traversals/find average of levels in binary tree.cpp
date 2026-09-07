class Solution {
public:
    vector<double> averageOfLevels(TreeNode* root) {
        vector<double> result;

        if (root == nullptr)
            return result;

        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {

            // Number of nodes in the current level.
            int levelSize = q.size();

            long long sum = 0;

            // Process all nodes of the current level.
            for (int i = 0; i < levelSize; i++) {

                TreeNode* node = q.front();
                q.pop();

                sum += node->val;

                // Add children for the next level.
                if (node->left)
                    q.push(node->left);

                if (node->right)
                    q.push(node->right);
            }

            // Average = sum of current level / number of nodes.
            result.push_back((double)sum / levelSize);
        }

        return result;
    }
};