class Solution {
public:
    int minTimeToReach(vector<vector<int>>& moveTime) {

        int n = moveTime.size();
        int m = moveTime[0].size();

        // time[i][j] = minimum time at which we can reach cell (i, j)
        vector<vector<int>> time(n, vector<int>(m, INT_MAX));

        // Min-heap: {current_time, row, col}
        // The cell that can be reached earliest is processed first
        priority_queue<
            vector<int>,
            vector<vector<int>>,
            greater<vector<int>>
        > pq;

        // We start at cell (0, 0) at time 0
        time[0][0] = 0;
        pq.push({0, 0, 0});

        vector<vector<int>> directions = {
            {0, 1}, {0, -1}, {1, 0}, {-1, 0}
        };

        while (!pq.empty()) {
            auto curr = pq.top();
            pq.pop();

            int currTime = curr[0];
            int r = curr[1];
            int c = curr[2];

            // Ignore old/worse heap entries
            if (currTime > time[r][c]) continue;

            for (auto& dir : directions) {
                int nr = r + dir[0];
                int nc = c + dir[1];

                if (nr < 0 || nr >= n || nc < 0 || nc >= m)
                    continue;

                /*
                    To move into cell (nr, nc):

                    - We are currently at time currTime.
                    - The next room can only be entered after moveTime[nr][nc].
                    - If we are early, we wait.
                    - Moving itself takes exactly 1 second.

                    So arrival time at next cell is:
                    max(currTime, moveTime[nr][nc]) + 1
                */
                int newTime = max(currTime, moveTime[nr][nc]) + 1;

                // Relaxation: found an earlier way to reach (nr, nc)
                if (newTime < time[nr][nc]) {
                    time[nr][nc] = newTime;
                    pq.push({newTime, nr, nc});
                }
            }
        }

        // Minimum time to reach the bottom-right room
        return time[n - 1][m - 1];
    }
};