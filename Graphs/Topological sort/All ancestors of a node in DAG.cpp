class Solution {
public:
    vector<vector<int>> getAncestors(int n, vector<vector<int>>& edges) {

        vector<vector<int>> adj(n);
        vector<int> indegree(n, 0);

        /*
            Build the directed graph.

            For an edge:
                u -> v

            u is a direct ancestor of v.
        */
        for (auto& edge : edges) {
            int u = edge[0];
            int v = edge[1];

            adj[u].push_back(v);
            indegree[v]++;
        }

        /*
            ancestors[i] stores all ancestors of node i.

            We use set because:
            1. The same ancestor may reach a node through multiple paths.
            2. The required answer must be sorted.
        */
        vector<set<int>> ancestors(n);

        queue<int> q;

        /*
            Nodes with indegree 0 have no incoming edges,
            so they have no ancestors.

            These nodes are the starting points of Kahn's algorithm.
        */
        for (int node = 0; node < n; node++) {
            if (indegree[node] == 0) {
                q.push(node);
            }
        }

        /*
            Process nodes in topological order.

            Important property:

            When a node is removed from the queue, all its parents
            have already been processed.

            Therefore, ancestors[node] is complete and can be
            propagated to its children.
        */
        while (!q.empty()) {

            int node = q.front();
            q.pop();

            for (int child : adj[node]) {

                /*
                    Since:
                        node -> child

                    node is a direct ancestor of child.
                */
                ancestors[child].insert(node);

                /*
                    Every ancestor of node is also an ancestor of child.

                    If:
                        ancestor -> ... -> node -> child

                    then ancestor also reaches child.
                */
                for (int ancestor : ancestors[node]) {
                    ancestors[child].insert(ancestor);
                }

                /*
                    Normal Kahn's algorithm step:

                    We have processed one incoming edge of child.
                */
                indegree[child]--;

                /*
                    When indegree becomes 0, all parents of child
                    have been processed.

                    Hence, all ancestor information from every parent
                    has already been collected.
                */
                if (indegree[child] == 0) {
                    q.push(child);
                }
            }
        }

        vector<vector<int>> answer(n);

        /*
            Convert each sorted set into a vector.
        */
        for (int node = 0; node < n; node++) {
            answer[node] = vector<int>(
                ancestors[node].begin(),
                ancestors[node].end()
            );
        }

        return answer;
    }
};