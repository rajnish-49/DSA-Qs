// @ https://www.geeksforgeeks.org/problems/number-of-distinct-islands/0

class Solution {
    private:
    
    // DFS function to explore and record the shape of an island
    void dfs(int row , int col , vector<vector<int>> &vis, vector<vector<int>>& grid,
             vector<pair<int,int>> &vec, int baser, int basec) {
        
        int n = grid.size();
        int m = grid[0].size();
        
        // Mark the current cell as visited
        vis[row][col] = 1;
        
        // Store the relative position of the current cell
        // (relative to the first cell of this island)
        vec.push_back({row - baser , col - basec});
        
        // Arrays for moving in 4 directions (Up, Right, Down, Left)
        int drow[] = {-1, 0, 1, 0}; 
        int dcol[] = {0, 1, 0, -1};
        
        // Explore all 4 possible directions
        for(int i = 0; i < 4; i++) {
            int nrow = row + drow[i];  // New row position
            int ncol = col + dcol[i];  // New col position
            
            // Check if the new position is valid (within grid bounds)
            if (nrow >= 0 && nrow < n && ncol >= 0 && ncol < m &&
                grid[nrow][ncol] == 1 && !vis[nrow][ncol]) {
                
                // Recursively call DFS on the next land cell
                dfs(nrow, ncol, vis, grid, vec, baser, basec);
            }
        }
    }
    
  public:
    int countDistinctIslands(vector<vector<int>>& grid) {
        
        int n = grid.size();
        int m = grid[0].size();
        
        // Visited matrix to keep track of visited land cells
        vector<vector<int>> vis(n , vector<int>(m, 0));
        
        // Set to store unique island shapes
        set<vector<pair<int,int>>> st;
        
        // Traverse the entire grid
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++) {
                
                // If we find an unvisited land cell ('1'), it's a new island
                if (!vis[i][j] && grid[i][j] == 1) {
                    
                    vector<pair<int,int>> vec;  // To store shape of current island
                    
                    // Start DFS from the current land cell
                    // Pass (i, j) as the base coordinates
                    dfs(i, j, vis, grid, vec, i, j);
                    
                    // Insert the relative shape of the island into the set
                    st.insert(vec);
                }
            }
        }
        
        // Return the number of distinct island shapes
        return st.size();
    }
};


