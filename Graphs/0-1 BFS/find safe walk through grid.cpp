// @https://leetcode.com/problems/find-a-safe-walk-through-a-grid/description/?envType=daily-question&envId=2026-07-03

class Solution {
public:
    bool findSafeWalk(vector<vector<int>>& grid, int health) {
        int m = grid.size();
        int n = grid[0].size();

        // dist[i][j] = minimum damage taken to reach cell (i, j)
        // damage = number of unsafe cells with value 1 visited
        vector<vector<int>> dist(m, vector<int>(n, INT_MAX));

        // 0-1 BFS uses deque instead of priority_queue
        // because every move has cost either 0 or 1
        // deque usually stores only: {row, col}
        deque<pair<int, int>> dq;

        // Starting cell also affects health if grid[0][0] == 1
        dist[0][0] = grid[0][0];
        dq.push_front({0, 0});

        vector<vector<int>> directions = {
            {0, 1}, {0, -1}, {1, 0}, {-1, 0}
        };

        while (!dq.empty()) {
            auto [r, c] = dq.front();
            dq.pop_front();

            for (auto &dir : directions) {
                int nr = r + dir[0];
                int nc = c + dir[1];

                if (nr < 0 || nr >= m || nc < 0 || nc >= n) continue;

                // Moving into a cell adds its danger cost:
                // 0 means safe, 1 means unsafe
                int newDamage = dist[r][c] + grid[nr][nc];

                // Relaxation step: found a safer way to reach (nr, nc)
                if (newDamage < dist[nr][nc]) {
                    dist[nr][nc] = newDamage;

                    // Cost 0 edge:
                    // process earlier because damage does not increase
                    if (grid[nr][nc] == 0) {
                        dq.push_front({nr, nc});
                    }
                    // Cost 1 edge:
                    // process later because damage increases by 1
                    else {
                        dq.push_back({nr, nc});
                    }
                }
            }
        }

        // Need health to remain positive after total damage
        // health - damage >= 1  =>  damage < health
        return dist[m - 1][n - 1] < health;
    }
};