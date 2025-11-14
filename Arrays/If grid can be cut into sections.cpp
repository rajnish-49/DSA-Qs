// @ https://leetcode.com/problems/check-if-grid-can-be-cut-into-sections/description/?envType=daily-question&envId=2025-03-25

class Solution
{
private:
    // CORE ALGORITHM: Merge overlapping intervals to find non-overlapping sections
    // This function implements the classic "Merge Intervals" technique
    // Purpose: Count how many distinct non-overlapping regions exist along an axis
    vector<vector<int>> merge(vector<vector<int>> &intervals)
    {

        // STEP 1: Sort by start position - ABSOLUTELY CRITICAL
        // Why? Allows linear scan to detect overlaps efficiently
        // After sorting: if interval A starts before B, we only need to check
        // if A's end overlaps with B's start
        sort(intervals.begin(), intervals.end());

        // STEP 2: Result container for merged intervals
        // Each element represents one continuous occupied region
        vector<vector<int>> ans;

        // STEP 3: Initialize with first interval
        // Base case: first interval is always added as-is
        ans.push_back(intervals[0]);

        // STEP 4: Process each subsequent interval
        for (int i = 1; i < intervals.size(); i++)
        {

            // KEY OVERLAP CHECK: current_start < previous_end
            // If intervals[i][0] < ans.back()[1], they overlap
            // Example: [1,3] and [2,5] → 2 < 3 → overlap exists
            if (intervals[i][0] < ans.back()[1])
            {

                // MERGE OPERATION: Extend the last interval
                // New end = max(current_end, previous_end)
                // This covers the entire merged region
                ans.back()[1] = max(ans.back()[1], intervals[i][1]);
            }
            else
            {
                // NO OVERLAP: Intervals are completely separate
                // This creates a new distinct section (gap exists between them)
                ans.push_back(intervals[i]);
            }
        }

        // FINAL RESULT: Number of elements = number of non-overlapping sections
        return ans;
    }

public:
    // MAIN PROBLEM: Determine if grid can be divided into 3+ sections
    // STRATEGY: Convert 2D problem to two 1D problems
    // Check if we can make cuts along X-axis OR Y-axis
    bool checkValidCuts(int n, vector<vector<int>> &rectangles)
    {

        // STEP 1: Sort rectangles (preprocessing for consistency)
        // Not strictly necessary but helps with organized processing
        sort(rectangles.begin(), rectangles.end());

        // STEP 2: Create projection containers
        // We'll project all rectangles onto both coordinate axes
        vector<vector<int>> xaxis; // X-axis intervals [left, right]
        vector<vector<int>> yaxis; // Y-axis intervals [bottom, top]

        // STEP 3: PROJECT each rectangle onto both axes
        // KEY INSIGHT: Rectangle [x1,y1,x2,y2] creates:
        // - X projection: [x1,x2] (horizontal span)
        // - Y projection: [y1,y2] (vertical span)
        for (auto coord : rectangles)
        {
            int x1 = coord[0]; // Left edge
            int y1 = coord[1]; // Bottom edge
            int x2 = coord[2]; // Right edge
            int y2 = coord[3]; // Top edge

            // Store where this rectangle occupies the X-axis
            xaxis.push_back({x1, x2});

            // Store where this rectangle occupies the Y-axis
            yaxis.push_back({y1, y2});
        }

        // STEP 4: FIND non-overlapping sections on each axis
        // Apply interval merging to count distinct regions
        vector<vector<int>> xSections = merge(xaxis); // Distinct X regions
        vector<vector<int>> ySections = merge(yaxis); // Distinct Y regions

        // STEP 5: CHECK cutting feasibility
        // CRITICAL INSIGHT: n sections allow (n-1) cuts
        // We need 3+ sections to make 2+ cuts (creating 3+ pieces)
        //
        // VISUALIZATION:
        // 3 sections on X-axis: [A] gap [B] gap [C]
        // We can make 2 vertical cuts in the gaps → 3 pieces
        if (xSections.size() >= 3 || ySections.size() >= 3)
        {
            return true; // Sufficient sections for valid cuts
        }
        else
        {
            return false; // Insufficient sections
        }
    }
};

/*
COMPLETE ALGORITHM WALKTHROUGH:

PROBLEM ESSENCE:
- Given rectangles on a grid, can we make 2+ cuts to create 3+ sections?
- A cut is only valid if it doesn't intersect any rectangle

SOLUTION APPROACH:
1. Convert 2D cutting problem to 1D interval analysis
2. Project rectangles onto X and Y axes independently
3. Find gaps between rectangles (where cuts are possible)
4. Count non-overlapping regions on each axis

MATHEMATICAL INSIGHT:
- n non-overlapping regions → (n-1) possible cuts → n resulting pieces
- Need 3+ regions to achieve 2+ cuts for 3+ pieces

DETAILED EXAMPLE:
Input: rectangles = [[0,0,2,2], [1,1,3,3], [4,0,6,2]]

Step 1 - X-axis projection:
[0,2], [1,3], [4,6]

Step 2 - Merge X intervals:
[0,2] and [1,3] overlap (1 < 2) → merge to [0,3]
[0,3] and [4,6] don't overlap (4 ≥ 3) → keep separate
Result: [0,3], [4,6] → 2 sections

Step 3 - Y-axis projection:
[0,2], [1,3], [0,2]

Step 4 - Merge Y intervals:
[0,2] and [1,3] overlap (1 < 2) → merge to [0,3]
[0,3] and [0,2] overlap (0 < 3) → merge to [0,3]
Result: [0,3] → 1 section

Step 5 - Final check:
X has 2 sections, Y has 1 section
Neither ≥ 3, so return false

TIME COMPLEXITY: O(n log n) - sorting dominates
SPACE COMPLEXITY: O(n) - storing projections and merged intervals
*/
