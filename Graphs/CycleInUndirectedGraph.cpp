// @ https://www.geeksforgeeks.org/problems/detect-cycle-in-an-undirected-graph/1?itm_source=geeksforgeeks&itm_medium=article&itm_campaign=practice_card

class Solution {
    private:
    
    // Function to detect a cycle in an undirected graph using BFS
    bool detect(int src, int vis[], vector<vector<int>> &adj) {
        
        vis[src] = 1; // Mark the starting node as visited
 
        // Queue to perform BFS traversal
        // Each element in the queue stores {current node, parent node}
        queue<pair<int, int>> q; 
        
        q.push({src, -1}); // Push the source node with parent as -1 (no parent)

        while (!q.empty()) {
            int node = q.front().first;   // Current node being processed
            int parent = q.front().second; // Parent of the current node
            q.pop(); // Remove the node from queue
            
            // Traverse all adjacent nodes (neighbors) of the current node
            for (auto adjacentNode : adj[node]) {
                // If the adjacent node is unvisited
                if (!vis[adjacentNode]) {
                    vis[adjacentNode] = 1; // Mark it as visited
                    q.push({adjacentNode, node}); // Push the node with its parent
                }
                // If the adjacent node is already visited and is NOT the parent
                else if (parent != adjacentNode) {
                    // Cycle detected
                    return true;
                }
            }
        }
        // No cycle found in this component
        return false;
    }
    
  public:
  
    // Function to check if the graph contains a cycle
    bool isCycle(vector<vector<int>>& adj) {
        int v = adj.size(); // Number of vertices in the graph
        
        int vis[v] = {0}; // Visited array initialized to 0 (all nodes unvisited)
        
        // Check all components of the graph (handles disconnected graphs)
        for (int i = 0; i < v; i++) {
            if (!vis[i]) { // If node is unvisited, perform BFS
                if (detect(i, vis, adj)) { // If cycle is detected in any component
                    return true; 
                }
            }
        }
        
        // No cycle detected in any component
        return false;
    }
};
