// @https://leetcode.com/problems/jump-game/description/

class Solution {
public:
    bool canJump(vector<int>& nums) {
        int maxReach = 0; // This tracks the furthest index we can reach so far 
                          // based on all jumps we've made up to the current point.
        
        int n = nums.size();
        
        // We walk through each index, imagining that we're "exploring" the path.
        // At each step, we check if we're still within reach of where we could get to earlier.
        for (int i = 0; i < n; i++) {
            
            // If our current position is beyond the furthest point we've been able to reach,
            // that means there's a "gap" in the path — we can't even stand on this index.
            // This is the immediate sign that the end is unreachable.
            if (i > maxReach) return false;
            
            // From this index, we can jump up to nums[i] steps ahead.
            // i + nums[i] gives the furthest index we could reach by jumping from here.
            // If that's further than what we already had, we extend our maxReach.
            maxReach = max(maxReach, i + nums[i]);
            
            // If at any point our reachable range already includes or passes the last index,
            // there's no need to check further — we know we can reach the goal.
            if (maxReach >= n - 1) return true;
        }
        
        // If we finish the loop without getting stuck, it means the last index is reachable.
        return true;
    }
};






