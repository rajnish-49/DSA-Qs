class Solution {
public:
    int dfs(int node, vector<vector<pair<int, int>>>& adj, vector<bool>& visited) {
        // Mark current city as visited
        visited[node] = true;

        int changes = 0;

        // Explore all connected cities
        for (auto [neighbour, cost] : adj[node]) {

            // If neighbour is not visited yet,
            // move to that city and add cost of this edge
            if (!visited[neighbour]) {

                // cost = 1 means this original road direction is wrong
                // cost = 0 means this road already points toward city 0
                changes += cost;

                // Continue DFS from neighbour
                changes += dfs(neighbour, adj, visited);
            }
        }

        return changes;
    }

    int minReorder(int n, vector<vector<int>>& connections) {
        // adj[u] stores {v, cost}
        //
        // cost = 1 means edge u -> v needs reversal
        // cost = 0 means edge u -> v does not need reversal
        vector<vector<pair<int, int>>> adj(n);

        // Build graph for traversal
        for (auto& edge : connections) {
            int a = edge[0];
            int b = edge[1];

            // Original road is a -> b.
            //
            // If we travel from a to b while starting from 0,
            // this edge is pointing away from 0, so it must be reversed.
            adj[a].push_back({b, 1});

            // If we travel from b to a,
            // original edge a -> b already helps b reach toward a.
            // So no reversal is needed.
            adj[b].push_back({a, 0});
        }

        vector<bool> visited(n, false);

        // Start traversal from city 0
        return dfs(0, adj, visited);
    }
};