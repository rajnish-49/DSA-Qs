// @ https://leetcode.com/problems/3sum/description/?envType=problem-list-v2&envId=array

class Solution {
    public:
        vector<vector<int>> threeSum(vector<int>& nums) {
            vector<vector<int>> ans;
            int n = nums.size();
            
            // Sort the array to use the two-pointer approach and handle duplicates
            sort(nums.begin(), nums.end());
    
            // Iterate through each element as the first element of the triplet (nums[i])
            for (int i = 0; i < n - 2; i++) {
                // Skip duplicate values for the first element (nums[i])
                // Avoids processing the same triplet starting with the same value
                if (i > 0 && nums[i] == nums[i - 1]) continue;
    
                // Initialize two pointers:
                int j = i + 1;  // Starts right after i
                int k = n - 1;  // Starts at the end of the array
    
                // Find pairs (nums[j], nums[k]) such that nums[i] + nums[j] + nums[k] == 0
                while (j < k) {
                    int sum = nums[i] + nums[j] + nums[k];
    
                    if (sum < 0) {
                        // Sum is too small: move j to the right to increase the sum
                        j++;
                    } else if (sum > 0) {
                        // Sum is too large: move k to the left to decrease the sum
                        k--;
                    } else {
                        // Found a valid triplet!
                        ans.push_back({nums[i], nums[j], nums[k]});
    
                        // Skip duplicates for the second element (nums[j])
                        // Move j until a new value is found (to avoid duplicate triplets)
                        while (j < k && nums[j] == nums[j + 1]) j++;
    
                        // Skip duplicates for the third element (nums[k])
                        // Move k until a new value is found (to avoid duplicate triplets)
                        while (j < k && nums[k] == nums[k - 1]) k--;
    
                        // After skipping duplicates, move j and k inward to check new values
                        // This ensures we don't reprocess the same triplet
                        j++;
                        k--;
                    }
                }
            }
    
            return ans;
        }
    };