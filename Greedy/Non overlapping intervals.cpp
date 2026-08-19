/*
Problem: 435. Non-overlapping Intervals
Link: https://leetcode.com/problems/non-overlapping-intervals/

Description:
You are given a list of intervals [start, end].

Return the minimum number of intervals that must be removed so that
the remaining intervals do not overlap.

Approach: Greedy

Instead of directly minimizing removals, we maximize the number of
non-overlapping intervals we can keep.

Greedy choice:
Always keep the interval that finishes earliest.

Why?
If two intervals overlap, keeping the one with the smaller end time
leaves more space for future intervals.

Steps:
1. Sort intervals by end time.
2. Keep the first interval.
3. For every next interval:
   - If currentStart >= lastEnd, keep it.
   - Otherwise, it overlaps, so remove it.
4. Count the removed intervals.

Time Complexity: O(n log n)
Space Complexity: O(1) apart from sorting.
*/

class Solution {
public:
    int eraseOverlapIntervals(vector<vector<int>>& intervals) {

        // Sort intervals according to their end time.
        sort(intervals.begin(), intervals.end(),
             [](vector<int>& a, vector<int>& b) {
                 return a[1] < b[1];
             });

        int removed = 0;
        int lastEnd = intervals[0][1];

        for (int i = 1; i < intervals.size(); i++) {

            // If current interval starts before the last selected
            // interval ends, they overlap.
            if (intervals[i][0] < lastEnd) {
                removed++;
            }

            // Otherwise, keep this interval and update lastEnd.
            else {
                lastEnd = intervals[i][1];
            }
        }

        return removed;
    }
};