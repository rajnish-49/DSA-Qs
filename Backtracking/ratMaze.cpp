class Solution {
    public:
        // Function to check if a cell (x, y) is within the grid boundaries, 
        // has a value of 1 (representing a valid path), and has not been visited yet.
        bool isValid(int x, int y, int n, vector<vector<int>> &mat, vector<vector<bool>> &visited) {
            return (x >= 0 && x < n && y >= 0 && y < n && mat[x][y] == 1 && !visited[x][y]);
        }
    
        // Recursive function to find all paths from (x, y) to the destination (n-1, n-1).
        void findPaths(int x, int y, int n, vector<vector<int>> &mat, vector<vector<bool>> &visited,
                       string path, vector<string> &result) {
            // Base case: If we've reached the destination cell.
            if (x == n - 1 && y == n - 1) {
                result.push_back(path); // Add the current path to the result.
                return; // Backtrack: No further exploration needed from this cell.
            }
    
            // Mark the current cell as visited to avoid cycles.
            visited[x][y] = true;
    
            // Explore possible paths in all four directions (Down, Left, Right, Up).
    
            // 1. Move Down: Check if the cell below is valid and explore the path.
            if (isValid(x + 1, y, n, mat, visited)) {
                findPaths(x + 1, y, n, mat, visited, path + 'D', result);
            }
    
            // 2. Move Left: Check if the cell to the left is valid and explore the path.
            if (isValid(x, y - 1, n, mat, visited)) {
                findPaths(x, y - 1, n, mat, visited, path + 'L', result);
            }
    
            // 3. Move Right: Check if the cell to the right is valid and explore the path.
            if (isValid(x, y + 1, n, mat, visited)) {
                findPaths(x, y + 1, n, mat, visited, path + 'R', result);
            }
    
            // 4. Move Up: Check if the cell above is valid and explore the path.
            if (isValid(x - 1, y, n, mat, visited)) {
                findPaths(x - 1, y, n, mat, visited, path + 'U', result);
            }
    
            // Backtrack: Unmark the current cell as visited. This is crucial for exploring other possible paths.
            // After exploring all directions from the current cell, we reset its visited status 
            // so that it can be part of other valid paths starting from different cells.
            visited[x][y] = false;
        }
    
        // Main function to find all paths in the given matrix.
        vector<string> findPath(vector<vector<int>> &mat) {
            int n = mat.size();
            vector<string> result;
    
            // Check if the starting or ending cell is blocked (value 0). If so, no path exists.
            if (mat[0][0] == 0 || mat[n - 1][n - 1] == 0) {
                return result; // Return an empty result if no path is possible.
            }
    
            // Create a visited matrix to keep track of visited cells during path exploration.
            vector<vector<bool>> visited(n, vector<bool>(n, false));
    
            // Start the recursive path finding process from the top-left cell (0, 0).
            findPaths(0, 0, n, mat, visited, "", result);
            return result; // Return the vector containing all valid paths.
        }
    };