// @ https://leetcode.com/problems/count-nice-pairs-in-an-array/

class Solution {
    public:
    
        // Function to reverse the digits of a given number
        int reverse(int &num) {
            int reversed = 0;
            int temp = num; // Creating a temporary copy of num to avoid modifying the original value
            while (temp > 0) {
                int digit = temp % 10; // Extract the last digit
                reversed = reversed * 10 + digit; // Append digit to reversed number
                temp /= 10; // Remove the last digit from temp
            }
            return reversed;
        }
    
        // Function to count the number of "nice pairs"
        int countNicePairs(vector<int>& nums) {
    
            unordered_map<long, long> m; // HashMap to store frequency of (nums[i] - reverse(nums[i]))
            int n = nums.size();
            long nicepair = 0; // Stores the count of nice pairs
    
            for (int i = 0; i < n; i++) {
                // Calculate the transformed value for each number
                int value = nums[i] - reverse(nums[i]);
    
                // Add the count of previous occurrences of this value to nicepair count
                nicepair += m[value];
    
                // Increment the frequency of this transformed value
                m[value]++;
            }
    
            // Return the count of nice pairs modulo 10^9 + 7 to prevent overflow
            return nicepair % 1000000007;
        }
    };
    