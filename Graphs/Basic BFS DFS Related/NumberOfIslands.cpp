// @ https://leetcode.com/problems/number-of-islands/description/

// SAME AS CONNECTED COMPONENTS 

class Solution {
    private:
    
        // BFS function to traverse and mark an island
        void bfs(int row, int col, vector<vector<int>> &vis, vector<vector<char>> &grid) {
            
            // Mark the current cell as visited
            vis[row][col] = 1;
    
            // Queue for BFS traversal (storing row and col positions)
            queue<pair<int, int>> q;
            q.push({row, col});
    
            // Grid dimensions
            int n = grid.size();
            int m = grid[0].size();
    
            // Arrays to move in 4 directions: Up, Right, Down, Left
            int drow[] = {-1, 0, 1, 0}; 
            int dcol[] = {0, 1, 0, -1}; 
    
            // BFS traversal
            while (!q.empty()) {
                // Get the front element from the queue
                int row = q.front().first;
                int col = q.front().second;
                q.pop();
    
                // Check all 4 possible movements (Up, Right, Down, Left)
                for (int k = 0; k < 4; k++) {  
                    int nrow = row + drow[k];  // New row position
                    int ncol = col + dcol[k];  // New col position
    
                    // Check if the new position is valid (within grid bounds)
                    if (nrow >= 0 && nrow < n && ncol >= 0 && ncol < m &&
                        grid[nrow][ncol] == '1' && !vis[nrow][ncol]) {  
                        
                        // Mark as visited
                        vis[nrow][ncol] = 1;
    
                        // Push into queue for further traversal
                        q.push({nrow, ncol});
                    }
                }
            }
        }
    
    public:
        int numIslands(vector<vector<char>>& grid) {
    
            // Get the number of rows and columns
            int n = grid.size();
            int m = grid[0].size();
    
            // Visited array to keep track of visited cells
            vector<vector<int>> vis(n, vector<int>(m, 0));
            
            int cnt = 0; // Counter to count number of islands
    
            // Traverse the entire grid
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    // If we find an unvisited land cell ('1')
                    if (!vis[i][j] && grid[i][j] == '1') {
                        cnt++;  // New island found, increment counter
                        bfs(i, j, vis, grid);  // Call BFS to mark the whole island
                    }
                }
            }
            return cnt;  // Return the total number of islands found
        }
    };
    

    // while (!q.empty()) {
    //     int row = q.front().first;
    //     int col = q.front().second;
    //     q.pop();
    
    //     // Traversing all 8 directions using two loops
    //     for (int delrow = -1; delrow <= 1; delrow++) {
    //         for (int delcol = -1; delcol <= 1; delcol++) {
    //             if (delrow == 0 && delcol == 0) continue; // Skip the current cell itself
    
    //             int nrow = row + delrow;
    //             int ncol = col + delcol;
    
    //             if (nrow >= 0 && nrow < n && ncol >= 0 && ncol < m &&
    //                 grid[nrow][ncol] == '1' && !vis[nrow][ncol]) {
    //                 vis[nrow][ncol] = 1;
    //                 q.push({nrow, ncol});
    //             }
    //         }
    //     }
    // }
    