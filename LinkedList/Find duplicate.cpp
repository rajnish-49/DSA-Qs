// @https://leetcode.com/problems/find-the-duplicate-number/description/

class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        /*
         We are solving "Find the Duplicate Number" without extra space.
         Key insight:
         - Treat nums as a "linked list":
             index -> nums[index] (like a pointer to the next node)
         - Because there are n+1 values in range [1..n], 
           some value repeats, creating a cycle in this "list".
         - The duplicate number = the entry point of the cycle.
         - Exactly the same problem as "Find start of cycle in linked list".
        */

        // --------------------------
        // Phase 1: Detect intersection point (inside the cycle)
        // --------------------------
        
        int slow = nums[0];         // "tortoise": moves 1 step each turn
        int fast = nums[0];         // "hare": moves 2 steps each turn

        do {
            slow = nums[slow];           // move 1 step
            fast = nums[nums[fast]];     // move 2 steps
            // Eventually they must meet because of the cycle (pigeonhole principle)
        } while (slow != fast);

        /*
         At this point:
         - slow and fast have met somewhere inside the cycle.
         - The meeting point is guaranteed, but it may not be the duplicate itself.
         - Now, we need to find the *entrance* of the cycle.
        */

        // --------------------------
        // Phase 2: Find the entry point of cycle
        // --------------------------
        
        slow = nums[0];  // reset slow to the "head" of the linked list (index 0)

        /*
         Key property:
         - If one pointer starts at head,
           and another starts at the meeting point inside the cycle,
           and both move 1 step at a time,
           they will meet at the *entrance* of the cycle.
         - Why? Because:
             Distance(head -> cycleStart) = Distance(meetingPoint -> cycleStart)
        */

        while (slow != fast) {
            slow = nums[slow];   // move one step
            fast = nums[fast];   // move one step
        }

        /*
         Now slow == fast:
         - They are pointing to the start of the cycle.
         - In our problem, that "start of cycle" = duplicate number.
        */
        return slow;
    }
};
