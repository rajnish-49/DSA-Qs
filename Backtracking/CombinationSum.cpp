// @ https://leetcode.com/problems/combination-sum/description/

class Solution {
    public:
    
        // Recursive helper function to find combinations
        void solve(int index, int target, vector<int>& candidates, vector<int>& output, vector<vector<int>>& sol) {
    
            // Base case: If we've reached the end of the candidates array
            if (index == candidates.size()) {
                // Check if the current combination sums up to the target
                if (target == 0) {
                    sol.push_back(output); // If it does, add it to the solution set
                }
                return; // Backtrack: We've explored this path
            }
    
            // Recursive calls: Explore two possibilities
    
            // 1. Include the current candidate (candidates[index])
            if (candidates[index] <= target) { // Only include if it doesn't exceed the target
                output.push_back(candidates[index]); // Add the current candidate to the combination
    
                // Explore further combinations by keeping the same index (we can reuse the same number)
                solve(index, target - candidates[index], candidates, output, sol); 
    
                output.pop_back(); // Backtrack: Remove the last added element to explore other possibilities
            }
    
            // 2. Exclude the current candidate and move to the next one
            solve(index + 1, target, candidates, output, sol); // Explore combinations without the current candidate
        }
    
        // Main function to find all combination sums
        vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
    
            vector<vector<int>> sol; // Store the final result (all combinations)
            vector<int> output; // Store the current combination being built
    
            solve(0, target, candidates, output, sol); // Start the recursive process from index 0
    
            return sol; // Return the result
        }
    };