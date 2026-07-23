//@ https://leetcode.com/problems/surrounded-regions/

class Solution {
public:
    int m, n;

    vector<vector<int>> directions = {
        {-1, 0}, // up
        {1, 0},  // down
        {0, -1}, // left
        {0, 1}   // right
    };

    void dfs(vector<vector<char>>& board, int row, int col) {
        // If outside the board, stop.
        if (row < 0 || row >= m || col < 0 || col >= n) {
            return;
        }

        // We only care about boundary-connected 'O'.
        // If current cell is not 'O', stop.
        if (board[row][col] != 'O') {
            return;
        }

        // Mark this 'O' as safe.
        // It is connected to boundary, so it should not be flipped.
        board[row][col] = '#';

        // Explore all 4 directions.
        for (auto& dir : directions) {
            int newRow = row + dir[0];
            int newCol = col + dir[1];

            dfs(board, newRow, newCol);
        }
    }

    void solve(vector<vector<char>>& board) {
        m = board.size();
        n = board[0].size();

        // Step 1:
        // Start DFS from all boundary 'O's.
        // Any 'O' connected to boundary is safe.

        // First row and last row
        for (int col = 0; col < n; col++) {
            if (board[0][col] == 'O') {
                dfs(board, 0, col);
            }

            if (board[m - 1][col] == 'O') {
                dfs(board, m - 1, col);
            }
        }

        // First column and last column
        for (int row = 0; row < m; row++) {
            if (board[row][0] == 'O') {
                dfs(board, row, 0);
            }

            if (board[row][n - 1] == 'O') {
                dfs(board, row, n - 1);
            }
        }

        // Step 2:
        // Remaining 'O's are surrounded, so flip them to 'X'.
        // Safe cells marked as '#' should be restored to 'O'.
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {

                if (board[i][j] == 'O') {
                    board[i][j] = 'X';
                }

                else if (board[i][j] == '#') {
                    board[i][j] = 'O';
                }
            }
        }
    }
};


class Solution {
public:
    void solve(vector<vector<char>>& board) {
        int m = board.size();
        int n = board[0].size();

        vector<vector<int>> directions = {
            {-1, 0}, // up
            {1, 0},  // down
            {0, -1}, // left
            {0, 1}   // right
        };

        queue<pair<int, int>> q;

        // Step 1:
        // Push all boundary 'O's into queue and mark them safe.

        // First row and last row
        for (int col = 0; col < n; col++) {
            if (board[0][col] == 'O') {
                q.push({0, col});
                board[0][col] = '#';
            }

            if (board[m - 1][col] == 'O') {
                q.push({m - 1, col});
                board[m - 1][col] = '#';
            }
        }

        // First column and last column
        for (int row = 0; row < m; row++) {
            if (board[row][0] == 'O') {
                q.push({row, 0});
                board[row][0] = '#';
            }

            if (board[row][n - 1] == 'O') {
                q.push({row, n - 1});
                board[row][n - 1] = '#';
            }
        }

        // Step 2:
        // BFS from all boundary-connected 'O's.
        while (!q.empty()) {
            auto [row, col] = q.front();
            q.pop();

            for (auto& dir : directions) {
                int newRow = row + dir[0];
                int newCol = col + dir[1];

                // Skip outside cells.
                if (newRow < 0 || newRow >= m || newCol < 0 || newCol >= n) {
                    continue;
                }

                // Only unvisited 'O' cells are part of safe region.
                if (board[newRow][newCol] != 'O') {
                    continue;
                }

                // Mark this cell safe.
                board[newRow][newCol] = '#';

                // Push for further BFS expansion.
                q.push({newRow, newCol});
            }
        }

        // Step 3:
        // Flip surrounded 'O' to 'X'.
        // Restore safe '#' back to 'O'.
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {

                if (board[i][j] == 'O') {
                    board[i][j] = 'X';
                }

                else if (board[i][j] == '#') {
                    board[i][j] = 'O';
                }
            }
        }
    }
};