class Solution {
public:
    vector<vector<int>> filterOccupiedIntervals(vector<vector<int>>& occupiedIntervals,
                                                int freeStart, int freeEnd) {
                                      
       
        sort(occupiedIntervals.begin(), occupiedIntervals.end());

        vector<vector<int>> merged;

        for (auto &interval : occupiedIntervals) {
            int start = interval[0];
            int end = interval[1];

            if (merged.empty() || start > merged.back()[1] + 1) {
                merged.push_back({start, end});
            } else {
                merged.back()[1] = max(merged.back()[1], end);
            }
        }

        vector<vector<int>> ans;

        for (auto &interval : merged) {
            int start = interval[0];
            int end = interval[1];

            if (end < freeStart || start > freeEnd) {
                ans.push_back({start, end});
            } else {
                if (start < freeStart) {
                    ans.push_back({start, freeStart - 1});
                }

                if (end > freeEnd) {
                    ans.push_back({freeEnd + 1, end});
                }
            }
        }

        return ans;
    }
};