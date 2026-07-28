class Solution {
public:
    int findCheapestPrice(
        int n,
        vector<vector<int>>& flights,
        int src,
        int dst,
        int k
    ) {
        // Build a directed weighted graph.
        //
        // Each flight is:
        // {from, to, price}
        //
        // adj[u] stores:
        // {neighbour, ticketPrice}
        //
        // We add only from -> to because flights are directed.
        vector<vector<pair<int, int>>> adj(n);

        for (auto& flight : flights) {
            int from = flight[0];
            int to = flight[1];
            int price = flight[2];

            adj[from].push_back({to, price});
        }

        // k stops means at most k + 1 flights.
        //
        // Example:
        // src -> A -> dst
        //
        // Stops = 1
        // Flights = 2
        //
        // Therefore, valid flight counts are:
        // 0, 1, 2, ..., k + 1
        //
        // We need k + 2 columns to represent all these values.
        //
        // dist[node][flightsUsed] stores:
        // the minimum cost found to reach 'node'
        // using exactly 'flightsUsed' flights.
        //
        // Reaching the same node with different flight counts
        // must be treated as different states.
        vector<vector<int>> dist(
            n,
            vector<int>(k + 2, INT_MAX)
        );

        // Min-heap entry format:
        //
        // {
        //     total cost so far,
        //     {
        //         current node,
        //         number of flights used
        //     }
        // }
        //
        // The state having the smallest total cost
        // is processed first.
        priority_queue<
            pair<int, pair<int, int>>,
            vector<pair<int, pair<int, int>>>,
            greater<pair<int, pair<int, int>>>
        > pq;

        // At the source:
        //
        // Cost = 0
        // Flights used = 0
        dist[src][0] = 0;
        pq.push({0, {src, 0}});

        while (!pq.empty()) {
            // Extract the cheapest state currently available.
            int currentCost = pq.top().first;
            int currentNode = pq.top().second.first;
            int flightsUsed = pq.top().second.second;
            pq.pop();

            // The same state:
            //
            // (currentNode, flightsUsed)
            //
            // may be pushed into the heap multiple times.
            //
            // Example:
            // Reach node 3 using 2 flights with cost 500.
            // Later reach node 3 using 2 flights with cost 350.
            //
            // Both entries may remain in the heap.
            // If the old cost 500 is popped later, skip it.
            if (currentCost > dist[currentNode][flightsUsed]) {
                continue;
            }

            // Because the heap processes states in increasing cost order,
            // the first valid destination state removed from the heap
            // has the minimum possible cost within the flight limit.
            if (currentNode == dst) {
                return currentCost;
            }

            // At most k + 1 flights are allowed.
            //
            // If this state has already used k + 1 flights,
            // we cannot take another flight from here.
            if (flightsUsed == k + 1) {
                continue;
            }

            // Explore every outgoing flight from currentNode.
            for (auto& edge : adj[currentNode]) {
                int neighbour = edge.first;
                int price = edge.second;

                // Taking this flight adds its ticket price.
                int newCost = currentCost + price;

                // We have taken one additional flight.
                int newFlightsUsed = flightsUsed + 1;

                // This accesses one specific state:
                //
                // dist[neighbour][newFlightsUsed]
                //
                // Example:
                // dist[4][2] means:
                // cheapest cost to reach node 4 using exactly 2 flights.
                //
                // newFlightsUsed does not perform a comparison.
                // It selects which flight-count state we are checking.
                //
                // Then newCost is compared against the cost stored
                // for that exact state.
                if (newCost < dist[neighbour][newFlightsUsed]) {
                    // We found a cheaper way to reach the neighbour
                    // using exactly newFlightsUsed flights.
                    dist[neighbour][newFlightsUsed] = newCost;

                    // Push the improved state into the min-heap.
                    pq.push({
                        newCost,
                        {neighbour, newFlightsUsed}
                    });
                }
            }
        }

        // If no valid route reaches the destination
        // using at most k + 1 flights, return -1.
        return -1;
    }
};