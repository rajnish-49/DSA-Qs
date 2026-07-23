#include <bits/stdc++.h>
using namespace std;

vector<int> topoSortBFS(int V, vector<vector<int>>& adj) {

    // indegree[node] = number of incoming edges to this node
    // It represents how many prerequisites are still remaining.
    vector<int> indegree(V, 0);

    // Calculate indegree of every node.
    // For an edge u -> v, indegree of v increases.
    for (int u = 0; u < V; u++) {
        for (int v : adj[u]) {
            indegree[v]++;
        }
    }

    queue<int> q;

    // Nodes with indegree 0 have no prerequisites,
    // so they can be processed first.
    for (int node = 0; node < V; node++) {
        if (indegree[node] == 0) {
            q.push(node);
        }
    }

    vector<int> topoOrder;

    while (!q.empty()) {

        int node = q.front();
        q.pop();

        // This node is now placed in the topological order.
        topoOrder.push_back(node);

        // Process all nodes that depend on the current node.
        for (int neighbour : adj[node]) {

            // Since the current node has been processed,
            // one prerequisite of neighbour is completed.
            indegree[neighbour]--;

            // If no prerequisites remain,
            // neighbour is now ready to be processed.
            if (indegree[neighbour] == 0) {
                q.push(neighbour);
            }
        }
    }

    // If all nodes were not processed, the graph has a cycle,
    // so a valid topological ordering does not exist.
    if (topoOrder.size() != V) {
        return {};
    }

    return topoOrder;
}