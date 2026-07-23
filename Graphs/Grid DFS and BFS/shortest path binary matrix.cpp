class Solution {
public:
    int shortestPathBinaryMatrix(vector<vector<int>>& grid) {
        int n = grid.size();
        int m = grid[0].size();

        // If starting cell is blocked, no path is possible.
        if (grid[0][0] == 1) {
            return -1;
        }

        // 8 possible directions:
        // top-left, top, top-right,
        // left, right,
        // bottom-left, bottom, bottom-right
        vector<vector<int>> directions = {
            {-1, -1}, {-1, 0}, {-1, 1},
            {0, -1},           {0, 1},
            {1, -1},  {1, 0},  {1, 1}
        };

        vector<vector<int>> vis(n, vector<int>(m, 0));

        // Queue stores: {{row, col}, path_length}
        queue<pair<pair<int, int>, int>> q;

        q.push({{0, 0}, 1});
        vis[0][0] = 1; // Mark start as visited when pushing into queue.

        while (!q.empty()) {
            auto cell = q.front();
            q.pop();

            int row = cell.first.first;
            int col = cell.first.second;
            int dist = cell.second;

            // Since BFS explores level by level,
            // first time we reach destination gives shortest path.
            if (row == n - 1 && col == m - 1) {
                return dist;
            }

            // Try moving in all 8 directions.
            for (auto& dir : directions) {
                int nrow = row + dir[0];
                int ncol = col + dir[1];

                // Skip if outside the grid.
                if (nrow < 0 || nrow >= n || ncol < 0 || ncol >= m) {
                    continue;
                }

                // Skip blocked cells and already visited cells.
                if (grid[nrow][ncol] == 1 || vis[nrow][ncol] == 1) {
                    continue;
                }

                // This is a valid unvisited 0-cell.
                // Mark visited immediately to avoid pushing it multiple times.
                vis[nrow][ncol] = 1;

                // Distance increases by 1 after moving to this neighbour.
                q.push({{nrow, ncol}, dist + 1});
            }
        }

        // If BFS ends without reaching bottom-right, no clear path exists.
        return -1;
    }
};