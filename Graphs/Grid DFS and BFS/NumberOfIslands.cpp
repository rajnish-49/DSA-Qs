// @ https://leetcode.com/problems/number-of-islands/description/

// SAME AS CONNECTED COMPONENTS 

class Solution {
public:
    int m, n;

    vector<vector<int>> directions = {
        {-1, 0}, // up
        {1, 0},  // down
        {0, -1}, // left
        {0, 1}   // right
    };

    void dfs(vector<vector<char>>& grid, int row, int col) {
        // If current position is outside the grid, stop.
        if (row < 0 || row >= m || col < 0 || col >= n) {
            return;
        }

        // If current cell is water or already visited, stop.
        if (grid[row][col] == '0') {
            return;
        }

        // Mark this land cell as visited.
        // We change '1' to '0' so it will not be counted again.
        grid[row][col] = '0';

        // Visit all 4 neighbours.
        for (auto& dir : directions) {
            int newRow = row + dir[0];
            int newCol = col + dir[1];

            dfs(grid, newRow, newCol);
        }
    }

    int numIslands(vector<vector<char>>& grid) {
        m = grid.size();
        n = grid[0].size();

        int islands = 0;

        // Traverse every cell.
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {

                // If we find unvisited land,
                // it is the start of a new island.
                if (grid[i][j] == '1') {
                    islands++;

                    // Remove/visit the full connected island.
                    dfs(grid, i, j);
                }
            }
        }

        return islands;
    }
};

class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        int m = grid.size();
        int n = grid[0].size();

        int islands = 0;

        vector<vector<int>> directions = {
            {-1, 0}, {1, 0}, {0, -1}, {0, 1}   
        };

        // Traverse every cell.
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {

                // If we find land, this is a new island.
                if (grid[i][j] == '1') {
                    islands++;

                    queue<pair<int, int>> q;
                    q.push({i, j});

                    // Mark starting cell as visited.
                    grid[i][j] = '0';

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

                            // Skip water or already visited cells.
                            if (grid[newRow][newCol] == '0') {
                                continue;
                            }

                            // Mark neighbour as visited.
                            grid[newRow][newCol] = '0';

                            // Push it into queue to explore further.
                            q.push({newRow, newCol});
                        }
                    }
                }
            }
        }

        return islands;
    }
};