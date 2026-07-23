class Solution {
public:
    bool containsCycle(vector<vector<char>>& grid) {

        int r = grid.size();
        int c = grid[0].size();

        vector<vector<int>> vis(r, vector<int>(c, 0));

        vector<int> dx = {-1, 1, 0, 0};
        vector<int> dy = {0, 0, -1, 1};

        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                if (!vis[i][j]) {

                    queue<pair<pair<int, int>, pair<int, int>>> q;
                    q.push({{i,j}, {-1,-1}});
                    vis[i][j] = 1;

                        while (!q.empty()) {
                        auto front = q.front();
                        q.pop();

                        int r0 = front.first.first;
                        int c0 = front.first.second;
                        int pr = front.second.first;
                        int pc = front.second.second;

                        for (int i = 0; i < 4; i++) {
                            int nrow = r0 + dx[i];
                            int ncol = c0 + dy[i];

                            if (nrow >= 0 && nrow < r && ncol >= 0 &&
                                ncol < c && grid[nrow][ncol] == grid[r0][c0]) {

                                if (vis[nrow][ncol] == 0) {
                                    vis[nrow][ncol] = 1;
                                    q.push({{nrow, ncol}, {r0, c0}});
                                }

                                else if (!(nrow == pr && ncol == pc)) {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }

        return false;
    }
};




class Solution {
public:

    bool dfs(int r0, int c0, int pr, int pc,
             vector<vector<char>>& grid,
             vector<vector<int>>& vis,
             vector<int>& dx, vector<int>& dy,
             int r, int c) {

        vis[r0][c0] = 1;

        for (int k = 0; k < 4; k++) {
            int nr = r0 + dx[k];
            int nc = c0 + dy[k];

            if (nr >= 0 && nr < r && nc >= 0 && nc < c &&
                grid[nr][nc] == grid[r0][c0]) {

                if (!vis[nr][nc]) {
                    if (dfs(nr, nc, r0, c0, grid, vis, dx, dy, r, c))
                        return true;
                }
                else if (!(nr == pr && nc == pc)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool containsCycle(vector<vector<char>>& grid) {

        int r = grid.size();
        int c = grid[0].size();

        vector<vector<int>> vis(r, vector<int>(c, 0));
        vector<int> dx = {-1, 1, 0, 0};
        vector<int> dy = {0, 0, -1, 1};

        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                if (!vis[i][j]) {
                    if (dfs(i, j, -1, -1, grid, vis, dx, dy, r, c)) {
                        return true;
                    }
                }
            }
        }

        return false;
    }
};