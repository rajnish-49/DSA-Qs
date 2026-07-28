// @https://leetcode.com/problems/find-the-city-with-the-smallest-number-of-neighbors-at-a-threshold-distance/description/

/*
 * PROBLEM CORE CONCEPT:
 * ====================
 * We have a directed weighted graph where we can use a special "switch" at each node.
 * 
 * THE SWITCH MECHANIC:
 * When we ARRIVE at node u, we can activate its switch ONCE to:
 *   1. Pick ONE of its INCOMING edges (some edge v → u)
 *   2. REVERSE it temporarily (making it u → v)
 *   3. Immediately traverse it at DOUBLE the original cost (2 * weight)
 * 
 * CONSTRAINT: Each node's switch can only be used ONCE.
 * 
 * BRILLIANT INSIGHT - Why Simple Dijkstra Works:
 * =============================================
 * Instead of tracking complex state (which node's switch we've used), we can simply
 * add REVERSED edges to our graph with doubled cost!
 * 
 * WHY THIS DOESN'T VIOLATE THE "ONE SWITCH PER NODE" CONSTRAINT:
 * 
 * Key Observation: Shortest paths with POSITIVE weights have NO LOOPS.
 * Proof: If a path has a loop, removing it gives a shorter path (since loop cost > 0)
 * 
 * Implication:
 *   - Shortest path visits each node AT MOST ONCE
 *   - If we visit node v only once, we can only traverse ONE outgoing edge from v
 *   - Reversed edges FROM v represent using v's switch
 *   - Therefore: We automatically use v's switch at most once!
 * 
 * GRAPH TRANSFORMATION:
 * For original edge u → v with cost w, we add:
 *   1. Forward edge: u → v (cost w)     - normal traversal
 *   2. Reverse edge: v → u (cost 2*w)   - using node v's switch to reverse incoming edge
 * 
 * EXAMPLE:
 * Original: 0 → 1 (cost 3), 3 → 1 (cost 1)
 * Augmented: 0 → 1 (cost 3), 1 → 0 (cost 6), 3 → 1 (cost 1), 1 → 3 (cost 2)
 * 
 * Path 0 → 1 → 3 means:
 *   - Use forward edge 0 → 1 (cost 3)
 *   - Use node 1's switch to reverse 3 → 1 into 1 → 3 (cost 2)
 *   - Total: 5
 */

class Solution {
public:
    typedef pair<int, int> P; // {distance, node}

    int minCost(int n, vector<vector<int>>& edges) {
        // ADJACENCY LIST: adj[node] = [{neighbor1, weight1}, {neighbor2, weight2}, ...]
        unordered_map<int, vector<P>> adj;

        // GRAPH CONSTRUCTION: Building the augmented graph
        // For each directed edge u → v, we create TWO edges:
        for(auto &edge : edges) {
            int u  = edge[0];  // source node
            int v  = edge[1];  // destination node
            int wt = edge[2];  // edge weight

            // FORWARD EDGE: Normal traversal from u to v
            // This represents using the edge as-is in the original graph
            adj[u].push_back({v, wt});
            
            // REVERSE EDGE: This is the KEY INSIGHT!
            // Adding edge v → u with cost 2*wt represents:
            // "When at node v, use its switch to reverse the incoming edge u → v,
            //  turning it into v → u, and traverse it at double cost"
            // 
            // Why this works: Since shortest paths have no loops (positive weights),
            // we visit each node once, so we can only use one reversed edge per node.
            adj[v].push_back({u, 2*wt});
        }

        // DIJKSTRA'S ALGORITHM SETUP
        // result[i] = minimum cost to reach node i from node 0
        vector<int> result(n, INT_MAX);
        result[0] = 0;  // Starting node has 0 cost
        
        // MIN-HEAP: priority queue stores {distance, node}
        // We always process the node with minimum distance first (greedy choice)
        priority_queue<P, vector<P>, greater<P>> pq;
        pq.push({0, 0}); // {distance = 0, node = 0}

        // DIJKSTRA'S MAIN LOOP
        while(!pq.empty()) {
            // GREEDY CHOICE: Always process the unvisited node with smallest distance
            int d    = pq.top().first;   // current best distance to reach this node
		    int node = pq.top().second;  // the node itself
		    pq.pop();
            
            // EARLY TERMINATION: Once we reach the destination with minimum cost, we're done
            // This works because Dijkstra guarantees the first time we pop a node,
            // we've found its shortest path (with non-negative weights)
            if(node == n-1)
                return result[node];

            // OPTIMIZATION CHECK: If we've already found a better path to this node, skip it
            // This handles duplicate entries in the priority queue
            // (We might add the same node multiple times with different distances)
            if(d > result[node]) continue;

            // EDGE RELAXATION: Try to improve distances to neighbors
		    for(auto &p : adj[node]) {
                int adjNode = p.first;   // neighbor node
                int dist    = p.second;  // edge weight to neighbor
                
                // RELAXATION STEP: Can we reach adjNode cheaper through current node?
                // Current path cost: d (cost to reach current node) + dist (edge weight)
                if(d + dist < result[adjNode]) {
                    // We found a better path! Update the distance
                    result[adjNode] = d + dist;
                    
                    // Add this improved state to priority queue for future processing
                    // Note: We might add the same node multiple times with different distances
                    // The optimization check above handles this
                    pq.push({d + dist, adjNode});
                }
		    }
        }

        // If we exit the loop without reaching node n-1, no path exists
        return -1;
    }
};

/*
 * TIME COMPLEXITY: O((V + E) log V)
 * - V = number of nodes (n)
 * - E = number of edges (original edges * 2, since we add reversed edges)
 * - Each node can be added to PQ multiple times, but processed once optimally
 * - PQ operations (push/pop) take O(log V)
 * 
 * SPACE COMPLEXITY: O(V + E)
 * - Adjacency list stores all edges (original + reversed)
 * - Distance array: O(V)
 * - Priority queue: O(V) in worst case
 * 
 * WHY THIS SOLUTION IS ELEGANT:
 * =============================
 * Instead of tracking complex state like "which switches have been used",
 * we transform the problem into a standard shortest path problem by:
 *   1. Adding reversed edges with doubled cost
 *   2. Relying on the property that shortest paths don't have loops
 *   3. This automatically ensures each node's switch is used at most once
 * 
 * This is a beautiful example of problem transformation - converting a constrained
 * problem into a standard one by clever modeling!
 */