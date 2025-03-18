//@ https://leetcode.com/problems/surrounded-regions/

class Solution {
    private:
        // Depth-First Search (DFS) function to traverse and mark connected 'O's
        void dfs(int row, int col, vector<vector<char>>& board,
                 vector<vector<int>>& vis) {
    
            int n = board.size();   // Number of rows
            int m = board[0].size(); // Number of columns
    
            vis[row][col] = 1; // Mark the current cell as visited
    
            // Possible directions for movement: Up, Right, Down, Left
            int delrow[] = {-1, 0, 1, 0};
            int delcol[] = {0, 1, 0, -1};
    
            // Explore all 4 possible directions
            for (int i = 0; i < 4; i++) {
                int nrow = row + delrow[i]; // Calculate new row index
                int ncol = col + delcol[i]; // Calculate new column index
    
                // Check if the new position is inside the board, not visited, and contains 'O'
                if (nrow >= 0 && nrow < n && ncol >= 0 && ncol < m &&
                    !vis[nrow][ncol]  && board[nrow][ncol] == 'O') {
                    dfs(nrow , ncol , board , vis); // Recursively visit the connected 'O'
                }
            }
        }
    
    public:
        void solve(vector<vector<char>>& board) {
            int n = board.size();   // Number of rows
            int m = board[0].size(); // Number of columns
    
            vector<vector<int>> vis(n, vector<int>(m, 0)); // Visited matrix initialized to 0
    
            // Step 1: Mark all 'O's connected to the top and bottom boundary
            for (int j = 0; j < m; j++) {
                // Check the first row (top boundary)
                if (!vis[0][j] && board[0][j] == 'O') {
                    dfs(0, j, board, vis);
                }
    
                // Check the last row (bottom boundary)
                if (!vis[n - 1][j] && board[n - 1][j] == 'O') {
                    dfs(n - 1, j, board, vis);
                }
            }
    
            // Step 2: Mark all 'O's connected to the left and right boundary
            for (int i = 0; i < n; i++) {
                // Check the first column (left boundary)
                if (!vis[i][0] && board[i][0] == 'O') {
                    dfs(i, 0, board, vis);
                }
    
                // Check the last column (right boundary)
                if (!vis[i][m - 1] && board[i][m - 1] == 'O') {
                    dfs(i, m - 1, board, vis);
                }
            }
    
            // Step 3: Convert all unvisited 'O's to 'X'
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    // If an 'O' is not marked as visited, it is enclosed and should be changed to 'X'
                    if (!vis[i][j] && board[i][j] == 'O') {
                        board[i][j] = 'X';
                    }
                }
            }
        }
    };
    

