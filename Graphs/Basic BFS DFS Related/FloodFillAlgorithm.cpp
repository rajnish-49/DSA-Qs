// @ https://leetcode.com/problems/flood-fill/

class Solution {
    private:
    // Depth-First Search (DFS) function to perform flood fill
    void dfs(int row, int col, vector<vector<int>> &ans, vector<vector<int>>& image,
             int newcolor, int delrow[], int delcol[], int inicolor) {

        int n = image.size();    // Number of rows in the image
        int m = image[0].size(); // Number of columns in the image

        // Change the color of the current pixel to the new color
        ans[row][col] = newcolor;

        // Explore all four possible directions (up, right, down, left)
        for (int i = 0; i < 4; i++) {
            int nrow = row + delrow[i]; // New row index
            int ncol = col + delcol[i]; // New column index

            // Check if the new position is within bounds and has the same initial color
            // Also, ensure it hasn't already been changed to the new color to avoid infinite recursion
            if (nrow >= 0 && nrow < n && ncol >= 0 && ncol < m && 
                image[nrow][ncol] == inicolor && ans[nrow][ncol] != newcolor) {
                
                // Recursively apply DFS to fill the connected region
                dfs(nrow, ncol, ans, image, newcolor, delrow, delcol, inicolor);
            }
        }
    }

public:
    // Function to perform the flood fill algorithm
    vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int color) {

        int inicolor = image[sr][sc]; // Store the initial color of the starting pixel
        vector<vector<int>> ans = image; // Copy the original image to avoid modifying the input

        // If the initial color is already the new color, no changes are needed
        if (inicolor == color) {
            return ans;
        }

        // Arrays to represent movement in four directions (up, right, down, left)
        int delrow[] = {-1, 0, 1, 0}; 
        int delcol[] = {0, 1, 0, -1}; 

        // Start DFS from the given starting pixel (sr, sc)
        dfs(sr, sc, ans, image, color, delrow, delcol, inicolor);

        return ans; // Return the modified image
    }
};
