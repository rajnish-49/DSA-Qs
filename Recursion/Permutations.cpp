class Solution {
public:
    
    void solve(vector<int>& nums, vector<vector<int>>& ans, int index) {
        // Base case: If we reach the end of the array, store the current permutation
        if (index >= nums.size()) {
            ans.push_back(nums); // Add the current permutation to the result
            return;
        }

        // Iterate through the array, swapping and generating permutations
        for (int j = index; j < nums.size(); j++) {
            swap(nums[index], nums[j]);     // Swap the current element with the element at `j`
            solve(nums, ans, index + 1);   // Recursively generate permutations for the next index
            swap(nums[index], nums[j]);    // Backtrack: Undo the swap to restore the original state
        }
    }
    
    vector<vector<int>> permute(vector<int>& nums) {

    vector<vector<int>> ans;
    int index = 0;
    solve(nums, ans, index);
    return ans;
    }
};