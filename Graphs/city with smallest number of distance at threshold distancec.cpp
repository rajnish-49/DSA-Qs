// @https://leetcode.com/problems/find-the-city-with-the-smallest-number-of-neighbors-at-a-threshold-distance/description/

class Solution {
public:
    int findTheCity(int n, vector<vector<int>>& edges, int distanceThreshold) {

        // ---------- DESIGN CHOICE & HIGH-LEVEL IDEA ----------
        // We want: for every city A, how many other cities B have shortest-path distance <= T.
        // The easiest reliable way to get *all-pairs shortest paths* for small n is Floyd–Warshall.
        // Intuition: start with direct edges, then allow each node k to act as a potential
        // 'bridge' that can shorten routes between any i and j. After processing k,
        // all shortest paths that may use only {0..k} as intermediate nodes are known.
        //
        // Why this works for the problem:
        //   - Once we have the true shortest distance dist[i][j] for every pair,
        //     counting how many are <= distanceThreshold is trivial.
        //   - Floyd–Warshall is simple and avoids repeated Dijkstra calls; it's a clean fit
        //     when n is small (typical constraints: n ≤ 100).
        //
        // Tradeoffs:
        //   - Time: O(n^3). Memory: O(n^2).
        //   - If n were large and the graph sparse, running Dijkstra from each node
        //     (O(n * (E log V))) would be preferable.
        // ----------------------------------------------------

        // Use INT_MAX to represent "infinite" / unreachable for now.
        // Intuition: treat distances like unknown large value; later we relax them.
        vector<vector<int>> dist(n, vector<int> (n, INT_MAX));

        // Populate direct roads.
        // Note: the code assumes undirected edges. If the input may contain multi-edges
        // (multiple edges between same pair), taking the minimum weight is better.
        for (auto it : edges) {
            // If there are multiple edges between same pair, taking min is safer:
            // dist[it[0]][it[1]] = min(dist[it[0]][it[1]], it[2]);
            // dist[it[1]][it[0]] = min(dist[it[1]][it[0]], it[2]);
            dist[it[0]][it[1]] = it[2];
            dist[it[1]][it[0]] = it[2];
        }

        // Distance to self is zero — this is the base case that anchors the DP.
        // Intuition: shortest path from a node to itself is always 0; this ensures
        // relaxing through nodes doesn't accidentally claim a shorter looped path.
        for (int i = 0; i < n; i++) dist[i][i] = 0;

        // ---------- CORE INTUITION: FLOYD–WARSHALL AS TRANSITIVE RELAXATION ----------
        // For each candidate intermediate k, check whether going i -> k -> j is shorter
        // than the current best known i -> j. Doing this for every k builds up
        // shortest paths that may use any subset of nodes as intermediates.
        //
        // Important invariant after finishing each k:
        //   dist[i][j] equals the shortest path from i to j that uses only nodes
        //   from the set {0, 1, ..., k} as intermediate nodes.
        //
        // After k loops 0..n-1 finish, we've considered all nodes as intermediates,
        // so dist[] holds the true all-pairs shortest path distances.
        //
        // Practical caution: when dist[i][k] or dist[k][j] is "infinite" (INT_MAX),
        // we must skip adding them — otherwise we'd overflow INT_MAX.
        // That's why the "continue" check exists.
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (dist[i][k] == INT_MAX || dist[k][j] == INT_MAX)
                        continue; // path via k impossible if either leg is unreachable
                    // Relax the distance via k
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }

        // ---------- COUNTING & TIE-BREAKER INTUITION ----------
        // Now we have true shortest distances. For each city, count how many cities
        // (including itself — see note) are reachable within the threshold.
        //
        // Two subtle points:
        // 1) Including self in the count: because dist[i][i] = 0 <= threshold (when threshold >= 0),
        //    every city will always count itself. This is harmless for comparisons because
        //    it's a constant +1 for every city; relative ordering remains determined by
        //    their reachability to other nodes. If you prefer excluding self, subtract 1
        //    from the count (or start adjCity from 0 and skip when adjCity == city).
        //
        // 2) Tie-breaker: problem asks for the city with smallest count, and if tied,
        //    return the city with the greatest index. Using `if(cnt <= cntCity)` achieves that:
        //    - We initialize cntCity = n, cityNo = -1, iterate city in increasing order.
        //    - When cnt == cntCity, the `<=` makes the later (higher index) city overwrite the previous.
        //    So this single comparison encodes the tie-break rule without extra logic.
        int cntCity = n;    // best (smallest) count found so far; setting to n is a safe upper bound
        int cityNo = -1;

        for (int city = 0; city < n; city++) {
            int cnt = 0;
            for (int adjCity = 0; adjCity < n; adjCity++) {
                if (dist[city][adjCity] <= distanceThreshold)
                    cnt++;
            }

            // If current city can reach fewer-or-equal cities than best so far,
            // update. `<=` makes later indices win ties (because we iterate low->high).
            if (cnt <= cntCity) {
                cntCity = cnt;
                cityNo = city;
            }
        }

        // cityNo now holds the index of the city with minimum reachable neighbors
        // within distanceThreshold, with ties broken in favor of larger index.
        return cityNo;
    }
};
