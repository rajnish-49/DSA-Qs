class Solution {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        // Result vector to store all levels
        vector<vector<int>> result;
        
        // Base case: if tree is empty, return empty result
        if (root == NULL) {
            return result;
        }

        // Queue to handle level-order traversal
        queue<TreeNode*> q;
        // Push root to start traversal
        q.push(root);

        // Flag to track direction of traversal
        // true = left-to-right, false = right-to-left
        bool lefttoright = true;

        // Process nodes level by level until queue is empty
        while (!q.empty()) {
            // Get number of nodes at current level
            int size = q.size();
            
            // Redundant check since while(!q.empty()) already handles this
            if (size == 0) {
                break;
            }

            // Create vector to store current level's values
            // Preallocate size to avoid index out of bounds error
            vector<int> ans(size);

            // Process all nodes at current level
            for (int i = 0; i < size; i++) {
                // Get front node from queue and remove it
                TreeNode* frontNode = q.front();
                q.pop();

                // Calculate position where value should be inserted
                // For left-to-right: use i
                // For right-to-left: use (size-i-1) to reverse order
                int index;
                if (lefttoright) {
                    index = i;
                } else {
                    index = size - i - 1;
                }

                // Store node's value at calculated index
                ans[index] = frontNode->val;

                // Add children to queue for next level processing
                // Left child is added first, then right
                // This maintains level order traversal
                if (frontNode->left) q.push(frontNode->left);
                if (frontNode->right) q.push(frontNode->right);
            }

            // Add current level's values to result
            result.push_back(ans);
            
            // Toggle direction for next level
            lefttoright = !lefttoright;
        }

        return result;
    }
};