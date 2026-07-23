// @https://leetcode.com/problems/minimum-cost-to-make-at-least-one-valid-path-in-a-grid/

class Solution {
public:
    int minCost(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();

        /*
            dist[i][j] = minimum cost needed to reach cell (i, j)

            Cost means:
            number of arrows changed so far.
        */
        vector<vector<int>> dist(m, vector<int>(n, INT_MAX));

        /*
            Min-heap stores:
            {cost_so_far, row, col}

            Dijkstra always processes the cell
            with the smallest cost first.
        */
        priority_queue<
            vector<int>,
            vector<vector<int>>,
            greater<vector<int>>
        > pq;

        // Start from top-left with cost 0
        dist[0][0] = 0;
        pq.push({0, 0, 0});

        /*
            Each direction stores:
            {row_change, col_change, direction_value}

            Problem direction values:
            1 = right
            2 = left
            3 = down
            4 = up
        */
        vector<vector<int>> directions = {
            {0, 1, 1},    // move right, arrow value 1
            {0, -1, 2},   // move left,  arrow value 2
            {1, 0, 3},    // move down,  arrow value 3
            {-1, 0, 4}    // move up,    arrow value 4
        };

        while (!pq.empty()) {
            auto curr = pq.top();
            pq.pop();

            int currCost = curr[0];
            int r = curr[1];
            int c = curr[2];

            /*
                Ignore outdated heap entries.

                Example:
                If we earlier pushed this cell with cost 5,
                but later found cost 3, then cost 5 is useless.
            */
            if (currCost > dist[r][c]) continue;

            // Try moving from current cell to all 4 neighboring cells
            for (auto& dir : directions) {
                int nr = r + dir[0];
                int nc = c + dir[1];

                // Direction value of the move we are trying
                int dirValue = dir[2];

                if (nr < 0 || nr >= m || nc < 0 || nc >= n)
                    continue;

                /*
                    Cost depends on the arrow of the CURRENT cell.

                    If grid[r][c] already points in the direction
                    we are trying to move, cost is 0.

                    Otherwise, we must change this cell's arrow,
                    so cost is 1.
                */
                int cost = (grid[r][c] == dirValue) ? 0 : 1;

                int newCost = currCost + cost;

                // Relaxation: found a cheaper way to reach (nr, nc)
                if (newCost < dist[nr][nc]) {
                    dist[nr][nc] = newCost;
                    pq.push({newCost, nr, nc});
                }
            }
        }

        // Minimum arrow changes needed to reach bottom-right
        return dist[m - 1][n - 1];
    }
};