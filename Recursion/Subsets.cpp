// @ https://leetcode.com/problems/subsets/description/

class Solution {
    private:
        // Recursive function to generate all subsets
        void solve(vector<int> nums, vector<int> output, int index, vector<vector<int>>& ans) {
    
            // Base case: If we have processed all elements in nums
            if (index >= nums.size()) {
                // We have formed a subset, so add it to the answer
                ans.push_back(output);
                return;
            }
    
            // EXCLUDE the current element:
            // We make a recursive call without including the current element (nums[index]) in the subset.
            solve(nums, output, index + 1, ans);
    
            // INCLUDE the current element:
            // We make a recursive call after including the current element (nums[index]) in the subset.
            int element = nums[index];
            output.push_back(element); // Add the element to the current subset being built.
            solve(nums, output, index + 1, ans); // Recurse to process the remaining elements.
    
            // Note: We don't need to explicitly remove the element from 'output' after the include call.
            // This is because each recursive call operates on a copy of the 'output' vector.  When the
            // include call returns, the 'output' vector in the previous (exclude) call remains unchanged.
        }
    
    public:
        vector<vector<int>> subsets(vector<int>& nums) {
    
            vector<vector<int>> ans; // Store all the generated subsets.
            vector<int> output;      // Store the current subset being built during recursion.
            int index = 0;          // Index of the current element being considered in nums.
    
            // Start the recursive process to generate subsets.
            solve(nums, output, index, ans);
    
            return ans; // Return the vector containing all subsets.
        }
    };