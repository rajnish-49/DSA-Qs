// @ https://leetcode.com/problems/rotting-oranges/

class Solution {
    public:
        int orangesRotting(vector<vector<int>>& grid) {
    
            // Get the number of rows and columns in the grid
            int n = grid.size();
            int m = grid[0].size();
    
            // Queue to store {{row, col}, time} of rotten oranges
            queue <pair<pair <int , int> , int >> q;
    
            // Visited matrix to track which oranges have been processed
            vector<vector<int>> vis(n, vector<int>(m, 0));
    
            // Step 1: Push all initially rotten oranges into the queue
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    if (grid[i][j] == 2) {
                        q.push({{i, j}, 0});  // Push rotten orange position with time 0
                        vis[i][j] = 2;  // Mark it as visited (already rotten)
                    } else {
                        vis[i][j] = 0;  // Initialize fresh oranges and empty cells as unvisited
                    }
                }
            }
    
            int tm = 0;  // Keeps track of the total time to rot all oranges
    
            // Direction vectors for 4 possible moves: up, right, down, left
            int delrow[] = {-1, 0, 1, 0}; 
            int delcol[] = {0, 1, 0, -1};
    
            // Step 2: Process the queue using BFS
            while (!q.empty()) {
                // Get the current rotten orange position and time from the front of the queue
                int r = q.front().first.first;
                int c = q.front().first.second;
                int t = q.front().second;
                q.pop();
    
                // Update maximum time taken
                tm = max(tm, t);
    
                // Explore all 4 directions
                for (int i = 0; i < 4; i++) {
                    int nrow = r + delrow[i]; 
                    int ncol = c + delcol[i];
    
                    // Check if the adjacent cell is within bounds and contains a fresh orange
                    if (nrow >= 0 && nrow < n && ncol >= 0 && ncol < m && 
                        vis[nrow][ncol] != 2 && grid[nrow][ncol] == 1) {
    
                        // Mark this orange as rotten
                        q.push({{nrow, ncol}, t + 1});
                        vis[nrow][ncol] = 2;  // Update visited matrix
                    }
                }
            }
    
            // Step 3: Check if any fresh orange is left unrotten
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    if (vis[i][j] != 2 && grid[i][j] == 1) {
                        return -1;  // If any fresh orange remains, return -1
                    }
                }
            }
    
            // Return the total time taken to rot all oranges
            return tm;
        }
    };
    