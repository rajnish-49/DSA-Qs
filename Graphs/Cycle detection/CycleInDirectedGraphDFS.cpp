// @ https://www.geeksforgeeks.org/problems/detect-cycle-in-a-directed-graph/1

class Solution {
private:
    bool dfs(int node, vector<bool>& visited, vector<bool>& pathVisited,
             vector<vector<int>>& adj) {
        
        // Mark node as visited globally
        visited[node] = true;

        // Mark node as part of the current DFS path
        pathVisited[node] = true;

        // Explore all outgoing edges from current node
        for (int neighbour : adj[node]) {

            // Case 1:
            // If neighbour is not visited, continue DFS from it
            if (!visited[neighbour]) {
                if (dfs(neighbour, visited, pathVisited, adj)) {
                    return true;
                }
            }

            // Case 2:
            // If neighbour is already in the current DFS path,
            // then we found a back edge, which means a directed cycle exists
            else if (pathVisited[neighbour]) {
                return true;
            }
        }

        // DFS from this node is finished.
        // Remove it from the current active DFS path.
        pathVisited[node] = false;

        // No cycle found from this node
        return false;
    }

public:
    bool isCyclic(vector<vector<int>>& adj) {
        int v = adj.size();

        vector<bool> visited(v, false);
        vector<bool> pathVisited(v, false);

        // Graph may be disconnected,
        // so start DFS from every unvisited node
        for (int i = 0; i < v; i++) {
            if (!visited[i]) {
                if (dfs(i, visited, pathVisited, adj)) {
                    return true;
                }
            }
        }

        // No directed cycle found
        return false;
    }
};
  

  // CAN BE DONE USING KAHN'S ALGORITHM AS WELL
  