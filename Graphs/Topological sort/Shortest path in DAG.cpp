// @https://www.geeksforgeeks.org/problems/shortest-path-in-undirected-graph/1?itm_source=geeksforgeeks&itm_medium=article&itm_campaign=practice_card

class Solution {
    private:
        // Function to perform Topological Sorting using DFS
        void topoSort(int node, vector<pair<int, int>> adj[], vector<int> &vis, stack<int> &st) {
            vis[node] = 1; // Mark the current node as visited
    
            // Traverse all adjacent nodes
            for(auto it : adj[node]) {
                int v = it.first; // Get the destination node
                if(!vis[v]) { // If not visited, recursively visit it
                    topoSort(v, adj, vis, st);
                }
            }
    
            // After visiting all neighbors, push the current node to the stack
            st.push(node);
        }
    
    public:
        // Function to find the shortest paths from node 0 in a Directed Acyclic Graph (DAG)
        vector<int> shortestPath(int V, int E, vector<vector<int>>& edges) {
            // Step 1: Create adjacency list representation of the graph
            vector<pair<int, int>> adj[V];  // An array of V adjacency lists
    
            // Build the graph from the given edge list
            for(int i = 0; i < E; i++) {  
                int u = edges[i][0];  // Source node
                int v = edges[i][1];  // Destination node
                int wt = edges[i][2]; // Weight of the edge
                adj[u].push_back({v, wt}); // Add directed edge to adjacency list
            }
    
            // Step 2: Perform Topological Sorting using DFS
            vector<int> vis(V, 0); // Visited array to keep track of visited nodes
            stack<int> st; // Stack to store topological ordering
    
            // Perform DFS-based Topological Sorting for every unvisited node
            for(int i = 0; i < V; i++) {
                if(!vis[i]) {
                    topoSort(i, adj, vis, st);
                }
            }
    
            // Step 3: Initialize distance array with a large value (infinity)
            vector<int> dist(V, 1e9);
            dist[0] = 0;  // Assuming source node is 0, set its distance to 0
    
            // Step 4: Process nodes in topological order to compute shortest paths
            while(!st.empty()) {
                int node = st.top();
                st.pop(); // Remove the node from the stack
    
                // If the node has been reached previously (distance is not infinity)
                if(dist[node] != 1e9) {  
                    // Relax all the outgoing edges from the current node
                    for(auto it : adj[node]) {
                        int v = it.first;  // Destination node
                        int wt = it.second; // Weight of the edge
    
                        // Relaxation condition: update distance if a shorter path is found
                        if(dist[node] + wt < dist[v]) {
                            dist[v] = dist[node] + wt;
                        }
                    }
                }
            }
    
            // Step 5: Replace unreachable nodes' distances (still 1e9) with -1
            for(int i = 0; i < V; i++) {
                if(dist[i] == 1e9) {
                    dist[i] = -1;
                }
            }
    
            // Return the final distance array
            return dist;
        }
    };
    