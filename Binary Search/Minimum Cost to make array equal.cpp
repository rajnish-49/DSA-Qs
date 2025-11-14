// @https://leetcode.com/problems/minimum-cost-to-make-array-equal/

#define ll long long

class Solution {
public:

    // Helper function to compute total cost of making all elements equal to 'mid'
    // This uses the formula: sum( abs(nums[i] - mid) * cost[i] )
    ll findcost(vector<int>& nums, vector<int>& cost, int mid) {
        ll result = 0;

        for (int i = 0; i < nums.size(); i++) {
            // Convert to long long to avoid integer overflow
            result += 1ll * abs(nums[i] - mid) * cost[i];
        }

        return result;
    }

    long long minCost(vector<int>& nums, vector<int>& cost) {
        // Step 1: Define search range for possible target values
        // The optimal value x must lie between the minimum and maximum of nums
        ll left = *min_element(nums.begin(), nums.end());
        ll right = *max_element(nums.begin(), nums.end());

        // Initialize answer with maximum possible value
        // This will store the minimum total cost found
        ll ans = INT_MAX;

        // Step 2: Perform Binary Search on the 'x' values (convex function)
        // Because the cost function is convex, we can use this trick:
        // If cost(x) > cost(x+1), move right. Else, move left.
        while (left <= right) {
            ll mid = left + (right - left) / 2;

            // Compute cost if all elements were changed to 'mid' or 'mid + 1'
            ll cand1 = findcost(nums, cost, mid);
            ll cand2 = findcost(nums, cost, mid + 1);

            // Track the minimum cost between these two candidates
            ans = min(cand1, cand2);

            // Since the function is convex:
            // If cost is increasing from mid to mid+1, move left (decreasing side)
            // Else, move right (increasing side)
            if (cand2 > cand1) {
                // We are on the decreasing slope; minimum is to the left
                right = mid - 1;
            } else {
                // We are on the increasing slope; minimum is to the right
                left = mid + 1;
            }
        }

        // If no valid answer was found (unlikely), return 0; else return the best cost
        return ans == INT_MAX ? 0 : ans;
    }
};
