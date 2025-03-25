// @ https://leetcode.com/problems/check-if-grid-can-be-cut-into-sections/description/?envType=daily-question&envId=2025-03-25

class Solution {
    private:
        // Function to merge overlapping intervals
        vector<vector<int>> merge(vector<vector<int>> &intervals) {
            // Step 1: Sort intervals based on the start value.
            sort(intervals.begin(), intervals.end());
    
            // Step 2: Initialize a vector to store the merged intervals.
            vector<vector<int>> ans;
    
            // Step 3: Insert the first interval into the result.
            ans.push_back(intervals[0]);
    
            // Step 4: Iterate through the remaining intervals.
            for (int i = 1; i < intervals.size(); i++) {
                // Step 4a: If the current interval overlaps with the last interval in 'ans'
                if (intervals[i][0] < ans.back()[1]) { 
                    // Merge the intervals by updating the end time of the last interval
                    ans.back()[1] = max(ans.back()[1], intervals[i][1]);
                }
                else {
                    // Step 4b: If no overlap, add the interval as a separate entry
                    ans.push_back(intervals[i]);
                }
            }
    
            // Step 5: Return the merged intervals.
            return ans;
        }
    
    public:
        bool checkValidCuts(int n, vector<vector<int>>& rectangles) {
            // Step 1: Sort rectangles by their bottom-left corner coordinates
            sort(rectangles.begin(), rectangles.end());
    
            // Step 2: Create two separate lists for x-axis and y-axis projections
            vector<vector<int>> xaxis;
            vector<vector<int>> yaxis;
    
            // Step 3: Extract x and y interval projections from the rectangles
            for (auto coord : rectangles) {
                int x1 = coord[0]; // Bottom-left x
                int y1 = coord[1]; // Bottom-left y
                int x2 = coord[2]; // Top-right x
                int y2 = coord[3]; // Top-right y
    
                // Step 3a: Store x-axis interval [x1, x2]
                xaxis.push_back({x1, x2});
    
                // Step 3b: Store y-axis interval [y1, y2]
                yaxis.push_back({y1, y2});
            }
    
            // Step 4: Merge intervals along x-axis and y-axis separately
            vector<vector<int>> res1 = merge(xaxis);
            vector<vector<int>> res2 = merge(yaxis);
    
            // Step 5: Check if at least three non-overlapping sections exist in either x or y
            if (res1.size() >= 3 || res2.size() >= 3) {
                return true;
            }
            else {
                return false;
            }
        }
    };
    