/*
Problem: 55. Jump Game
Link: https://leetcode.com/problems/jump-game/

Description:
You are given an array nums where nums[i] tells the maximum distance
you can jump forward from index i.

Return true if you can reach the last index, otherwise return false.

Approach: Greedy

Keep track of the farthest index that can be reached so far.

While traversing:
- If the current index is greater than farthest, then we cannot even
  reach this index, so reaching the end is impossible.
- Otherwise, update farthest using:

      farthest = max(farthest, i + nums[i])

If farthest reaches or crosses the last index, return true.

Why this works:
We do not need to decide the exact jump sequence.

At every index, we only care about the maximum reachable boundary.
If an index lies within that boundary, then it is reachable and can
possibly extend our reach further.

Time Complexity: O(n)
Space Complexity: O(1)
*/

class Solution {
public:
    bool canJump(vector<int>& nums) {

        int farthest = 0;

        for (int i = 0; i < nums.size(); i++) {

            // If current index is beyond the farthest reachable position,
            // then we cannot reach this index at all.
            if (i > farthest) {
                return false;
            }

            // From index i, we can jump at most nums[i] steps.
            // So i + nums[i] is the farthest position reachable
            // using this index.
            farthest = max(farthest, i + nums[i]);

            // Once we can reach or cross the last index,
            // the answer is already true.
            if (farthest >= nums.size() - 1) {
                return true;
            }
        }

        return true;
    }
};