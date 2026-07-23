class Solution {
public:
    int minimumObstacles(vector<vector<int>>& grid) {

        int m = grid.size();
        int n = grid[0].size();

        /*
            dist[i][j] = minimum number of obstacles removed
                         to reach cell (i, j)

            grid[i][j] = 0 -> empty cell, cost 0
            grid[i][j] = 1 -> obstacle, cost 1 to remove
        */
        vector<vector<int>> dist(m, vector<int>(n, INT_MAX));

        /*
            0-1 BFS uses deque because every move has cost either 0 or 1.

            If moving to a 0-cell:
                cost does not increase -> push_front

            If moving to a 1-cell:
                cost increases by 1 -> push_back
        */
        deque<pair<int, int>> dq;

        // Start cell is guaranteed to be 0, so no obstacle is removed initially
        dist[0][0] = 0;
        dq.push_front({0, 0});

        vector<vector<int>> directions = {
            {0, 1}, {0, -1}, {1, 0}, {-1, 0}
        };

        while (!dq.empty()) {
            auto [r, c] = dq.front();
            dq.pop_front();

            for (auto& dir : directions) {
                int nr = r + dir[0];
                int nc = c + dir[1];

                if (nr < 0 || nr >= m || nc < 0 || nc >= n)
                    continue;

                /*
                    Cost of entering the next cell:

                    grid[nr][nc] == 0 -> no obstacle removed
                    grid[nr][nc] == 1 -> remove one obstacle

                    So new cost = current cost + grid[nr][nc]
                */
                int newObstacleCount = dist[r][c] + grid[nr][nc];

                // Relaxation: found a path requiring fewer obstacle removals
                if (newObstacleCount < dist[nr][nc]) {
                    dist[nr][nc] = newObstacleCount;

                    /*
                        If next cell is empty, process it earlier.
                        This is like a 0-cost edge.
                    */
                    if (grid[nr][nc] == 0) {
                        dq.push_front({nr, nc});
                    }

                    /*
                        If next cell is obstacle, process it later.
                        This is like a 1-cost edge.
                    */
                    else {
                        dq.push_back({nr, nc});
                    }
                }
            }
        }

        // Minimum obstacles that must be removed to reach bottom-right
        return dist[m - 1][n - 1];
    }
};