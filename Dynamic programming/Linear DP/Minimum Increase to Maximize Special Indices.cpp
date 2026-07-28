class Solution {
public:
    long long minIncrease(vector<int>& nums) {
        int n = nums.size();
        using State = pair<int, long long>;

        auto cost = [&](int i) -> long long {
            return max(0LL, (long long)max(nums[i-1], nums[i+1]) + 1 - nums[i]);
        };

        auto best = [](State a, State b) -> State {
            if (a.first != b.first) return a.first > b.first ? a : b;
            return a.second < b.second ? a : b;
        };

        vector<array<State, 2>> dp(n);
        dp[1][0] = {0, 0};
        dp[1][1] = {1, cost(1)};

        for (int i = 2; i <= n-2; i++) {
            dp[i][0] = best(dp[i-1][0], dp[i-1][1]);
            dp[i][1] = {dp[i-1][0].first + 1, dp[i-1][0].second + cost(i)};
        }

        State ans = best(dp[n-2][0], dp[n-2][1]);
        return ans.second;
    }
};