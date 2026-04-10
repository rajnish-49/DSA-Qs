/*
 * ============================================================
 * PROBLEM: Minimum Distance Between Three Equal Elements II
 * ============================================================
 *
 * WHAT THE PROBLEM ASKS:
 * Given an integer array nums, find a "good tuple" (i, j, k)
 * where i < j < k and nums[i] == nums[j] == nums[k].
 *
 * The distance of such a tuple is defined as:
 *   abs(i-j) + abs(j-k) + abs(k-i)
 *
 * Since i < j < k, this simplifies to:
 *   (j-i) + (k-j) + (k-i) = 2*(k-i)
 *
 * So the distance is ALWAYS 2 * (span from first to last index).
 * Return the minimum such distance across all good tuples, or -1.
 *
 * ============================================================
 * KEY OBSERVATION (why brute force is bad here):
 * ============================================================
 * Brute force: for every value, collect all its indices, then
 * check all C(m, 3) triples. With n=1e5 and a value appearing
 * n times, that's O(n^3) in the worst case — completely unacceptable.
 *
 * But we just proved: distance = 2*(k - i), which only depends
 * on the OUTERMOST indices of the triple. So for any 3 consecutive
 * occurrences [a, b, c] in sorted index order, their distance is
 * 2*(c - a). Skipping any occurrence (taking non-consecutive ones)
 * only widens the span, making the distance worse.
 *
 * CONCLUSION: The minimum distance always comes from 3 CONSECUTIVE
 * occurrences of the same value. We never need to check non-adjacent
 * triples — they are provably suboptimal.
 *
 * ============================================================
 * OPTIMAL APPROACH: Single-pass with a rolling 2-index window
 * ============================================================
 * Instead of collecting all indices upfront, we maintain for each
 * value only its TWO most recently seen indices: (last, secondLast).
 *
 * When we encounter a value for the 3rd+ time at index i:
 *   - secondLast = the index 2 steps back
 *   - distance = 2 * (i - secondLast)   [consecutive triple span]
 *   - update the answer, then slide the window forward
 *
 * This is O(n) time, O(n) space — optimal.
 */

class Solution {
public:
    int minimumDistance(vector<int>& nums) {

        // Maps each value to (last_seen_index, second_last_seen_index).
        // We only need these two because the minimum distance triple
        // is always formed by 3 consecutive occurrences, and we evaluate
        // it the moment we see the 3rd one (current index i).
        unordered_map<int, pair<int,int>> freq;

        int ans = INT_MAX;

        for (int i = 0; i < nums.size(); i++) {
            int x = nums[i];

            // Start with sentinels. If x has been seen < 2 times,
            // we can't form a valid triple yet, so these stay -1.
            int last = -1, secondLast = -1;

            if (freq.count(x)) {
                last = freq[x].first;        // most recent index of x before i
                secondLast = freq[x].second; // second most recent index of x before i
            }

            // We can only form a triple if x has appeared at least twice before.
            // secondLast != -1 guarantees we have indices at positions
            // [secondLast, last, i] — exactly 3 consecutive occurrences.
            if (secondLast != -1) {
                // distance = 2*(k - i) where k=i (current), i=secondLast (oldest).
                // 'last' is the middle index — it doesn't affect the distance
                // formula at all since distance = 2*(rightmost - leftmost).
                int dist = 2 * (i - secondLast);
                ans = min(ans, dist);
            }

            // Slide the window forward: current index i becomes the new 'last',
            // and the old 'last' becomes the new 'secondLast'.
            // We deliberately DROP anything older than secondLast because
            // those can only produce wider (worse) spans.
            freq[x] = {i, last};
        }

        return (ans == INT_MAX) ? -1 : ans;
    }
};