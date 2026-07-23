// For every unvisited node:

// Start BFS from that node.
// Push {startNode, -1} into queue.
// Mark start node as visited.
// While queue is not empty:
    // Take out {node, parent}.
    // Traverse all neighbours of node.
    // If neighbour is unvisited:
        // Mark it visited.
        // Push {neighbour, node} into queue.
    // Else if neighbour is visited and neighbour is not parent:
    // Cycle exists.

class Solution {
private:
    bool bfs(int start, vector<bool>& visited, vector<vector<int>>& adj) {
        // Queue stores {current node, parent node}
        queue<pair<int, int>> q;

        // Start node has no parent, so parent = -1
        q.push({start, -1});
        visited[start] = true;

        while (!q.empty()) {
            int node = q.front().first;
            int parent = q.front().second;
            q.pop();

            // Traverse all neighbours of current node
            for (int neighbour : adj[node]) {

                // Case 1: If neighbour is not visited,
                // visit it and store current node as its parent
                if (!visited[neighbour]) {
                    visited[neighbour] = true;
                    q.push({neighbour, node});
                }

                // Case 2: If neighbour is already visited
                // and it is not the parent, then cycle exists.
                //
                // In an undirected graph, seeing parent again is normal.
                // Seeing any other visited node means there is another path.
                else if (neighbour != parent) {
                    return true;
                }
            }
        }

        // No cycle found in this connected component
        return false;
    }

public:
    bool isCycle(vector<vector<int>>& adj) {
        int v = adj.size();

        vector<bool> visited(v, false);

        // Graph may be disconnected,
        // so check every component separately
        for (int i = 0; i < v; i++) {
            if (!visited[i]) {
                if (bfs(i, visited, adj)) {
                    return true;
                }
            }
        }

        // No cycle found in any component
        return false;
    }
};