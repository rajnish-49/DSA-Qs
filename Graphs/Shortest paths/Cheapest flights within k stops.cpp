// @https://leetcode.com/problems/cheapest-flights-within-k-stops/

class Solution {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
        // -------------------------------
        // STEP 1: Build adjacency list
        // -------------------------------
        // Each city (u) has a list of pairs {v, price}, meaning:
        // there’s a direct flight from u -> v with given cost.
        vector<vector<pair<int, int>>> adj(n + 1);

        for (auto& f : flights)
            adj[f[0]].push_back({f[1], f[2]});

        // ----------------------------------------------------
        // STEP 2: Distance matrix with (n x (k+2)) dimensions
        // ----------------------------------------------------
        // dist[city][stops] = minimum cost to reach `city` 
        // using exactly `stops` flights (stops = number of edges taken).
        //
        // Why (k + 2)?
        // Because with `k` stops, you can take up to `k + 1` flights.
        // (e.g., 1 stop means 2 flights).
        //
        // We initialize all costs as infinity (1e9),
        // and starting city (src) with 0 cost at 0 flights.
        vector<vector<int>> dist(n, vector<int>(k + 2, 1e9));
        dist[src][0] = 0;

        // ----------------------------------------------------
        // STEP 3: Min-heap (Dijkstra style)
        // ----------------------------------------------------
        // Each entry in the heap = (total_cost, {city, flights_taken})
        // We expand the cheapest path first, similar to Dijkstra.
        //
        // Why store flights_taken?
        // Because we can’t exceed k+1 flights (or k stops).
        priority_queue<pair<int, pair<int, int>>,
                       vector<pair<int, pair<int, int>>>,
                       greater<pair<int, pair<int, int>>>>
            pq;

        // Start from source: cost = 0, city = src, flights_taken = 0
        pq.push({0, {src, 0}});

        // ----------------------------------------------------
        // STEP 4: Modified Dijkstra traversal
        // ----------------------------------------------------
        while (!pq.empty()) {
            auto it = pq.top();
            pq.pop();

            int ccost = it.first;          // current total cost so far
            int currentcity = it.second.first;
            int stops = it.second.second;  // number of flights used so far

            // Explore all outgoing flights from current city
            for (auto& edge : adj[currentcity]) {
                int adjcity = edge.first;
                int cost = edge.second;

                // Only explore if we still have flights left to use
                if (stops <= k && ccost + cost < dist[adjcity][stops + 1]) {
                    // Compute new total cost to reach adjcity
                    dist[adjcity][stops + 1] = ccost + cost;

                    // Push into heap for further exploration
                    pq.push({ccost + cost, {adjcity, stops + 1}});
                }
            }
        }

        // ----------------------------------------------------
        // STEP 5: Find the minimum cost among all possible
        // paths reaching `dst` using up to (k + 1) flights.
        // ----------------------------------------------------
        // dist[dst][0] -> cost using 0 flights (impossible except src)
        // dist[dst][1] -> cost using 1 flight (0 stops)
        // dist[dst][2] -> cost using 2 flights (1 stop)
        // ...
        // dist[dst][k+1] -> cost using k+1 flights (k stops)
        //
        // We take the minimum across all, because we only care
        // about reaching dst within *at most* k stops.
        int ans = 1e9;
        for (int i = 0; i <= k + 1; i++)
            ans = min(ans, dist[dst][i]);

        // If ans is still infinity, it means no valid route was found
        return ans == 1e9 ? -1 : ans;
    }
};


// USING BFS 

class Solution {
public:
    int CheapestFlight(int n, vector<vector<int>>& flights, int src, int dst, int K) {
        // Build adjacency list representation of the graph
        // Why use adjacency list? Because we want to quickly iterate over outgoing flights from any city.
        // Each entry adj[i] stores pairs {destination, cost}, making it easy to traverse neighbors.
        vector<pair<int, int>> adj[n];
        for(auto it : flights) {
            adj[it[0]].push_back({it[1], it[2]});
        }
        
        // BFS-style traversal using a queue
        // We store a tuple: {stops taken so far, {current node, total cost so far}}
        // Why BFS? Because we want the cheapest price within K stops, not necessarily the shortest path in edges.
        // BFS allows us to explore all options level by level (number of stops)
        queue<pair<int, pair<int, int>>> q;
        q.push({0, {src, 0}}); // Start from source with 0 cost and 0 stops

        // Distance array to keep track of minimum cost to reach each city
        // Initialized to a very high value (infinity equivalent) because initially we haven't reached any city
        vector<int> dist(n, 1e9);
        dist[src] = 0; // Cost to reach source is 0

        while(!q.empty()) {
            auto it = q.front();
            q.pop();

            int stops = it.first;          // Number of stops used to reach this node
            int node = it.second.first;    // Current city/node
            int cost = it.second.second;   // Total cost to reach this city

            // Why skip if stops > K? Because the problem restricts the number of stops to K.
            // Any path exceeding K stops is invalid.
            if(stops > K) continue;

            // Explore all neighbors (flights from current city)
            for(auto iter : adj[node]) {
                int adjNode = iter.first; // Neighbor city
                int edW = iter.second;    // Cost of flight to neighbor

                // Key insight:
                // We update dist[adjNode] only if the new cost is cheaper.
                // This prevents pushing worse paths that are guaranteed not to give the minimum price.
                // Why check stops <= K again? 
                // It's a safeguard to not propagate invalid paths further.
                if(cost + edW < dist[adjNode] && stops <= K) {
                    dist[adjNode] = cost + edW;              // Update minimum cost to reach adjNode
                    q.push({stops + 1, {adjNode, cost + edW}}); // Push neighbor with incremented stops
                }
            }
        }

        // If we never updated dist[dst], it means we couldn't reach destination within K stops
        if(dist[dst] == 1e9) return -1;
        
        // Otherwise, return the minimum cost found
        return dist[dst];
    }
};


/*

-Why BFS works here instead of Dijkstra:

Classic Dijkstra doesn’t limit number of stops; it greedily expands by cost alone.
Here, we care about cheapest flight under a stop constraint, so we also track stops and level-by-level expansion.

-Why we need dist array:

Prevents pushing paths that are already more expensive than known cheapest paths to a city.
Acts as pruning, reducing unnecessary BFS explorations.

-Why stops > K check matters:

It enforces the constraint directly in BFS.
Helps avoid exploring paths that are invalid due to excessive stops.

-Why adjacency list is used instead of adjacency matrix:

Sparsity optimization: Most cities don’t have direct flights to all other cities.
Efficient iteration over neighbors: O(E) time for all edges instead of O(V^2).

-Level-order BFS intuition:

Each BFS level represents “number of stops.”
By keeping track of stops in the queue, we can prune paths that exceed K stops.


*/