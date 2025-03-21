// @ https://leetcode.com/problems/number-of-provinces/

#include <vector>
using namespace std;

class Solution {
    private:

    // Depth First Search (DFS) function to traverse the graph
    void dfs(int node, vector<vector<int>> &isConnected, vector<int> &vis) {
        // Mark the current node as visited
        vis[node] = 1;

        // Traverse all cities (nodes) to check for direct connections
        for (int i = 0; i < isConnected.size(); i++) {
            // If there's a direct connection and the node is not visited yet
            if (isConnected[node][i] == 1 && !vis[i]) {
                // Recursively call DFS to visit the connected node
                dfs(i, isConnected, vis);
            }
        }
    }

    public:
    // Function to count the number of provinces (connected components)
    int findCircleNum(vector<vector<int>>& isConnected) {
        int v = isConnected.size();  // Number of cities (nodes)
        vector<int> vis(v, 0);  // Visited array to track visited cities
        int cnt = 0;  // Province counter

        // Iterate through all cities (nodes)
        for (int i = 0; i < v; i++) {
            // If the city is not visited, it's a new province
            if (!vis[i]) {
                cnt++;  // Increment the province count
                dfs(i, isConnected, vis);  // Perform DFS to mark all connected cities
            }
        }

        return cnt;  // Return the total number of provinces
    }
};
