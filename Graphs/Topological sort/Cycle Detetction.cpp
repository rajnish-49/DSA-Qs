class Solution {
    public:
      // Function to detect a cycle in a directed graph using Kahn's Algorithm (Topological Sorting via BFS).
      bool isCyclic(vector<vector<int>> &adj) {
          int V = adj.size();  // Number of vertices in the graph.
  
          vector<int> indeg(V, 0);  // Array to store the in-degree (number of incoming edges) of each vertex.
          vector<int> ans;  // Vector to store the topological order of nodes.
  
          // Step 1: Compute the in-degree for each vertex.
          for (int i = 0; i < V; i++) {
              // Traverse all adjacent nodes of vertex 'i'
              for (auto it : adj[i]) {  
                  indeg[it]++;  // Increment in-degree of the adjacent node 'it'
              }
          }
  
          queue<int> q; // Queue to store nodes with in-degree 0 (nodes with no dependencies).
  
          // Step 2: Add all nodes with in-degree 0 to the queue.
          for (int i = 0; i < V; i++) {
              if (indeg[i] == 0) {  // If a node has no incoming edges, add it to the queue.
                  q.push(i);
              }
          }
  
          // Step 3: Process nodes in topological order using BFS.
          while (!q.empty()) {
              int node = q.front();  // Get the front element of the queue.
              q.pop();  // Remove it from the queue.
              ans.push_back(node);  // Store the node in the topological order list.
  
              // Step 4: Reduce the in-degree of all adjacent nodes.
              for (auto it : adj[node]) {
                  indeg[it]--;  // Decrease the in-degree as we're "removing" an edge.
  
                  if (indeg[it] == 0) {  // If in-degree becomes 0, add it to the queue.
                      q.push(it);
                  }
              }
          }
          
          // Step 5: Check if a cycle exists.
          // If the number of nodes in the topological order is equal to the total nodes, no cycle exists.
          if (ans.size() == V) {
              return false;  // No cycle found.
          } else {
              return true;   // Cycle detected.
          }
      }
  };
  