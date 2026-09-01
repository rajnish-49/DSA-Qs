/*
 * Problem: Merge K Sorted Lists
 * Link: https://leetcode.com/problems/merge-k-sorted-lists/
 *
 * Given an array of k linked lists, each sorted in ascending order,
 * merge all of them into one sorted linked list and return it.
 *
 * Example:
 *   Input:  lists = [[1,4,5], [1,3,4], [2,6]]
 *   Output: [1,1,2,3,4,4,5,6]
 *
 * ─────────────────────────────────────────────────────────────────────────────
 * APPROACH: Min-heap of k frontier nodes
 *
 * Core insight:
 *   The globally smallest element at any point must be one of the k head
 *   nodes — one from each list. You don't need to look further into any
 *   list until its current head is consumed. So instead of merging lists
 *   pairwise or sorting everything, maintain a min-heap of exactly k nodes
 *   — the current frontier of each list — and repeatedly extract the minimum.
 *
 * Process:
 *   1. Push the head of every non-null list into the min-heap
 *   2. Extract the minimum node → append to result list
 *   3. If that node has a next, push it — advance that list's frontier by one
 *   4. Repeat until heap is empty
 *
 * Why this maintains correctness:
 *   The heap always holds the current front of each non-exhausted list.
 *   Extracting the min and pushing its next preserves this invariant —
 *   at every step the heap contains the smallest remaining candidates
 *   across all k lists.
 *
 * ─────────────────────────────────────────────────────────────────────────────
 * Complexity:
 *   Time:  O(N log k) — N total nodes, each pushed and popped once;
 *                        each heap operation is O(log k) since heap holds at most k nodes
 *   Space: O(k) — heap holds at most one node per list at any time
 */

// Comparator for the min-heap — orders nodes by value so the smallest
// value sits at the top. operator() returns true when a should come
// AFTER b, i.e. a is "greater" → a->val > b->val makes it a min-heap.
class Compare {
public:
    bool operator()(ListNode* a, ListNode* b) {
        return a->val > b->val;
    }
};

class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {

        // Min-heap ordered by node value.
        // At any point holds exactly one frontier node per non-exhausted list.
        priority_queue<ListNode*, vector<ListNode*>, Compare> minHeap;

        // Step 1: Seed the heap with the head of each non-null list.
        // Null check is necessary — lists may contain empty lists.
        for (int i = 0; i < lists.size(); i++) {
            if (lists[i] != NULL) {
                minHeap.push(lists[i]);
            }
        }

        // Pointers to build the result list.
        // head is the fixed entry point to return; tail tracks where to append next.
        ListNode* head = nullptr, *tail = nullptr;

        while (!minHeap.empty()) {

            // Step 2: Extract the globally smallest node across all k frontiers.
            ListNode* smallest = minHeap.top();
            minHeap.pop();

            // Step 3: Append to result list.
            // First node initializes both head and tail.
            // Subsequently, tail is advanced one step forward.
            if (!head) {
                head = tail = smallest;
            } else {
                tail->next = smallest;
                tail = tail->next;
            }

            // Step 4: Advance this list's frontier by one.
            // If smallest has a next node, push it — that node is now this
            // list's new candidate for the globally smallest position.
            if (smallest->next) {
                minHeap.push(smallest->next);
            }
        }

        return head;
    }
};