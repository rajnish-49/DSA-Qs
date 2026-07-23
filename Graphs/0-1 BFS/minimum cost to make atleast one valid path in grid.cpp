// @ https://leetcode.com/problems/minimum-cost-to-make-at-least-one-valid-path-in-a-grid/

class Solution {
public:
    int minCost(vector<vector<int>>& grid) {

        int m = grid.size();
        int n = grid[0].size();

        /*
            dist[i][j] = minimum cost needed to reach cell (i, j)

            Cost means:
            number of arrows we have changed so far.
        */
        vector<vector<int>> dist(m, vector<int>(n, INT_MAX));

        /*
            0-1 BFS because every move has cost either 0 or 1.

            cost 0 -> move in the direction already shown by current cell's arrow
            cost 1 -> move in some other direction, so we must change current cell's arrow
        */
        deque<pair<int, int>> dq;

        // Start from top-left with 0 cost
        dist[0][0] = 0;
        dq.push_front({0, 0});

        /*
            Each entry stores:
            {row_change, col_change, direction_value}

            Problem direction values:
            1 = right
            2 = left
            3 = down
            4 = up
        */
        vector<vector<int>> directions = {
            {0, 1, 1},   // move right, arrow value 1
            {0, -1, 2},  // move left,  arrow value 2
            {1, 0, 3},   // move down,  arrow value 3
            {-1, 0, 4}   // move up,    arrow value 4
        };

        while (!dq.empty()) {
            auto [r, c] = dq.front();
            dq.pop_front();

            // Try moving from current cell to all 4 adjacent cells
            for (auto &dir : directions) {
                int nr = r + dir[0];
                int nc = c + dir[1];

                // Direction value of the move we are trying
                int dirValue = dir[2];

                if (nr < 0 || nr >= m || nc < 0 || nc >= n)
                    continue;

                /*
                    Cost depends on the arrow of the CURRENT cell.

                    If grid[r][c] already points in the direction we are moving:
                        cost = 0

                    Otherwise:
                        we need to change the current cell's arrow
                        cost = 1
                */
                int cost = (grid[r][c] == dirValue) ? 0 : 1;

                int newCost = dist[r][c] + cost;

                // Relaxation: found a cheaper way to reach (nr, nc)
                if (newCost < dist[nr][nc]) {
                    dist[nr][nc] = newCost;

                    /*
                        0-cost move should be processed earlier,
                        because total cost did not increase.
                    */
                    if (cost == 0) {
                        dq.push_front({nr, nc});
                    }

                    /*
                        1-cost move should be processed later,
                        because total cost increased by 1.
                    */
                    else {
                        dq.push_back({nr, nc});
                    }
                }
            }
        }

        // Minimum number of arrow changes needed to create at least one valid path
        return dist[m - 1][n - 1];
    }
};