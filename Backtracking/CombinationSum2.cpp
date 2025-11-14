// @ https://leetcode.com/problems/combination-sum/

class Solution {
    public:
    
        // Recursive function to find all unique combinations that sum up to the target.
        void solve(int index, int target, vector<int>& candidates, vector<int>& output, vector<vector<int>>& sol) {
    
            // Base case 1: If the target becomes 0, we found a valid combination.
            if (target == 0) {
                sol.push_back(output); // Add the current combination to the result.
                return;
            }
    
            // Base case 2: If we reach the end of the candidates array, stop the recursion.
            if (index == candidates.size() || target < 0 ) {
                return;
            }
    
            // If the current candidate is less than or equal to the remaining target,
            // we can include it in the current combination.
            if (candidates[index] <= target) {
                output.push_back(candidates[index]); // Add the candidate to the current combination.
    
                // Recursively call solve() with the updated target and the next index.
                // Note that we move to the next index (index + 1).
                solve(index + 1, target - candidates[index], candidates, output, sol);
    
                // Backtrack: Remove the last added candidate to explore other combinations.
                output.pop_back();
            }
    
            // Skip duplicate candidates to avoid duplicate combinations.
            // This is crucial for combinationSum2, which handles duplicate numbers in the input.
            // We iterate through the candidates until we find a different number.
            while (index + 1 < candidates.size() && candidates[index] == candidates[index + 1]) {
                index++;
            }
    
            // Recursively call solve() without including the current candidate.
            // This explores combinations that don't use the current candidate.
            solve(index + 1, target, candidates, output, sol);
        }
    
        // Main function to find all unique combinations that sum up to the target.
        vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
    
            // Sort the candidates array to handle duplicates efficiently.
            // Sorting allows us to easily skip duplicate numbers in the 'solve' function.
            sort(candidates.begin(), candidates.end());
    
            vector<vector<int>> ans; // Store the result (all valid combinations).
            vector<int> output; // Store the current combination being built.
    
            // Start the recursive 'solve' function from the beginning of the candidates array.
            solve(0, target, candidates, output, ans);
    
            return ans; // Return the result.
        }
    };