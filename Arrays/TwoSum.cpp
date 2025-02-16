// @ https://leetcode.com/problems/two-sum/description/?envType=problem-list-v2&envId=array

class Solution {
    public:
    
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> numMap; // Stores number and its index
    
        for (int i = 0; i < nums.size(); i++) {
            int complement = target - nums[i]; // Required number to form the sum
    
            if (numMap.find(complement) != numMap.end()) {
                return {numMap[complement], i}; // Return indices of the two numbers
            }
    
            numMap[nums[i]] = i; // Store the number with its index
        }
    
        return {}; // Not needed as per constraints
    }
    };

