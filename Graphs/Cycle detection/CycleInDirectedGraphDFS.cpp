#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool dfs(int node,
             vector<vector<int>>& adj,
             vector<bool>& visited,
             vector<bool>& pathVisited) {

        /*
        visited[node] means:
        this node has been explored at some point in the whole DFS traversal.

        pathVisited[node] means:
        this node is currently present in the active recursion path.

        Example active path:
        0 -> 1 -> 2 -> 3

        Here, pathVisited is true for 0, 1, 2 and 3.
        */
        visited[node] = true;
        pathVisited[node] = true;

        for (int neighbour : adj[node]) {

            if (visited[neighbour] == false) {

                /*
                The neighbour has never been explored before,
                so continue the current DFS path through it.

                If that deeper DFS finds a cycle, we immediately
                return true because one cycle anywhere is enough.
                */
                if (dfs(neighbour, adj, visited, pathVisited)) {
                    return true;
                }
            }

            /*
            The neighbour has already been visited.

            But visited alone does not prove a cycle.

            A cycle exists only when the neighbour is still present
            in the current recursion path.

            Example:
            current path = 0 -> 1 -> 2 -> 3
            and there is an edge 3 -> 1

            Node 1 is already in the current path, so:
            1 -> 2 -> 3 -> 1 forms a cycle.
            */
            else if (pathVisited[neighbour] == true) {
                return true;
            }
        }

        /*
        We are now returning from this node.

        So this node must be removed from the current DFS path.

        We do NOT reset visited[node], because the node has already
        been completely explored and should not be explored again.

        Therefore:

        visited[node] remains true
        pathVisited[node] becomes false
        */
        pathVisited[node] = false;

        return false;
    }

    bool isCyclic(int V, vector<vector<int>>& adj) {

        /*
        visited:
        tells whether a node has ever been explored.

        pathVisited:
        tells whether a node is currently inside the active DFS chain.
        */
        vector<bool> visited(V, false);
        vector<bool> pathVisited(V, false);

        /*
        The graph may be disconnected.

        A cycle may exist in a component that is not reachable
        from node 0, so we start DFS from every unvisited node.
        */
        for (int node = 0; node < V; node++) {

            if (visited[node] == false) {

                if (dfs(node, adj, visited, pathVisited)) {
                    return true;
                }
            }
        }

        return false;
    }
};