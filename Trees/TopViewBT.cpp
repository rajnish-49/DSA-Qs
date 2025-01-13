
class Solution {
  public:
    // Function to return a list of nodes visible from the top view from left to right in Binary Tree.
    vector<int> topView(Node *root) {
        vector<int> ans;
        if (root == NULL) {
            return ans; // Return empty if the tree is empty
        }
        
        // Map to store the topmost node at each horizontal distance
        map<int, int> topViewMap;
        
        // Queue for BFS traversal: stores pairs of (node, horizontal distance)
        queue<pair<Node*, int>> q;
        q.push({root, 0}); // Start with the root node at HD = 0
        
        // BFS traversal
        while (!q.empty()) {
            pair<Node*, int> temp = q.front();
            q.pop();
            
            Node *frontNode = temp.first; // Get the current node
            int hd = temp.second;         // Get its horizontal distance
            
            // If this HD is being visited for the first time, store the node's value
            if (topViewMap.find(hd) == topViewMap.end()) {
                topViewMap[hd] = frontNode->data;
            }
            
            // Enqueue the left child with HD - 1
            if (frontNode->left) {
                q.push({frontNode->left, hd - 1});
            }
            
            // Enqueue the right child with HD + 1
            if (frontNode->right) {
                q.push({frontNode->right, hd + 1});
            }
        }
        
        // Traverse the map and store the top view in `ans`
        for ( auto i:  topViewMap) {
            ans.push_back(i.second);
        }
        return ans;
    }
};


