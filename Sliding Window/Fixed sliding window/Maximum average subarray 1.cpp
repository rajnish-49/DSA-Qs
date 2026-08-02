class Solution {
public:
    double findMaxAverage(vector<int>& nums, int k) {
        int windowSum = 0;

        // Build the first window containing indices [0 ... k - 1].
        for (int i = 0; i < k; i++) {
            windowSum += nums[i];
        }

        // Since every window has the same size k, the window with the
        // maximum sum will also have the maximum average.
        int maxSum = windowSum;

        // Slide the fixed-size window one position at a time.
        // 'right' is the new element entering the window.
        for (int right = k; right < nums.size(); right++) {

            // Add the new element entering from the right.
            windowSum += nums[right];

            // Remove the element that is no longer part of the window.
            // For right = k, nums[0] leaves.
            // For right = k + 1, nums[1] leaves, and so on.
            windowSum -= nums[right - k];

            // Store the largest sum among all windows of size k.
            maxSum = max(maxSum, windowSum);
        }

        // All windows have size k, so calculate the average only once
        // after finding the window with the maximum sum.
        return static_cast<double>(maxSum) / k;
    }
};