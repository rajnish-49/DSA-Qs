// @ https://leetcode.com/problems/number-of-enclaves/

class Solution {
public:
    int m, n;

    vector<vector<int>> directions = {
        {-1, 0}, // up
        {1, 0},  // down
        {0, -1}, // left
        {0, 1}   // right
    };

    void dfs(vector<vector<int>>& grid, int row, int col) {
        // If current cell is outside the grid, stop.
        if (row < 0 || row >= m || col < 0 || col >= n) {
            return;
        }

        // If current cell is sea or already removed, stop.
        if (grid[row][col] == 0) {
            return;
        }

        // This land cell is connected to boundary,
        // so it can escape. Remove it.
        grid[row][col] = 0;

        // Explore all 4 connected land cells.
        for (auto& dir : directions) {
            int newRow = row + dir[0];
            int newCol = col + dir[1];

            dfs(grid, newRow, newCol);
        }
    }

    int numEnclaves(vector<vector<int>>& grid) {
        m = grid.size();
        n = grid[0].size();

        // Step 1:
        // Remove all land cells connected to the boundary.
        // These land cells can walk out of the grid,
        // so they are not enclaves.

        // First row and last row
        for (int col = 0; col < n; col++) {
            if (grid[0][col] == 1) {
                dfs(grid, 0, col);
            }

            if (grid[m - 1][col] == 1) {
                dfs(grid, m - 1, col);
            }
        }

        // First column and last column
        for (int row = 0; row < m; row++) {
            if (grid[row][0] == 1) {
                dfs(grid, row, 0);
            }

            if (grid[row][n - 1] == 1) {
                dfs(grid, row, n - 1);
            }
        }

        // Step 2:
        // After removing boundary-connected land,
        // whatever land remains is trapped inside.
        int ans = 0;

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    ans++;
                }
            }
        }

        return ans;
    }
};


class Solution {
public:
    int numEnclaves(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();

        vector<vector<int>> directions = {
            {-1, 0}, // up
            {1, 0},  // down
            {0, -1}, // left
            {0, 1}   // right
        };

        queue<pair<int, int>> q;

        // Step 1:
        // Push all boundary land cells into the queue.
        // These cells can escape, so we will remove them.

        // First row and last row
        for (int col = 0; col < n; col++) {
            if (grid[0][col] == 1) {
                q.push({0, col});
                grid[0][col] = 0;
            }

            if (grid[m - 1][col] == 1) {
                q.push({m - 1, col});
                grid[m - 1][col] = 0;
            }
        }

        // First column and last column
        for (int row = 0; row < m; row++) {
            if (grid[row][0] == 1) {
                q.push({row, 0});
                grid[row][0] = 0;
            }

            if (grid[row][n - 1] == 1) {
                q.push({row, n - 1});
                grid[row][n - 1] = 0;
            }
        }

        // Step 2:
        // BFS from boundary land and remove every connected land cell.
        while (!q.empty()) {
            auto [row, col] = q.front();
            q.pop();

            for (auto& dir : directions) {
                int newRow = row + dir[0];
                int newCol = col + dir[1];

                // Skip cells outside the grid.
                if (newRow < 0 || newRow >= m || newCol < 0 || newCol >= n) {
                    continue;
                }

                // Only land cells connected to boundary need to be removed.
                if (grid[newRow][newCol] == 0) {
                    continue;
                }

                // This land cell can escape through boundary-connected land.
                // Remove it and push it for further expansion.
                grid[newRow][newCol] = 0;
                q.push({newRow, newCol});
            }
        }

        // Step 3:
        // Count remaining land cells.
        // These cannot reach the boundary, so they are enclaves.
        int ans = 0;

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    ans++;
                }
            }
        }

        return ans;
    }
};

