class Solution {
public:
    bool findSafeWalk(vector<vector<int>>& grid, int health) {
        int m = grid.size();
        int n = grid[0].size();

        // dist[r][c] = minimum total damage required
        // to reach cell (r, c) from the starting cell (0, 0).
        //
        // A cell contributes:
        // 0 damage if grid[r][c] == 0
        // 1 damage if grid[r][c] == 1
        //
        // We minimize damage instead of directly tracking remaining health.
        // Once minimum damage to the destination is known, we can check
        // whether the initial health is enough to survive that damage.
        vector<vector<int>> dist(m, vector<int>(n, INT_MAX));

        // Min-heap entry format:
        // {total damage to reach this cell, row, column}
        //
        // priority_queue normally places the largest element on top.
        // greater<vector<int>> makes it behave as a min-heap.
        //
        // Since damage is stored at index 0, the entry with the
        // smallest damage is processed first.
        priority_queue<
            vector<int>,
            vector<vector<int>>,
            greater<vector<int>>
        > pq;

        // The starting cell is also visited.
        //
        // Therefore, if grid[0][0] == 1, we immediately lose 1 health.
        // If grid[0][0] == 0, no health is lost at the start.
        dist[0][0] = grid[0][0];

        // Push:
        // {damage required to reach start, start row, start column}
        pq.push({dist[0][0], 0, 0});

        // Four possible movements:
        // right, left, down and up.
        vector<vector<int>> directions = {
            {0, 1},
            {0, -1},
            {1, 0},
            {-1, 0}
        };

        while (!pq.empty()) {
            // Extract the cell that currently has the
            // smallest known damage from the starting cell.
            vector<int> curr = pq.top();
            pq.pop();

            int damage = curr[0];
            int r = curr[1];
            int c = curr[2];

            // The same cell may be inserted into the heap multiple times.
            //
            // Example:
            // We may first reach a cell with damage 5.
            // Later, we may discover another route with damage 3.
            //
            // Both entries can remain inside the heap.
            // If the older damage-5 entry is popped later,
            // dist[r][c] will already be 3, so we skip it.
            if (damage > dist[r][c]) {
                continue;
            }

            // Explore all four cells adjacent to the current cell.
            for (auto& dir : directions) {
                int nr = r + dir[0];
                int nc = c + dir[1];

                // Ignore positions that fall outside the grid.
                if (nr < 0 || nr >= m || nc < 0 || nc >= n) {
                    continue;
                }

                // To reach the neighbour, we take:
                //
                // damage already taken to reach the current cell
                // +
                // damage caused by entering the neighbour cell.
                //
                // grid[nr][nc] is either:
                // 0 for a safe cell
                // 1 for an unsafe cell
                int newDamage = damage + grid[nr][nc];

                // Relaxation:
                //
                // If reaching the neighbour through the current cell
                // causes less total damage than the best route found
                // earlier, update its minimum damage.
                if (newDamage < dist[nr][nc]) {
                    dist[nr][nc] = newDamage;

                    // Push the improved route into the min-heap
                    // so the neighbour can be processed later.
                    pq.push({newDamage, nr, nc});
                }
            }
        }

        // dist[m - 1][n - 1] is the minimum damage required
        // to reach the bottom-right cell.
        //
        // Health must remain strictly positive throughout the walk.
        //
        // Remaining health:
        // health - minimumDamage
        //
        // To remain alive:
        // health - minimumDamage >= 1
        //
        // Therefore:
        // minimumDamage < health
        return dist[m - 1][n - 1] < health;
    }
};