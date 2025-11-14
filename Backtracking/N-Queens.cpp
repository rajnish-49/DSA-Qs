// @ https://leetcode.com/problems/n-queens/description/

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Solution {
public:
    /**
     * @brief Checks if it's safe to place a queen at board[row][col].
     *        It checks for attacks in the same column, upper-left diagonal, and upper-right diagonal.
     * @param row The row index to check.
     * @param col The column index to check.
     * @param board The current state of the chessboard (2D vector of strings).
     * @param n The size of the board (n x n).
     * @return True if it's safe to place a queen, false otherwise.
     */
    bool isSafe1(int row, int col, vector<string> board, int n) {
        // Check upper-left diagonal
        int duprow = row;
        int dupcol = col;

        while (row >= 0 && col >= 0) {
            if (board[row][col] == 'Q') return false; // Check if a queen is present
            row--;
            col--;
        }

        // Check the column above
        row = duprow; // Reset row to original value
        col = dupcol; // Reset col to original value

        while (col >= 0) {
            if (board[row][col] == 'Q') return false; // Check if a queen is present
            col--;
        }

        // Check upper-right diagonal
        row = duprow; // Reset row to original value
        col = dupcol; // Reset col to original value

        while (row < n && col >= 0) {
            if (board[row][col] == 'Q') return false; // Check if a queen is present
            row++;
            col--;
        }

        return true; // It's safe to place a queen here
    }

    /**
     * @brief Recursive helper function to solve the N-Queens problem using backtracking.
     * @param col The current column being processed.
     * @param board The current state of the chessboard (2D vector of strings).
     * @param ans A vector to store all the solutions (each solution is a 2D vector of strings).
     * @param n The size of the board (n x n).
     */
    void solve(int col, vector<string>& board, vector<vector<string>>& ans, int n) {
        // Base case: If all columns are filled, we've found a solution
        if (col == n) {
            ans.push_back(board); // Add the current board configuration to the solutions
            return;
        }

        // Try placing a queen in each row of the current column
        for (int row = 0; row < n; row++) {
            if (isSafe1(row, col, board, n)) { // Check if it's safe to place a queen at (row, col)
                board[row][col] = 'Q'; // Place the queen
                solve(col + 1, board, ans, n); // Recursively solve for the next column
                board[row][col] = '.'; // Backtrack: Remove the queen to explore other possibilities
            }
        }
    }

    /**
     * @brief Solves the N-Queens problem and returns all distinct solutions.
     * @param n The size of the board (n x n).
     * @return A vector of vectors of strings, where each inner vector represents a solution.
     */
    vector<vector<string>> solveNQueens(int n) {
        vector<vector<string>> ans; // Store the solutions
        vector<string> board(n, string(n, '.')); // Initialize an empty board
        solve(0, board, ans, n); // Start the recursive process from the first column (col = 0)
        return ans; // Return the solutions
    }
};