class Solution {
    private:
        // DFS function to check if the graph is bipartite
        bool dfs(int node, int col, vector<int>& color, vector<int> adj[]) {
            color[node] = col;  // Assign color to the current node
    
            // Traverse all adjacent nodes
            for (auto it : adj[node]) {
                if (color[it] == -1) {  // If the neighbor is uncolored
                    if (!dfs(it, !col, color, adj)) return false;  // Assign opposite color and DFS
                } 
                else if (color[it] == color[node]) {  // If the neighbor has the same color
                    return false;  // Not bipartite
                }
            }
            return true;  // No conflicts, graph remains bipartite
        }
    
    public:
        // Function to check if the graph is bipartite
        bool isBipartite(int V, vector<int> adj[]) {
            vector<int> color(V, -1);  // Initialize color array (-1 means uncolored)
    
            // Check for all connected components
            for (int i = 0; i < V; i++) {
                if (color[i] == -1) {  // If not colored, start DFS
                    if (!dfs(i, 0, color, adj)) return false;  // Start with color 0
                }
            }
            return true;  // If all components are bipartite
        }
    };
    