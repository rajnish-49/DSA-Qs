// @ https://www.geeksforgeeks.org/problems/shortest-path-in-a-binary-maze-1655453161/1

// Driver Code Ends
// User function Template for C++

class Solution {
public:
    int shortestPath(vector<vector<int>> &grid, pair<int, int> source,
                     pair<int, int> destination) {
        
        // Queue for BFS: stores (distance_so_far, (row, col))
        // Using distance explicitly makes it like "Dijkstra with equal weights"
        queue<pair<int, pair<int,int>>> q;
        
        int n = grid.size();       // Number of rows in the grid
        int m = grid[0].size();    // Number of columns in the grid

        // Distance matrix: stores shortest distance from source to each cell
        // Initialize all distances to "infinity"
        vector<vector<int>> dist(n, vector<int>(m, 1e9));

        // Distance to source is 0
        dist[source.first][source.second] = 0;

        // Start BFS from the source cell
        q.push({0, {source.first, source.second}});

        // 4-directional moves: Up, Right, Down, Left
        int dr[] = {-1, 0, 1, 0};
        int dc[] = {0, 1, 0, -1};

        // BFS traversal
        while(!q.empty()) {
            auto it = q.front();  // get the current cell
            q.pop();

            int dis = it.first;           // current distance from source
            int r = it.second.first;      // current row
            int c = it.second.second;     // current column

            // Explore all 4 neighbors
            for(int i = 0; i < 4; i++) {
                int newr = r + dr[i];     // compute neighbor row
                int newc = c + dc[i];     // compute neighbor column

                // Check bounds, whether the cell is walkable (1), and if the new distance is better
                if(newr >= 0 && newr < n && newc >= 0 && newc < m
                   && grid[newr][newc] == 1 && dis + 1 < dist[newr][newc]) {

                    // Update distance for neighbor
                    dist[newr][newc] = 1 + dis;

                    // Early exit: if neighbor is the destination, return distance immediately
                    if(newr == destination.first && newc == destination.second)
                        return dis + 1;

                    // Push neighbor into the queue with updated distance
                    q.push({1 + dis, {newr, newc}});
                }
            }
        }

        // If BFS completes without reaching destination, return -1
        return -1;
    }
};
