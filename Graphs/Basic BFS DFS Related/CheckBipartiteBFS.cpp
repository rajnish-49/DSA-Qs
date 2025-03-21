// @ https://leetcode.com/problems/is-graph-bipartite/

class Solution {
    public:
        bool isBipartite(vector<vector<int>>& graph) {
            int v = graph.size();  // Get the number of vertices in the graph
    
            vector<int> color(v, -1);  // Color array initialized to -1 (unvisited)
            queue<int> q;  // Queue for BFS traversal
    
            // Iterate through all nodes to handle disconnected components
            for (int i = 0; i < v; i++) {
                if (color[i] == -1) {  // If the node is not colored, start BFS from here
                    q.push(i);  // Push the first node of this component into the queue
                    color[i] = 0;  // Assign the first color (0)
    
                    // Standard BFS traversal
                    while (!q.empty()) {
                        int node = q.front();
                        q.pop();
    
                        // Traverse all adjacent nodes
                        for (int neighbor : graph[node]) {
                            if (color[neighbor] == -1) {  // If neighbor is uncolored
                                color[neighbor] = !color[node];  // Assign the opposite color
                                q.push(neighbor);  // Push the neighbor into the queue for BFS
                            } 
                            else if (color[neighbor] == color[node]) {  
                                // If neighbor has the same color as the current node, it's not bipartite
                                return false;
                            }
                        }
                    }
                }
            }
            
            // If BFS completes for all components without conflict, the graph is bipartite
            return true;
        }
    };
    