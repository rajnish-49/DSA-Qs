#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class Solution {
public:
    // Function to return list containing vertices in Topological order.
    vector<int> topologicalSort(vector<vector<int>>& adj) {
        int V = adj.size();  // Number of vertices in the graph

        vector<int> indeg(V, 0);  // Array to store in-degrees of all vertices
        vector<int> ans;          // Stores the final topological order

        // Step 1: Compute in-degree of each vertex
        for (int i = 0; i < V; i++) {
            for (auto it : adj[i]) { // Iterating over all adjacent nodes of 'i'
                indeg[it]++; // Increment in-degree of node 'it' (destination node)
            }
        }

        queue<int> q; // Queue to process nodes with in-degree 0

        // Step 2: Add all nodes with in-degree 0 to the queue
        for (int i = 0; i < V; i++) {
            if (indeg[i] == 0) { // Nodes with no incoming edges
                q.push(i);
            }
        }

        // Step 3: Process the queue using BFS
        while (!q.empty()) {
            int node = q.front(); // Get front element of the queue
            q.pop();
            ans.push_back(node); // Store the node in the result

            // Step 4: Reduce the in-degree of adjacent nodes
            for (auto it : adj[node]) {
                indeg[it]--; // Remove the incoming edge

                if (indeg[it] == 0) { // If in-degree becomes 0, add to queue
                    q.push(it);
                }
            }
        }

        return ans; // Return the topological order
    }
};