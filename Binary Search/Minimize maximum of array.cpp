// @https://leetcode.com/problems/minimize-maximum-of-array/

#define ll long long 

class Solution {

private:

    // Function to check if it is possible to make
    // all elements ≤ candidate by performing the allowed operations
    bool possible(vector<int> nums, ll candidate) {
        // Create a copy of nums in long long for safe arithmetic
        vector<long long> arr(begin(nums), end(nums));
        int n = nums.size();

        // Iterate through array from left to right (except last element)
        for (int i = 0; i < n - 1; i++) {
            // If current element already exceeds candidate,
            // it's impossible to balance with allowed operations
            if (arr[i] > candidate) {
                return false;
            }

            // Calculate how much space is left at current index
            // before reaching candidate value
            ll allowed = candidate - arr[i];

            // Since we can push values from right to left,
            // reduce the next element by 'allowed' amount
            // simulating "pulling" values from right to left to balance arr[i]
            arr[i + 1] = arr[i + 1] - allowed;
        }

        // After adjusting all elements, check the last element
        // If it's still ≤ candidate, the entire array can be balanced
        return arr[n - 1] <= candidate;
    }

public:
    int minimizeArrayValue(vector<int>& nums) {
        int n = nums.size();

        // Minimum possible answer can start from nums[0] or 0 (better to start from 0)
        // But starting from nums[0] is fine as it won't affect correctness,
        // because answer can't be smaller than the first element after operations.
        ll mini = nums[0];

        // Maximum possible answer is the maximum element in nums
        ll maxi = *max_element(nums.begin(), nums.end());

        ll ans;

        // Standard binary search between mini and maxi
        while (mini <= maxi) {
            ll mid = mini + (maxi - mini) / 2;

            // Check if 'mid' is a valid candidate max value
            if (possible(nums, mid)) {
                ans = mid;        // If yes, save it
                maxi = mid - 1;   // Try to find a smaller valid candidate
            } else {
                mini = mid + 1;   // Otherwise, try bigger candidates
            }
        }

        return ans; // Return the smallest candidate that works
    }
};


#define ll long long

class Solution {
private:

    // Checks if it is possible to make all array elements <= candidate
    // by moving excess values from right elements to left elements.
    bool isValid(vector<int>& nums, int candidate) {
        int n = nums.size();
        vector<ll> arr(nums.begin(), nums.end()); // Use long long to avoid overflow

        // Start from the right end, move leftwards
        for (int i = n - 1; i > 0; i--) {
            // If current element is greater than candidate, excess needs to be moved left
            if (arr[i] > candidate) {
                ll excess = arr[i] - candidate; // Calculate excess amount

                arr[i] -= excess;     // Reduce current element down to candidate
                arr[i - 1] += excess; // Push excess to the left element
            }
        }

        // After pushing excess leftwards, check if the first element is still <= candidate
        // Because we cannot push any excess further left from index 0
        return arr[0] <= candidate;
    }

public:
    int minimizeArrayValue(vector<int>& nums) {
        int n = nums.size();

        int low = 0;                                     // Minimum possible max value
        int high = *max_element(nums.begin(), nums.end()); // Maximum possible max value (original max in array)
        int result = high;                               // Store best found answer

        // Binary search to find the smallest candidate max value
        while (low <= high) {
            int mid = low + (high - low) / 2;            // Mid candidate max

            if (isValid(nums, mid)) {
                // If it’s possible to redistribute so that max ≤ mid,
                // try to find a smaller max
                result = mid;
                high = mid - 1;
            } else {
                // Otherwise, try bigger max values
                low = mid + 1;
            }
        }

        return result;
    }
};
