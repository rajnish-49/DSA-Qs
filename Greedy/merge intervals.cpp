class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        sort(intervals.begin(), intervals.end(),
             [](const vector<int>& a, const vector<int>& b) {
                 return a[0] < b[0];
             });

        vector<vector<int>> result;
        for (auto& cur : intervals) {
            if (result.empty() || cur[0] > result.back()[1]) {
                // no overlap with the last kept interval -> start fresh
                result.push_back(cur);
            } else {
                // overlap -> extend the end, only if cur pushes it further
                result.back()[1] = max(result.back()[1], cur[1]);
            }
        }
        return result;
    }
};