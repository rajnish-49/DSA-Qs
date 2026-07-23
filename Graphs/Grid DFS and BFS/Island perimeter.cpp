class Solution {
public:
    int islandPerimeter(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();

        // 4 directions: up, down, left, right
        vector<vector<int>> directions = {
            {-1, 0}, // up
            {1, 0},  // down
            {0, -1}, // left
            {0, 1}   // right
        };

        int perimeter = 0;

        // Traverse every cell in the grid
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {

                // Only land cells contribute to perimeter
                if (grid[i][j] == 1) {

                    // Check all 4 sides of this land cell
                    for (auto& dir : directions) {
                        int newRow = i + dir[0];
                        int newCol = j + dir[1];

                        // If neighbour is outside the grid,
                        // this side contributes to perimeter
                        if (newRow < 0 || newRow >= m || newCol < 0 || newCol >= n) {
                            perimeter++;
                        }

                        // If neighbour is water,
                        // this side also contributes to perimeter
                        else if (grid[newRow][newCol] == 0) {
                            perimeter++;
                        }
                    }
                }
            }
        }

        return perimeter;
    }
};