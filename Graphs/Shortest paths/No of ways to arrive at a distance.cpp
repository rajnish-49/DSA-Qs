// @https://leetcode.com/problems/number-of-ways-to-arrive-at-destination/


class Solution {
public:
    int countPaths(int n, vector<vector<int>>& roads) {

        // -------------------------
        // 1) Build adjacency list
        // -------------------------
        // We use an adjacency list because we need to iterate edges of each node many times.
        // Each entry: adj[u] contains pairs {v, time} for edges u <-> v with weight time.
        vector<vector<pair<int,int>>> adj(n);
        for (auto &r : roads) {
            int u = r[0], v = r[1], t = r[2];
            // Graph is undirected, so add both directions.
            adj[u].push_back({v, t});
            adj[v].push_back({u, t});
        }

        // ---------------------------------------------------------
        // 2) Priority queue for Dijkstra: (distance, node)
        //    Use long long for distance since edge weights can be large
        // ---------------------------------------------------------
        // pq.top() gives the smallest distance entry. We store pairs as {distance, node}.
        priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<pair<long long,int>>> pq;

        // ---------------------------------------------------------
        // 3) dist[] and ways[] arrays
        //    - dist[i]  : shortest known distance from node 0 to i
        //    - ways[i]  : number of distinct shortest paths from 0 to i (mod MOD)
        // Use long long to avoid overflow when summing distances or ways temporarily.
        // ---------------------------------------------------------
        const long long MOD = 1000000007LL;
        vector<long long> dist(n, (long long)1e18); // "infinity"
        vector<long long> ways(n, 0);

        // -------------------------
        // 4) Initialization
        // -------------------------
        dist[0] = 0;     // shortest distance to source is 0
        ways[0] = 1;     // one way to be at the source (the empty path)
        pq.push({0, 0}); // push source as {distance, node}

        // ---------------------------------------------------------
        // 5) Main Dijkstra loop with path counting
        // ---------------------------------------------------------
        // Invariant maintained:
        //   - dist[x] always holds the best distance found so far to x.
        //   - ways[x] holds the number of distinct shortest paths corresponding to dist[x].
        // Reasoning:
        //   - When we relax an edge node->adjnode, we either improve adjnode's shortest distance
        //     (so we inherit node's ways) or find another shortest path (so we add node's ways).
        while (!pq.empty()) {
            // pop smallest-distance entry
            long long dis = pq.top().first;
            int node = pq.top().second;
            pq.pop();

            // This entry can be stale: Dijkstra may have pushed multiple entries for the same node.
            // If we've already found a strictly better distance earlier, skip processing.
            if (dis > dist[node]) continue;

            // Explore all neighbors of the current node
            for (auto it : adj[node]) {
                int adjnode = it.first;
                long long edw = it.second; // edge weight (time)

                long long candidate = dis + edw; // distance to adjnode through node

                // CASE A: we found a strictly shorter path to adjnode
                if (candidate < dist[adjnode]) {
                    // Update shortest distance
                    dist[adjnode] = candidate;

                    // Since this is the new shortest distance, the number of ways to reach adjnode
                    // becomes exactly the number of ways to reach 'node' (we just extend those paths).
                    ways[adjnode] = ways[node] % MOD;

                    // Push updated best distance into PQ
                    // (we push even if adjnode was already in pq with a worse distance;
                    //  skipping stale entries is handled by the 'if (dis > dist[node]) continue;' check)
                    pq.push({dist[adjnode], adjnode});
                }
                // CASE B: we found another path that ties the known shortest distance
                else if (candidate == dist[adjnode]) {
                    // We discovered additional distinct shortest paths to adjnode.
                    // Add the number of ways to reach 'node' to ways[adjnode].
                    // Important: take modulo on the sum, not just on the addend.
                    ways[adjnode] = (ways[adjnode] + ways[node]) % MOD;
                }

                // CASE C: candidate > dist[adjnode] -> ignore, it's not a shortest path.
            }
        }

        // Final answer: number of shortest paths to node n-1 (mod MOD).
        // Cast to int to match function signature.
        return (int)(ways[n - 1] % MOD);
    }
};
