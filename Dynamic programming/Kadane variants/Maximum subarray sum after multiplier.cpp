class Solution {
public:
    using ll = long long ;
    long long maxSubarraySum(vector<int>& nums, int k) {

        // maximum sum of a non-empty subarray after exact one operation is
        // applied to some subarray
        int n = nums.size();
        const ll NEG = -4e18;

        vector<vector<ll>> dp(n, vector<ll>(5, NEG));

        // dp[i][0] = best subarray ending at i with no operation used
        // dp[i][1] = best subarray ending at i while inside multiply segment
        // dp[i][2] = best subarray ending at i after multiply segment is finished 
        // dp[i][3] = best subarray ending at i while inside divide segment 
        // dp[i][4] = best subarray ending at i after divide segment is finished

        ll x = nums[0];

        dp[0][0] = x;
        dp[0][1] = x * k;
        dp[0][2] = NEG;
        dp[0][3] = x / k;
        dp[0][4] = NEG;

        ll ans = max({dp[0][0], dp[0][1], dp[0][2], dp[0][3], dp[0][4]});

        for (int i = 1; i < n; i++) {
            x = nums[i];

            ll mul = x * k;
            ll div = x / k;

            dp[i][0] = max(dp[i - 1][0] + x, x);

            dp[i][1] = max({dp[i - 1][1] + mul, dp[i - 1][0] + mul, mul});

            dp[i][2] = max({dp[i - 1][2] + x, dp[i - 1][1] + x});

            dp[i][3] = max({dp[i - 1][3] + div, dp[i - 1][0] + div, div});

            dp[i][4] = max({dp[i - 1][4] + x, dp[i - 1][3] + x});

            ans = max({ans, dp[i][0], dp[i][1], dp[i][2], dp[i][3], dp[i][4]});
        }

        return ans;
    }
};