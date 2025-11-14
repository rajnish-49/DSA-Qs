// @ https://www.geeksforgeeks.org/problems/detect-cycle-in-a-directed-graph/1

class Solution {
    private:
      // DFS function to check for cycles
      bool dfs(int node, vector<vector<int>> &adj, vector<bool> &visited, vector<bool> &pathVisited) {
          visited[node] = true;     // Mark the node as visited (part of any DFS traversal)
          pathVisited[node] = true; // Mark the node as visited in the current DFS path
  
          // Traverse all adjacent nodes (neighbors)
          for (int neighbor : adj[node]) {
              // If the adjacent node has not been visited, perform DFS on it
              if (!visited[neighbor]) {
                  if (dfs(neighbor, adj, visited, pathVisited) == true ) {  
                      return true;  // Cycle detected
                  }
              } 
              // If the adjacent node is already in the current DFS path, a cycle exists
              else if (pathVisited[neighbor]) {  
                  return true;  // Cycle detected due to a back edge
              }
          }
  
          // Backtracking step: Remove the node from the current DFS path
          pathVisited[node] = false;
          return false; // No cycle found in this DFS path
      }
  
    public:
      // Function to detect cycle in a directed graph
      bool isCyclic(vector<vector<int>> &adj) {
          int V = adj.size(); // Number of vertices
  
          // Arrays to track visited nodes and path-specific visited nodes
          vector<bool> visited(V, false);
          vector<bool> pathVisited(V, false);
  
          // Run DFS from every unvisited node to ensure all components are checked
          for (int i = 0; i < V; i++) {
              if (!visited[i]) {  // If the node is not yet visited, start DFS
                  if (dfs(i, adj, visited, pathVisited)) {  
                      return true;  // If any DFS call detects a cycle, return true
                  }
              }
          }
          return false;  // No cycle found in any component of the graph
      }
  };
  

  // CAN BE DONE USING KAHN'S ALGORITHM AS WELL
  