class Solution {
public:
    bool bfs(int start,vector<vector<int>>& graph, vector<int>& color) {

        queue<int> q;

        /*
            Begin this connected component by assigning any one color
            to the starting vertex.

            It does not matter whether we start with 0 or 1.
            All other colors will be decided relative to this vertex.
        */
        color[start] = 0;
        q.push(start);

        /*
            BFS visits the component while trying to divide its vertices
            into two groups.

            Every neighbour of a color-0 vertex must receive color 1,
            and every neighbour of a color-1 vertex must receive color 0.
        */
        while (!q.empty()) {
            int node = q.front();
            q.pop();

            for (int neighbor : graph[node]) {

                /*
                    If the neighbour has not been colored, assign it
                    the opposite color and continue BFS from it.
                */
                if (color[neighbor] == -1) {
                    color[neighbor] = 1 - color[node];
                    q.push(neighbor);
                }

                /*
                    If the neighbour is already colored, its color must
                    be opposite to the current node's color.

                    The same color on both endpoints means this edge
                    cannot connect the two separate groups.
                */
                else if (color[neighbor] == color[node]) {
                    return false;
                }
            }
        }

        return true;
    }

    bool isBipartite(vector<vector<int>>& graph) {
        int n = graph.size();

        /*
            -1 means the vertex has not been visited or colored.
             0 and 1 represent the two bipartite groups.
        */
        vector<int> color(n, -1);

        /*
            The graph may be disconnected.

            Therefore, start a separate BFS from every vertex that has
            not yet been colored.
        */
        for (int node = 0; node < n; node++) {
            if (color[node] == -1) {
                if (!bfs(node, graph, color)) {
                    return false;
                }
            }
        }

        return true;
    }
};