// @ https://leetcode.com/problems/subsets-ii/description/

class Solution {
    public:
        void solve(vector<int>& nums, vector<int> output, int index, vector<vector<int>>& ans) {
            // Base case: when we have processed all elements in the array
            // Add the current subset (output) to the result (ans) and return
            if (index >= nums.size()) {
                ans.push_back(output);
                return;
            }
    
            // Include the current element (nums[index]) in the subset
            output.push_back(nums[index]);
            // Recur for the next index (index + 1) to explore subsets that include nums[index]
            solve(nums, output, index + 1, ans);
            // Backtrack: Remove the last element (nums[index]) to explore subsets that exclude nums[index]
            output.pop_back();
    
            // Skip duplicates to avoid generating duplicate subsets
            // If the next element is the same as the current element, skip it
            while (index + 1 < nums.size() && nums[index] == nums[index + 1]) {
                index++;
            }
    
            // Exclude the current element and recur for the next index
            // This ensures we explore all subsets that do not include nums[index]
            solve(nums, output, index + 1, ans);
        }
    
        vector<vector<int>> subsetsWithDup(vector<int>& nums) {
            // Result vector to store all unique subsets
            vector<vector<int>> ans;
            // Sort the input array to handle duplicates efficiently
            sort(nums.begin(), nums.end());
            // Start the recursive process with an empty subset and index 0
            solve(nums, {}, 0, ans);
            // Return the final result
            return ans;
        }
    };