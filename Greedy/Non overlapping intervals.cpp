class Solution {
public:
    int eraseOverlapIntervals(vector<vector<int>>& intervals) {
        if (intervals.empty()) return 0; // Edge case: no intervals

        // STEP 1: Sort intervals by their END time (ascending)
        // Why? Picking intervals that end earlier leaves more room
        // for future intervals → maximizes non-overlapping set.
        sort(intervals.begin(), intervals.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[1] < b[1];
        });

        int end = INT_MIN; // Tracks end of the last chosen (kept) interval
        int count = 0;     // Number of non-overlapping intervals we can keep

        // STEP 2: Iterate over sorted intervals
        for (auto &interval : intervals) {
            // If current interval starts AFTER or AT the last chosen end,
            // it does NOT overlap → we can keep it
            if (interval[0] >= end) {
                end = interval[1]; // Update end to current interval's end
                count++;           // Increment kept intervals
            }
            // Else: it overlaps → skip/remove it (just don't count it)
        }

        // STEP 3: Total removals = total intervals - intervals we kept
        return intervals.size() - count;
    }
};
