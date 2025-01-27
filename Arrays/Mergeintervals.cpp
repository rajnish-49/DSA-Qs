// @ https://leetcode.com/problems/merge-intervals/

class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>> &intervals) {

        // Step 1: Sort intervals based on the starting value of each interval
        sort(intervals.begin(), intervals.end());

        // Step 2: Create a result vector to store the merged intervals
        vector<vector<int>> ans;

        // Step 3: Add the first interval to the result as there is nothing to merge initially
        ans.push_back(intervals[0]);

        // Step 4: Iterate over the remaining intervals
        for (int i = 1; i < intervals.size(); i++) {

            // Step 5: Check if the current interval overlaps with the last interval in the result
            if (ans[ans.size() - 1][1] >= intervals[i][0]) {
                // If they overlap, merge the intervals by updating the end value of the last interval in 'ans'
                ans[ans.size() - 1][1] = max(ans[ans.size() - 1][1], intervals[i][1]);
            }
            else {
                // If they do not overlap, add the current interval to the result as a new entry
                ans.push_back(intervals[i]);
            }
        }

        // Step 6: Return the final merged intervals
        return ans;
    }
};
