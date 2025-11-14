// @https://leetcode.com/problems/most-profit-assigning-work/

class Solution {
public:
    int maxProfitAssignment(vector<int>& difficulty, vector<int>& profit,
                            vector<int>& worker) {
        // Step 1: Combine difficulty and profit into pairs (job difficulty, job profit)
        vector<pair<int, int>> jobs;
        for (int i = 0; i < difficulty.size(); i++) {
            jobs.push_back({difficulty[i], profit[i]});
        }

        // Step 2: Sort the jobs by difficulty in ascending order
        sort(jobs.begin(), jobs.end());

        // Step 3: Preprocess the jobs array to store maximum profit up to that difficulty
        // For example, if at difficulty 4 max profit is 10, and at difficulty 6 profit is 8,
        // we keep max profit at difficulty 6 as 10 (because a worker who can do difficulty 6
        // can also do difficulty 4 job which pays more)
        for (int i = 1; i < jobs.size(); i++) {
            jobs[i].second = max(jobs[i].second, jobs[i - 1].second);
        }

        // Step 4: Separate difficulties and max profits into their own vectors
        // This simplifies binary search later on
        vector<int> sorted_difficulty;
        vector<int> max_profit;
        for (int i = 0; i < jobs.size(); i++) {
            sorted_difficulty.push_back(jobs[i].first); // difficulty array
            if (i == 0)
                max_profit.push_back(jobs[i].second);   // first max profit is just jobs[0].second
            else
                // store max profit up to current difficulty for quick access
                max_profit.push_back(max(max_profit[i - 1], jobs[i].second));
        }

        // Step 5: For each worker, determine the best job they can do using binary search
        // upper_bound returns iterator to first element > ability,
        // so subtract 1 to get last element <= ability (best job they can do)
        int totalProfit = 0;
        for (int ability : worker) {
            auto it = upper_bound(sorted_difficulty.begin(), sorted_difficulty.end(), ability);
            if (it != sorted_difficulty.begin()) {
                int idx = it - sorted_difficulty.begin() - 1; // index of job they can do
                totalProfit += max_profit[idx];                // add max profit up to that difficulty
            }
            // If worker can't do any job (it == begin), profit is 0 (skip)
        }

        // Step 6: Return total profit earned by all workers combined
        return totalProfit;
    }
};
