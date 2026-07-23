class Solution {
public:
    int nearestExit(vector<vector<char>>& maze, vector<int>& entrance) {
        int m = maze.size();
        int n = maze[0].size();

        // 4 directions: up, down, left, right
        vector<vector<int>> directions = {
            {-1, 0}, // up
            {1, 0},  // down
            {0, -1}, // left
            {0, 1}   // right
        };

        int startRow = entrance[0];
        int startCol = entrance[1];

        // Queue stores: {{row, col}, distance_from_entrance}
        queue<pair<pair<int, int>, int>> q;

        q.push({{startRow, startCol}, 0});

        // Mark entrance as visited.
        // We can mark visited by converting '.' to '+'
        // because '+' means blocked/unusable.
        maze[startRow][startCol] = '+';

        while (!q.empty()) {
            auto cell = q.front();
            q.pop();

            int row = cell.first.first;
            int col = cell.first.second;
            int dist = cell.second;

            // Try moving in all 4 directions
            for (auto& dir : directions) {
                int newRow = row + dir[0];
                int newCol = col + dir[1];

                // Skip if outside the maze
                if (newRow < 0 || newRow >= m || newCol < 0 || newCol >= n) {
                    continue;
                }

                // Skip walls and already visited cells
                if (maze[newRow][newCol] == '+') {
                    continue;
                }

                // If this valid empty cell is on boundary,
                // then it is an exit.
                // Since we reached it from current cell,
                // answer is dist + 1.
                if (newRow == 0 || newRow == m - 1 || newCol == 0 || newCol == n - 1) {
                    return dist + 1;
                }

                // Mark visited before pushing to avoid duplicate pushes
                maze[newRow][newCol] = '+';

                // Continue BFS
                q.push({{newRow, newCol}, dist + 1});
            }
        }

        // No reachable exit found
        return -1;
    }
};