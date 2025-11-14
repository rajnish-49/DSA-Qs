class Solution {
  public:
    // Function to find the kth smallest element
    int kthSmallest(vector<int> &arr, int k) {
        // Step 1: Find the maximum element in the array
        // We need to know the maximum value in the array to create a frequency array of the correct size.
        int maxElement = *max_element(arr.begin(), arr.end());

        // Step 2: Build the frequency array
        // Create a frequency array of size (maxElement + 1), initialized to 0.
        // Each index of this array represents a number, and the value at the index represents its frequency.
        vector<int> freq(maxElement + 1, 0);

        // Traverse the array and increment the frequency of each number.
        for (int num : arr) {
            freq[num]++;
        }

        // Step 3: Find the kth smallest element
        // Iterate through the frequency array to determine the kth smallest element.
        int cumulativeCount = 0; // Keep track of the cumulative count of elements.

        for (int i = 0; i <= maxElement; i++) {
            // Add the frequency of the current number to the cumulative count.
            cumulativeCount += freq[i];

            // If the cumulative count becomes greater than or equal to k,
            // it means the kth smallest element is the current index.
            if (cumulativeCount >= k) {
                return i; // Return the kth smallest element.
            }
        }

        // If we reach here, it means k is invalid (shouldn't happen with valid input as per constraints).
        return -1;
    }
};

/*
freq[i] tells us how many times the number i appears in arr.
cumulativeCount keeps track of how many numbers we have seen so far in sorted order.
When cumulativeCount reaches k, we know we have found the k-th smallest number.
Each index represents a number, and the value at that index represents its count.
*/