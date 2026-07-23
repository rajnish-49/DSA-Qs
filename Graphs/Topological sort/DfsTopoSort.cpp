#include <bits/stdc++.h>
using namespace std;

void dfs(int node,
         vector<vector<int>>& adj,
         vector<int>& visited,
         vector<int>& topoOrder) {

    // Mark the current node as visited.
    visited[node] = 1;

    // First visit all nodes that come after the current node.
    for (int neighbour : adj[node]) {
        if (!visited[neighbour]) {
            dfs(neighbour, adj, visited, topoOrder);
        }
    }

    // Add the node only after all its outgoing neighbours
    // have been completely processed.
    //
    // Therefore, nodes are stored in reverse topological order.
    topoOrder.push_back(node);
}

vector<int> topoSortDFS(int V, vector<vector<int>>& adj) {

    vector<int> visited(V, 0);
    vector<int> topoOrder;

    // The graph may have multiple disconnected components,
    // so start DFS from every unvisited node.
    for (int node = 0; node < V; node++) {
        if (!visited[node]) {
            dfs(node, adj, visited, topoOrder);
        }
    }

    // Nodes were added after their neighbours,
    // so reverse the result to obtain the topological order.
    reverse(topoOrder.begin(), topoOrder.end());

    return topoOrder;
}