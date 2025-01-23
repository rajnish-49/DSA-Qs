// @ https://leetcode.com/problems/subsets/

class Solution {
private:
    // Helper function to recursively generate subsets
    // Parameters:
    // - nums: The input vector of integers
    // - output: Current subset being formed
    // - index: Current index in nums being processed
    // - ans: Reference to the vector of all subsets
    void solve(vector<int> nums, vector<int> output, int index, vector<vector<int>>& ans) {
        // Base case: If we have processed all elements in nums
        if (index >= nums.size()) {
            ans.push_back(output); // Add the current subset (output) to the result
            return;
        }

        // Recursive case 1: Exclude the current element and move to the next
        solve(nums, output, index + 1, ans);

        // Recursive case 2: Include the current element in the subset
        int element = nums[index];  // Get the current element
        output.push_back(element);  // Include the element in the current subset
        solve(nums, output, index + 1, ans); // Recursively generate subsets with the element included
    }

public:
    // Main function to generate all subsets of a given vector
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> ans;    // To store all subsets
        vector<int> output;         // Temporary vector to form each subset
        int index = 0;              // Start from the first element in nums
        solve(nums, output, index, ans); // Call the recursive helper function
        return ans;                 // Return the vector of subsets
    }
};
