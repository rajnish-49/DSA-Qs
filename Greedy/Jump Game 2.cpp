/*
 * ============================================================
 * PROBLEM: Jump Game II (LeetCode 45)
 * ============================================================
 *
 * WHAT THE PROBLEM ASKS:
 * Given an array nums where nums[i] = maximum jump length from index i,
 * find the MINIMUM number of jumps to reach the last index.
 * You start at index 0. It is guaranteed you can reach the last index.
 *
 * IMPORTANT DEFINITIONS:
 * - Jump: moving forward from index i to any index i+j where 1 <= j <= nums[i]
 * - nums[i] is a MAXIMUM jump length, not fixed. You can jump less.
 * - Minimum jumps: among all valid paths to the last index, the one
 *   with fewest jumps.
 *
 * CONSTRAINTS AND IMPLICATIONS:
 * - 1 <= nums.length <= 10^4
 * - 0 <= nums[i] <= 1000
 * - Guaranteed reachable → no need to handle impossible case
 *
 * EDGE CASES:
 * - nums.size() == 1: already at last index, 0 jumps needed.
 *   Loop runs 0 times, returns jumps=0. Handled correctly.
 * - nums[0] large enough to reach end in one jump: loop forces one
 *   jump at i=0, returns 1. Correct.
 *
 * ============================================================
 * THINKING FROM FIRST PRINCIPLES
 * ============================================================
 *
 * STEP 1: What does minimum jumps mean?
 * We want to reach the end using as few jumps as possible.
 * Each jump should therefore cover as much ground as possible.
 * Greedy intuition: at each jump, extend reach as far as you can.
 *
 * STEP 2: Think in levels.
 * Reframe the problem as BFS on indices:
 * - Level 0: {0}             you start here, 0 jumps taken
 * - Level 1: all indices reachable from level 0 in exactly 1 jump
 * - Level 2: all indices reachable from level 1 in exactly 1 more jump
 * - ...
 * The answer is: which level does the last index first appear in?
 *
 * STEP 3: Levels are contiguous ranges, not arbitrary sets.
 * Level 1 is some range [1, x], level 2 is [x+1, y], and so on.
 * So you only need to track the END of each level, not every index in it.
 * currentLevelEnd = the last index belonging to the current level.
 *
 * STEP 4: When are you forced to jump?
 * As long as i <= currentLevelEnd, you are still inside the current level.
 * No new jump needed yet.
 * The moment i == currentLevelEnd, the current level is fully exhausted.
 * You MUST take one more jump to proceed.
 *
 * STEP 5: Where do you jump to?
 * You don't need to know which specific index in the current level to
 * jump from. You just need the FARTHEST any index in this level can reach.
 * That is what farthest tracks continuously as i sweeps through the level.
 * When the level ends, farthest already holds the end of the next level.
 *
 * INVARIANTS:
 * - farthest = max(i + nums[i]) for all i seen so far.
 * - currentLevelEnd = farthest at the time the last jump was taken.
 * - jumps = number of level boundaries crossed so far.
 *
 * ============================================================
 * WHY THIS IS CORRECT
 * ============================================================
 *
 * Every index is processed exactly once. For each index i in the current
 * level, farthest is updated. When the level ends, exactly one jump is
 * taken and the next level's boundary is set to farthest.
 * This is equivalent to BFS level-order traversal but without storing
 * any indices explicitly — just two boundary variables.
 *
 * Greedily taking the farthest reach at each level is optimal because:
 * extending reach maximally at each jump minimizes the number of levels
 * needed, which directly minimizes the number of jumps.
 *
 * WHY THE LOOP STOPS AT n-2 (i < nums.size() - 1):
 * Once you reach the last index you don't need to jump further.
 * If you included i = n-1 in the loop, you might count one extra jump
 * unnecessarily when i == currentLevelEnd at the last index.
 *
 * ============================================================
 * DRY RUN: nums = [2, 3, 1, 1, 4], n = 5
 * ============================================================
 *
 * Initial state: jumps=0, currentLevelEnd=0, farthest=0
 *
 * i=0: farthest = max(0, 0+2) = 2
 *       i == currentLevelEnd(0) → forced to jump
 *       jumps=1, currentLevelEnd=2
 *       meaning: level 1 spans indices 1 and 2
 *
 * i=1: farthest = max(2, 1+3) = 4
 *       i != currentLevelEnd, still inside level 1
 *
 * i=2: farthest = max(4, 2+1) = 4
 *       i == currentLevelEnd(2) → forced to jump
 *       jumps=2, currentLevelEnd=4
 *       meaning: level 2 spans indices 3 and 4
 *
 * i=3: loop condition i < n-1 = 4, so i=3 runs
 *       farthest = max(4, 3+1) = 4
 *       i != currentLevelEnd(4), no jump
 *
 * loop exits (i=4 would be n-1, excluded)
 * return jumps = 2. Correct.
 *
 * ============================================================
 * COMPLEXITY
 * ============================================================
 *
 * TIME: O(n)
 * Single pass through the array from index 0 to n-2.
 * Each index processed exactly once with O(1) work per index.
 *
 * SPACE: O(1)
 * Only three integer variables used regardless of input size.
 *
 * OPTIMALITY:
 * You must read every element at least once to know the reachability
 * from each index, so O(n) is a lower bound. This achieves it.
 *
 * ============================================================
 */

class Solution {
public:
    int jump(vector<int>& nums) {

        // jumps: counts how many level boundaries have been crossed.
        // each crossing corresponds to one unavoidable jump.
        int jumps = 0;

        // currentLevelEnd: the last index reachable within the current
        // jump level. as long as i <= currentLevelEnd, no new jump is
        // needed. the moment i reaches this boundary, the level is
        // exhausted and a new jump must be taken.
        int currentLevelEnd = 0;

        // farthest: the farthest index reachable from any index seen so
        // far within the current level. continuously updated as i sweeps
        // through the level. when a jump is taken, this becomes the new
        // currentLevelEnd — the boundary of the next level.
        int farthest = 0;

        // loop runs until n-2, not n-1. at the last index there is no
        // need to jump further. including n-1 would risk counting one
        // extra jump if the last index happens to be a level boundary.
        for (int i = 0; i < (int)nums.size() - 1; i++) {

            // update farthest with the best reach from index i.
            // i + nums[i] = farthest index reachable if you jump
            // maximally from index i.
            // we take max because a previous index may have had a better
            // reach than the current one.
            farthest = max(farthest, i + nums[i]);

            // i == currentLevelEnd means we have processed every index
            // in the current level. we cannot move forward without
            // taking a new jump. so we take one jump and set the next
            // level's boundary to farthest — the best any index in
            // this level could reach.
            // if this block were removed, jumps would never increment
            // and we'd always return 0.
            if (i == currentLevelEnd) {
                jumps++;
                currentLevelEnd = farthest;
            }
        }

        return jumps;
    }
};