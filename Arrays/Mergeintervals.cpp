// @ https://leetcode.com/problems/merge-intervals/

class Solution {
    public:
        vector<vector<int>> merge(vector<vector<int>> &intervals) {
            // Step 1: Sort the intervals based on their start times.
            // Sorting ensures that if two intervals overlap, they will be next to each other.
            // The default sorting function sorts based on the first element of each vector (i.e., start time).
            sort(intervals.begin(), intervals.end());
    
            // Step 2: Create a result vector to store the merged intervals.
            vector<vector<int>> ans;
    
            // Step 3: Insert the first interval from the sorted list into the result.
            ans.push_back(intervals[0]);
    
            // Step 4: Iterate through the remaining intervals.
            for (int i = 1; i < intervals.size(); i++) {
                // Step 4a: Check if the current interval overlaps with the last interval in 'ans'.
                // Two intervals [a, b] and [c, d] overlap if c <= b.
                if (intervals[i][0] <= ans.back()[1]) {
                    // Overlapping case: Merge the current interval with the last interval in 'ans'.
                    // Update the end time of the last interval in 'ans' to be the maximum end time.
                    ans.back()[1] = max(ans.back()[1], intervals[i][1]);
                }
                else {
                    // Non-overlapping case: The current interval does not overlap with the last one.
                    // Simply add the current interval as a new separate interval in 'ans'.
                    ans.push_back(intervals[i]);
                }
            }
    
            // Step 5: Return the result, which contains all merged intervals.
            return ans;
        }
    };
    