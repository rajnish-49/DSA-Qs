// User function Template for C++
#include <vector>
#include <cstring>
using namespace std;

// Disjoint Set Union (DSU) class for efficient union-find operations
class DSU
{
public:
    vector<int> parent; // Stores the parent of each node
    vector<int> rank;   // Stores the rank (depth) of each node for union by rank

    // Constructor: Initialize DSU for n nodes
    DSU(int n)
    {
        parent.resize(n);
        rank.resize(n, 0);
        // Initially, each node is its own parent (disjoint sets)
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }

    // Find the ultimate parent of node x with path compression
    // Path compression: Attach all nodes directly to root for faster future queries
    int findParent(int x)
    {
        if (parent[x] == x)
            return x;
        // Path compression: Update parent[x] to root while returning
        return parent[x] = findParent(parent[x]);
    }

    // Union by rank: Merge two sets containing nodes a and b
    void unionSet(int a, int b)
    {
        a = findParent(a); // Find root of set containing a
        b = findParent(b); // Find root of set containing b

        if (a != b) // Only merge if they belong to different sets
        {
            // Attach smaller rank tree under root of higher rank tree
            if (rank[a] < rank[b])
                parent[a] = b;
            else if (rank[b] < rank[a])
                parent[b] = a;
            else // If ranks are same, make one as root and increment its rank
            {
                parent[b] = a;
                rank[a]++;
            }
        }
    }
};

class Solution
{
private:
    // Helper function to check if a cell (nrow, ncol) is within grid boundaries
    bool isvalid(int nrow, int ncol, int n, int m)
    {

        if (nrow >= 0 && nrow < n && ncol >= 0 && ncol < m)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

public:
    // Function to return the number of islands after each operation
    // Problem: Given n x m grid, operators add islands at positions. Return island count after each operation.
    vector<int> numOfIslands(int n, int m, vector<vector<int>> &operators)
    {

        // Initialize DSU for all cells in the grid (n*m cells)
        DSU ds(n * m);

        // Visited array to track which cells are already land
        int vis[n][m];

        // Initialize all cells as water (0)
        memset(vis, 0, sizeof vis);

        int cnt = 0;     // Current count of islands
        vector<int> ans; // Store island count after each operation

        // Process each operation (adding a land cell)
        for (auto it : operators)
        {
            int row = it[0];
            int col = it[1];

            // If cell is already land, no change in island count
            if (vis[row][col] == 1)
            {
                ans.push_back(cnt);
                continue;
            }

            // Mark the cell as land
            vis[row][col] = 1; // BUG FIX: Should be assignment (=) not comparison (==)
            cnt++;             // Initially assume this creates a new island

            // Direction arrays for 4 neighbors: up, right, down, left
            int dr[] = {-1, 0, 1, 0};
            int dc[] = {0, 1, 0, -1};

            // Check all 4 adjacent cells
            for (int id = 0; id < 4; id++)
            {
                int nrow = row + dr[id];
                int ncol = col + dc[id];

                // If adjacent cell is valid and is land
                if (isvalid(nrow, ncol, n, m))
                {
                    if (vis[nrow][ncol] == 1)
                    {
                        // Convert 2D coordinates to 1D node numbers for DSU
                        int nodeno = row * m + col;    // Current cell
                        int adjnode = nrow * m + ncol; // Adjacent cell

                        // If they belong to different components, merge them
                        if (ds.findParent(nodeno) != ds.findParent(adjnode))
                        {
                            cnt--; // Merging two islands reduces count by 1
                            ds.unionSet(nodeno, adjnode);
                        }
                    }
                }
            }

            // Store the current island count after this operation
            ans.push_back(cnt);
        }

        return ans;
    }
};
