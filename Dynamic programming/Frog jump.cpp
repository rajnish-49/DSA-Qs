// https://leetcode.com/problems/frog-jump/

/*
 * PROBLEM OVERVIEW:
 * A frog wants to cross a river by jumping on stones. The frog starts at stone 0
 * and must reach the last stone. The constraint is that if the frog's last jump
 * was k units, the next jump must be k-1, k, or k+1 units (no other distances allowed).
 * The first jump MUST be 1 unit from position 0.
 *
 * KEY INSIGHT:
 * This is a state-space DP problem where the state is defined by (stone_index, last_jump_size).
 * We need to track which jump sizes can reach each stone because the same stone can be
 * reached with different jump sizes, and each jump size opens up different possibilities
 * for the next jump.
 *
 * APPROACH:
 * - Use a set for each stone to store all possible jump sizes that can land on that stone
 * - Process stones in order (left to right) to ensure all ways to reach a stone are computed
 * - For each stone and each jump size that reached it, explore all valid next jumps (k-1, k, k+1)
 * - Use a hash map for O(1) position lookup since stone positions can be sparse
 *
 * TIME COMPLEXITY: O(n^2) in worst case where n = number of stones
 * SPACE COMPLEXITY: O(n^2) for storing jump sizes at each stone
 */

class Solution
{
public:
    bool canCross(vector<int> &stones)
    {
        int n = stones.size();

        // Create a reverse mapping: stone_position -> array_index
        // WHY: Stone positions can be sparse (e.g., [0, 1, 3, 5, 8]), but we need
        // to quickly check if a calculated landing position exists as a stone
        // HOW: Hash map provides O(1) lookup instead of O(n) linear search
        unordered_map<int, int> pos;
        for (int i = 0; i < n; i++)
        {
            pos[stones[i]] = i;
        }

        // dp[i] = set of all valid jump distances that can successfully land on stone i
        // WHY use SET: The same stone can be reached with different jump sizes, and we
        // need to track ALL of them because each creates different future possibilities.
        // A set prevents duplicates and provides O(1) insertion/lookup.
        // EXAMPLE: Stone at position 3 might be reached by jump of 2 (from pos 1) or
        // jump of 3 (from pos 0), and each enables different next jumps.
        vector<unordered_set<int>> dp(n);

        // CRITICAL CONSTRAINT: The frog's first jump from position 0 MUST be exactly 1 unit
        // WHY check this: If there's no stone at position 1, the problem is impossible from the start
        if (pos.count(1) == 0)
            return false;

        // Initialize: Mark that we reached stone at position 1 with a jump of size 1
        // This is our starting state for the DP propagation
        dp[pos[1]].insert(1);

        // EDGE CASE: If the second stone is already the destination, we're done
        if (pos[1] == n - 1)
            return true;

        // MAIN DP LOOP: Process each stone in left-to-right order
        // WHY this order works: By processing stones sequentially by index (not position),
        // we ensure that when we process stone i, all possible ways to reach stones 0...i-1
        // have already been computed and their jump sizes recorded in dp[].
        for (int i = 0; i < n; i++)
        {
            // For each jump size that successfully landed on current stone i
            // WHY iterate through all jumps: Each jump size at stone i represents a
            // different path taken to get here, and each enables a unique set of next moves
            for (int jump : dp[i])
            {
                // CORE RULE: From a k-unit jump, the next jump must be k-1, k, or k+1 units
                // WHY these three: Problem constraint simulates momentum/physics - you can't
                // dramatically change jump distance, only adjust by ±1
                for (int nextJump = jump - 1; nextJump <= jump + 1; nextJump++)
                {
                    // VALIDATION: Jump size must be positive (can't jump 0 or backwards)
                    if (nextJump <= 0)
                        continue;

                    // Calculate landing position if we take this jump from current stone
                    // HOW: current_stone_position + jump_distance = landing_position
                    int nextPos = stones[i] + nextJump;

                    // Check if a stone exists at the calculated landing position
                    // WHY: We can only land on stones that exist; can't land in water
                    if (pos.count(nextPos))
                    {
                        int nextIndex = pos[nextPos];

                        // STATE PROPAGATION: Record that stone nextIndex can be reached with
                        // a jump of size nextJump. This information will be used when we
                        // process nextIndex stone later to explore further jumps.
                        // WHY insert into set: Multiple paths might reach this stone with
                        // the same jump size - set automatically handles duplicates
                        dp[nextIndex].insert(nextJump);

                        // EARLY EXIT OPTIMIZATION: If we've reached the last stone,
                        // we've found a valid path - no need to continue
                        // WHY this works: We only care about reachability, not all paths
                        if (nextIndex == n - 1)
                            return true;
                    }
                }
            }
        }

        // If we've processed all stones and never reached the last one, it's impossible
        // This means no combination of valid jumps can create a path to the destination
        return false;
    }
};
