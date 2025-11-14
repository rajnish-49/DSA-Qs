#include <vector>
using namespace std;

/*
 * ═══════════════════════════════════════════════════════════════════════════════
 * PROBLEM: NUMBER OF PROVINCES (LeetCode 547)
 * ═══════════════════════════════════════════════════════════════════════════════
 *
 * PROBLEM STATEMENT:
 * There are n cities. A province is a group of directly or indirectly connected cities.
 * Given an n x n matrix isConnected where isConnected[i][j] = 1 if city i and city j
 * are directly connected, and isConnected[i][j] = 0 otherwise.
 * Return the total number of provinces.
 *
 * INTUITION:
 * This is essentially finding the number of connected components in an undirected graph.
 * - Each city is a node
 * - Direct connections are edges
 * - A province is a connected component (group of cities that can reach each other)
 *
 * EXAMPLE:
 * isConnected = [[1,1,0],
 *                [1,1,0],
 *                [0,0,1]]
 *
 * Visualization:
 *   City 0 — City 1        City 2
 *   (Province 1)           (Province 2)
 *
 * Answer: 2 provinces
 *
 * ═══════════════════════════════════════════════════════════════════════════════
 * WHY DSU IS PERFECT FOR THIS PROBLEM?
 * ═══════════════════════════════════════════════════════════════════════════════
 *
 * DSU APPROACH:
 * ✓ Efficiently merges cities into provinces as we discover connections
 * ✓ O(α(n)) per union/find operation (practically constant time)
 * ✓ Natural fit: each province is represented by a disjoint set
 * ✓ Simple to count: number of distinct roots = number of provinces
 *
 * ALTERNATIVE APPROACHES:
 * 1. DFS/BFS:
 *    - Run DFS/BFS from each unvisited city, count components
 *    - Time: O(n²), Space: O(n)
 *    - Works well, but DSU is more elegant for connectivity queries
 *
 * 2. Floyd-Warshall (Transitive Closure):
 *    - Find all indirectly connected pairs
 *    - Time: O(n³), Space: O(n²)
 *    - Overkill for this problem
 *
 * DSU wins in simplicity and efficiency for dynamic connectivity!
 */

/*
 * DISJOINT SET UNION (DSU) Data Structure
 *
 * PURPOSE: Efficiently manage city groupings into provinces
 * - Initially: each city is its own province (n separate sets)
 * - As we find connections: merge cities into larger provinces
 * - Finally: count how many distinct provinces (roots) remain
 */
class DSU
{
public:
    vector<int> parent; // parent[i] = parent of city i in the tree
    vector<int> rank;   // rank[i] = approximate height of tree rooted at city i

    /*
     * CONSTRUCTOR: Initialize n cities as separate provinces
     *
     * WHY: At the start, we don't know any connections yet
     * So each city is its own independent province
     */
    DSU(int n)
    {
        parent.resize(n);
        rank.resize(n, 0); // All trees start with height 0
        // Make each city its own parent (n singleton sets)
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }

    /*
     * FIND OPERATION: Find which province (root) a city belongs to
     *
     * @param x: The city we're querying
     * @return: The representative (root) of the province containing city x
     *
     * PATH COMPRESSION OPTIMIZATION:
     * As we traverse up to find the root, we make all cities on the path
     * point directly to the root. This flattens the structure for future queries.
     *
     * WHY THIS MATTERS:
     * Without compression: Chain of cities 0→1→2→3→4 takes O(n) to find root
     * With compression: After first find(0), all point directly to root → O(1) subsequent finds
     */
    int findParent(int x)
    {
        if (parent[x] == x)
            return x; // x is the province representative
        // Path compression: update parent to root during backtracking
        return parent[x] = findParent(parent[x]);
    }

    /*
     * UNION OPERATION: Merge the provinces containing cities a and b
     *
     * @param a: City from first province
     * @param b: City from second province
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
        // Find which provinces these cities belong to
        a = findParent(a);
        b = findParent(b);

        // Already in same province? Nothing to do
        if (a != b)
        {
            // Attach smaller rank tree under larger rank tree
            if (rank[a] < rank[b])
                parent[a] = b; // Province a joins province b
            else if (rank[b] < rank[a])
                parent[b] = a; // Province b joins province a
            else
            {
                // Equal rank: attach b under a, increase a's rank
                parent[b] = a;
                rank[a]++; // Tree height increased by 1
            }
        }
    }
};

class Solution
{
public:
    /*
     * ═══════════════════════════════════════════════════════════════════════════
     * SOLUTION: Count Number of Provinces using DSU
     * ═══════════════════════════════════════════════════════════════════════════
     *
     * ALGORITHM STRATEGY:
     * 1. Initialize DSU with n cities (each city starts as its own province)
     * 2. Process adjacency matrix: merge cities that are directly connected
     * 3. Count distinct provinces: count how many cities are their own parent (roots)
     *
     * KEY INSIGHT:
     * After processing all connections, the number of roots = number of provinces
     * Why? Each connected component has exactly one root (the province representative)
     *
     * TIME COMPLEXITY: O(n² × α(n)) ≈ O(n²)
     * - O(n²) to iterate through adjacency matrix
     * - O(α(n)) per union/find operation (inverse Ackermann, practically constant)
     * - Overall: O(n²) dominates
     *
     * SPACE COMPLEXITY: O(n)
     * - DSU uses O(n) for parent and rank arrays
     * - No additional space needed
     *
     * ADJACENCY MATRIX PROPERTIES:
     * - isConnected[i][j] == 1 means cities i and j are directly connected
     * - Symmetric: isConnected[i][j] == isConnected[j][i]
     * - Reflexive: isConnected[i][i] == 1 (city is connected to itself)
     */
    int findCircleNum(vector<vector<int>> &isConnected)
    {

        int V = isConnected.size(); // Number of cities
        DSU ds(V);                  // Initialize DSU with V cities

        /*
         * STEP 1: Process all connections and merge provinces
         *
         * STRATEGY: Iterate through adjacency matrix
         * - When isConnected[i][j] == 1, cities i and j are directly connected
         * - Merge them into the same province using union
         *
         * OPTIMIZATION NOTE:
         * Since the matrix is symmetric, we could optimize by checking only i < j
         * (upper triangle), but current approach is clearer and still O(n²)
         *
         * WHY UNION EVERYTHING FIRST?
         * DSU automatically handles transitive connections!
         * If city A connects to B, and B connects to C, DSU ensures A, B, C
         * all end up in the same province, even though A and C aren't directly connected.
         */
        for (int i = 0; i < V; i++)
        {
            for (int j = 0; j < V; j++)
            {
                if (isConnected[i][j] == 1)
                {
                    // Cities i and j are connected → merge their provinces
                    ds.unionSet(i, j);

                    // Note: unionSet(i, i) is harmless (same province check prevents issues)
                    // unionSet(i, j) and unionSet(j, i) both do the same thing
                }
            }
        }

        /*
         * STEP 2: Count the number of distinct provinces
         *
         * KEY OBSERVATION:
         * After all unions, each province has exactly one root city (representative)
         * A city is a root if parent[i] == i
         *
         * Therefore: number of roots = number of provinces
         *
         * EXAMPLE:
         * Cities: 0, 1, 2, 3, 4
         * After unions: parent = [1, 1, 2, 1, 2]
         *
         * Analysis:
         * - City 0: parent[0] = 1 → not a root
         * - City 1: parent[1] = 1 → ROOT (Province 1)
         * - City 2: parent[2] = 2 → ROOT (Province 2)
         * - City 3: parent[3] = 1 → not a root
         * - City 4: parent[4] = 2 → not a root
         *
         * Count = 2 provinces: {0,1,3} and {2,4}
         */
        int cnt = 0;
        for (int i = 0; i < V; i++)
        {
            // City i is a province representative (root)
            if (ds.findParent(i) == i)
            {
                cnt++; // Found a distinct province
            }
        }

        return cnt; // Total number of provinces

        /*
         * ═══════════════════════════════════════════════════════════════════════
         * COMPLEXITY ANALYSIS SUMMARY
         * ═══════════════════════════════════════════════════════════════════════
         *
         * TIME COMPLEXITY: O(n²)
         * - Matrix iteration: O(n²)
         * - Union operations: O(n² × α(n)) ≈ O(n²) since α(n) ≈ 4 for practical n
         * - Counting roots: O(n × α(n)) ≈ O(n)
         * - Total: O(n²)
         *
         * SPACE COMPLEXITY: O(n)
         * - DSU parent array: O(n)
         * - DSU rank array: O(n)
         * - Total: O(n)
         *
         * ═══════════════════════════════════════════════════════════════════════
         * COMPARISON WITH DFS/BFS APPROACH
         * ═══════════════════════════════════════════════════════════════════════
         *
         * DFS/BFS Approach:
         * - Time: O(n²) - same as DSU
         * - Space: O(n) - visited array + recursion stack
         * - Code: Need to implement graph traversal
         *
         * DSU Approach (Current):
         * - Time: O(n²) - same as DFS/BFS
         * - Space: O(n) - parent and rank arrays
         * - Code: Cleaner, more reusable DSU component
         * - Bonus: Can handle dynamic edge additions efficiently
         *
         * VERDICT: Both are equally efficient. DSU is more elegant and extensible!
         */
    }
};