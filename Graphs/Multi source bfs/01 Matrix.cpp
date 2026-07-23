class Solution {
public:
    vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
        int m = mat.size();
        int n = mat[0].size();

        // Movement from one cell to its 4 adjacent cells.
        vector<vector<int>> directions = {
            {-1, 0}, // up
            {1, 0},  // down
            {0, -1}, // left
            {0, 1}   // right
        };

        /*
            dist[row][col] stores the minimum distance from this cell
            to the nearest cell containing 0.

            Initially, -1 means that the cell has not been reached yet.
            Therefore, dist also works as our visited array.
        */
        vector<vector<int>> dist(m, vector<int>(n, -1));

        /*
            Every 0 is a starting point because we want the distance
            of each cell from its nearest 0.

            Instead of running BFS separately from every cell containing 1,
            we start BFS from all cells containing 0 simultaneously.
        */
        queue<pair<int, int>> q;

        /*
            Multi-source BFS initialisation:

            Add every 0 to the queue with distance 0.

            All these cells are considered to be present at BFS level 0,
            so distance spreads outward from every 0 at the same time.
        */
        for (int row = 0; row < m; row++) {
            for (int col = 0; col < n; col++) {
                if (mat[row][col] == 0) {
                    q.push({row, col});
                    dist[row][col] = 0;
                }
            }
        }

        /*
            BFS processes cells in increasing order of distance.

            When a cell has distance d from its nearest 0,
            any unvisited adjacent cell will have distance d + 1.

            Since all 0 cells were inserted initially, the first time
            a cell is reached is through its nearest 0.
        */
        while (!q.empty()) {
            auto [row, col] = q.front();
            q.pop();

            // Visit all 4 adjacent cells.
            for (auto& direction : directions) {
                int newRow = row + direction[0];
                int newCol = col + direction[1];

                // Ignore neighbours outside the matrix.
                if (newRow < 0 || newRow >= m ||
                    newCol < 0 || newCol >= n) {
                    continue;
                }

                /*
                    If dist is not -1, this cell has already been reached
                    through the shortest possible path.

                    BFS does not need to process it again.
                */
                if (dist[newRow][newCol] != -1) {
                    continue;
                }

                /*
                    The neighbour is one step farther from the nearest 0
                    than the current cell.
                */
                dist[newRow][newCol] = dist[row][col] + 1;

                // Add it to the queue so distance can spread from it further.
                q.push({newRow, newCol});
            }
        }

        return dist;
    }
};