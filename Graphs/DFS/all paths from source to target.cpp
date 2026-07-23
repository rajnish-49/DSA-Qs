class Solution {
public:
    void dfs(int node, vector<vector<int>>& graph, vector<int>& path, vector<vector<int>>& ans) {
        // If current node is target node,
        // current path is one complete valid path.
        if (node == graph.size() - 1) {
            ans.push_back(path);
            return;
        }

        // Explore all nodes reachable from current node
        for (int next : graph[node]) {

            // Choose this next node as part of current path
            path.push_back(next);

            // Continue DFS from next node
            dfs(next, graph, path, ans);

            // Backtrack:
            // remove next node so we can try another path
            path.pop_back();
        }
    }

    vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {
        vector<vector<int>> ans;
        vector<int> path;

        // Every path starts from source node 0
        path.push_back(0);

        // Start DFS from source node 0
        dfs(0, graph, path, ans);

        return ans;
    }
};