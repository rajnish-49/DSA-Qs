class Solution {
public:
    bool dfs(int node,
             int currentColor,
             vector<vector<int>>& graph,
             vector<int>& color) {

        color[node] = currentColor;

        for (int neighbor : graph[node]) {

            // Uncolored neighbour: give it the opposite color.
            if (color[neighbor] == -1) {
                if (!dfs(neighbor, 1 - currentColor, graph, color)) {
                    return false;
                }
            }

            // Adjacent vertices cannot belong to the same group.
            else if (color[neighbor] == currentColor) {
                return false;
            }
        }

        return true;
    }

    bool isBipartite(vector<vector<int>>& graph) {
        int n = graph.size();

        vector<int> color(n, -1);

        // Check every connected component.
        for (int node = 0; node < n; node++) {
            if (color[node] == -1) {
                if (!dfs(node, 0, graph, color)) {
                    return false;
                }
            }
        }

        return true;
    }
};