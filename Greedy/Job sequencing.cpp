#include <bits/stdc++.h>
using namespace std;

/*
PROBLEM: Job Sequencing with Deadlines
Given a set of jobs with deadlines and profits, schedule jobs to maximize profit.
Each job takes 1 unit of time and must be completed before its deadline.

GREEDY STRATEGY:
Always pick the job with HIGHEST PROFIT first, then try to schedule it
as LATE AS POSSIBLE (but before deadline) to leave room for other jobs.

TIME COMPLEXITY: O(n²) - for each job, we scan up to maxDeadline slots
SPACE COMPLEXITY: O(maxDeadline) - for slot array
*/

class Solution
{
public:
    vector<int> jobSequencing(vector<int> &deadline, vector<int> &profit)
    {
        int n = deadline.size();
        vector<pair<int, int>> jobs(n); // pair<profit, deadline>

        // Step 1: Create pairs of (profit, deadline) for easier sorting
        // We need both values together to apply greedy strategy
        for (int i = 0; i < n; i++)
        {
            jobs[i] = {profit[i], deadline[i]};
        }

        // Step 2: Sort jobs by profit in DESCENDING order
        // 🎯 GREEDY CHOICE: Always consider highest profit job first
        // This ensures we prioritize jobs that contribute most to total profit
        sort(jobs.begin(), jobs.end(), [](auto &a, auto &b)
             {
                 return a.first > b.first; // higher profit first
             });

        // Step 3: Find maximum deadline to determine our time horizon
        // We need to know how many time slots are available (1 to maxDeadline)
        int maxDeadline = 0;
        for (auto &job : jobs)
            maxDeadline = max(maxDeadline, job.second);

        // Step 4: Initialize time slots array
        // slot[i] = -1 means time slot i is FREE
        // slot[i] = profit means time slot i is occupied by job with that profit
        // Index 0 is unused (time slots are 1-indexed)
        vector<int> slot(maxDeadline + 1, -1);

        int countJobs = 0;   // Number of jobs successfully scheduled
        int totalProfit = 0; // Sum of profits from scheduled jobs

        // Step 5: Schedule jobs using greedy approach
        for (auto &job : jobs)
        {
            int currentProfit = job.first;
            int deadline = job.second;

            // 🔑 KEY STRATEGY: Try to schedule as LATE as possible
            // Search from deadline backwards to 1 to find latest available slot
            // This leaves earlier slots free for jobs with tighter deadlines
            for (int timeSlot = deadline; timeSlot > 0; timeSlot--)
            {
                if (slot[timeSlot] == -1)
                {                                   // Found a free slot!
                    slot[timeSlot] = currentProfit; // Occupy this slot
                    totalProfit += currentProfit;   // Add to total profit
                    countJobs++;                    // Increment job count
                    break;                          // Job scheduled, move to next job
                }
                // If slot is occupied, try the previous time slot
            }
            // If no free slot found before deadline, this job is skipped
        }

        return {countJobs, totalProfit}; // Return [number of jobs, total profit]
    }
};

/*
WHY THIS GREEDY APPROACH WORKS:

1. OPTIMAL SUBSTRUCTURE: If we have an optimal solution, removing any job
   still leaves an optimal solution for remaining jobs.

2. GREEDY CHOICE PROPERTY: Choosing the highest profit job first is always
   safe because:
   - If this job can be scheduled, it should be (highest profit)
   - If it can't be scheduled, no other arrangement will help
   - Scheduling it late preserves options for other jobs

#include <bits/stdc++.h>
using namespace std;

class Solution {
  public:
    vector<int> jobSequencing(vector<int> &deadline, vector<int> &profit) {
        int n = deadline.size();

        // Step 1: Create pairs of (deadline, profit)
        vector<pair<int, int>> jobs(n); // pair<deadline, profit>
        for (int i = 0; i < n; i++) {
            jobs[i] = {deadline[i], profit[i]};
        }

        // Step 2: Sort jobs by deadline ascending
        sort(jobs.begin(), jobs.end(), [](auto &a, auto &b){
            return a.first < b.first;
        });

        // Step 3: Use min-heap to track selected jobs' profits
        priority_queue<int, vector<int>, greater<int>> pq; // min-heap

        for (auto &job : jobs) {
            int d = job.first;
            int p = job.second;

            if (pq.size() < d) {
                pq.push(p); // schedule this job
            } else if (!pq.empty() && pq.top() < p) {
                pq.pop();   // remove least profitable job
                pq.push(p); // add current job
            }
        }

        int totalProfit = 0;
        int countJobs = pq.size();

        while (!pq.empty()) {
            totalProfit += pq.top();
            pq.pop();
        }

        return {countJobs, totalProfit};
    }
};
