#include <iostream>
#include <vector>
#include <algorithm> // For max function
using namespace std;

int longestIncreasingOrDecreasingSubarray(vector<int>& nums) {
    int n = nums.size();
    if (n == 1) return 1; // A single element itself is a valid subarray

    int maxLength = 1;
    int incLength = 1, decLength = 1; // Track increasing & decreasing lengths

    for (int i = 1; i < n; i++) {
        if (nums[i] > nums[i - 1]) { // Increasing sequence
            incLength++;
            decLength = 1; // Reset decreasing length
        } 
        else if (nums[i] < nums[i - 1]) { // Decreasing sequence
            decLength++;
            incLength = 1; // Reset increasing length
        } 
        else { // nums[i] == nums[i - 1], reset both
            incLength = 1;
            decLength = 1;
        }

        maxLength = max(maxLength, max(incLength, decLength)); // Track max length
    }

    return maxLength;
}

int main() {
    vector<int> nums1 = {1, 4, 3, 3, 2};
    cout << "Output: " << longestIncreasingOrDecreasingSubarray(nums1) << endl; // 2

    vector<int> nums2 = {3, 3, 3, 3};
    cout << "Output: " << longestIncreasingOrDecreasingSubarray(nums2) << endl; // 1

    vector<int> nums3 = {3, 2, 1};
    cout << "Output: " << longestIncreasingOrDecreasingSubarray(nums3) << endl; // 3

    return 0;
}


/*

class Solution {
public:
    int longestMonotonicSubarray(vector<int>& nums) {
        int n = nums.size();
        if (n == 1) return 1;

        int maxlength = 1, start = 0; // `start` tracks the beginning of current monotonic sequence

        for (int i = 1; i < n; i++) {
            if (nums[i] == nums[i - 1]) {
                start = i; // Reset `start` since equal elements break the sequence
            }
            else if ((nums[i] > nums[i - 1] && nums[start] >= nums[start + 1]) || 
                     (nums[i] < nums[i - 1] && nums[start] <= nums[start + 1])) {
                start = i - 1; // Reset `start` to the previous index
            }

            maxlength = max(maxlength, i - start + 1); // Update max length
        }

        return maxlength;
    }
};


*/