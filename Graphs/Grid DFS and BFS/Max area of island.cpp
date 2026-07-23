// @ 


class Solution {
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();

        // 4 possible directions: up, down, left, right
        vector<vector<int>> dirn = {
            {-1, 0}, // up
            {1, 0},  // down
            {0, -1}, // left
            {0, 1}   // right
        };

        int ans = 0; // stores maximum island area found so far

        // Traverse every cell of the grid
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {

                // If we find land, it means we found a new island
                if (grid[i][j] == 1) {
                    int current = 1; // area of this island

                    queue<pair<int, int>> q;
                    q.push({i, j});

                    // Mark starting land cell as visited
                    grid[i][j] = 0;

                    // BFS to visit the whole connected island
                    while (!q.empty()) {
                        auto [row, col] = q.front();
                        q.pop();

                        // Check all 4 neighbours
                        for (auto& dir : dirn) {
                            int newr = row + dir[0];
                            int newc = col + dir[1];

                            // Skip if neighbour is outside the grid
                            if (newr < 0 || newr >= m || newc < 0 || newc >= n) {
                                continue;
                            }

                            // Skip water or already visited land
                            if (grid[newr][newc] == 0) {
                                continue;
                            }

                            // This is a valid connected land cell
                            current++;

                            // Mark it visited so we do not count it again
                            grid[newr][newc] = 0;

                            // Push it into queue for further BFS expansion
                            q.push({newr, newc});
                        }
                    }

                    // After full island is visited, update max area
                    ans = max(ans, current);
                }
            }
        }

        return ans;
    }
};


class Solution {
public:
    int m, n;

    // 4 possible directions: up, down, left, right
    vector<vector<int>> dirn = {
        {-1, 0}, // up
        {1, 0},  // down
        {0, -1}, // left
        {0, 1}   // right
    };

    int dfs(vector<vector<int>>& grid, int row, int col) {
        // If current cell is outside the grid, it contributes 0 area
        if (row < 0 || row >= m || col < 0 || col >= n) {
            return 0;
        }

        // If current cell is water or already visited, it contributes 0 area
        if (grid[row][col] == 0) {
            return 0;
        }

        // Current cell is land, so count it as area 1
        int area = 1;

        // Mark current land cell as visited
        grid[row][col] = 0;

        // Explore all 4 neighbours and add their connected land area
        for (auto& dir : dirn) {
            int newr = row + dir[0];
            int newc = col + dir[1];

            area += dfs(grid, newr, newc);
        }

        return area;
    }

    int maxAreaOfIsland(vector<vector<int>>& grid) {
        m = grid.size();
        n = grid[0].size();

        int ans = 0;

        // Traverse every cell of the grid
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {

                // If we find land, calculate the area of this full island
                if (grid[i][j] == 1) {
                    int currentArea = dfs(grid, i, j);

                    // Update maximum area found so far
                    ans = max(ans, currentArea);
                }
            }
        }

        return ans;
    }
};