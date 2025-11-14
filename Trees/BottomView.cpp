class Solution {
  public:
    // Function to return a list of nodes visible from the bottom view from left to right in Binary Tree.
    vector<int> bottomView(Node *root) {
        vector<int> ans;
        if (root == NULL) {
            return ans; // Return empty if the tree is empty
        }
        
        // Map to store the bottommost node at each horizontal distance
        map<int, int> bottomViewMap;
        
        // Queue for BFS traversal: stores pairs of (node, horizontal distance)
        queue<pair<Node*, int>> q;
        q.push({root, 0}); // Start with the root node at HD = 0
        
        // BFS traversal
        while (!q.empty()) {
            pair<Node*, int> temp = q.front();
            q.pop();
            
            Node *frontNode = temp.first; // Get the current node
            int hd = temp.second;         // Get its horizontal distance
            
            // Update the map for the bottom view (always overwrite)
            bottomViewMap[hd] = frontNode->data;
    
            // Enqueue the left child with HD - 1
            if (frontNode->left) {
                q.push({frontNode->left, hd - 1});
            }
            
            // Enqueue the right child with HD + 1
            if (frontNode->right) {
                q.push({frontNode->right, hd + 1});
            }
        }
        
        // Traverse the map and store the bottom view in `ans`
        for (auto i : bottomViewMap) {
            ans.push_back(i.second);  // Get the bottommost node at each HD
        }
        
        return ans;
    }
};
