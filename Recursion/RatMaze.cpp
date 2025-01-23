class Solution {
    // Function to check if a cell is valid to visit
    bool isValid(int x, int y, int n, vector<vector<int>> &mat, vector<vector<bool>> &visited) {
        return (x >= 0 && x < n && y >= 0 && y < n && mat[x][y] == 1 && !visited[x][y]);
    }

    // Recursive function to explore all paths
    void findPaths(int x, int y, int n, vector<vector<int>> &mat, vector<vector<bool>> &visited,
                   string path, vector<string> &result) {
        // Base case: If destination is reached
        if (x == n - 1 && y == n - 1) {
            result.push_back(path);
            return;
        }

        // Mark the current cell as visited
        visited[x][y] = true;

        // Move Down
        if (isValid(x + 1, y, n, mat, visited)) {
            findPaths(x + 1, y, n, mat, visited, path + 'D', result);
        }

        // Move Left
        if (isValid(x, y - 1, n, mat, visited)) {
            findPaths(x, y - 1, n, mat, visited, path + 'L', result);
        }

        // Move Right
        if (isValid(x, y + 1, n, mat, visited)) {
            findPaths(x, y + 1, n, mat, visited, path + 'R', result);
        }

        // Move Up
        if (isValid(x - 1, y, n, mat, visited)) {
            findPaths(x - 1, y, n, mat, visited, path + 'U', result);
        }

        // Backtrack: Unmark the cell as visited
        visited[x][y] = false;
    }

public:
    vector<string> findPath(vector<vector<int>> &mat) {
        int n = mat.size();
        vector<string> result;

        // If the start or end cell is blocked, return an empty result
        if (mat[0][0] == 0 || mat[n - 1][n - 1] == 0) {
            return result;
        }

        // Visited matrix to track visited cells
        vector<vector<bool>> visited(n, vector<bool>(n, false));

        // Start backtracking from the top-left corner
        findPaths(0, 0, n, mat, visited, "", result);
        return result;
    }
};
