// If you're going to keep one interval and must sacrifice another among two overlapping ones, 
// always keep the one that ends earliest — 
// because it leaves the most room for future intervals to still fit without overlapping.

class Solution {
public:
    int eraseOverlapIntervals(vector<vector<int>>& intervals) {
        sort(intervals.begin(), intervals.end(),
             [](const vector<int>& a, const vector<int>& b) {
                 return a[1] < b[1];   // sort by END
             });

        int removals = 0;
        long long lastEnd = LLONG_MIN;

        for (auto& cur : intervals) {
            if (cur[0] >= lastEnd) {
                // no overlap, keep this one
                lastEnd = cur[1];
            } else {
                // overlaps the kept interval -> must discard cur
                removals++;
            }
        }
        return removals;
    }
};