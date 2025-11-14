//@ https://leetcode.com/problems/01-matrix/

class Solution {
public:
    vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {

        int n = mat.size();    // total number of rows
        int m = mat[0].size(); // total number of columns

        // `vis` marks whether a cell has already been processed in BFS.
        // This prevents revisiting and redundant work.
        vector<vector<int>> vis(n, vector<int>(m, 0));

        // `dist` will store the final shortest distance from each cell to its nearest 0.
        // Initialized with -1 to indicate “not computed yet.”
        vector<vector<int>> dist(n, vector<int>(m, -1));

        // The queue will store ((row, col), distance)
        // Each entry represents a frontier cell and how far it is from the nearest 0.
        queue<pair<pair<int,int>, int>> q;

        // STEP 1: Initialization.
        // Push all cells that already contain 0 into the queue first.
        // Why? Because BFS expands in waves, and these zeros are the sources of the wavefront.
        // Their distance to the nearest 0 is, trivially, 0.
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (mat[i][j] == 0) {
                    q.push({{i, j}, 0}); // start BFS from all zeros
                    vis[i][j] = 1;       // mark zero-cells as visited
                } else {
                    vis[i][j] = 0;       // mark one-cells as unvisited
                }
            }
        }

        // Movement vectors for exploring four directions (up, right, down, left).
        // These allow us to generalize BFS expansion in a grid.
        int delrow[] = {-1, 0, 1, 0};
        int delcol[] = {0, 1, 0, -1};

        // STEP 2: Multi-source BFS.
        // Each iteration pulls the closest unprocessed cell, updates its distance,
        // and pushes its valid unvisited neighbors into the queue.
        while (!q.empty()) {
            int row = q.front().first.first;
            int col = q.front().first.second;
            int steps = q.front().second;
            q.pop();

            // The current cell’s shortest distance to a 0 is now finalized.
            dist[row][col] = steps;

            // Explore its 4 neighbors.
            for (int i = 0; i < 4; i++) {
                int nrow = row + delrow[i];
                int ncol = col + delcol[i];

                // Valid neighbor check:
                // 1. Inside grid bounds
                // 2. Not visited already
                if (nrow >= 0 && nrow < n && ncol >= 0 && ncol < m && vis[nrow][ncol] == 0) {
                    vis[nrow][ncol] = 1; // mark as visited so we don’t push twice
                    q.push({{nrow, ncol}, steps + 1}); // increment distance by 1
                }
            }
        }

        // Every reachable cell now has its shortest distance to a 0.
        // BFS ensures minimal distance since each move costs 1 unit.
        return dist;
    }
};


#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
    int n = mat.size();
    int m = mat[0].size();

    // Queue for BFS traversal (stores cell coordinates)
    queue<pair<int, int>> q;

    // `dist` starts as ∞ for all cells (INT_MAX = effectively unreachable).
    // Any smaller value means “visited and processed.”
    vector<vector<int>> dist(n, vector<int>(m, INT_MAX));

    // STEP 1: Initialize BFS with all 0-cells.
    // Instead of a separate visited array, we’ll directly set dist = 0 for these.
    // This serves two roles:
    //  (a) marks them as visited
    //  (b) defines the starting layer of BFS
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (mat[i][j] == 0) {
                q.push({i, j});
                dist[i][j] = 0; // zero distance from itself
            }
        }
    }

    // Four possible directions of movement.
    int delrow[] = {-1, 0, 1, 0};
    int delcol[] = {0, 1, 0, -1};

    // STEP 2: BFS expansion.
    // Each step propagates the shortest known distance from 0 to its neighboring cells.
    // If a better distance is discovered, we update it and push the cell back into the queue.
    while (!q.empty()) {
        int row = q.front().first;
        int col = q.front().second;
        q.pop();

        for (int i = 0; i < 4; i++) {
            int nrow = row + delrow[i];
            int ncol = col + delcol[i];

            // Check boundary validity.
            if (nrow >= 0 && nrow < n && ncol >= 0 && ncol < m) {

                // Relaxation condition:
                // If going through (row, col) gives a smaller distance to neighbor,
                // we’ve found a shorter path to the nearest 0.
                if (dist[nrow][ncol] > dist[row][col] + 1) {
                    dist[nrow][ncol] = dist[row][col] + 1;
                    q.push({nrow, ncol});
                }
            }
        }
    }

    // By the end, every cell holds its shortest distance to a zero.
    // BFS’s layer-ordering guarantees that no smaller distance can exist.
    return dist;
}
