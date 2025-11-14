#include <vector>
#include <queue>
using namespace std;

/*
 * ═══════════════════════════════════════════════════════════════════════════════
 * PROBLEM: NUMBER OF OPERATIONS TO MAKE NETWORK CONNECTED (LeetCode 1319)
 * APPROACH: BFS/DFS Solution
 * ═══════════════════════════════════════════════════════════════════════════════
 *
 * PROBLEM STATEMENT:
 * There are n computers numbered from 0 to n-1 connected by ethernet cables.
 * You can extract cables between two directly connected computers and place them
 * between any pair of disconnected computers to make them connected.
 *
 * Return the minimum number of times you need to do this in order to make all
 * the computers connected. Return -1 if it's not possible.
 *
 * ═══════════════════════════════════════════════════════════════════════════════
 * DFS/BFS APPROACH INTUITION
 * ═══════════════════════════════════════════════════════════════════════════════
 *
 * CORE STRATEGY:
 * 1. Build adjacency list representation of the network
 * 2. Count total number of cables (edges)
 * 3. Use DFS/BFS to count number of separate networks (connected components)
 * 4. Calculate:
 *    - Minimum cables needed to connect n computers: n-1
 *    - Cables currently used to maintain connectivity in each component
 *    - Extra cables = total - cables needed for current components
 *    - Operations needed = components - 1
 *
 * KEY MATHEMATICAL INSIGHTS:
 *
 * 1. MINIMUM EDGES FOR n NODES: n-1 (Spanning Tree Property)
 *    ───────────────────────────────────────────────────────────
 *    To connect n nodes into ONE connected tree, we need exactly n-1 edges.
 *
 *    Example: 4 nodes
 *      0―1―2―3        Need 3 edges (4-1 = 3)
 *
 *    Why? Each edge reduces the number of components by 1.
 *    Start: 4 isolated nodes (4 components)
 *    After 1 edge: 3 components
 *    After 2 edges: 2 components
 *    After 3 edges: 1 component (fully connected!)
 *
 * 2. IF TOTAL EDGES < n-1: IMPOSSIBLE
 *    ───────────────────────────────────────────────────────────
 *    If we have fewer than n-1 cables, we can't possibly connect all n computers.
 *
 *    Example: 5 computers, only 3 cables
 *      Need minimum: 5-1 = 4 cables
 *      Have: 3 cables
 *      Missing: 1 cable → IMPOSSIBLE, no matter how we arrange them!
 *
 * 3. FOR k COMPONENTS WITH TOTAL m EDGES:
 *    ───────────────────────────────────────────────────────────
 *
 *    a) MINIMUM EDGES NEEDED TO MAINTAIN k COMPONENTS: (n - k)
 *       ─────────────────────────────────────────────────────────
 *       Think of each component as a mini-tree:
 *
 *       Example: n = 7 nodes, k = 3 components
 *
 *       Component 1: {0,1,2}    Component 2: {3,4}    Component 3: {5,6}
 *           0                       3                     5―6
 *          / \                      |
 *         1   2                     4
 *       (3 nodes,               (2 nodes,             (2 nodes,
 *        2 edges)                1 edge)               1 edge)
 *
 *       Minimum edges for each component:
 *       - Component 1: 3 nodes → needs 3-1 = 2 edges
 *       - Component 2: 2 nodes → needs 2-1 = 1 edge
 *       - Component 3: 2 nodes → needs 2-1 = 1 edge
 *
 *       Total minimum = (3-1) + (2-1) + (2-1) = 2 + 1 + 1 = 4 edges
 *
 *       FORMULA: If component i has nᵢ nodes, needs (nᵢ - 1) edges
 *       Total minimum = Σ(nᵢ - 1) = Σnᵢ - Σ1 = n - k
 *
 *       Where:
 *       - Σnᵢ = total nodes across all components = n
 *       - Σ1 = number of components = k
 *
 *       Therefore: Minimum edges = n - k
 *
 *       In our example: 7 - 3 = 4 edges ✓ (matches our calculation!)
 *
 *    b) EXTRA (REDUNDANT) EDGES: m - (n - k)
 *       ─────────────────────────────────────────────────────────
 *       If we have MORE edges than minimum needed, the extras are redundant.
 *       These create cycles within components and can be reused!
 *
 *       Example: Same 3 components, but with 6 total edges
 *
 *       Component 1: {0,1,2}    Component 2: {3,4}    Component 3: {5,6}
 *           0―1                     3                     5―6
 *           |  |                   / \
 *           2――┘                  4――┘
 *       (3 nodes,               (2 nodes,             (2 nodes,
 *        3 edges)                2 edges)              1 edge)
 *
 *       Total edges (m) = 3 + 2 + 1 = 6
 *       Minimum needed (n-k) = 7 - 3 = 4
 *       Extra edges = 6 - 4 = 2 redundant cables
 *
 *       These 2 extra cables (marked with ┘) create cycles and can be removed
 *       and reused to connect different components!
 *
 *    c) OPERATIONS NEEDED TO CONNECT ALL: k - 1
 *       ─────────────────────────────────────────────────────────
 *       To merge k separate components into 1, we need k-1 operations.
 *       Each operation connects two components, reducing total by 1.
 *
 *       Example: 3 components → need 2 operations
 *
 *       Initial:
 *       Component 1    Component 2    Component 3
 *           0―1            3               5―6
 *           |              |
 *           2              4
 *
 *       Operation 1: Connect component 1 and 2 (use 1 extra cable)
 *       Component 1+2                   Component 3
 *       0―1       3                        5―6
 *       |         |
 *       2―――――――――4
 *       (Now have 2 components)
 *
 *       Operation 2: Connect component 1+2 and 3 (use 1 extra cable)
 *       All connected!
 *       0―1       3
 *       |         |
 *       2―――――――――4―――――――――5―6
 *       (Now have 1 component - fully connected!)
 *
 *       Total operations: 3 - 1 = 2 ✓
 *
 * PUTTING IT ALL TOGETHER:
 * ───────────────────────────────────────────────────────────
 *
 * Complete Example: n = 6 computers, connections = [[0,1],[1,2],[0,2],[3,4]]
 *
 * Step 1: Build the network
 *    0―1     3―4    5
 *    \ |
 *     \|
 *      2
 *
 * Step 2: Identify components
 *    k = 3 components: {0,1,2}, {3,4}, {5}
 *
 * Step 3: Count edges
 *    m = 4 edges total (the 4 connections given)
 *
 * Step 4: Calculate minimum edges needed for 3 components
 *    Minimum = n - k = 6 - 3 = 3 edges
 *    Component {0,1,2}: needs 2 edges (has 3, so 1 extra)
 *    Component {3,4}: needs 1 edge (has 1, so 0 extra)
 *    Component {5}: needs 0 edges (has 0, so 0 extra)
 *
 * Step 5: Calculate extra (redundant) edges
 *    Extra = m - (n-k) = 4 - 3 = 1 extra edge
 *    (The triangle 0―1―2―0 has one redundant edge)
 *
 * Step 6: Calculate operations needed
 *    Operations = k - 1 = 3 - 1 = 2 operations needed
 *    (Need to connect 3 components → need 2 bridges)
 *
 * Step 7: Check feasibility
 *    Extra cables (1) vs Operations needed (2)
 *    1 < 2 → NOT ENOUGH! Return -1
 *
 *    (If we had 5 or 6 connections instead of 4, we'd have enough extras!)
 *
 * EXAMPLE:
 * n = 4, connections = [[0,1],[0,2],[1,2]]
 *
 * Network visualization:
 *     0 — 1
 *     |   |     3 (isolated)
 *     └—— 2
 *
 * Analysis:
 * - Total cables: 3
 * - Components: 2 ({0,1,2} and {3})
 * - Minimum cables to maintain 2 components: n - components = 4-2 = 2
 * - Extra cables: 3 - 2 = 1
 * - Operations needed: components - 1 = 2-1 = 1
 * - Since extra (1) >= needed (1): Answer = 1 ✓
 *
 * ═══════════════════════════════════════════════════════════════════════════════
 * COMPARISON: DFS vs BFS vs DSU
 * ═══════════════════════════════════════════════════════════════════════════════
 *
 * DFS APPROACH:
 * ✓ Intuitive recursive traversal
 * ✓ Less code than BFS (no queue management)
 * ✓ Natural for tree-like exploration
 * ✗ Uses recursion stack space
 * ✗ Risk of stack overflow for very deep graphs
 * Time: O(n + e), Space: O(n + e)
 *
 * BFS APPROACH:
 * ✓ Iterative (no recursion stack)
 * ✓ Better for level-order exploration
 * ✓ No stack overflow risk
 * ✗ Slightly more code (queue management)
 * ✗ Extra space for queue
 * Time: O(n + e), Space: O(n + e)
 *
 * DSU APPROACH:
 * ✓ Most elegant for connectivity problems
 * ✓ Natural redundancy detection
 * ✓ No graph construction needed
 * ✓ Better space efficiency O(n)
 * ✓ Handles dynamic edge additions
 * Time: O(e × α(n)) ≈ O(e), Space: O(n)
 *
 * VERDICT:
 * - For this problem specifically: DSU is optimal
 * - For general graph traversal: DFS/BFS are more versatile
 * - For learning: Understanding all three is valuable!
 */

class Solution_DFS
{
public:
    /*
     * ═══════════════════════════════════════════════════════════════════════════
     * SOLUTION USING DEPTH-FIRST SEARCH (DFS)
     * ═══════════════════════════════════════════════════════════════════════════
     *
     * ALGORITHM STEPS:
     * 1. Early check: if edges < n-1, return -1 (impossible)
     * 2. Build adjacency list representation of graph
     * 3. Use DFS to count number of connected components
     * 4. Calculate: operations needed = components - 1
     * 5. Calculate: extra cables = total cables - minimum needed
     * 6. Return operations if we have enough extra cables, else -1
     *
     * TIME COMPLEXITY: O(n + e)
     * - Building adjacency list: O(e)
     * - DFS traversal: O(n + e)
     *   - Visit each node once: O(n)
     *   - Explore each edge once: O(e)
     * - Total: O(n + e)
     *
     * SPACE COMPLEXITY: O(n + e)
     * - Adjacency list: O(n + e)
     * - Visited array: O(n)
     * - Recursion stack: O(n) worst case (chain graph)
     * - Total: O(n + e)
     *
     * WHERE:
     * n = number of computers
     * e = number of cables (edges)
     */
    int makeConnected(int n, vector<vector<int>> &connections)
    {

        int totalCables = connections.size();

        /*
         * EARLY TERMINATION: Check if we have enough cables
         *
         * MATHEMATICAL FACT:
         * To connect n computers, we need at least n-1 cables
         * This is the minimum number of edges in a spanning tree
         *
         * If we have fewer cables, it's mathematically impossible
         * to connect all computers, no matter how we rearrange them
         */
        if (totalCables < n - 1)
        {
            return -1; // Not enough cables to connect all computers
        }

        /*
         * STEP 1: Build Adjacency List
         *
         * WHY ADJACENCY LIST?
         * - Efficient for sparse graphs (most networks are sparse)
         * - Fast neighbor access during DFS traversal: O(1)
         * - Space efficient: O(n + e) vs O(n²) for adjacency matrix
         *
         * STRUCTURE:
         * adj[i] = list of all computers directly connected to computer i
         *
         * EXAMPLE:
         * connections = [[0,1], [1,2], [0,2]]
         * adj[0] = {1, 2}
         * adj[1] = {0, 2}
         * adj[2] = {1, 0}
         */
        vector<vector<int>> adj(n);
        for (auto &conn : connections)
        {
            int u = conn[0];
            int v = conn[1];
            // Undirected graph: add edge in both directions
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        /*
         * STEP 2: Count Connected Components using DFS
         *
         * STRATEGY:
         * - Maintain a visited array to track which computers we've explored
         * - For each unvisited computer, start a DFS
         * - Each DFS call explores one complete connected component
         * - Count how many times we start a new DFS = number of components
         *
         * WHY THIS WORKS:
         * - DFS from a computer reaches all computers in its network
         * - If we need to start another DFS, it means we found a separate network
         * - Number of DFS starts = number of separate networks (components)
         */
        vector<bool> visited(n, false);
        int components = 0;

        // Try to start DFS from each computer
        for (int i = 0; i < n; i++)
        {
            if (!visited[i])
            {
                // Found an unvisited computer → new component discovered!
                components++;
                // Explore this entire component
                dfs(i, adj, visited);
            }
        }

        /*
         * STEP 3: Calculate Operations and Check Feasibility
         *
         * MATHEMATICAL REASONING:
         *
         * 1. Operations needed to connect k components:
         *    operations = k - 1
         *    (Each operation merges two components into one)
         *
         * 2. Minimum cables needed to maintain current k components:
         *    minNeeded = n - k
         *    (Each component with nodes forms a tree with at least nodes-1 edges)
         *
         * 3. Extra (redundant) cables we can reuse:
         *    extra = totalCables - minNeeded
         *    extra = totalCables - (n - components)
         *
         * 4. Feasibility check:
         *    We can perform operations if: extra >= operations
         *    i.e., (totalCables - n + components) >= (components - 1)
         *    Simplifying: totalCables >= n - 1 (which we already checked!)
         *
         * THEREFORE:
         * If we passed the early check (totalCables >= n-1), we ALWAYS have
         * enough extra cables! The answer is simply (components - 1).
         */
        int operations = components - 1;
        return operations;

        /*
         * WHY WE DON'T NEED EXTRA CHECK HERE:
         *
         * Mathematical proof:
         * - We have: totalCables >= n - 1 (from early check)
         * - Minimum cables to maintain k components: (n - k)
         * - Extra cables: totalCables - (n - k)
         * - Operations needed: k - 1
         *
         * Need to show: extra >= operations
         * i.e., totalCables - (n - k) >= k - 1
         * i.e., totalCables >= n - 1
         *
         * This is guaranteed by our early check! QED.
         */
    }

private:
    /*
     * DFS HELPER FUNCTION: Explores entire connected component
     *
     * @param node: Current computer we're exploring
     * @param adj: Adjacency list of the network
     * @param visited: Tracks which computers we've already visited
     *
     * PURPOSE:
     * Starting from 'node', recursively visit all computers that are
     * reachable through direct or indirect connections.
     *
     * RECURSION EXPLANATION:
     * - Base case: When all neighbors are visited, recursion stops
     * - Recursive step: For each unvisited neighbor, explore its component
     *
     * MARKING VISITED:
     * - Mark node as visited BEFORE exploring neighbors
     * - This prevents infinite loops in cyclic graphs
     *
     * EXAMPLE EXECUTION:
     * Network: 0-1-2, 3 (isolated)
     * DFS(0) explores: 0 → 1 → 2, marks all as visited
     * When we reach computer 3, DFS(3) starts new component
     *
     * TIME: O(V + E) for entire component
     * - Each node visited once
     * - Each edge explored twice (once from each endpoint)
     *
     * SPACE: O(V) recursion depth
     * - Worst case: chain graph 0-1-2-3-...-n
     * - Stack depth = n
     */
    void dfs(int node, vector<vector<int>> &adj, vector<bool> &visited)
    {
        // Mark current computer as visited
        visited[node] = true;

        /*
         * Explore all neighbors (directly connected computers)
         *
         * For each neighbor:
         * - If already visited: skip (avoid cycles and redundant work)
         * - If unvisited: recursively explore its component
         */
        for (int neighbor : adj[node])
        {
            if (!visited[neighbor])
            {
                // Recursively explore this neighbor's connections
                dfs(neighbor, adj, visited);
            }
        }

        /*
         * When we return from this function:
         * - 'node' and all reachable computers are marked visited
         * - We've fully explored one connected component
         */
    }
};

class Solution_BFS
{
public:
    /*
     * ═══════════════════════════════════════════════════════════════════════════
     * SOLUTION USING BREADTH-FIRST SEARCH (BFS)
     * ═══════════════════════════════════════════════════════════════════════════
     *
     * ALGORITHM: Same as DFS, but uses BFS for component exploration
     *
     * BFS CHARACTERISTICS:
     * - Iterative (uses queue instead of recursion)
     * - Explores level by level (all immediate neighbors first)
     * - Better for shortest path problems
     * - No recursion stack overflow risk
     *
     * BFS vs DFS for this problem:
     * - Both work equally well for counting components
     * - BFS: Better for very deep/long chains (no stack overflow)
     * - DFS: Slightly cleaner code (less queue management)
     * - Performance: Practically identical O(n + e)
     *
     * TIME COMPLEXITY: O(n + e)
     * SPACE COMPLEXITY: O(n + e)
     * (Same as DFS approach)
     */
    int makeConnected(int n, vector<vector<int>> &connections)
    {

        int totalCables = connections.size();

        // Early check: need at least n-1 cables
        if (totalCables < n - 1)
        {
            return -1;
        }

        // Build adjacency list
        vector<vector<int>> adj(n);
        for (auto &conn : connections)
        {
            int u = conn[0];
            int v = conn[1];
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        // Count components using BFS
        vector<bool> visited(n, false);
        int components = 0;

        for (int i = 0; i < n; i++)
        {
            if (!visited[i])
            {
                components++;
                bfs(i, adj, visited);
            }
        }

        // Return operations needed
        return components - 1;
    }

private:
    /*
     * BFS HELPER FUNCTION: Explores entire connected component iteratively
     *
     * @param start: Starting computer for BFS
     * @param adj: Adjacency list of the network
     * @param visited: Tracks which computers we've already visited
     *
     * BFS ALGORITHM:
     * 1. Start with source node in queue
     * 2. While queue not empty:
     *    a. Dequeue front node
     *    b. Process it (mark visited)
     *    c. Enqueue all unvisited neighbors
     * 3. Continue until queue is empty
     *
     * LEVEL-ORDER EXPLORATION:
     * BFS explores nodes in layers based on distance from start:
     * - Layer 0: Start node
     * - Layer 1: Direct neighbors of start
     * - Layer 2: Neighbors of layer 1 nodes
     * - And so on...
     *
     * EXAMPLE EXECUTION:
     * Network: 0-1-2-3
     *          |
     *          4
     *
     * BFS(0):
     * Queue: [0] → Process 0, add neighbors → Queue: [1, 4]
     * Queue: [1, 4] → Process 1, add 2 → Queue: [4, 2]
     * Queue: [4, 2] → Process 4 (no new) → Queue: [2]
     * Queue: [2] → Process 2, add 3 → Queue: [3]
     * Queue: [3] → Process 3 (no new) → Queue: []
     * Done! All nodes in component visited.
     *
     * COMPARISON WITH DFS:
     * DFS would go: 0 → 1 → 2 → 3, then backtrack to explore 4
     * BFS goes: 0, then {1,4}, then {2}, then {3}
     *
     * Both visit all nodes, just in different order!
     *
     * TIME: O(V + E) for component
     * SPACE: O(V) for queue (worst case: all nodes in queue)
     */
    void bfs(int start, vector<vector<int>> &adj, vector<bool> &visited)
    {
        /*
         * Queue for BFS traversal
         *
         * INVARIANT: Queue contains only unvisited nodes that we've discovered
         * but haven't processed yet (frontier of exploration)
         */
        queue<int> q;

        // Initialize: Add starting node to queue and mark as visited
        q.push(start);
        visited[start] = true;

        /*
         * BFS Main Loop:
         * Continue while there are nodes to process in the queue
         */
        while (!q.empty())
        {
            // Get next node to process (front of queue)
            int current = q.front();
            q.pop();

            /*
             * Explore all neighbors of current node
             *
             * For each neighbor:
             * - If already visited: skip
             * - If unvisited: mark visited and add to queue
             *
             * KEY DIFFERENCE FROM DFS:
             * - DFS: Immediately recurse on neighbor (depth-first)
             * - BFS: Add neighbor to queue for later (breadth-first)
             */
            for (int neighbor : adj[current])
            {
                if (!visited[neighbor])
                {
                    // Mark as visited NOW (when discovered)
                    // This prevents adding same node multiple times to queue
                    visited[neighbor] = true;

                    // Add to queue for processing
                    q.push(neighbor);
                }
            }
        }

        /*
         * When we exit the while loop:
         * - Queue is empty (no more nodes to explore)
         * - All nodes reachable from 'start' are marked visited
         * - We've fully explored one connected component
         */
    }
};

/*
 * ═══════════════════════════════════════════════════════════════════════════════
 * COMPLETE COMPLEXITY COMPARISON: DFS vs BFS vs DSU
 * ═══════════════════════════════════════════════════════════════════════════════
 *
 * METRICS COMPARISON:
 *
 * ┌─────────────────┬────────────┬────────────┬─────────────┐
 * │                 │    DFS     │    BFS     │     DSU     │
 * ├─────────────────┼────────────┼────────────┼─────────────┤
 * │ Time Complexity │  O(n + e)  │  O(n + e)  │ O(e × α(n)) │
 * │ Space (Total)   │  O(n + e)  │  O(n + e)  │    O(n)     │
 * │ Space (Graph)   │  O(n + e)  │  O(n + e)  │    None     │
 * │ Space (Aux)     │    O(n)    │    O(n)    │    O(n)     │
 * │ Code Lines      │    ~40     │    ~45     │    ~35      │
 * │ Stack Overflow? │    Yes     │     No     │     No      │
 * │ Intuitive?      │    ★★★★★   │   ★★★★☆    │   ★★★☆☆     │
 * │ Elegant?        │    ★★★☆☆   │   ★★★☆☆    │   ★★★★★     │
 * │ For This Prob.  │    Good    │    Good    │   Perfect   │
 * └─────────────────┴────────────┴────────────┴─────────────┘
 *
 * WHEN TO USE EACH:
 *
 * Use DFS when:
 * - You need to explore all paths/branches
 * - Recursion is natural for the problem
 * - Graph is not too deep
 * - Prefer simpler code over iterative approach
 *
 * Use BFS when:
 * - You need shortest path (unweighted)
 * - Want level-order traversal
 * - Graph might be very deep (avoid stack overflow)
 * - Need to find nodes at specific distance
 *
 * Use DSU when:
 * - Problem is about connectivity queries
 * - Need to detect cycles efficiently
 * - Want to avoid building full graph
 * - Space efficiency is important
 * - May have dynamic edge additions
 *
 * FOR THIS PROBLEM SPECIFICALLY:
 * - All three work correctly ✓
 * - DSU is theoretically optimal (better space, no graph needed)
 * - DFS/BFS are more intuitive if you're familiar with graph traversal
 * - Performance difference is negligible in practice
 *
 * LEARNING RECOMMENDATION:
 * Understand all three approaches! Each teaches important concepts:
 * - DFS: Recursion, backtracking
 * - BFS: Queue-based exploration, level-order processing
 * - DSU: Union-Find, amortized analysis, path compression
 */
