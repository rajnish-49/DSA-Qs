// @https://leetcode.com/problems/minimize-maximum-of-array/

#define ll long long  // Define ll as an alias for long long to handle large integers

class Solution {
public:
    int minimizeArrayValue(vector<int>& nums) {

        ll sum = 0;    // To store the running sum of the array
        ll maxi = 0;   // To store the answer: the minimum possible maximum value

        // Traverse each element of the array
        for (int i = 0; i < nums.size(); i++) {

            sum += nums[i];  // Update prefix sum from index 0 to i

            // Calculate the minimum possible value of the maximum element for the current prefix
            // We use (sum + i) / (i + 1) to compute the ceiling of (sum / (i + 1)) using integer math
            // Why ceil? Because if the average isn't an integer, we need to round it *up* to ensure
            // that it's large enough to accommodate all elements without exceeding it
            ll average = (sum + i) / (i + 1);

            // Update the overall max value seen so far among all prefixes
            // This ensures that the final answer is the highest ceiling value over all prefixes
            maxi = max(maxi, average);
        }

        // Return the minimum possible value of the maximum element after redistribution
        return maxi;
    }
};
