class Solution {
public:
    double maxProbability(
        int n,
        vector<vector<int>>& edges,
        vector<double>& succProb,
        int start,
        int end
    ) {
        // adj[u] stores all nodes connected to u.
        //
        // Each pair stores:
        // {neighbour, probability of the edge u <-> neighbour}
        //
        // The graph is undirected, so every edge is added in both directions.
        vector<vector<pair<int, double>>> adj(n);

        for (int i = 0; i < edges.size(); i++) {
            int u = edges[i][0];
            int v = edges[i][1];
            double edgeProbability = succProb[i];

            adj[u].push_back({v, edgeProbability});
            adj[v].push_back({u, edgeProbability});
        }

        // bestProbability[node] stores the highest probability
        // currently known for reaching 'node' from 'start'.
        //
        // Initially, no node is reachable, so every value is 0.
        vector<double> bestProbability(n, 0.0);

        // We are already present at the starting node.
        // Therefore, the probability of reaching start is 1.0.
        bestProbability[start] = 1.0;

        // Max-heap storing:
        // {probability of reaching the node, node}
        //
        // We use a max-heap because Dijkstra should process
        // the node with the highest probability first.
        priority_queue<pair<double, int>> pq;

        pq.push({1.0, start});

        while (!pq.empty()) {
            // Extract the node having the highest probability
            // among all nodes currently present in the heap.
            double probabilityToCurrent = pq.top().first;
            int currentNode = pq.top().second;
            pq.pop();

            // The same node may be inserted multiple times.
            //
            // Example:
            // First, we may reach a node with probability 0.4.
            // Later, we may find a better route with probability 0.7.
            //
            // Both entries remain in the heap.
            // If the old 0.4 entry is popped later, skip it.
            if (probabilityToCurrent < bestProbability[currentNode]) {
                continue;
            }

            // Since the max-heap processes the highest probability first,
            // the first time we pop the destination, we have found
            // the maximum possible probability of reaching it.
            if (currentNode == end) {
                return probabilityToCurrent;
            }

            // Explore every neighbour of the current node.
            for (auto& edge : adj[currentNode]) {
                int neighbour = edge.first;
                double edgeProbability = edge.second;

                // To reach the neighbour through currentNode,
                // we must:
                //
                // 1. successfully reach currentNode
                // 2. successfully travel across the current edge
                //
                // Therefore, the probabilities are multiplied.
                double probabilityThroughCurrent =
                    probabilityToCurrent * edgeProbability;

                // Relaxation:
                //
                // If this route reaches the neighbour with a higher
                // probability than any route found before, update it.
                if (probabilityThroughCurrent >
                    bestProbability[neighbour]) {

                    bestProbability[neighbour] =
                        probabilityThroughCurrent;

                    // Push the improved probability into the heap
                    // so this neighbour can be processed later.
                    pq.push({
                        probabilityThroughCurrent,
                        neighbour
                    });
                }
            }
        }

        // If the destination was never reached,
        // the maximum success probability is 0.
        return 0.0;
    }
};