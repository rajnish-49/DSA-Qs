class Solution {
public:
    int swimInWater(vector<vector<int>>& grid) {
        int n = grid.size();

        // minTime[r][c] stores the earliest time at which
        // we can reach cell (r, c) from the starting cell.
        //
        // For any path, the required time is the maximum
        // elevation present on that path.
        vector<vector<int>> minTime(
            n,
            vector<int>(n, INT_MAX)
        );

        // Min-heap entry:
        // {time required to reach this cell, {row, col}}
        //
        // We always process the cell that can currently
        // be reached at the earliest time.
        priority_queue<
            pair<int, pair<int, int>>,
            vector<pair<int, pair<int, int>>>,
            greater<pair<int, pair<int, int>>>
        > pq;

        // We can stand on the starting cell only when
        // the water level reaches grid[0][0].
        minTime[0][0] = grid[0][0];
        pq.push({grid[0][0], {0, 0}});

        // Four possible movements:
        // up, down, left and right.
        int directions[4][2] = {
            {-1, 0},
            {1, 0},
            {0, -1},
            {0, 1}
        };

        while (!pq.empty()) {
            // Extract the cell that requires the smallest
            // water level among all current possibilities.
            int currentTime = pq.top().first;
            int row = pq.top().second.first;
            int col = pq.top().second.second;
            pq.pop();

            // The same cell may be inserted multiple times.
            //
            // Example:
            // First, we may find a route requiring time 8.
            // Later, we may find a better route requiring time 5.
            //
            // If the older value 8 is popped later, skip it.
            if (currentTime > minTime[row][col]) {
                continue;
            }

            // Because the heap processes the smallest time first,
            // the first time the destination is popped,
            // its minimum required time has been found.
            if (row == n - 1 && col == n - 1) {
                return currentTime;
            }

            // Explore all four neighbouring cells.
            for (auto& dir : directions) {
                int newRow = row + dir[0];
                int newCol = col + dir[1];

                // Ignore positions outside the grid.
                if (
                    newRow < 0 || newRow >= n ||
                    newCol < 0 || newCol >= n
                ) {
                    continue;
                }

                // currentTime represents the highest elevation
                // encountered on the path to the current cell.
                //
                // To include the neighbour, the water level must be
                // high enough for both:
                //
                // 1. all cells already used on the path
                // 2. the neighbour's elevation
                //
                // Therefore, the required time for the extended path
                // is the larger of these two values.
                int timeThroughCurrent = max(
                    currentTime,
                    grid[newRow][newCol]
                );

                // Relaxation:
                //
                // If this route reaches the neighbour at an earlier
                // time than the best route found before, update it.
                if (timeThroughCurrent < minTime[newRow][newCol]) {
                    minTime[newRow][newCol] = timeThroughCurrent;

                    // Push the improved route into the min-heap.
                    pq.push({
                        timeThroughCurrent,
                        {newRow, newCol}
                    });
                }
            }
        }

        // A valid path always exists in this problem,
        // so this is only a fallback.
        return -1;
    }
};