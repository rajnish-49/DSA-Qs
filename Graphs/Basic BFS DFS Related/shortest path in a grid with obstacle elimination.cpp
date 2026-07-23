class Solution {
public:
    int shortestPath(vector<vector<int>>& grid, int k) {

        int m = grid.size();
        int n = grid[0].size();

        /*
            dist[r][c][remK] = minimum steps needed to reach cell (r, c)
                               with remK obstacle eliminations remaining

            Why 3D dist?

            Because reaching the same cell with different remainingK values
            are different states.

            Example:
            reaching (r, c) with 3 eliminations left is better/different
            than reaching (r, c) with 0 eliminations left.
        */
        vector<vector<vector<int>>> dist(
            m, vector<vector<int>>(n, vector<int>(k + 1, INT_MAX))
        );

        /*
            Normal BFS is used because every move costs exactly 1 step.

            Queue stores:
            {row, col, remainingK}
        */
        queue<tuple<int, int, int>> q;

        // Start at top-left with all k eliminations available
        dist[0][0][k] = 0;
        q.push({0, 0, k});

        vector<vector<int>> directions = {
            {0, 1}, {0, -1}, {1, 0}, {-1, 0}
        };

        while (!q.empty()) {
            auto [r, c, remK] = q.front();
            q.pop();

            // Try moving to all 4 adjacent cells
            for (auto& dir : directions) {
                int nr = r + dir[0];
                int nc = c + dir[1];

                if (nr < 0 || nr >= m || nc < 0 || nc >= n)
                    continue;

                /*
                    Case 1: next cell is empty.

                    No obstacle elimination is used,
                    so remainingK stays the same.
                */
                if (grid[nr][nc] == 0) {
                    if (dist[nr][nc][remK] == INT_MAX) {
                        dist[nr][nc][remK] = dist[r][c][remK] + 1;
                        q.push({nr, nc, remK});
                    }
                }

                /*
                    Case 2: next cell is an obstacle.

                    We can enter it only if remK > 0.
                    Entering it uses one elimination,
                    so remainingK becomes remK - 1.
                */
                else {
                    if (remK > 0 && dist[nr][nc][remK - 1] == INT_MAX) {
                        dist[nr][nc][remK - 1] = dist[r][c][remK] + 1;
                        q.push({nr, nc, remK - 1});
                    }
                }
            }
        }

        /*
            Destination can be reached with any remainingK value.

            We only care about minimum steps,
            so take the minimum over all remainingK states.
        */
        int ans = INT_MAX;

        for (int remK = 0; remK <= k; remK++) {
            ans = min(ans, dist[m - 1][n - 1][remK]);
        }

        // If destination was never reached, return -1
        return ans == INT_MAX ? -1 : ans;
    }
};