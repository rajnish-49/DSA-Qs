// @https://leetcode.com/problems/evaluate-division/

/*
PROBLEM: Evaluate Division
Given equations like a/b = 2.0, b/c = 3.0, answer queries like a/c = ?

KEY INSIGHT: Model as a WEIGHTED DIRECTED GRAPH
- Each variable is a node
- Each equation creates TWO edges:
  * a → b with weight 2.0 (a/b = 2.0)
  * b → a with weight 0.5 (b/a = 1/2.0)
- Query a/c = ? becomes: find path from a to c, multiply all edge weights

APPROACHES:
1. DFS (Recursive): Natural for path accumulation with product parameter
2. BFS (Iterative): Queue-based level-by-level exploration with product tracking

TIME COMPLEXITY: O(Q * (V + E)) where Q = queries, V = variables, E = equations
SPACE COMPLEXITY: O(V + E) for graph storage
*/

class Solution
{
public:
    // ==================== DFS APPROACH ====================
    // Depth-First Search helper to explore all possible conversion paths
    // between `src` and `dst`. The parameter `product` accumulates the
    // multiplicative value of the current path, and `ans` stores the
    // final answer once the destination is reached.
    void dfs(unordered_map<string, vector<pair<string, double>>> &adj,
             string src, string dst, unordered_set<string> &visited,
             double product, double &ans)
    {

        // If we've already visited this node, return immediately to prevent
        // infinite loops in graphs with cycles (e.g., a -> b -> a).
        if (visited.find(src) != visited.end())
            return;

        // Mark the current node as visited before exploring further.
        visited.insert(src);

        // Base case: if we've reached the destination node,
        // the current accumulated product represents the full ratio.
        if (src == dst)
        {
            ans = product; // record the result
            return;        // stop exploring further
        }

        // Explore all neighbors (connected variables) of the current node.
        // Each neighbor represents a valid conversion: src / neighbor = val.
        for (auto p : adj[src])
        {
            string v = p.first;    // neighboring variable
            double val = p.second; // conversion factor (src / v = val)

            // Recur deeper: multiply the current product by this conversion factor.
            // The recursive chain simulates multiplying ratios along the path.
            dfs(adj, v, dst, visited, product * val, ans);
        }
    }

    // ==================== BFS APPROACH ====================
    // Breadth-First Search to find path from src to dst iteratively
    // Returns the product of all edge weights along the path, or -1.0 if no path exists
    double bfs(unordered_map<string, vector<pair<string, double>>> &adj,
               string src, string dst)
    {

        // 🔍 Edge case: If source doesn't exist in graph, no path possible
        if (adj.find(src) == adj.end())
            return -1.0;

        // ✅ Edge case: If src == dst, the ratio is 1.0 (a/a = 1)
        if (src == dst)
            return 1.0;

        // Queue stores: {current_node, accumulated_product_so_far}
        // We start from src with product = 1.0 (identity for multiplication)
        queue<pair<string, double>> q;
        unordered_set<string> visited;

        q.push({src, 1.0});
        visited.insert(src);

        // 🔄 BFS EXPLORATION: Process nodes level by level
        while (!q.empty())
        {
            auto [curr, product] = q.front();
            q.pop();

            // 🎯 BASE CASE: Reached destination
            if (curr == dst)
            {
                return product; // This is our answer!
            }

            // 🧭 EXPLORE NEIGHBORS: Try all possible conversions from current node
            for (auto &[neighbor, weight] : adj[curr])
            {

                // Skip already visited nodes to avoid cycles
                if (visited.find(neighbor) != visited.end())
                    continue;

                // 🔑 KEY OPERATION: Multiply accumulated product by edge weight
                // This simulates: (a/b) * (b/c) = (a/c)
                double newProduct = product * weight;

                // Add neighbor to queue with updated product
                q.push({neighbor, newProduct});
                visited.insert(neighbor);

                // 💡 INTUITION:
                // If we're at node 'a' with product P, and there's edge a→b with weight W,
                // then reaching 'b' gives us accumulated product P*W
                // This chains ratios: src/a * a/b * b/dst = src/dst
            }
        }

        // ❌ No path found from src to dst
        return -1.0;
    }

    vector<double> calcEquation(vector<vector<string>> &equations,
                                vector<double> &values,
                                vector<vector<string>> &queries)
    {

        int n = equations.size();

        // -----------------------------
        // 1. Build a bidirectional graph
        // -----------------------------
        // Each equation "u / v = val" defines two directed edges:
        //   u → v (weight = val)
        //   v → u (weight = 1/val)
        // This ensures we can traverse the graph in either direction.
        unordered_map<string, vector<pair<string, double>>> adj;

        for (int i = 0; i < n; i++)
        {
            string u = equations[i][0];
            string v = equations[i][1];
            double val = values[i];

            adj[u].push_back({v, val});       // forward relation
            adj[v].push_back({u, 1.0 / val}); // reverse relation
        }

        // -----------------------------------------
        // 2. Resolve each query using graph traversal
        // -----------------------------------------
        vector<double> result;

        for (auto &query : queries)
        {

            string src = query[0];
            string dst = query[1];

            // 🔀 CHOOSE YOUR APPROACH:

            // Option 1: DFS (Recursive)
            /*
            double ans = -1.0; // default to -1.0 (no valid path)
            double product = 1.0; // neutral element for multiplication

            if (adj.find(src) != adj.end()) {
                unordered_set<string> visited;
                dfs(adj, src, dst, visited, product, ans);
            }
            result.push_back(ans);
            */

            // Option 2: BFS (Iterative) - Currently active
            double ans = bfs(adj, src, dst);
            result.push_back(ans);
        }

        // Return all computed results in the same order as queries.
        return result;
    }
};

/*
BFS vs DFS COMPARISON:

EXAMPLE: equations = [["a","b"],["b","c"]], values = [2.0, 3.0], query = ["a","c"]

Graph: a →(2.0)→ b →(3.0)→ c

DFS TRAVERSAL:
- Start at 'a', product = 1.0
- Visit 'b', product = 1.0 * 2.0 = 2.0 (recursion)
- Visit 'c', product = 2.0 * 3.0 = 6.0 (recursion)
- Found dst, return 6.0

BFS TRAVERSAL:
- Queue: [(a, 1.0)], visited: {a}
- Dequeue (a, 1.0), enqueue (b, 2.0), visited: {a, b}
- Dequeue (b, 2.0), enqueue (c, 6.0), visited: {a, b, c}
- Dequeue (c, 6.0), c == dst, return 6.0

WHEN TO USE WHICH:

DFS:
✅ Cleaner code with recursion
✅ Natural parameter passing for product
✅ Less memory (just recursion stack)
❌ Harder to debug (implicit stack)

BFS:
✅ Explicit state tracking (queue makes it visible)
✅ Finds shortest path (though any path works here)
✅ Easier to convert to iterative deepening
❌ More verbose with queue management

BOTH are correct and have same time complexity O(V+E) per query!
*/
