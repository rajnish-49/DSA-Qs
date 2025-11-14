// @ 

class Solution {
public:
    int minimumEffortPath(vector<vector<int>>& heights) {

        int n = heights.size();
        int m = heights[0].size();

        // Priority queue (min-heap) for Dijkstra-style processing
        // The heap always pops the cell with the currently smallest "effort" so far.
        // This ensures that when we first reach the destination, it is guaranteed
        // to be via the path with minimum effort.
        priority_queue<pair<int, pair<int, int>>,
                       vector<pair<int, pair<int, int>>>,
                       greater<pair<int, pair<int, int>>>>
            pq;

        // dist[i][j] = minimum effort required to reach cell (i, j)
        // Initialize with infinity, except the starting cell (0,0)
        vector<vector<int>> dist(n, vector<int>(m, 1e9));
        dist[0][0] = 0;  // Starting cell requires 0 effort to reach

        pq.push({0, {0, 0}}); // Push starting cell into heap

        // Directions for Up, Right, Down, Left movement
        int dr[] = {-1, 0, 1, 0};
        int dc[] = {0, 1, 0, -1};

        while (!pq.empty()) {

            auto it = pq.top();
            pq.pop();

            int effort = it.first;         // Current max jump along path
            int row = it.second.first;
            int col = it.second.second;

            // ✅ Key point: if we reached destination first time from the heap,
            // it is guaranteed to be via minimum-effort path. No need to explore further.
            if (row == n - 1 && col == m - 1)
                return effort;

            // Explore all 4 neighboring cells
            for (int i = 0; i < 4; i++) {
                int nrow = row + dr[i];
                int ncol = col + dc[i];

                // Skip out-of-bound neighbors
                if (nrow >= 0 && nrow < n && ncol >= 0 && ncol < m) {

                    // Step effort = effort to move from current cell to neighbor
                    int stepEffort = abs(heights[nrow][ncol] - heights[row][col]);

                    // Path effort = maximum jump along the path
                    // Why max()?
                    // - The total effort of a path is defined as the **largest jump along it**
                    // - If we came from a cell where max jump so far was 3, and the new step is 5,
                    //   the path effort becomes max(3,5) = 5
                    // - This ensures we always keep track of the true "largest jump" along the path
                    int newEffort = max(dist[row][col], stepEffort);

                    // Relaxation: only update neighbor if this new path offers smaller effort
                    // This is similar to Dijkstra: we only push better paths into the heap
                    if (newEffort < dist[nrow][ncol]) {
                        dist[nrow][ncol] = newEffort;
                        pq.push({newEffort, {nrow, ncol}});
                    }
                }
            }
        }

        // Destination unreachable (problem constraints guarantee at least one path)
        return dist[n - 1][m - 1];
    }
};
