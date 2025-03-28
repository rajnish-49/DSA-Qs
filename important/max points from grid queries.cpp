// @ https://leetcode.com/problems/maximum-number-of-points-from-grid-queries/description/?envType=daily-question&envId=2025-03-28

class Solution {
    private:
        // Performs BFS to count the number of cells with values < `value` reachable from (0,0)
        int bfs(vector<vector<int>>& grid, int value, int m, int n) {
            // If the starting cell (0,0) is > `value`, we can't proceed (return 0 points)
            if (grid[0][0] > value) {
                return 0;
            }
    
            // Queue for BFS traversal (stores {row, col} pairs)
            queue<pair<int, int>> q;
            q.push({0, 0}); // Start BFS from top-left cell (0,0)
    
            // Visited matrix (0 = unvisited, 1 = visited) to avoid revisiting cells
            vector<vector<int>> vis(m, vector<int>(n, 0));
            vis[0][0] = 1; // Mark starting cell as visited
    
            int points = 0; // Tracks the number of valid cells visited
    
            while (!q.empty()) {
                // Get current cell coordinates
                int row = q.front().first;
                int col = q.front().second;
                q.pop(); // Remove from queue
    
                points++; // Increment points (since we're visiting this cell)
    
                // Directions: up, right, down, left (delta row, delta col)
                int delrow[] = {-1, 0, 1, 0};
                int delcol[] = {0, 1, 0, -1};
    
                // Check all 4 adjacent cells
                for (int i = 0; i < 4; i++) {
                    int nrow = row + delrow[i]; // New row
                    int ncol = col + delcol[i]; // New column
    
                    // Check if new cell is within bounds, unvisited, and has value < `value`
                    if (nrow >= 0 && nrow < m && ncol >= 0 && ncol < n && 
                        vis[nrow][ncol] == 0 && grid[nrow][ncol] < value) {
                        vis[nrow][ncol] = 1; // Mark as visited
                        q.push({nrow, ncol}); // Add to queue for further exploration
                    }
                }
            }
    
            return points; // Total points collected for this query
        }
    
    public:
        // Processes each query and returns the max points for each
        vector<int> maxPoints(vector<vector<int>>& grid, vector<int>& queries) {
            int m = grid.size();    // Number of rows in grid
            int n = grid[0].size(); // Number of columns in grid
            int q = queries.size(); // Number of queries
    
            vector<int> ans(q, 0);  // Initialize answer array with 0s
    
            // For each query, compute max points using BFS
            for (int i = 0; i < q; i++) {
                ans[i] = bfs(grid, queries[i], m, n);
            }
    
            return ans; // Final results for all queries
        }
    };


    class Solution {
        private:
        public:
            vector<int> maxPoints(vector<vector<int>>& grid, vector<int>& queries) {
        
                int m = grid.size();       // Number of rows in the grid
                int n = grid[0].size();    // Number of columns in the grid
                int q = queries.size();    // Number of queries
        
                vector<int> result(q, 0);  // Stores the final results for each query
        
                // Pairing each query with its original index before sorting
                vector<pair<int, int>> sorted;
                for (int i = 0; i < q; i++) {
                    sorted.push_back({queries[i], i});
                }
        
                // Sorting queries in increasing order to process them sequentially
                sort(sorted.begin(), sorted.end());
        
                // Min-Heap to process grid cells in increasing order of value
                priority_queue<vector<int>, vector<vector<int>>, greater<>> pq;
        
                // 2D vector to track visited cells, initialized with 0 (false)
                vector<vector<int>> vis(m, vector<int>(n, 0));
                int points = 0;  // Keeps track of the number of accessible cells
        
                // Start from the top-left cell (0,0)
                pq.push({grid[0][0], 0, 0});
                vis[0][0] = true; // Mark the starting cell as visited
        
                // Process each query in sorted order
                for (int i = 0; i < q; i++) {
        
                    int value = sorted[i].first;  // Query value to compare grid cells
                    int idx = sorted[i].second;   // Original index of the query
        
                    // Expand the region while the smallest element in the heap is < query value
                    while (!pq.empty() && pq.top()[0] < value) {
        
                        int i_ = pq.top()[1]; // Row index of the current cell
                        int j_ = pq.top()[2]; // Column index of the current cell
                        pq.pop(); // Remove the processed cell from the heap
        
                        points++; // Increase the count of accessible cells
        
                        // Directions for moving up, right, down, and left
                        int delrow[] = {-1, 0, 1, 0};  
                        int delcol[] = {0, 1, 0, -1};
        
                        // Explore all 4 possible directions
                        for (int d = 0; d < 4; d++) {
                            int nrow = i_ + delrow[d];  
                            int ncol = j_ + delcol[d];  
        
                            // Check if the new position is within grid bounds and unvisited
                            if (nrow >= 0 && nrow < m && ncol >= 0 && ncol < n && vis[nrow][ncol] == 0) {
                                vis[nrow][ncol] = 1;  // Mark the cell as visited
                                pq.push({grid[nrow][ncol], nrow, ncol}); // Push new cell to the heap
                            }
                        }
                    }
        
                    result[idx] = points; // Store the number of accessible cells for the current query
                }
        
                return result; // Return results for all queries
            }
        };
        