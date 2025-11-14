// @ https://leetcode.com/problems/maximum-level-sum-of-a-binary-tree/description/


class Solution {
    public:
        int maxLevelSum(TreeNode* root) {
            // Queue for level-order traversal (BFS)
            queue<TreeNode*> q;
            q.push(root); // Start with the root node
    
            int level = 0;        // Keeps track of the current level
            int maxsum = INT_MIN; // Stores the maximum level sum found so far
            int levelno = 0;      // Stores the level number with the maximum sum
    
            // BFS traversal (level-order)
            while (!q.empty()) {
                int k = q.size();  // Number of nodes at the current level
                int currentsum = 0; // Sum of node values at the current level
                level++; // Move to the next level
    
                // Process all nodes at the current level
                while (k--) {
                    TreeNode* temp = q.front(); // Get the front node
                    q.pop(); // Remove it from the queue
                    currentsum += temp->val; // Add its value to the current level sum
    
                    // Push left child if it exists
                    if (temp->left) {
                        q.push(temp->left);
                    }
    
                    // Push right child if it exists
                    if (temp->right) {
                        q.push(temp->right);
                    }
                }
    
                // Update maxsum and levelno if we found a new maximum sum level
                if (currentsum > maxsum) {
                    maxsum = currentsum;
                    levelno = level;
                }
            }
    
            // Return the level number with the maximum sum
            return levelno;
        }
    };
    