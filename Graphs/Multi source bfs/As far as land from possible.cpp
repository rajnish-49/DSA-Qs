class Solution {
public:
    int maxDistance(vector<vector<int>>& grid) {
        int n = grid.size();

        queue<pair<int, int>> q;

        // dist[i][j] = minimum distance of this cell from any land cell.
        // -1 also acts as "unvisited", so a separate visited array is unnecessary.
        vector<vector<int>> dist(n, vector<int>(n, -1));

        // Start BFS from every land cell at the same time.
        // This guarantees that each water cell is first reached
        // from its nearest land cell.
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    q.push({i, j});
                    dist[i][j] = 0;
                }
            }
        }

        // No land means BFS has no source.
        // All land means there is no water cell whose distance can be calculated.
        if (q.empty() || q.size() == n * n) {
            return -1;
        }

        int directions[4][2] = {
            {-1, 0},
            {1, 0},
            {0, -1},
            {0, 1}
        };

        int answer = 0;

        while (!q.empty()) {
            pair<int, int> current = q.front();
            q.pop();

            int row = current.first;
            int col = current.second;

            for (int i = 0; i < 4; i++) {
                int newRow = row + directions[i][0];
                int newCol = col + directions[i][1];

                if (newRow >= 0 && newRow < n &&
                    newCol >= 0 && newCol < n &&
                    dist[newRow][newCol] == -1) {

                    // BFS moves one edge at a time, so this cell is exactly
                    // one step farther from land than the current cell.
                    dist[newRow][newCol] = dist[row][col] + 1;

                    // BFS distances increase level by level.
                    // The largest distance assigned belongs to the water cell
                    // farthest from its nearest land cell.
                    answer = max(answer, dist[newRow][newCol]);

                    // Marking the distance before pushing prevents the same cell
                    // from being inserted by multiple neighbouring cells.
                    q.push({newRow, newCol});
                }
            }
        }

        return answer;
    }
};