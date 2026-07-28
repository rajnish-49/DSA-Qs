class Solution {
public:
    int networkDelayTime(vector<vector<int>>& times, int n, int k) {

        // adj[u] stores all directed outgoing edges from node u.
        // Each pair is {neighbour, travelTime}.
        vector<vector<pair<int, int>>> adj(n + 1);

        for (auto &edge : times) {
            int u = edge[0];
            int v = edge[1];
            int w = edge[2];

            // Directed edge: u -> v with weight w.
            adj[u].push_back({v, w});
        }

        // dist[i] = minimum time currently known
        // to reach node i from source k.
        vector<int> dist(n + 1, INT_MAX);

        // The signal starts at node k, so reaching k takes 0 time.
        dist[k] = 0;

        // Min-heap storing {distance from k, node}.
        // The node with the smallest current distance is processed first.
        priority_queue<
            pair<int, int>,
            vector<pair<int, int>>,
            greater<pair<int, int>>
        > pq;

        pq.push({0, k});

        while (!pq.empty()) {

            // Extract the node with the smallest distance
            // currently available in the heap.
            int currDist = pq.top().first;
            int node = pq.top().second;
            pq.pop();

            // A node can be inserted into the heap multiple times.
            //
            // Example:
            // First, we may reach a node with distance 10.
            // Later, we may find a better path with distance 6.
            //
            // Both {10, node} and {6, node} remain in the heap.
            // When {10, node} comes out, it is outdated, so skip it.
            if (currDist > dist[node]) {
                continue;
            }

            // Try every outgoing edge from the current node.
            for (auto &edge : adj[node]) {

                int neighbour = edge.first;
                int weight = edge.second;

                // New possible distance to neighbour
                // by travelling through the current node.
                int newDist = currDist + weight;

                // Relaxation:
                // If this path is better than the previously known path,
                // update the shortest distance.
                if (newDist < dist[neighbour]) {

                    dist[neighbour] = newDist;

                    // Push the improved distance into the min-heap
                    // so this neighbour can later be processed.
                    pq.push({newDist, neighbour});
                }
            }
        }

        // dist[i] now stores the minimum time required
        // for the signal to reach node i.
        //
        // All nodes receive the signal only when the last/farthest
        // reachable node receives it, so the answer is the maximum
        // shortest distance.
        int answer = 0;

        for (int node = 1; node <= n; node++) {

            // INT_MAX means this node was never reachable from k.
            if (dist[node] == INT_MAX) {
                return -1;
            }

            answer = max(answer, dist[node]);
        }

        return answer;
    }
};