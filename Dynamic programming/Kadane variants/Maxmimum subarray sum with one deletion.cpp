class Solution {
public:
    int maximumSum(vector<int>& arr) {

        // dp[i][0] = max subarray til i without using deletion
        // dp[i][1] = max ......................using 1 deletion

        int n = arr.size();

        vector<vector<int>> dp(n, vector<int>(2, 0));

        const int NEG = -1e9;

        dp[0][0] = arr[0];
        dp[0][1] = NEG; // deleting arr[0] leaves empty

        int ans =  arr[0];

        for (int i = 1; i < n; i++) {

            dp[i][0] = max(arr[i], dp[i - 1][0] + arr[i]);
            dp[i][1] = max(dp[i - 1][1] + arr[i], dp[i - 1][0]);

            ans = max(ans, dp[i][0]);
            ans = max(ans, dp[i][1]);
        }

        return ans;
    }
};