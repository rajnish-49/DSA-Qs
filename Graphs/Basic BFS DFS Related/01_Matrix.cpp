//@ https://leetcode.com/problems/01-matrix/

class Solution {
    public:
        vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
    
            int n = mat.size();  // Number of rows
            int m = mat[0].size();  // Number of columns
    
            // Create a visited matrix to keep track of visited cells
            vector<vector<int>> vis(n, vector<int>(m, 0));
    
            // Distance matrix to store the shortest distance to the nearest 0
            vector<vector<int>> dist(n, vector<int>(m, -1));
    
            // Queue for BFS traversal, storing (row, col) and the distance from the nearest 0
            queue<pair<pair<int,int>,int>> q;
    
            // Step 1: Initialize the queue with all 0s in the matrix
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    if (mat[i][j] == 0) {
                        q.push({{i, j}, 0});  // Push 0 cells into the queue with distance 0
                        vis[i][j] = 1;  // Mark them as visited
                    }
                    else {
                        vis[i][j] = 0;  // Mark 1 cells as unvisited initially
                    }
                }
            }
    
            // Directions for moving up, right, down, and left
            int delrow[] = {-1, 0, 1, 0};  
            int delcol[] = {0, 1, 0, -1};
    
            // Step 2: BFS Traversal
            while (!q.empty()) {
                int row = q.front().first.first;  // Current row
                int col = q.front().first.second; // Current column
                int steps = q.front().second;  // Distance from nearest 0
                q.pop();
    
                // Update the distance for the current cell
                dist[row][col] = steps;
    
                // Explore all 4 possible directions
                for (int i = 0; i < 4; i++) {
                    int nrow = row + delrow[i];  // New row position
                    int ncol = col + delcol[i];  // New column position
    
                    // Check if the new position is within bounds and not visited
                    if (nrow >= 0 && nrow < n && ncol >= 0 && ncol < m && vis[nrow][ncol] == 0) {
                        vis[nrow][ncol] = 1;  // Mark as visited
                        q.push({{nrow, ncol}, steps + 1});  // Push into queue with incremented distance
                    }
                }
            }
    
            // Step 3: Return the final distance matrix
            return dist;
        }
    };

    
    // WITHOUT EXTRA SPACE

    #include <iostream>
    #include <vector>
    #include <queue>
    #include <climits>
    
    using namespace std;
    
    vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
        int n = mat.size();
        int m = mat[0].size();
    
        queue<pair<int, int>> q;
        vector<vector<int>> dist(n, vector<int>(m, INT_MAX));  // Use INT_MAX instead of -1
    
        // Step 1: Push all '0' cells into the queue and initialize distances
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (mat[i][j] == 0) {
                    q.push({i, j});
                    dist[i][j] = 0;  // Distance for 0 is already 0
                }
            }
        }
    
        // Step 2: Define directions for moving in 4 directions
        int delrow[] = {-1, 0, 1, 0};
        int delcol[] = {0, 1, 0, -1};
    
        // Step 3: Perform BFS
        while (!q.empty()) {
            int row = q.front().first;
            int col = q.front().second;
            q.pop();
    
            for (int i = 0; i < 4; i++) {
                int nrow = row + delrow[i];
                int ncol = col + delcol[i];
    
                // Check if inside bounds and not already at minimum distance
                if (nrow >= 0 && nrow < n && ncol >= 0 && ncol < m) {
                    if (dist[nrow][ncol] > dist[row][col] + 1) {
                        dist[nrow][ncol] = dist[row][col] + 1;
                        q.push({nrow, ncol});
                    }
                }
            }
        }
    
        return dist;
    }
    