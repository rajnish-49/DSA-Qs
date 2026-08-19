/*
Problem: 452. Minimum Number of Arrows to Burst Balloons
Link: https://leetcode.com/problems/minimum-number-of-arrows-to-burst-balloons/

Description:
Each balloon is an interval [start, end].
An arrow shot at x bursts every balloon for which:

    start <= x <= end

We need the minimum number of arrows to burst all balloons.

Approach: Greedy

Key idea:
Consider the balloon that ends earliest.

We are forced to burst this balloon, so we must shoot an arrow
somewhere inside its interval.

Among all possible positions, shooting at its END is the best choice
because it places the arrow as far right as possible while still
bursting this balloon.

That gives the same arrow the maximum chance of also lying inside
the upcoming balloons.

Therefore we sort by END time.

After sorting:
- Shoot an arrow at the end of the first balloon.
- If the next balloon starts <= current arrow position,
  the same arrow also bursts it.
- If its start > current arrow position, the old arrow can never
  burst it, so we are forced to shoot another arrow.

Time Complexity: O(n log n)
Space Complexity: O(1) apart from sorting.
*/

class Solution {
public:
    int findMinArrowShots(vector<vector<int>>& points) {

        // Sort by ending position because we want to handle the
        // balloon that finishes earliest first.
        //
        // For that balloon, we must place an arrow somewhere before
        // or at its end. Choosing exactly its end is the latest possible
        // valid position, so it gives the arrow the best chance to also
        // hit balloons that start later.
        sort(points.begin(), points.end(),
             [](vector<int>& a, vector<int>& b) {
                 return a[1] < b[1];
             });

        int arrows = 1;

        // Greedy choice:
        // Burst the earliest-ending balloon by shooting at its end.
        long long arrowPosition = points[0][1];

        for (int i = 1; i < points.size(); i++) {

            // If current balloon starts at or before arrowPosition,
            // then arrowPosition lies inside this balloon too.
            //
            // Since we sorted by end, its ending position is guaranteed
            // to be >= the current arrow position.
            //
            // So no new arrow is needed.
            if (points[i][0] <= arrowPosition) {
                continue;
            }

            // If current.start > arrowPosition, the previous arrow lies
            // completely to the left of this balloon.
            //
            // Hence this balloon cannot be burst by the previous arrow,
            // so a new arrow is unavoidable.
            arrows++;

            // Again place the new arrow at this balloon's end so that
            // it is as far right as possible and may also burst future
            // overlapping balloons.
            arrowPosition = points[i][1];
        }

        return arrows;
    }
};