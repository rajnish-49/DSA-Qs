/*
Problem: Activity Selection Problem
Link: https://www.geeksforgeeks.org/problems/activity-selection-1587115620/1

Description:
You are given start and finish times of N activities.

Only one activity can be performed at a time, so selected activities
must not overlap.

The goal is to select the maximum number of non-overlapping activities.

Approach: Greedy

The greedy choice is to always select the activity that finishes earliest,
because it leaves maximum time for future activities.

*/


class Solution {
public:
    int activitySelection(vector<int> &start, vector<int> &finish) {

        int n = start.size();

        // Store each activity as {start, finish}.
        vector<vector<int>> activities(n);

        for (int i = 0; i < n; i++) {
            activities[i] = {start[i], finish[i]};
        }

        // Custom sort according to finish time.
        sort(activities.begin(), activities.end(),
             [](vector<int>& a, vector<int>& b) {
                 return a[1] < b[1];
             });

        int count = 1;
        int lastFinish = activities[0][1];

        for (int i = 1; i < n; i++) {

            // Current activity can be selected only if it starts
            // after the previously selected activity has finished.
            if (activities[i][0] >= lastFinish) {
                count++;
                lastFinish = activities[i][1];
            }
        }

        return count;
    }
};