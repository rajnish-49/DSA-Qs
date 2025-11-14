/*
Outer map<int, map<int, vector<int>>>:

The key in the outer map is the horizontal distance (HD) from the root node.
Each value in the outer map is another map that organizes nodes by their level.
Inner map<int, vector<int>>:

The key in the inner map is the level (row) of the node.
Each value in the inner map is a vector<int> that stores all the node values at that particular level and horizontal distance.
Innermost vector<int>:

Contains the values of the nodes that share the same horizontal distance and level.
The vector<int> is used because multiple nodes can exist at the same horizontal distance and level.*/


/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    // Function to perform the vertical order traversal
    vector<vector<int>> verticalTraversal(TreeNode* root) {
        // Result vector to store the vertical order traversal
        vector<vector<int>> ans; 
        
        // Return empty result if root is NULL
        if (root == NULL) {
            return ans;
        }

        // Map to store the vertical levels of the tree
        // nodes[hd] = {row, list of values at this row}
        map<int, map<int, vector<int>>> nodes;

        // Queue for BFS traversal, storing {node, {row, hd}}.
        queue<pair<TreeNode*, pair<int, int>>> q;

        // Push the root into the queue with initial horizontal distance (hd = 0) and row (level = 0)
        q.push({root, {0, 0}});

        // BFS traversal of the binary tree
        while (!q.empty()) {
            // Dequeue the front element
            pair<TreeNode*, pair<int, int>> temp = q.front();
            q.pop();

            // Extract the current node, its horizontal distance (hd), and its row (level)
            TreeNode* frontnode = temp.first;
            int hd = temp.second.first;
            int level = temp.second.second;

            // Store the node's value in the map at the correct hd and level
            nodes[hd][level].push_back(frontnode->val);

            // If left child exists, enqueue it with updated hd and level
            if (frontnode->left) {
                q.push(make_pair(frontnode->left, make_pair(hd - 1, level + 1)));
            }

            // If right child exists, enqueue it with updated hd and level
            if (frontnode->right) {
                q.push(make_pair(frontnode->right, make_pair(hd + 1, level + 1)));
            }
        }

        // Now, process the map to generate the final vertical order result
        for (auto& i : nodes) {
            vector<int> col;

            // For each row at this horizontal distance (hd)
            for (auto& j : i.second) {
                // Sort the values at the same level to maintain left-to-right order
                sort(j.second.begin(), j.second.end());

                // Add the sorted values to the column
                col.insert(col.end(), j.second.begin(), j.second.end());
            }

            // Add the entire column (with all nodes at this horizontal distance) to the result
            ans.push_back(col);
        }

        // Return the final vertical order traversal
        return ans;
    }
};

/*
Initialize Data Structures:

map<int, map<int, vector<int>>> nodes:
To store nodes grouped by horizontal distance (hd) and row (level).
queue<pair<TreeNode*, pair<int, int>>> q:
For BFS traversal, storing {node, {row, hd}}.
Push Root into the Queue:

Start BFS with q.push({root, {0, 0}}) (root is at row = 0 and hd = 0).
Perform BFS Traversal:

While the queue is not empty:
Pop the Front Node:
Extract node, row, and hd from q.front(); then q.pop().
Store the Node Value:
Add node->val to nodes[hd][row].
Push Left and Right Children (if exist):
Left child: q.push({node->left, {row + 1, hd - 1}}).
Right child: q.push({node->right, {row + 1, hd + 1}}).
Extract Results:

Traverse nodes in increasing order of hd:
For each hd, traverse its rows in increasing order.
Append all values to form columns.
Return the Result:

Return a vector of columns representing the vertical order.*/