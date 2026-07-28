class Solution {
public:
    using ll = long long;

    long long subArrayRanges(vector<int>& nums) {
        int n = nums.size();

        vector<int> prevLess(n), nextLess(n);
        vector<int> prevGreater(n), nextGreater(n);
        stack<int> st;
        for (int i = 0; i < n; i++) {
            while (!st.empty() && nums[st.top()] >= nums[i]) {
                st.pop();
            }

            prevLess[i] = st.empty() ? -1 : st.top();
            st.push(i);
        }

        while (!st.empty()) st.pop();

        for (int i = n - 1; i >= 0; i--) {
            while (!st.empty() && nums[st.top()] > nums[i]) {
                st.pop();
            }

            nextLess[i] = st.empty() ? n : st.top();
            st.push(i);
        }

        while (!st.empty()) st.pop();

        for (int i = 0; i < n; i++) {
            while (!st.empty() && nums[st.top()] <= nums[i]) {
                st.pop();
            }

            prevGreater[i] = st.empty() ? -1 : st.top();
            st.push(i);
        }

        while (!st.empty()) st.pop();
        for (int i = n - 1; i >= 0; i--) {
            while (!st.empty() && nums[st.top()] < nums[i]) {
                st.pop();
            }

            nextGreater[i] = st.empty() ? n : st.top();
            st.push(i);
        }

        ll ans = 0;

        for (int i = 0; i < n; i++) {
            ll leftMin = i - prevLess[i];
            ll rightMin = nextLess[i] - i;
            ll countMin = leftMin * rightMin;

            ll leftMax = i - prevGreater[i];
            ll rightMax = nextGreater[i] - i;
            ll countMax = leftMax * rightMax;

            ans += (ll)nums[i] * countMax;
            ans -= (ll)nums[i] * countMin;
        }

        return ans;
    }
};