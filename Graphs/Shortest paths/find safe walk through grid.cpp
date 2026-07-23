class Solution {
public:
    bool findSafeWalk(vector<vector<int>>& grid, int health) {
        int m = grid.size();
        int n = grid[0].size();

        // dist[i][j] = minimum damage taken to reach cell (i, j)
        // damage = number of unsafe cells with value 1 visited
        vector<vector<int>> dist(m, vector<int>(n, INT_MAX));

        // Min-heap: {damage_so_far, row, col}
        // The cell with least damage is processed first
        priority_queue<
            vector<int>,
            vector<vector<int>>,
            greater<vector<int>>
        > pq;

        // Starting cell also affects health if grid[0][0] == 1
        dist[0][0] = grid[0][0];
        pq.push({dist[0][0], 0, 0});

        vector<vector<int>> directions = {
            {0, 1}, {0, -1}, {1, 0}, {-1, 0}
        };

        while (!pq.empty()) {
            auto curr = pq.top();
            pq.pop();

            int damage = curr[0];
            int r = curr[1];
            int c = curr[2];

            // If this is an older/worse entry, ignore it
            if (damage > dist[r][c]) continue;

            for (auto &dir : directions) {
                int nr = r + dir[0];
                int nc = c + dir[1];

                if (nr < 0 || nr >= m || nc < 0 || nc >= n) continue;

                // Moving into a cell adds its danger cost:
                // 0 means safe, 1 means unsafe
                int newDamage = damage + grid[nr][nc];

                // Relaxation step: found a safer way to reach (nr, nc)
                if (newDamage < dist[nr][nc]) {
                    dist[nr][nc] = newDamage;
                    pq.push({newDamage, nr, nc});
                }
            }
        }

        // Need health to remain positive after total damage
        // health - damage >= 1  =>  damage < health
        return dist[m - 1][n - 1] < health;
    }
};