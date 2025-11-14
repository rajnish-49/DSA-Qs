// @ https://leetcode.com/problems/check-if-array-is-sorted-and-rotated/description/

class Solution {
public:
    // Function to check if the array can be rotated to become non-decreasing
    bool check(vector<int>& nums) {
        int k = 0; // Counter to track the number of "decreasing" points
        int n = nums.size(); // Get the size of the array

        // Loop through the array to count decreasing points
        for (int i = 1; i < n; i++) {
            if (nums[i - 1] > nums[i]) { 
                // Increment counter if current element is less than the previous
                k++;
            }
        }

        // Check if the last element is greater than the first (wrap-around case)
        if (nums[n - 1] > nums[0]) {
            k++;
        }

        // If there is at most one "decreasing" point, the array can be rotated
        return k <= 1;
    }
};



class Solution {
public:
    bool check(vector<int>& nums) {
        int count = 0;
        int n = nums.size();
        for (int i = 0; i < n; i++) {
            if (nums[i] > nums[(i + 1) % n])
                count++;
            if (count > 1)
                return false;
        }
        return true;
    }
};


/*

Without % n, accessing the next element beyond the last index would lead to out-of-bounds access, which is undefined behavior in C++.

For example:

If 
𝑖
=
4
i=4 (last element) and you try to access nums[5] directly, it will cause an error because the valid indices are from 0 to 
𝑛
−
1
n−1 (0 to 4 in this case).

*/
