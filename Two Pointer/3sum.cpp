#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> result;
        int n = nums.size();
        
        // Sort to enable two-pointer search and easily group/skip duplicates
        sort(nums.begin(), nums.end());
        
        // Fix the first element of our potential triplet
        for (int i = 0; i < n - 2; i++) {
            
            // Skip duplicate fixed elements to prevent duplicate triplets in the output
            if (i > 0 && nums[i] == nums[i - 1]) {
                continue;
            }
            
            int left = i + 1;
            int right = n - 1;
            
            // Two-pointer search for the remaining two elements
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];
                
                if (sum == 0) {
                    // Match found, add to results
                    result.push_back({nums[i], nums[left], nums[right]});
                    
                    // Skip duplicate values for the left pointer
                    while (left < right && nums[left] == nums[left + 1]) {
                        left++;
                    }
                    // Skip duplicate values for the right pointer
                    while (left < right && nums[right] == nums[right - 1]) {
                        right--;
                    }
                    
                    // Move both pointers inward to search for other pairs for the same fixed 'i'
                    left++;
                    right--;
                } 
                else if (sum < 0) {
                    // Sum is too small, move left pointer rightward to get a larger value
                    left++;
                } 
                else {
                    // Sum is too large, move right pointer leftward to get a smaller value
                    right--;
                }
            }
        }
        
        return result;
    }
};