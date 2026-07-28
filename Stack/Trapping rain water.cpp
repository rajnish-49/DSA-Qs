class Solution {
public:
    int trap(vector<int>& height) {
        int n = height.size();
        stack<int> st; // stores indices
        int water = 0;
        
        for (int i = 0; i < n; i++) {
            while (!st.empty() && height[st.top()] < height[i]) {
                int bottom = st.top(); st.pop();
                if (st.empty()) break;
                int left = st.top();
                int width = i - left - 1;
                int level = min(height[left], height[i]) - height[bottom];
                water += level * width;
            }
            st.push(i);
        }
        
        return water;
    }
};