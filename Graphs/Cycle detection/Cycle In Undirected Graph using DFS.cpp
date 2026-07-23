// @ https://www.geeksforgeeks.org/problems/detect-cycle-in-an-undirected-graph/1?itm_source=geeksforgeeks&itm_medium=article&itm_campaign=practice_card


// If it is not visited, start DFS from it.
// Mark the current node as visited.
// Traverse all its neighbors.
// If neighbor is not visited, call DFS on it.
// If neighbor is already visited and neighbor is not parent, cycle exists.

 class Solution {
private:
    bool dfs(int node, int parent, vector<bool>& visited, vector<vector<int>>& adj) {
        // Mark current node as visited
        visited[node] = true;

        // Traverse all directly connected neighbours of current node
        for (int neighbour : adj[node]) {

            // If neighbour is not visited, continue DFS
            if (!visited[neighbour]) {
                if (dfs(neighbour, node, visited, adj)) {
                    return true;
                }
            }

            // Case 2: If neighbour is already visited
            // and it is not the parent, then we found a back edge.
            //
            // In an undirected graph, seeing the parent again is normal
            // because every edge exists in both directions.
            // But seeing any other visited node means a cycle exists.
            else if (neighbour != parent) {
                return true;
            }
        }

        // No cycle found from this DFS path
        return false;
    }

public:
    bool isCycle(vector<vector<int>>& adj) {
        int v = adj.size();

        vector<bool> visited(v, false);

        // Graph may be disconnected,
        // so start DFS from every unvisited node
        for (int i = 0; i < v; i++) {
            if (!visited[i]) {
                if (dfs(i, -1, visited, adj)) {
                    return true;
                }
            }
        }

        return false;
    }
};