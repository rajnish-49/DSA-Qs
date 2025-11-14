// @ https://leetcode.com/problems/count-elements-with-maximum-frequency/description/

class Solution {
    public:
        int maxFrequencyElements(vector<int>& nums) {
            unordered_map<int, int> fq; // Hash map to store the frequency of each number
    
            // Step 1: Count the frequency of each element in nums
            for (int i = 0; i < nums.size(); i++) {
                fq[nums[i]]++; // Increment the frequency of the current number
            }
    
            int maxFreq = 0; // Variable to store the highest frequency found
    
            // Step 2: Find the maximum frequency among all elements
            for (auto it : fq) {
                if (it.second > maxFreq) { // If the current frequency is greater than maxFreq, update it
                    maxFreq = it.second;
                }
            }
    
            int count = 0; // Variable to count occurrences of elements with max frequency
    
            // Step 3: Count how many elements have the maximum frequency
            for (auto num : nums) {
                if (fq[num] == maxFreq) { // If the element appears with max frequency, count it
                    count++;
                }
            }
    
            return count; // Return the total count of elements appearing with max frequency
        }
    };
    