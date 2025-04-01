// @ https://leetcode.com/problems/max-sum-of-a-pair-with-equal-sum-of-digits/

class Solution {
    public:
        // Helper function to calculate the sum of digits of a given number.
        int sumdigits(int num) {
            int digitsum = 0; // Initialize the digit sum to 0.
            
            // Process each digit until the number becomes 0.
            while (num > 0) {
                digitsum += num % 10; // Add the last digit of num to digitsum.
                num /= 10;            // Remove the last digit from num.
            }
            
            return digitsum; // Return the computed sum of digits.
        }
        
        // Function to find the maximum sum of two numbers in the vector
        // that share the same sum of digits.
        int maximumSum(vector<int>& nums) {
            // Map to store the maximum number encountered for each digit sum.
            // The key is the sum of digits and the value is the maximum number with that digit sum.
            unordered_map<int, int> mp;
            
            int n = nums.size(); // Get the number of elements in the array.
            int maxsum = -1;     // Initialize maxsum to -1 to handle cases with no valid pair.
            
            // Loop through each number in the array.
            for (int i = 0; i < n; i++) {
                // Calculate the sum of digits for the current number.
                int value = sumdigits(nums[i]);
                
                // Check if there's already a number with the same sum of digits.
                if (mp.find(value) != mp.end()) {
                    // If so, update maxsum with the sum of the current number and the stored number.
                    maxsum = max(maxsum, mp[value] + nums[i]);
                }
                
                // Update the map for this digit sum:
                // Store the maximum number encountered so far for this sum of digits.
                mp[value] = max(mp[value], nums[i]);
            }
            
            // Return the maximum sum found, or -1 if no valid pair exists.
            return maxsum;
        }
    };



    int sumOfDigits(int num) {
        int sum = 0;
        while (num > 0) {
            sum += num % 10;
            num /= 10;
        }
        return sum;
    }
    
    // Function to find the maximum sum using priority queue
    int maxPairSum(vector<int>& nums) {
        unordered_map<int, priority_queue<int>> digitSumMap; // Hash map with max heaps
        int maxSum = -1;
    
        for (int num : nums) {
            int digitSum = sumOfDigits(num);
            digitSumMap[digitSum].push(num); // Push into the heap for this sum
            if (digitSumMap[digitSum].size() > 2) {
                digitSumMap[digitSum].pop(); // Keep only the top 2 largest numbers
            }
        }
    
        // Iterate over the map to find the max sum of top two elements
        for (auto& entry : digitSumMap) {
            if (entry.second.size() < 2) continue; // Skip if fewer than 2 numbers
    
            int first = entry.second.top(); entry.second.pop();
            int second = entry.second.top();
            maxSum = max(maxSum, first + second);
        }
    
        return maxSum;
    }
    