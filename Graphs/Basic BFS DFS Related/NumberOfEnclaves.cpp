// @ https://leetcode.com/problems/number-of-enclaves/

class Solution {
    private:

    // BFS function to mark all connected land cells from a given boundary cell
    void bfs(int row , int col , vector<vector<int>> &vis , vector<vector<int>>& grid ){

        vis[row][col] = 1; // Mark the current cell as visited

        queue<pair<int, int>> q; // Queue for BFS traversal
        q.push({row, col}); // Push the starting boundary cell into the queue

        // Arrays to explore 4 possible directions (up, right, down, left)
        int drow[] = {-1, 0, 1, 0}; 
        int dcol[] = {0, 1, 0, -1};

        while(!q.empty()){ // Continue until all reachable cells are visited

            int row = q.front().first; // Get current row
            int col = q.front().second; // Get current column
            q.pop(); // Remove the processed cell from the queue

            // Traverse all 4 possible directions
            for(int i = 0; i < 4; i++){
                int nrow = row + drow[i]; // Calculate new row index
                int ncol = col + dcol[i]; // Calculate new column index

                // Check if the new position is within bounds and is an unvisited land cell
                if(nrow >= 0 && nrow < grid.size() && ncol >= 0 && ncol < grid[0].size() && 
                   !vis[nrow][ncol] && grid[nrow][ncol] == 1)
                {
                    vis[nrow][ncol] = 1; // Mark the cell as visited
                    q.push({nrow, ncol}); // Add it to the queue for further traversal
                }
            }
        }
    }

public:
    int numEnclaves(vector<vector<int>>& grid) {

        int n = grid.size();  // Number of rows
        int m = grid[0].size();  // Number of columns

        // Visited array to keep track of visited land cells
        vector<vector<int>> vis(n, vector<int> (m, 0));

        // Traverse the first and last row to eliminate boundary-connected land cells
        for(int j = 0; j < m; j++){

            // If the first row has an unvisited land cell, start BFS
            if(!vis[0][j] && grid[0][j] == 1){
                bfs(0 , j , vis , grid);
            }

            // If the last row has an unvisited land cell, start BFS
            if(!vis[n-1][j] && grid[n-1][j] == 1){
                bfs(n-1 , j , vis , grid);
            }
        }

        // Traverse the first and last column to eliminate boundary-connected land cells
        for(int i = 0; i < n; i++){

            // If the first column has an unvisited land cell, start BFS
            if(!vis[i][0] && grid[i][0] == 1){
                bfs(i , 0 , vis , grid);
            }

            // If the last column has an unvisited land cell, start BFS
            if(!vis[i][m-1] && grid[i][m-1] == 1){
                bfs(i , m-1 , vis, grid);
            }
        }

        int count = 0; // To count land cells that are not connected to the boundary

        // Count the number of land cells that are not visited (i.e., enclaved land)
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                if(grid[i][j] == 1 && vis[i][j] == 0){
                    count++; // Increment the count for enclaved land cells
                }
            }
        }

        return count; // Return the number of enclaved land cells
    }
};


