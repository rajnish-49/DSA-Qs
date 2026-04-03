/*
 * ============================================================
 * PROBLEM: Event Manager
 * ============================================================
 *
 * WHAT IS GIVEN:
 *   A list of events, each having:
 *     - eventId   : a unique identifier for the event
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
 *        Remove and return the eventId of the highest priority
 *        active event.
 *        Tiebreak: if multiple events share the highest priority,
 *        return the one with the SMALLEST eventId.
 *        If no active events remain, return -1.
 *
 * WHAT IS AN "ACTIVE" EVENT:
 *   An event that has NOT yet been removed by pollHighest().
 *
 * ============================================================
 * BRUTE FORCE AND WHY IT FAILS:
 * ============================================================
 *
 * Brute force — keep events in an array:
 *   pollHighest()    → scan entire array for max priority → O(n)
 *   updatePriority() → find element and update → O(n)
 *   With 10^5 events and 10^5 calls → 10^10 operations → too slow.
 *
 * ============================================================
 * OBSERVATION: WE NEED A SORTED STRUCTURE
 * ============================================================
 *
 * Every operation revolves around ordering:
 *   - pollHighest() always wants the "best" element.
 *   - updatePriority() changes an element's position in that order.
 *
 * What if we kept all events in a structure that:
 *   1. Always knows which element is "best"      → O(1) lookup
 *   2. Can insert a new element in sorted order  → O(log n)
 *   3. Can DELETE ANY arbitrary element          → O(log n)
 *
 * std::set (balanced BST) does exactly all three.
 *
 * ============================================================
 * WHY SET OVER HEAP?
 * ============================================================
 *
 * A heap supports (1) and (2) but NOT (3) efficiently.
 * Heap cannot delete an arbitrary buried element in O(log n).
 * That forces us to use "lazy deletion" with heap — leaving
 * stale entries behind and filtering them out later.
 *
 * A set solves this cleanly:
 *   updatePriority → DIRECTLY erase old entry, insert new entry.
 *   No stale entries. No extra filtering. Set always reflects reality.
 *
 * ============================================================
 * HOW TO ENCODE "BEST" IN A SET:
 * ============================================================
 *
 * std::set sorts in ASCENDING order by default.
 * The SMALLEST element sits at the front (st.begin()).
 *
 * We want: highest priority first, tiebreak by smallest eventId.
 *
 * Store pairs as {-priority, eventId}:
 *
 *   Highest priority → most negative → numerically smallest → front ✓
 *   Tiebreak (same priority): smaller eventId → smaller number → front ✓
 *
 * Both rules map naturally to the front of a min-sorted set.
 *
 * Example:
 *   Events: id=5 priority=7, id=2 priority=7, id=9 priority=4
 *   Stored: {-7,2}, {-7,5}, {-4,9}
 *   Sorted: {-7,2} ← {-7,5} ← {-4,9}
 *   Front = {-7,2} → id=2 ✓ (highest priority, smallest id among ties)
 *
 * ============================================================
 * THE ROLE OF THE HASHMAP:
 * ============================================================
 *
 * When updatePriority(eventId, newPriority) is called,
 * we need to erase the OLD entry from the set.
 * The old entry looks like: {-oldPriority, eventId}
 * We have eventId, but we need oldPriority to reconstruct it.
 *
 * The hashmap mp stores: eventId → current priority
 * This lets us look up oldPriority in O(1) whenever needed.
 *
 * Secondary role: mp[id] existing means event is still active.
 * (Though with set approach, we don't need staleness checks —
 *  this role was more critical in the heap/lazy-deletion approach.)
 *
 * ============================================================
 */

class EventManager {
public:

    // A sorted set of {-priority, eventId} pairs.
    // std::set uses a balanced BST internally, keeping elements
    // in ascending order at all times.
    // Storing -priority means highest priority events naturally
    // sort to the front (most negative = smallest = first).
    // Among ties in priority, smaller eventId sorts to front naturally.
    // Result: st.begin() always points to the "best" active event.
    set<pair<int,int>> st;

    // Ground truth map: eventId → current true priority.
    // Critical for updatePriority: we need the OLD priority to
    // reconstruct the exact entry {-oldPriority, eventId} that
    // currently sits in the set, so we can erase it precisely.
    // Without this, we cannot find the old set entry to remove.
    unordered_map<int,int> mp;

    EventManager(vector<vector<int>>& events) {

        for (auto& e : events) {
            int id = e[0], pr = e[1];

            // Record the true priority in the map.
            // This will be needed later when updatePriority
            // asks "what was this event's old priority?"
            mp[id] = pr;

            // Insert into set as {-priority, eventId}.
            // Negating priority ensures highest priority floats
            // to the front of the ascending-sorted set.
            st.insert({-pr, id});
        }
    }

    void updatePriority(int eventId, int newPriority) {

        // Retrieve the current true priority from the map.
        // We need this to reconstruct the exact pair that is
        // currently stored in the set for this event.
        int oldPriority = mp[eventId];

        // Directly erase the old entry from the set.
        // This is the key advantage of set over heap:
        // we can remove ANY element in O(log n), not just the top.
        // The set finds {-oldPriority, eventId} via BST search and removes it.
        // After this line, no outdated entry remains — set is always clean.
        st.erase({-oldPriority, eventId});

        // Update the ground truth map to the new priority.
        // From this point, mp[eventId] reflects the new reality.
        mp[eventId] = newPriority;

        // Insert a fresh entry with the updated priority.
        // The set will place it in the correct sorted position automatically.
        st.insert({-newPriority, eventId});
    }

    int pollHighest() {

        // No active events remain.
        if (st.empty()) return -1;

        // st.begin() points to the smallest element in the set.
        // Since we stored {-priority, eventId}, the smallest element
        // corresponds to the highest priority event, with smallest
        // eventId as tiebreaker — exactly what pollHighest() demands.
        auto it = st.begin();
        auto [negPr, id] = *it;

        // Remove this event from the set — it is no longer active.
        // Direct erase by iterator is O(log n).
        // Unlike the heap approach, no loop or staleness check needed
        // because the set never contains outdated entries.
        st.erase(it);

        // Remove from the map as well — event is now inactive.
        // If updatePriority were ever called on a removed event
        // (which the problem says won't happen), this would catch it.
        mp.erase(id);

        return id;
    }
};