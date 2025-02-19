// @ https://leetcode.com/problems/sudoku-solver/description/

class Solution {
    public:
        // Function to solve the Sudoku board using backtracking
        void solveSudoku(vector<vector<char>>& board) {
            solve(board); // Call the recursive function to start solving
        }
    
        // Recursive function to solve Sudoku using backtracking
        bool solve(vector<vector<char>>& board) {
    
            // Iterate over each cell in the board
            for (int row = 0; row < board.size(); row++) {
                for (int col = 0; col < board[0].size(); col++) {
    
                    // If the cell is empty ('.'), try filling it with a number
                    if (board[row][col] == '.') {
    
                        // Try placing numbers '1' to '9' in the current empty cell
                        for (char digit = '1'; digit <= '9'; digit++) {
    
                            // Check if placing 'digit' is valid in this position
                            if (isValid(board, row, col, digit)) {
                                board[row][col] = digit; // Place 'digit' in the cell
    
                                // Recursively try solving the rest of the board
                                if (solve(board) == true) {
                                    return true; // If solving succeeds, return true (solution found!)
                                } else {
                                    board[row][col] = '.'; // Undo (backtrack) if it fails.  Crucial for exploring other possibilities.
                                }
                            }
                        }
                        return false; // If no number works in this cell, return false (dead end, backtrack)
                    }
                }
            }
            return true; // If no empty cells left, the board is solved (base case for recursion)
        }
    
        // Function to check if placing 'digit' at (row, col) is valid
        bool isValid(vector<vector<char>>& board, int row, int col, char digit) {
    
            // Check the entire column for duplicate 'digit'
            for (int i = 0; i < 9; i++) {
                if (board[i][col] == digit) {
                    return false; // Found a duplicate in the column
                }
            }
    
            // Check the entire row for duplicate 'digit'
            for (int i = 0; i < 9; i++) {
                if (board[row][i] == digit) {
                    return false; // Found a duplicate in the row
                }
            }
    
            // Determine the starting position of the 3x3 subgrid
            int subgridRowStart = row - row % 3;
            int subgridColStart = col - col % 3;
    
            // Check the 3x3 subgrid for duplicate 'digit'
            for (int i = subgridRowStart; i < subgridRowStart + 3; i++) {
                for (int j = subgridColStart; j < subgridColStart + 3; j++) {
                    if (board[i][j] == digit) {
                        return false; // Found a duplicate in the subgrid
                    }
                }
            }
    
            return true; // If no conflicts found, the placement is valid
        }
    };