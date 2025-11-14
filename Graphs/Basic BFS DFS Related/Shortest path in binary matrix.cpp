// @https://leetcode.com/problems/shortest-path-in-binary-matrix/

class Solution {
public:
    int shortestPathBinaryMatrix(vector<vector<int>>& grid) {

        int n = grid.size();       // Number of rows in the grid
        int m = grid[0].size();    // Number of columns in the grid (n == m, but kept for clarity)

        // Visited matrix to ensure we do not process the same cell multiple times.
        // This prevents cycles and redundant BFS expansions.
        vector<vector<int>> vis(n, vector<int>(m, 0));

        // BFS queue: stores ((row, col), distance_from_start)
        // Distance starts at 1 because the starting cell counts as a step.
        queue<pair<pair<int, int>, int>> q;

        // Special case: if starting cell is blocked, no path is possible
        if (grid[0][0] == 1) {
            return -1;
        }

        // Start BFS from the top-left cell (0,0)
        q.push({{0, 0}, 1});

        // Directions for 8 possible moves (diagonal + horizontal + vertical)
        // Each pair represents a relative move from the current cell
        int delrow[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        int delcol[] = {-1, 0, 1, -1, 1, -1, 0, 1};

        // Multi-directional BFS
        while (!q.empty()) {

            auto cell = q.front();
            q.pop();

            int row = cell.first.first;   // current row
            int col = cell.first.second;  // current column
            int dist = cell.second;       // distance traveled so far

            // If we reach the bottom-right cell, return the path length
            // BFS guarantees this is the shortest path because it explores in layers
            if (row == n - 1 && col == m - 1) {
                return dist;
            }

            // Mark the current cell as visited to avoid revisiting it
            vis[row][col] = 1;

            // Explore all 8 neighbors
            for (int i = 0; i < 8; i++) {
                int nrow = row + delrow[i];  // compute neighbor row
                int ncol = col + delcol[i];  // compute neighbor column

                // Boundary check + ensure neighbor is unvisited and not blocked
                if (nrow >= 0 && nrow < n && ncol >= 0 && ncol < m &&
                    !vis[nrow][ncol] && grid[nrow][ncol] == 0) {
                    
                    // Push neighbor to queue with incremented distance
                    q.push({{nrow, ncol}, dist + 1});

                    // Mark as visited immediately to prevent multiple insertions in the queue
                    vis[nrow][ncol] = 1;
                }
            }
        }

        // If BFS completes without reaching bottom-right, no path exists
        return -1;
    }
};
