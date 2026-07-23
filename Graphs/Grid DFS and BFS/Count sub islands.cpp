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

    bool dfs(vector<vector<int>>& grid1, vector<vector<int>>& grid2, int row, int col) {
        // If outside grid, this path does not affect validity.
        if (row < 0 || row >= m || col < 0 || col >= n) {
            return true;
        }

        // If current cell is water or already visited in grid2,
        // it is not part of the current grid2 island.
        if (grid2[row][col] == 0) {
            return true;
        }

        // Mark current grid2 land cell as visited.
        grid2[row][col] = 0;

        // This cell is valid only if grid1 also has land here.
        bool valid = (grid1[row][col] == 1);

        // Explore the full island in grid2.
        for (auto& dir : directions) {
            int newRow = row + dir[0];
            int newCol = col + dir[1];

            // If any connected cell is invalid,
            // the whole grid2 island is not a sub-island.
            valid = dfs(grid1, grid2, newRow, newCol) && valid;
        }

        return valid;
    }

    int countSubIslands(vector<vector<int>>& grid1, vector<vector<int>>& grid2) {
        m = grid2.size();
        n = grid2[0].size();

        int ans = 0;

        // Traverse every cell of grid2.
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {

                // Every unvisited 1 in grid2 starts one island.
                if (grid2[i][j] == 1) {

                    // DFS checks whether this whole grid2 island
                    // lies completely on land cells of grid1.
                    bool isSubIsland = dfs(grid1, grid2, i, j);

                    if (isSubIsland) {
                        ans++;
                    }
                }
            }
        }

        return ans;
    }
};




class Solution {
public:
    int countSubIslands(vector<vector<int>>& grid1, vector<vector<int>>& grid2) {
        int m = grid2.size();
        int n = grid2[0].size();

        // 4 directions: up, down, left, right
        vector<vector<int>> directions = {
            {-1, 0}, // up
            {1, 0},  // down
            {0, -1}, // left
            {0, 1}   // right
        };

        int ans = 0;

        // Traverse every cell of grid2.
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {

                // Start BFS only from unvisited land in grid2.
                if (grid2[i][j] == 1) {
                    bool isSubIsland = true;

                    queue<pair<int, int>> q;
                    q.push({i, j});

                    // Mark starting grid2 land cell as visited.
                    grid2[i][j] = 0;

                    while (!q.empty()) {
                        auto [row, col] = q.front();
                        q.pop();

                        // If this grid2 land cell is water in grid1,
                        // then the whole current grid2 island is invalid.
                        if (grid1[row][col] == 0) {
                            isSubIsland = false;
                        }

                        // Explore all 4 neighbours.
                        for (auto& dir : directions) {
                            int newRow = row + dir[0];
                            int newCol = col + dir[1];

                            // Skip outside cells.
                            if (newRow < 0 || newRow >= m || newCol < 0 || newCol >= n) {
                                continue;
                            }

                            // Only continue through land cells in grid2.
                            // grid2 defines the island shape.
                            if (grid2[newRow][newCol] == 0) {
                                continue;
                            }

                            // Mark visited before pushing to avoid duplicate visits.
                            grid2[newRow][newCol] = 0;
                            q.push({newRow, newCol});
                        }
                    }

                    // Count only if every cell of this grid2 island
                    // was also land in grid1.
                    if (isSubIsland) {
                        ans++;
                    }
                }
            }
        }

        return ans;
    }
};