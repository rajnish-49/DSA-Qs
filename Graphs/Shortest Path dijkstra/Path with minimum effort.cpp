class Solution {
public:
    int minimumEffortPath(vector<vector<int>>& heights) {
        int rows = heights.size();
        int cols = heights[0].size();

        // minEffort[r][c] stores the minimum effort needed
        // to reach cell (r, c) from the starting cell (0, 0).
        //
        // Effort of a path = maximum height difference
        // encountered between any two consecutive cells on that path.
        vector<vector<int>> minEffort(
            rows,
            vector<int>(cols, INT_MAX)
        );

        // Min-heap stores:
        // {effort needed to reach this cell, {row, col}}
        //
        // The cell with the smallest known effort is processed first.
        priority_queue<
            pair<int, pair<int, int>>,
            vector<pair<int, pair<int, int>>>,
            greater<pair<int, pair<int, int>>>
        > pq;

        // We start at (0, 0).
        // No movement has happened yet, so the effort is 0.
        minEffort[0][0] = 0;
        pq.push({0, {0, 0}});

        // Four possible movements:
        // up, down, left and right.
        int directions[4][2] = {
            {-1, 0},
            {1, 0},
            {0, -1},
            {0, 1}
        };

        while (!pq.empty()) {
            // Extract the cell having the smallest known effort.
            int effort = pq.top().first;
            int row = pq.top().second.first;
            int col = pq.top().second.second;
            pq.pop();

            // The same cell can be pushed into the heap multiple times.
            //
            // Example:
            // We may first reach a cell with effort 10,
            // then later find a better route with effort 6.
            //
            // Both entries remain in the heap.
            // If the old effort 10 is popped later, skip it.
            if (effort > minEffort[row][col]) {
                continue;
            }

            // Since Dijkstra always processes the cell with the
            // smallest effort first, the first time we pop the
            // destination, its effort is guaranteed to be minimum.
            if (row == rows - 1 && col == cols - 1) {
                return effort;
            }

            // Explore all four neighbouring cells.
            for (auto& direction : directions) {
                int newRow = row + direction[0];
                int newCol = col + direction[1];

                // Skip cells outside the grid.
                if (
                    newRow < 0 || newRow >= rows ||
                    newCol < 0 || newCol >= cols
                ) {
                    continue;
                }

                // Effort of taking only this one step.
                int heightDifference = abs(
                    heights[row][col] -
                    heights[newRow][newCol]
                );

                // 'effort' is the largest height difference seen
                // on the path used to reach the current cell.
                //
                // After taking this new edge, the effort of the
                // complete path becomes the larger of:
                //
                // 1. the previous path effort
                // 2. the current height difference
                int effortThroughCurrent = max(
                    effort,
                    heightDifference
                );

                // Relaxation:
                //
                // If reaching the neighbour through the current cell
                // requires less effort than the best route known so far,
                // update the neighbour's effort.
                if (
                    effortThroughCurrent <
                    minEffort[newRow][newCol]
                ) {
                    minEffort[newRow][newCol] =
                        effortThroughCurrent;

                    // Push the improved value so this cell
                    // can be processed later.
                    pq.push({
                        effortThroughCurrent,
                        {newRow, newCol}
                    });
                }
            }
        }

        // The grid always has a valid path from the top-left
        // to the bottom-right, so this line is only a fallback.
        return 0;
    }
};