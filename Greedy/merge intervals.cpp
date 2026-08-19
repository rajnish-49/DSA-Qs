/*
Problem: 56. Merge Intervals
Link: https://leetcode.com/problems/merge-intervals/

Description:
You are given a list of intervals [start, end].

If two intervals overlap, merge them into one larger interval.

Example:
[1,3] and [2,6] overlap, so they become [1,6].

Approach:

1. Sort intervals by start time.
2. Keep a current interval that we are trying to extend.
3. For every next interval:
   - If next.start <= current.end, they overlap.
     Extend current.end to max(current.end, next.end).
   - Otherwise, current interval is complete.
     Add it to the answer and start a new current interval.
4. Add the final current interval at the end.

Why sorting by start works:
After sorting, if the next interval starts after current.end,
then all later intervals will also start even later, so none of them
can overlap with the current interval.

Time Complexity: O(n log n)
Space Complexity: O(n) for the answer.
*/

class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {

        // Sort intervals according to start time.
        sort(intervals.begin(), intervals.end(),
             [](vector<int>& a, vector<int>& b) {
                 return a[0] < b[0];
             });

        vector<vector<int>> ans;

        int start = intervals[0][0];
        int end = intervals[0][1];

        for (int i = 1; i < intervals.size(); i++) {

            // If current interval starts before the previous
            // merged interval ends, they overlap.
            if (intervals[i][0] <= end) {

                // Extend the merged interval if needed.
                end = max(end, intervals[i][1]);
            }

            else {

                // No overlap, so the previous merged interval
                // is complete.
                ans.push_back({start, end});

                // Start a new interval.
                start = intervals[i][0];
                end = intervals[i][1];
            }
        }

        // Add the last merged interval.
        ans.push_back({start, end});

        return ans;
    }
};