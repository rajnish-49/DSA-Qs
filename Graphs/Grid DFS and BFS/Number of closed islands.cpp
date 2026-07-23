// @https://leetcode.com/problems/number-of-closed-islands/description/


class Solution {
public:
    int m, n;

    // 4 directions: up, down, left, right
    vector<vector<int>> directions = {
        {-1, 0}, // up
        {1, 0},  // down
        {0, -1}, // left
        {0, 1}   // right
    };

    void dfs(vector<vector<int>>& grid, int row, int col) {
        // If outside the grid, stop.
        if (row < 0 || row >= m || col < 0 || col >= n) {
            return;
        }

        // In this problem:
        // 0 = land
        // 1 = water / already visited
        //
        // If current cell is water or already visited, stop.
        if (grid[row][col] == 1) {
            return;
        }

        // Mark current land cell as visited by converting it to water.
        grid[row][col] = 1;

        // Visit all connected land cells in 4 directions.
        for (auto& dir : directions) {
            int newRow = row + dir[0];
            int newCol = col + dir[1];

            dfs(grid, newRow, newCol);
        }
    }

    int closedIsland(vector<vector<int>>& grid) {
        m = grid.size();
        n = grid[0].size();

        // Step 1:
        // Remove all land cells connected to the boundary.
        // These cannot be closed islands.

        // First row and last row
        for (int col = 0; col < n; col++) {
            if (grid[0][col] == 0) {
                dfs(grid, 0, col);
            }

            if (grid[m - 1][col] == 0) {
                dfs(grid, m - 1, col);
            }
        }

        // First column and last column
        for (int row = 0; row < m; row++) {
            if (grid[row][0] == 0) {
                dfs(grid, row, 0);
            }

            if (grid[row][n - 1] == 0) {
                dfs(grid, row, n - 1);
            }
        }

        // Step 2:
        // Now any remaining 0-component is fully surrounded by 1s.
        // Count such components.
        int ans = 0;

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {

                // Found an unvisited closed island.
                if (grid[i][j] == 0) {
                    ans++;

                    // Mark the full island as visited.
                    dfs(grid, i, j);
                }
            }
        }

        return ans;
    }
};





class Solution {
public:
    void bfs(vector<vector<int>>& grid, int startRow, int startCol) {
        int m = grid.size();
        int n = grid[0].size();

        // 4 directions: up, down, left, right
        vector<vector<int>> directions = {
            {-1, 0}, // up
            {1, 0},  // down
            {0, -1}, // left
            {0, 1}   // right
        };

        queue<pair<int, int>> q;
        q.push({startRow, startCol});

        // Mark starting land cell as visited.
        grid[startRow][startCol] = 1;

        while (!q.empty()) {
            auto [row, col] = q.front();
            q.pop();

            // Explore all 4 neighbours.
            for (auto& dir : directions) {
                int newRow = row + dir[0];
                int newCol = col + dir[1];

                // Skip if outside the grid.
                if (newRow < 0 || newRow >= m || newCol < 0 || newCol >= n) {
                    continue;
                }

                // In this problem:
                // 0 = land
                // 1 = water / already visited
                //
                // So only move to cells with value 0.
                if (grid[newRow][newCol] == 1) {
                    continue;
                }

                // Mark connected land cell as visited.
                grid[newRow][newCol] = 1;

                // Push it into queue for further BFS expansion.
                q.push({newRow, newCol});
            }
        }
    }

    int closedIsland(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();

        // Step 1:
        // Remove all boundary-connected land.
        // Boundary-connected land cannot be a closed island.

        // First row and last row
        for (int col = 0; col < n; col++) {
            if (grid[0][col] == 0) {
                bfs(grid, 0, col);
            }

            if (grid[m - 1][col] == 0) {
                bfs(grid, m - 1, col);
            }
        }

        // First column and last column
        for (int row = 0; row < m; row++) {
            if (grid[row][0] == 0) {
                bfs(grid, row, 0);
            }

            if (grid[row][n - 1] == 0) {
                bfs(grid, row, n - 1);
            }
        }

        // Step 2:
        // Count remaining 0-components.
        // Each remaining 0-component is a closed island.
        int ans = 0;

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {

                // Found one closed island.
                if (grid[i][j] == 0) {
                    ans++;

                    // Mark the full island as visited.
                    bfs(grid, i, j);
                }
            }
        }

        return ans;
    }
};