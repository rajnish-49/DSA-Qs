// @ https://leetcode.com/problems/flood-fill/

class Solution {
public:
    vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int color) {
        int m = image.size();
        int n = image[0].size();

        int originalColor = image[sr][sc];

        // If the new color is same as original color,
        // no change is needed.
        if (originalColor == color) {
            return image;
        }

        // 4 directions: up, down, left, right
        vector<vector<int>> directions = {
            {-1, 0}, // up
            {1, 0},  // down
            {0, -1}, // left
            {0, 1}   // right
        };

        queue<pair<int, int>> q;
        q.push({sr, sc});

        // Recolor the starting cell.
        image[sr][sc] = color;

        while (!q.empty()) {
            auto [row, col] = q.front();
            q.pop();

            // Check all 4 neighbours
            for (auto& dir : directions) {
                int newRow = row + dir[0];
                int newCol = col + dir[1];

                // Skip if outside the image
                if (newRow < 0 || newRow >= m || newCol < 0 || newCol >= n) {
                    continue;
                }

                // Only cells with the original color are part of the region
                if (image[newRow][newCol] != originalColor) {
                    continue;
                }

                // Recolor this cell so it is marked as visited
                image[newRow][newCol] = color;

                // Push it into queue to expand further
                q.push({newRow, newCol});
            }
        }

        return image;
    }
};


class Solution {
public:
    int m, n;
    int originalColor;
    int newColor;

    vector<vector<int>> directions = {
        {-1, 0}, // up
        {1, 0},  // down
        {0, -1}, // left
        {0, 1}   // right
    };

    void dfs(vector<vector<int>>& image, int row, int col) {
        // If outside the image, stop.
        if (row < 0 || row >= m || col < 0 || col >= n) {
            return;
        }

        // If this cell is not of the original color,
        // it is not part of the connected region.
        if (image[row][col] != originalColor) {
            return;
        }

        // Recolor current cell.
        // This also marks it as visited.
        image[row][col] = newColor;

        // Explore all 4 neighbours.
        for (auto& dir : directions) {
            int newRow = row + dir[0];
            int newCol = col + dir[1];

            dfs(image, newRow, newCol);
        }
    }

    vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int color) {
        m = image.size();
        n = image[0].size();

        originalColor = image[sr][sc];
        newColor = color;

        // Important edge case:
        // if original color and new color are same,
        // DFS would keep revisiting cells.
        if (originalColor == newColor) {
            return image;
        }

        dfs(image, sr, sc);

        return image;
    }
};