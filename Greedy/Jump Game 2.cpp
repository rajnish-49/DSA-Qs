/*
Problem: 45. Jump Game II
Link: https://leetcode.com/problems/jump-game-ii/

Description:
You are given an array nums where nums[i] tells the maximum distance
you can jump forward from index i.

Return the minimum number of jumps needed to reach the last index.

It is guaranteed that the last index is reachable.

Approach: Greedy / BFS-like range expansion

Instead of deciding the exact index we should jump to, think in terms
of ranges.

currentEnd:
    The farthest index reachable using the number of jumps
    we have already counted.

farthest:
    While scanning the current reachable range, this stores
    the farthest index we could reach using ONE MORE jump.

Example:
nums = [2,3,1,1,4]

Initially:
0 jumps -> we are only at index 0
currentEnd = 0

From index 0:
we can reach up to index 2

So after taking 1 jump:
currentEnd = 2

Now indices 1 and 2 are both reachable using 1 jump.
We examine BOTH before taking the second jump.

From index 1 -> can reach 4
From index 2 -> can reach 3

So farthest = 4.

When i reaches currentEnd (= 2), we have finished checking
every position reachable with 1 jump.

Now another jump is necessary:
jumps++
currentEnd = farthest = 4

Thus we reach the last index in 2 jumps.

Why minimum?
Before taking the next jump, we examine every position reachable
with the current number of jumps and keep the maximum possible
next reach.

This is similar to BFS:
each reachable range represents one BFS level / one jump.

Time Complexity: O(n)
Space Complexity: O(1)
*/

class Solution {
public:
    int jump(vector<int>& nums) {

        int n = nums.size();

        // Already at the destination.
        if (n == 1)
            return 0;

        int jumps = 0;

        // Right boundary of the range reachable using
        // the jumps that we have already counted.
        //
        // Initially jumps = 0, so we can only reach index 0.
        int currentEnd = 0;

        // While scanning the current range, this stores
        // how far ONE MORE jump could take us.
        int farthest = 0;

        // No need to process the last index because once we
        // reach it, we do not need another jump.
        for (int i = 0; i < n - 1; i++) {

            // If we were standing at index i, the farthest
            // position we could jump to is i + nums[i].
            //
            // We take the maximum over all indices in the
            // current reachable range.
            farthest = max(farthest, i + nums[i]);

            // currentEnd is the last index reachable with
            // the CURRENT number of jumps.
            //
            // Therefore when i reaches currentEnd, we have
            // finished examining every possible position
            // reachable using those jumps.
            if (i == currentEnd) {

                // To move beyond the current range,
                // we now have to take one more jump.
                jumps++;

                // farthest was calculated by checking all
                // positions in the previous range.
                //
                // So after taking this new jump, everything
                // up to 'farthest' becomes reachable.
                currentEnd = farthest;

                // We can already reach the destination.
                if (currentEnd >= n - 1)
                    break;
            }
        }

        return jumps;
    }
};