// Start from an unvisited city.
// DFS will visit all cities connected to it directly or indirectly.
// That whole DFS traversal is one province.


using namespace std;

class Solution {
public:
    void dfs(vector<vector<int>>& adj, int u, vector<bool>& visited) {
        // Mark current city/node as visited
        visited[u] = true;

        // Visit all cities directly connected to current city
        for (int v : adj[u]) {

            // If this connected city is not visited yet,
            // continue DFS from that city
            if (!visited[v]) {
                dfs(adj, v, visited);
            }
        }
    }

    int findCircleNum(vector<vector<int>>& isConnected) {
        int n = isConnected.size();

        // visited[i] = false means city i has not been visited yet
        // visited[i] = true means city i already belongs to some province
        vector<bool> visited(n, false);

        // Convert adjacency matrix into adjacency list
        //
        // isConnected[i][j] == 1 means city i is directly connected to city j
        vector<vector<int>> adj(n);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {

                // If there is a direct connection from i to j,
                // add j to the neighbour list of i
                if (isConnected[i][j] == 1) {
                    adj[i].push_back(j);
                }
            }
        }

        int cnt = 0;

        // Each DFS call from an unvisited city covers one complete province
        for (int i = 0; i < n; i++) {

            // If city i is still unvisited,
            // it means we found a new province/component
            if (!visited[i]) {
                cnt++;

                // Mark all cities in this province as visited
                dfs(adj, i, visited);
            }
        }

        return cnt;
    }
};