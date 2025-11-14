// @ https://leetcode.com/problems/find-eventual-safe-states/description/

class Solution {
    public:
        vector<int> eventualSafeNodes(vector<vector<int>>& graph) {
            int V = graph.size();  // Number of nodes in the graph
    
            // Reverse adjacency list to store the reversed graph
            vector<vector<int>> adjrev(V);
    
            // In-degree array to count incoming edges for each node
            vector<int> indeg(V, 0);
    
            // Step 1: Reverse the graph and compute in-degrees
            for (int i = 0; i < V; i++) {
                // Original graph: i → it
                // Reversed graph: it → i
    
                for (auto it : graph[i]) {
                    adjrev[it].push_back(i);  // Reverse the edge (i → it) to (it → i)
                    indeg[i]++;  // Increase in-degree of node i
                }
            }
    
            queue<int> q;  // Queue for processing nodes
            set<int> ans;  // Set to store safe nodes (stores in sorted order automatically)
    
            // Step 2: Add terminal nodes (in-degree 0) to the queue
            for (int i = 0; i < V; i++) {
                if (indeg[i] == 0) {  // Terminal nodes have no outgoing edges
                    q.push(i);
                }
            }
    
            // Step 3: Process nodes in topological order
            while (!q.empty()) {
                int node = q.front();  // Get the front node from the queue
                q.pop();
                ans.insert(node);  // Mark node as safe
    
                // Reduce in-degree for all nodes pointing to this node
                for (auto it : adjrev[node]) {
                    indeg[it]--;  // Decrease in-degree
                    if (indeg[it] == 0) {  // If in-degree becomes 0, it's safe
                        q.push(it);
                    }
                }
            }
    
            // Step 4: Convert the set of safe nodes into a sorted vector and return
            return vector<int>(ans.begin(), ans.end());
        }
    };
    