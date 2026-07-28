class Solution {
public:
    int countPaths(int n, vector<vector<int>>& roads) {

        const int MOD = 1e9 + 7;

        // Build an undirected weighted graph.
        //
        // adj[u] contains pairs of:
        // {neighbour, travelTime}
        //
        // Since every road can be travelled in both directions,
        // we add both u -> v and v -> u.
        vector<vector<pair<int, int>>> adj(n);

        for (auto& road : roads) {
            int u = road[0];
            int v = road[1];
            int travelTime = road[2];

            adj[u].push_back({v, travelTime});
            adj[v].push_back({u, travelTime});
        }

        // dist[node] stores the shortest travel time found
        // from node 0 to this node.
        //
        // Initially, all nodes are considered unreachable,
        // so their distances are set to infinity.
        vector<long long> dist(n, LLONG_MAX);

        // ways[node] stores the number of different paths
        // that reach this node using exactly dist[node] time.
        vector<long long> ways(n, 0);

        // We start at node 0.
        //
        // Distance from node 0 to itself is 0.
        dist[0] = 0;

        // There is exactly one way to be at the starting node:
        // start there without travelling through any road.
        ways[0] = 1;

        // Min-heap storing:
        // {shortest distance currently known, node}
        //
        // Dijkstra always processes the node with the
        // smallest currently known distance first.
        priority_queue<
            pair<long long, int>,
            vector<pair<long long, int>>,
            greater<pair<long long, int>>
        > pq;

        pq.push({0, 0});

        while (!pq.empty()) {

            // Extract the node having the smallest distance
            // among all entries currently present in the heap.
            long long currentDistance = pq.top().first;
            int currentNode = pq.top().second;
            pq.pop();

            // The same node may be pushed into the heap multiple times.
            //
            // Example:
            // We may first reach a node with distance 10.
            // Later, we may find a shorter route with distance 6.
            //
            // Both {10, node} and {6, node} remain in the heap.
            // If {10, node} is popped later, it is outdated.
            if (currentDistance > dist[currentNode]) {
                continue;
            }

            // Try travelling from currentNode to each of its neighbours.
            for (auto& edge : adj[currentNode]) {
                int neighbour = edge.first;
                int travelTime = edge.second;

                // Distance to the neighbour through currentNode:
                //
                // distance from source to currentNode
                // +
                // weight of the current edge
                long long newDistance =
                    currentDistance + travelTime;

                // Case 1:
                // We found a strictly shorter route to the neighbour.
                if (newDistance < dist[neighbour]) {

                    // Replace the previously known distance.
                    dist[neighbour] = newDistance;

                    // All previously counted paths to neighbour were
                    // longer, so they must be discarded.
                    //
                    // Every shortest path reaching currentNode can be
                    // extended through this edge to form a shortest
                    // path reaching neighbour.
                    ways[neighbour] = ways[currentNode];

                    // Push the improved distance into the heap.
                    pq.push({newDistance, neighbour});
                }

                // Case 2:
                // We found another route whose total distance is equal
                // to the shortest distance already known for neighbour.
                else if (newDistance == dist[neighbour]) {

                    // The paths reaching currentNode give us additional
                    // shortest paths to neighbour through this edge.
                    ways[neighbour] =
                        (ways[neighbour] + ways[currentNode]) % MOD;
                }
            }
        }

        // The destination is node n - 1.
        //
        // ways[n - 1] contains the number of paths from node 0
        // to node n - 1 whose total travel time is minimum.
        return ways[n - 1] % MOD;
    }
};