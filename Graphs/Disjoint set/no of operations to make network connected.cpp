#include <vector>
using namespace std;

/*
 * ═══════════════════════════════════════════════════════════════════════════════
 * PROBLEM: NUMBER OF OPERATIONS TO MAKE NETWORK CONNECTED (LeetCode 1319)
 * ═══════════════════════════════════════════════════════════════════════════════
 *
 * PROBLEM STATEMENT:
 * There are n computers numbered from 0 to n-1 connected by ethernet cables
 * connections forming a network where connections[i] = [a, b] represents a
 * connection between computers a and b.
 * 
 * You can extract certain cables between two directly connected computers,
 * and place them between any pair of disconnected computers to make them connected.
 * 
 * Return the minimum number of times you need to do this in order to make all
 * the computers connected. Return -1 if it's not possible.
 *
 * INTUITION:
 * This is a graph connectivity problem with cable redistribution:
 * - Each computer is a node
 * - Each cable is an edge
 * - We need to connect all computers (make the graph fully connected)
 * - We can reuse redundant cables (cables creating cycles)
 *
 * KEY INSIGHTS:
 * 1. To connect n computers, we need at least (n-1) cables
 *    - A tree with n nodes has exactly n-1 edges
 *    - Any fewer cables means it's impossible
 * 
 * 2. If there are k disconnected components, we need (k-1) operations
 *    - Each operation connects two components
 *    - Example: 3 components need 2 operations to connect all
 * 
 * 3. Extra cables (redundant connections) can be reused
 *    - If two computers are already connected and we add another cable → redundant
 *    - These redundant cables can be moved to connect different components
 *
 * EXAMPLE 1:
 * n = 4, connections = [[0,1],[0,2],[1,2]]
 * 
 * Initial state:
 *     0 — 1
 *     |   |     3 (isolated)
 *     └—— 2
 * 
 * Analysis:
 * - Component 1: {0, 1, 2} - triangle with 3 edges (1 redundant)
 * - Component 2: {3} - isolated
 * - Extra cables: 1 (the triangle has 1 redundant edge)
 * - Components: 2
 * - Need: 2-1 = 1 operation
 * - Have: 1 extra cable
 * - Answer: 1 ✓
 *
 * EXAMPLE 2:
 * n = 6, connections = [[0,1],[0,2],[0,3],[1,2],[1,3]]
 * 
 * Initial state:
 *       0
 *      /|\
 *     1-+-2    4    5 (both isolated)
 *      \|
 *       3
 * 
 * Analysis:
 * - Component 1: {0,1,2,3} - 5 edges for 4 nodes (2 redundant)
 * - Component 2: {4} - isolated
 * - Component 3: {5} - isolated
 * - Extra cables: 2
 * - Components: 3
 * - Need: 3-1 = 2 operations
 * - Have: 2 extra cables
 * - Answer: 2 ✓
 *
 * EXAMPLE 3:
 * n = 5, connections = [[0,1],[0,2]]
 * 
 * Analysis:
 * - Only 2 cables for 5 computers
 * - Minimum needed: 5-1 = 4 cables
 * - We're short by 2 cables
 * - Answer: -1 (impossible)
 *
 * ═══════════════════════════════════════════════════════════════════════════════
 * WHY DSU IS PERFECT FOR THIS PROBLEM?
 * ═══════════════════════════════════════════════════════════════════════════════
 *
 * DSU APPROACH:
 * ✓ Detects redundant connections efficiently during union operations
 * ✓ Counts connected components naturally (count roots)
 * ✓ O(α(n)) per union/find operation (practically constant time)
 * ✓ Single pass through connections to identify extra cables
 * ✓ No need to actually reconstruct the graph
 *
 * ALTERNATIVE APPROACHES:
 * 1. DFS/BFS:
 *    - Build adjacency list, run DFS/BFS to count components
 *    - Time: O(n + e), Space: O(n + e)
 *    - Works, but need extra logic to count redundant edges
 *    - Requires graph construction
 * 
 * 2. Cycle Detection:
 *    - Detect cycles to find redundant edges
 *    - Time: O(n + e), Space: O(n)
 *    - More complex to implement
 *
 * DSU wins for:
 * - Cleaner code
 * - Natural redundancy detection
 * - No graph construction needed
 * - Efficient component counting
 */

/*
 * DISJOINT SET UNION (DSU) Data Structure
 *
 * PURPOSE: Efficiently manage computer groupings into networks
 * - Initially: each computer is its own network (n separate sets)
 * - As we add connections: merge computers into larger networks
 * - Detect redundant connections: if two computers are already connected
 * - Finally: count how many distinct networks (components) remain
 */
class DSU
{
public:
    vector<int> parent; // parent[i] = parent of computer i in the tree
    vector<int> rank;   // rank[i] = approximate height of tree rooted at computer i

    /*
     * CONSTRUCTOR: Initialize n computers as separate networks
     *
     * WHY: At the start, no cables are connected yet
     * So each computer is its own independent network
     */
    DSU(int n)
    {
        parent.resize(n);
        rank.resize(n, 0); // All trees start with height 0
        // Make each computer its own parent (n singleton sets)
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }

    /*
     * FIND OPERATION: Find which network (root) a computer belongs to
     *
     * @param x: The computer we're querying
     * @return: The representative (root) of the network containing computer x
     *
     * PATH COMPRESSION OPTIMIZATION:
     * As we traverse up to find the root, we make all computers on the path
     * point directly to the root. This flattens the structure for future queries.
     *
     * WHY THIS MATTERS:
     * Without compression: Chain of computers 0→1→2→3→4 takes O(n) to find root
     * With compression: After first find(0), all point directly to root → O(1) subsequent finds
     */
    int findParent(int x)
    {
        if (parent[x] == x)
            return x; // x is the network representative
        // Path compression: update parent to root during backtracking
        return parent[x] = findParent(parent[x]);
    }

    /*
     * UNION OPERATION: Merge the networks containing computers a and b
     *
     * @param a: Computer from first network
     * @param b: Computer from second network
     *
     * UNION BY RANK HEURISTIC:
     * Always attach the shorter tree under the taller tree to keep trees balanced.
     * This prevents the tree from becoming a long chain, which would slow down finds.
     *
     * WHY THIS MATTERS:
     * - Balanced trees → O(log n) height
     * - Combined with path compression → O(α(n)) amortized (practically O(1))
     * - Without heuristic → tree could degenerate to O(n) height chain
     */
    void unionSet(int a, int b)
    {
        // Find which networks these computers belong to
        a = findParent(a);
        b = findParent(b);

        // Already in same network? Nothing to do
        if (a != b)
        {
            // Attach smaller rank tree under larger rank tree
            if (rank[a] < rank[b])
                parent[a] = b; // Network a joins network b
            else if (rank[b] < rank[a])
                parent[b] = a; // Network b joins network a
            else
            {
                // Equal rank: attach b under a, increase a's rank
                parent[b] = a;
                rank[a]++; // Tree height increased by 1
            }
        }
    }
};


class Solution {
public:
    /*
     * ═══════════════════════════════════════════════════════════════════════════
     * SOLUTION: Minimum Operations to Connect Network using DSU
     * ═══════════════════════════════════════════════════════════════════════════
     *
     * ALGORITHM STRATEGY:
     * 1. Check if we have enough cables (need at least n-1)
     * 2. Use DSU to process connections:
     *    - If two computers already connected → mark cable as "extra"
     *    - If two computers not connected → merge them with cable
     * 3. Count how many separate networks (components) remain
     * 4. Need (components - 1) operations to connect all
     * 5. Check if we have enough extra cables to perform operations
     *
     * KEY INSIGHT:
     * - Redundant cables = cables connecting already-connected computers
     * - These can be reused to connect separate components
     * - Need exactly (components - 1) cables to connect k components
     *
     * TIME COMPLEXITY: O(e × α(n)) ≈ O(e)
     * - e = number of connections
     * - O(α(n)) per union/find operation (inverse Ackermann, practically constant)
     * - Processing connections: O(e × α(n))
     * - Counting components: O(n × α(n))
     * - Overall: O((e + n) × α(n)) ≈ O(e + n)
     *
     * SPACE COMPLEXITY: O(n)
     * - DSU uses O(n) for parent and rank arrays
     * - No additional space needed
     *
     * MATHEMATICAL FOUNDATION:
     * - Minimum edges to connect n nodes: n-1 (spanning tree property)
     * - To connect k components into 1: need k-1 edges (bridge between components)
     * - Redundant edges in component: (actual edges) - (minimum edges for spanning tree)
     */
    int makeConnected(int n, vector<vector<int>>& connections) {

        int v = connections.size(); // Total number of cables

        /*
         * EARLY TERMINATION CHECK: Not Enough Cables
         *
         * MATHEMATICAL FACT:
         * To connect n computers, we need at least (n-1) cables
         * This is because a tree with n nodes has exactly n-1 edges
         *
         * WHY THIS WORKS:
         * - If we have fewer than n-1 cables, it's mathematically impossible
         * - Even in the best case (all cables used optimally), we'd still be short
         * - No amount of rearranging can create cables we don't have
         *
         * EXAMPLE: n=5, connections.size()=3
         * - Need minimum: 5-1 = 4 cables
         * - Have: 3 cables
         * - Short by: 1 cable → IMPOSSIBLE
         */
        if (v < n - 1) {
            return -1; // Not enough cables to connect all computers
        }

        DSU ds(n); // Initialize DSU with n computers

        int extra = 0; // Count of redundant cables we can reuse

        /*
         * STEP 1: Process all connections and identify redundant cables
         *
         * STRATEGY:
         * For each connection [a, b]:
         * - Check if computers a and b are already in the same network
         * - If YES → this cable is redundant (creates a cycle), mark as "extra"
         * - If NO → use this cable to connect the networks
         *
         * WHY THIS WORKS:
         * DSU tells us if two computers are already connected (same root)
         * If they're already connected, this cable doesn't help connectivity
         * but can be reused elsewhere!
         *
         * CYCLE DETECTION:
         * When findParent(a) == findParent(b), adding edge [a,b] creates a cycle
         * This is the redundant edge we can extract and reuse
         */
        for (auto& e : connections) {
            int a = e[0]; // First computer
            int b = e[1]; // Second computer

            /*
             * Check if computers a and b are already connected
             * (i.e., in the same network component)
             */
            if (ds.findParent(a) == ds.findParent(b)) {
                // Already connected! This cable is redundant
                extra++;      
                // Don't union them - they're already in same component
                // This cable can be extracted and used elsewhere
            } else {
                // Not connected yet - use this cable to merge networks
                ds.unionSet(a, b);
                // After union, both computers are in the same network
            }
        }

        /*
         * STEP 2: Count the number of disconnected components
         *
         * KEY OBSERVATION:
         * After processing all connections, some computers may still be in
         * separate networks (components). Each component has exactly one root.
         * A computer is a root if parent[i] == i
         *
         * Therefore: number of roots = number of separate networks
         *
         * EXAMPLE:
         * Computers: 0, 1, 2, 3, 4
         * After unions: parent = [1, 1, 2, 1, 4]
         *
         * Analysis:
         * - Computer 0: parent[0] = 1 → not a root
         * - Computer 1: parent[1] = 1 → ROOT (Network 1)
         * - Computer 2: parent[2] = 2 → ROOT (Network 2)
         * - Computer 3: parent[3] = 1 → not a root
         * - Computer 4: parent[4] = 4 → ROOT (Network 3)
         *
         * Components = 3: {0,1,3}, {2}, {4}
         */
        int comp = 0;
        for (int i = 0; i < n; i++) {
            // Computer i is a network representative (root)
            if (ds.findParent(i) == i) {
                comp++; // Found a distinct network component
            }
        }

        /*
         * STEP 3: Calculate operations needed and check feasibility
         *
         * MATHEMATICAL INSIGHT:
         * To connect k separate components into 1 connected network,
         * we need exactly (k-1) cables as bridges between components.
         *
         * PROOF:
         * - Initially: k components
         * - Each operation: reduces components by 1 (merges 2 into 1)
         * - To go from k components to 1 component: need k-1 operations
         *
         * EXAMPLE:
         * Components: {0,1,2}, {3,4}, {5}  (3 components)
         * Operation 1: Connect component 1 and 2 → 2 components remain
         * Operation 2: Connect those 2 → 1 component (fully connected!)
         * Total: 3-1 = 2 operations
         */
        int needed = comp - 1; // Operations needed to connect all components

        /*
         * FEASIBILITY CHECK:
         * We can perform the operations if we have enough redundant cables
         * 
         * - extra: number of redundant cables we can reuse
         * - needed: number of operations required
         * 
         * If extra >= needed: SUCCESS! We have enough cables to connect all
         * If extra < needed: FAILURE! We don't have enough redundant cables
         *
         * NOTE: The early check (v < n-1) ensures we have enough total cables,
         * but we still need to verify we have enough extra cables to redistribute
         */
        if (extra >= needed) return needed; // Can connect all with 'needed' operations
        return -1; // Not enough extra cables to redistribute

        /*
         * ═══════════════════════════════════════════════════════════════════════
         * COMPLEXITY ANALYSIS SUMMARY
         * ═══════════════════════════════════════════════════════════════════════
         *
         * TIME COMPLEXITY: O(e × α(n)) ≈ O(e)
         * - Early check: O(1)
         * - Processing e connections: O(e × α(n))
         *   - Each connection: 2 findParent calls + possibly 1 union
         *   - Each operation: O(α(n)) amortized
         * - Counting components: O(n × α(n))
         * - Total: O((e + n) × α(n))
         * - Since α(n) ≤ 4 for practical values: effectively O(e + n)
         *
         * SPACE COMPLEXITY: O(n)
         * - DSU parent array: O(n)
         * - DSU rank array: O(n)
         * - Few integer variables: O(1)
         * - Total: O(n)
         *
         * WHERE α(n) IS THE INVERSE ACKERMANN FUNCTION:
         * - Extremely slow-growing function
         * - α(n) ≤ 4 for all practical values of n
         * - Even for n = number of atoms in universe, α(n) < 5
         * - Therefore, treated as O(1) in practice
         *
         * ═══════════════════════════════════════════════════════════════════════
         * COMPARISON WITH DFS/BFS APPROACH
         * ═══════════════════════════════════════════════════════════════════════
         *
         * DFS/BFS Approach:
         * 1. Build adjacency list: O(e)
         * 2. Count components with DFS/BFS: O(n + e)
         * 3. Count redundant edges: need additional logic
         * - Time: O(n + e)
         * - Space: O(n + e) for adjacency list + visited array
         * - Code: More complex to track redundant edges
         *
         * DSU Approach (Current):
         * - Time: O(e × α(n)) ≈ O(e)
         * - Space: O(n) - more space efficient
         * - Code: Cleaner, naturally detects redundancy
         * - Bonus: Can handle dynamic edge additions/queries efficiently
         *
         * KEY ADVANTAGES OF DSU:
         * 1. No graph construction needed (space efficient)
         * 2. Natural redundancy detection (when union fails)
         * 3. Extremely fast in practice due to path compression
         * 4. Code is more concise and easier to understand
         * 5. Extensible for dynamic connectivity queries
         *
         * VERDICT: DSU is the optimal choice for this problem!
         */
    }
};
