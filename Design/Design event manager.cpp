/*
 * ============================================================
 * PROBLEM: Event Manager
 * ============================================================
 *
 * WHAT IS GIVEN:
 *   A list of events, each having:
 *     - eventId   : unique identifier for the event
 *     - priority  : how important the event is (higher = more important)
 *
 * WHAT OPERATIONS MUST WE SUPPORT:
 *   1. EventManager(events)
 *        Initialize the manager with the given list of events.
 *
 *   2. updatePriority(eventId, newPriority)
 *        Change the priority of an existing active event.
 *
 *   3. pollHighest()
 *        Remove and return the eventId of the event with the
 *        highest priority among all currently active events.
 *        Tiebreak rule: if multiple events share the same highest
 *        priority, return the one with the SMALLEST eventId.
 *        If no active events remain, return -1.
 *
 * WHAT IS AN "ACTIVE" EVENT:
 *   An event that has NOT yet been removed by pollHighest().
 *
 * ============================================================
 * APPROACH: WHY NOT BRUTE FORCE?
 * ============================================================
 *
 * Brute force for pollHighest():
 *   Scan the entire list of events, find max priority, handle ties.
 *   → O(n) per poll call. With 10^5 calls on 10^5 events = too slow.
 *
 * What we really need:
 *   A data structure that always gives us the "best" event quickly.
 *   "Best" = highest priority, tiebreak by smallest id.
 *   → This is exactly what a MAX-HEAP (priority_queue) does.
 *
 * ============================================================
 * THE REAL PROBLEM: HEAP DOESN'T SUPPORT UPDATES
 * ============================================================
 *
 * A heap gives O(log n) insert and O(log n) remove-top.
 * But updatePriority() needs to change a value BURIED inside the heap.
 * There is no efficient way to find and update an arbitrary element
 * in a heap → it would require O(n) search.
 *
 * ============================================================
 * KEY INSIGHT: LAZY DELETION
 * ============================================================
 *
 * Instead of updating an entry inside the heap (hard),
 * we just PUSH A NEW ENTRY with the updated priority (easy).
 * The old entry becomes STALE (outdated) but stays in the heap.
 *
 * When we pop from the heap, we CHECK if the entry is still valid:
 *   - Does this eventId still exist as an active event?
 *   - Does the priority in this heap entry match the TRUE priority?
 * If both yes → valid, process it.
 * If either no → stale, discard and check the next entry.
 *
 * GROUND TRUTH is maintained in a separate hashmap:
 *   mp[eventId] = current true priority of that event
 * An event being present in mp means it is still active.
 * An event absent from mp means it was already polled/removed.
 *
 * ============================================================
 * HOW WE STORE PAIRS IN THE HEAP: {priority, -eventId}
 * ============================================================
 *
 * C++ max-heap on pairs compares first element first, then second.
 * We want: highest priority first → store priority as first element ✓
 * Tiebreak: smallest eventId first → but max-heap gives LARGEST second element.
 * Fix: store -eventId instead of eventId.
 *   Smaller eventId → more negative → larger as a negative number → wins ✓
 *
 * Example: id=5 and id=2, both priority=7
 *   Store {7,-5} and {7,-2}
 *   Max-heap picks {7,-2} (since -2 > -5) → id = -(-2) = 2 ✓
 *
 * ============================================================
 */

class EventManager {
public:

    // Max-heap storing {priority, -eventId}.
    // Gives us the "best" event at the top in O(1).
    // May contain stale entries which are filtered during pollHighest().
    priority_queue<pair<int, int>> pq;

    // Ground truth map: eventId → current true priority.
    // Two roles:
    //   1. Tells us the REAL priority of any active event.
    //   2. Tells us WHETHER an event is still active (present = active).
    // This is what we check against heap entries to detect staleness.
    unordered_map<int, int> mp;

    EventManager(vector<vector<int>>& events) {

        for (auto& i : events) {
            int id = i[0], pr = i[1];

            // Record this event's true priority in the ground truth map.
            // mp now knows this event exists and what its priority is.
            mp[id] = pr;

            // Push into heap as {priority, -eventId} so that:
            //   - higher priority naturally floats to top (max-heap)
            //   - among ties, smaller eventId wins (via negation trick)
            pq.push({pr, -id});
        }
    }

    void updatePriority(int eventId, int newPriority) {

        // Update the ground truth map immediately.
        // From this point on, mp[eventId] = newPriority is the real priority.
        // The old heap entry for this eventId (if any) now has a
        // priority that no longer matches mp → it becomes stale.
        mp[eventId] = newPriority;

        // Push a fresh heap entry with the new priority.
        // We do NOT try to find and modify the old entry inside the heap
        // (that would be O(n)). Instead we just add a new valid entry
        // and let the old one become stale. It will be discarded lazily
        // when it surfaces to the top during pollHighest().
        pq.push({newPriority, -eventId});
    }

    int pollHighest() {

        // Keep popping until we find a valid (non-stale) entry or exhaust the heap.
        // We need a loop because the top entry might be stale and must be skipped.
        while (!pq.empty()) {
            auto [pr, negId] = pq.top();
            pq.pop();

            // Recover the actual eventId.
            // We stored -eventId in the heap, so negate again to get back eventId.
            int id = -negId;

            // STALENESS CHECK — two conditions must both be true for a valid entry:
            //
            // Condition 1: mp.count(id)
            //   Is this eventId still present in the ground truth map?
            //   If NOT present → this event was already returned by a previous
            //   pollHighest() call (we erase from mp when we return an event).
            //   → Entry is stale, skip it.
            //
            // Condition 2: mp[id] == pr
            //   Does the priority stored in this heap entry match the
            //   TRUE current priority in the map?
            //   If NOT equal → updatePriority() was called after this entry
            //   was pushed, making this entry outdated.
            //   → Entry is stale, skip it.
            //
            // Only if BOTH pass is this entry the real current state of the event.
            if (mp.count(id) && mp[id] == pr) {

                // Valid entry found. Remove this event from the ground truth map.
                // Erasing from mp serves two purposes:
                //   1. Marks the event as no longer active.
                //   2. Ensures any remaining stale heap entries for this
                //      same id will fail condition 1 and get discarded.
                mp.erase(id);

                return id;
            }

            // If we reach here, the entry was stale (failed one or both checks).
            // We simply discard it and continue the loop to check the next entry.
        }

        // Heap is empty (or all remaining entries were stale) → no active events.
        return -1;
    }
};

/*
 * Your EventManager object will be instantiated and called as such:
 * EventManager* obj = new EventManager(events);
 * obj->updatePriority(eventId, newPriority);
 * int param_2 = obj->pollHighest();
 */