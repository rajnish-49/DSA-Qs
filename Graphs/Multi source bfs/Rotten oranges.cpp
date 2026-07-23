class Solution {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();

        // Movement from one cell to its 4 adjacent cells.
        vector<vector<int>> directions = {
            {-1, 0}, // up
            {1, 0},  // down
            {0, -1}, // left
            {0, 1}   // right
        };

        /*
            Queue stores:
            {{row, column}, time at which this orange becomes rotten}

            This is multi-source BFS because every orange that is already
            rotten at time 0 is inserted into the queue before BFS starts.
        */
        queue<pair<pair<int, int>, int>> q;

        // Number of fresh oranges that still need to become rotten.
        int fresh = 0;

        /*
            Initialisation:

            1. Every initially rotten orange is a BFS source with time 0.
            2. Count all fresh oranges so that after BFS we can check
               whether every fresh orange was reachable.
        */
        for (int row = 0; row < m; row++) {
            for (int col = 0; col < n; col++) {

                if (grid[row][col] == 2) {
                    q.push({{row, col}, 0});
                }
                else if (grid[row][col] == 1) {
                    fresh++;
                }
            }
        }

        // Final answer: the latest time at which any orange becomes rotten.
        int time = 0;

        /*
            BFS processes oranges in increasing order of time.

            When an orange becomes rotten at currTime, it can rot each
            adjacent fresh orange at currTime + 1.

            Since all initially rotten oranges were inserted together,
            the rotting spreads from all of them simultaneously.
        */
        while (!q.empty()) {
            auto current = q.front();
            q.pop();

            int row = current.first.first;
            int col = current.first.second;
            int currTime = current.second;

            // Keep track of the maximum time reached during BFS.
            time = max(time, currTime);

            // Try spreading rot to all 4 adjacent cells.
            for (auto& direction : directions) {
                int newRow = row + direction[0];
                int newCol = col + direction[1];

                // The neighbouring cell must lie inside the grid.
                if (newRow < 0 || newRow >= m ||
                    newCol < 0 || newCol >= n) {
                    continue;
                }

                /*
                    Rot can spread only to a fresh orange.

                    Empty cells cannot be crossed.
                    Already rotten oranges do not need to be processed again.
                */
                if (grid[newRow][newCol] != 1) {
                    continue;
                }

                /*
                    Mark the orange rotten immediately when adding it
                    to the queue.

                    This acts as the visited marking and prevents the same
                    fresh orange from being added by multiple neighbours.
                */
                grid[newRow][newCol] = 2;
                fresh--;

                // The neighbouring orange rots one minute later.
                q.push({{newRow, newCol}, currTime + 1});
            }
        }

        /*
            If fresh > 0, some fresh oranges were isolated by empty cells
            and could not be reached by any rotten orange.
        */
        if (fresh > 0) {
            return -1;
        }

        // Includes the case where there were no fresh oranges initially.
        return time;
    }
};