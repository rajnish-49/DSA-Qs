/*
Problem: Job Sequencing with Deadlines
Link: https://www.geeksforgeeks.org/problems/job-sequencing-problem-1587115620/1

Description:
Each job has:
- a deadline
- a profit

Every job takes exactly 1 unit of time.

A job earns its profit only if it is completed on or before its deadline.

Goal:
Schedule jobs so that the total profit is maximum.

Approach: Greedy

1. Sort jobs by profit in descending order.
   We want to try scheduling the most profitable jobs first.

2. For each job, try to place it in the latest free slot
   on or before its deadline.

Why latest possible slot?

Suppose a job has deadline 3.
It can be placed in slot 1, 2, or 3.

If we place it in slot 1 unnecessarily, we may block another job
whose deadline is only 1.

So placing a job as late as possible keeps earlier slots free
for jobs with tighter deadlines.

3. If no slot is free before its deadline, skip that job.

Time Complexity: O(n log n + n * maxDeadline)
Space Complexity: O(maxDeadline)
*/

class Solution {
public:
    vector<int> jobSequencing(vector<int> &deadline, vector<int> &profit) {

        int n = deadline.size();

        vector<vector<int>> jobs(n);

        for (int i = 0; i < n; i++) {
            jobs[i] = {deadline[i], profit[i]};
        }

        // Process higher-profit jobs first because if we have to choose
        // between conflicting jobs, keeping the more profitable one
        // gives a better total profit.
        sort(jobs.begin(), jobs.end(),
             [](vector<int>& a, vector<int>& b) {
                 return a[1] > b[1];
             });

        int maxDeadline = 0;

        for (int i = 0; i < n; i++) {
            maxDeadline = max(maxDeadline, jobs[i][0]);
        }

        // slot[t] tells whether time slot t is already occupied.
        vector<int> slot(maxDeadline + 1, 0);

        int jobsDone = 0;
        int totalProfit = 0;

        for (int i = 0; i < n; i++) {

            int d = jobs[i][0];
            int p = jobs[i][1];

            // Try to place this job in the latest possible free slot.
            //
            // We go backward from its deadline because using a later
            // slot preserves earlier slots for jobs with smaller deadlines.
            for (int t = d; t >= 1; t--) {

                if (slot[t] == 0) {

                    slot[t] = 1;
                    jobsDone++;
                    totalProfit += p;

                    break;
                }
            }
        }

        return {jobsDone, totalProfit};
    }
};