// @https://www.geeksforgeeks.org/problems/shortest-path-in-weighted-undirected-graph/1

class Solution {
public:
    vector<int> shortestPath(int n, int m, vector<vector<int>>& edges) {
        
        // The graph is undirected and weighted.
        // We'll convert the edge list into an adjacency list for efficient traversal.
        // Adjacency list helps us quickly explore all neighbors of a given node.
        vector<vector<pair<int, int>>> adj(n + 1);
        for (auto& edge : edges) {
            int u = edge[0];
            int v = edge[1];
            int w = edge[2];
            
            // Each edge connects u <-> v with weight w.
            // Since graph is undirected, we must add both directions.
            adj[u].push_back({v, w});
            adj[v].push_back({u, w});
        }

        // dist[i] = current known shortest distance from source(1) to node i
        // parent[i] = previous node used to reach i along shortest path
        // Initially, we assume every node is unreachable (∞ distance)
        vector<int> dist(n + 1, 1e9);
        vector<int> parent(n + 1, -1);

        // Our source is always node 1.
        // Its distance is 0 because cost to reach itself = 0.
        dist[1] = 0;

        // We'll use a **min-heap (priority queue)** to always expand the node
        // with the *smallest distance so far* — this ensures Dijkstra’s correctness.
        // The pair stores: {distance_so_far, node}
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        // Start from source node (1) with distance = 0
        pq.push({0, 1});

        // Dijkstra’s algorithm:
        // We repeatedly take the node with the smallest distance discovered so far
        // and try to improve (relax) its neighbors.
        while (!pq.empty()) {
            auto [currDist, node] = pq.top();
            pq.pop();

            // Explore all adjacent nodes of this current node.
            // We try to “relax” the edge — meaning, check if going via current node
            // offers a cheaper route to reach that neighbor.
            for (auto& it : adj[node]) {
                int adjnode = it.first;
                int edgeweight = it.second;

                // If the current path gives a smaller distance to adjnode,
                // we’ve found a better route — update it.
                if (currDist + edgeweight < dist[adjnode]) {
                    dist[adjnode] = currDist + edgeweight;  // new shorter distance
                    parent[adjnode] = node;                 // record how we reached this node
                    pq.push({dist[adjnode], adjnode});      // push updated distance into heap
                }
            }
        }

        // If after running Dijkstra, the destination (n) still has ∞ distance,
        // that means there is no valid path from 1 → n.
        if (dist[n] == 1e9) {
            return {-1};
        }

        // Reconstruct the actual path from destination → source using the parent array.
        // Each node points to its predecessor along the shortest route.
        vector<int> path;
        int node = n;

        // Keep following the chain of parents until we reach the source,
        // whose parent is -1 (meaning it has no predecessor).
        while (node != -1) {
            path.push_back(node);
            node = parent[node];
        }

        // Currently, path is built in reverse (n → 1).
        // Reverse it to make it 1 → n.
        reverse(path.begin(), path.end());

        // The problem expects:
        // path[0] = total weight of the path
        // remaining elements = sequence of nodes in that path.
        path.insert(path.begin(), dist[n]);

        return path;
    }
};
