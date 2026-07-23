class Solution {
public:
    bool possibleBipartition(int n, vector<vector<int>>& dislikes) {

        // Build an undirected graph.
        // If dislikes contains [a, b], then:
        // a dislikes b and b dislikes a.
        vector<vector<int>> graph(n + 1);

        for (auto& edge : dislikes) {
            int a = edge[0];
            int b = edge[1];

            graph[a].push_back(b);
            graph[b].push_back(a);
        }

        // color[i]:
        //  0  -> person i has not been assigned to any group
        //  1  -> person i belongs to group 1
        // -1  -> person i belongs to group 2
        vector<int> color(n + 1, 0);

        // The graph may be disconnected, so we must start BFS
        // separately from every person who has not been visited.
        for (int person = 1; person <= n; person++) {

            // This person is already part of a previously processed component.
            if (color[person] != 0) {
                continue;
            }

            queue<int> q;

            // Arbitrarily place this person in group 1.
            color[person] = 1;
            q.push(person);

            while (!q.empty()) {
                int current = q.front();
                q.pop();

                // Every disliked person must belong to the opposite group.
                for (int neighbor : graph[current]) {

                    // If the neighbor has not been assigned a group,
                    // assign it the opposite group of the current person.
                    if (color[neighbor] == 0) {
                        color[neighbor] = -color[current];
                        q.push(neighbor);
                    }

                    // If both people are already in the same group,
                    // the required bipartition is impossible.
                    else if (color[neighbor] == color[current]) {
                        return false;
                    }
                }
            }
        }

        // No conflict was found in any connected component.
        return true;
    }
};