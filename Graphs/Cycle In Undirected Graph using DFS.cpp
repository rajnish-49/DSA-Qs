// @ https://www.geeksforgeeks.org/problems/detect-cycle-in-an-undirected-graph/1?itm_source=geeksforgeeks&itm_medium=article&itm_campaign=practice_card

class Solution {
    private: 
      // DFS function to check for a cycle in an undirected graph
      bool dfs(int node, int parent, vector<int>& vis, vector<vector<int>>& adj) {
          // Mark the current node as visited
          vis[node] = 1;  
          
          // Traverse all adjacent nodes of the current node
          for(auto adjacentNode: adj[node]) {
              // If the adjacent node is not visited, perform DFS recursively
              if(!vis[adjacentNode]) {
                  if(dfs(adjacentNode, node, vis, adj) == true ) {
                    return true; 
                   } // If cycle found in recursion, return true
              }
              // If the adjacent node is already visited and is NOT the parent of the current node,
              // then a cycle exists in the graph.
              else if(adjacentNode != parent) {
                  return true;  // Cycle detected
              }
          }
          return false;  // No cycle found in this path
      }
  
    public:
      // Function to detect cycle in an undirected graph
      bool isCycle(vector<vector<int>>& adj) {
          int v = adj.size();  // Number of vertices in the graph
          
          // Visited array initialized to 0 (all nodes unvisited)
          vector<int> vis(v, 0);  
          
          // Traverse all nodes (handling disconnected graphs)
          for (int i = 0; i < v; i++) {
              // If the node is unvisited, perform DFS
              if (!vis[i]) {  
                  if (dfs(i, -1, vis, adj)) { // Start DFS with -1 as parent for the first node
                      return true;  // Cycle detected in any component
                  }
              }
          }
          
          return false;  // No cycle found in any component
      }
  };
  