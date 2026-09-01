/*
 * Problem: Top K Frequent Elements
 * Link: https://leetcode.com/problems/top-k-frequent-elements/
 *
 * Given an integer array nums and an integer k, return the k most frequent elements.
 * You may return the answer in any order.
 *
 * Example:
 *   Input:  nums = [1,1,1,2,2,3], k = 2
 *   Output: [1,2]
 *
 * Constraints:
 *   - 1 <= nums.length <= 10^5
 *   - -10^4 <= nums[i] <= 10^4
 *   - k is in the range [1, number of unique elements in nums]
 *   - The answer is guaranteed to be unique.
 *
 * ─────────────────────────────────────────────────────────────────────────────
 * APPROACH: Fixed-size min-heap of size k
 *
 * Core idea:
 *   We want to keep only the TOP k elements by frequency.
 *   The natural enemy of "top k" is a MIN-heap of size k —
 *   because the min-heap always surfaces the WEAKEST element in our
 *   current candidate set. The moment a new element is stronger than
 *   the weakest, we evict the weakest and let the new one in.
 *
 * Why min-heap and not max-heap?
 *   A max-heap would give us the global maximum on every pop, but to
 *   collect k elements we'd have to pop k times from a heap of ALL
 *   unique elements — O(N log N) overall.
 *   With a min-heap capped at size k, every insertion is O(log k),
 *   and we never store more than k candidates at once.
 *
 * Invariant maintained at every step:
 *   The heap always holds the k highest-frequency elements seen so far.
 *   When a (k+1)-th element arrives, it competes with the current minimum.
 *   If it wins (higher frequency), the minimum is evicted. Otherwise it's
 *   discarded immediately. Either way, the heap stays at size k.
 *
 * ─────────────────────────────────────────────────────────────────────────────
 * DRY RUN: nums = [1,1,1,2,2,3], k = 2
 *
 *   After frequency count:  {1:3, 2:2, 3:1}
 *
 *   Heap state after each insertion (shown as min-heap of {freq, num}):
 *     Push {3,1}: heap = [{3,1}]                  size=1 ≤ k, no eviction
 *     Push {2,2}: heap = [{2,2},{3,1}]             size=2 ≤ k, no eviction
 *     Push {1,3}: heap = [{1,3},{3,1},{2,2}]       size=3 > k → pop min
 *                  min = {1,3} → evict 3 (least frequent)
 *                  heap = [{2,2},{3,1}]
 *
 *   Final heap: [{2,2},{3,1}]  →  answer: [2, 1]
 *
 * ─────────────────────────────────────────────────────────────────────────────
 * Complexity:
 *   Time:  O(N log k) — N insertions, each heap op is O(log k)
 *   Space: O(N + k)   — freq map holds up to N unique elements, heap holds k
 */

class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        // Step 1: Count frequencies — standard prerequisite before any
        // "top k by frequency" problem. This collapses the raw array
        // into a compact {element → count} view.
        unordered_map<int, int> freq;
        for (int num : nums) {
            freq[num]++;
        }

        // Step 2: Min-heap keyed on frequency.
        // Pair is {count, num} so the heap's comparator acts on count first —
        // the element with the LOWEST frequency sits at the top, ready to be
        // evicted the moment a more frequent element shows up.
        priority_queue
            pair<int, int>,
            vector<pair<int, int>>,
            greater<pair<int, int>>   // min-heap: smallest count at top
        > minHeap;

        for (auto& [num, count] : freq) {
            minHeap.push({count, num});

            // Step 3: Enforce the size-k window.
            // Exceeding k means we have one extra candidate — and the one
            // least deserving to be in the top-k is sitting right at the top
            // of our min-heap. Pop it immediately.
            if (minHeap.size() > k) {
                minHeap.pop();   // evict the currently least-frequent element
            }
        }

        // Step 4: Drain the heap into the answer.
        // Whatever survived the eviction tournament IS the top-k set.
        // Order doesn't matter per the problem statement.
        vector<int> ans;
        while (!minHeap.empty()) {
            ans.push_back(minHeap.top().second);
            minHeap.pop();
        }
        return ans;
    }
};