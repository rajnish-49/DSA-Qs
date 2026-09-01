/*
 * Problem: Top K Frequent Words
 * Link: https://leetcode.com/problems/top-k-frequent-words/
 *
 * Given an array of strings words and an integer k, return the k most
 * frequent strings sorted by frequency (highest first). Words with the
 * same frequency must be sorted in lexicographical order.
 *
 * Example:
 *   Input:  words = ["i","love","leetcode","i","love","coding"], k = 2
 *   Output: ["i","love"]
 *
 *   Input:  words = ["the","day","is","sunny","the","the","the","sunny","is","is"], k = 4
 *   Output: ["the","is","sunny","day"]
 *
 * ─────────────────────────────────────────────────────────────────────────────
 * APPROACH: Same fixed-size min-heap pattern as Top K Frequent Elements,
 *           but with a tiebreaker — and the tiebreaker is what makes the
 *           comparator subtle.
 *
 * The structural challenge:
 *   The output has a two-level sort: frequency DESC, then lexicographic ASC.
 *   A plain min-heap evicts the "weakest" candidate at each step.
 *   We need to define "weakest" to match this two-level ordering:
 *     - Lower frequency  → weaker (evict first)
 *     - Equal frequency  → lexicographically LARGER → weaker (evict first)
 *       because we want to KEEP the lexicographically smaller one.
 *
 * How the comparator encodes "weakest at top":
 *   In a priority_queue, the element where cmp(top, X) = false for all X
 *   ends up at the top — i.e., the top is the element that loses every
 *   comparison, which is our definition of "weakest".
 *
 *   cmp(a, b) = true means "a is STRONGER than b" (a should stay, b evicted).
 *   So cmp returns true when:
 *     - a has higher frequency  (a.first > b.first)
 *     - equal frequency AND a is lexicographically smaller (a.second < b.second)
 *
 *   The "weaker" element naturally rises to the top and gets popped when
 *   the heap exceeds size k.
 *
 * Why reverse() at the end?
 *   When we drain the heap, top() gives the weakest surviving element first
 *   (lowest freq, or largest lex among ties). But the answer wants strongest
 *   first. A single reverse() fixes the order instead of using a separate
 *   sort — O(k) instead of O(k log k).
 *
 * Complexity:
 *   Time:  O(N log k) — N insertions, each heap op is O(log k);
 *                        string comparisons inside the heap add an O(L) factor
 *                        where L = average word length → O(NL log k) precise
 *   Space: O(N + k)   — freq map + heap
 */

class Solution {
public:
    vector<string> topKFrequent(vector<string>& words, int k) {

        // Step 1: Frequency count — same as the integers variant.
        unordered_map<string, int> freq;
        for (string word : words) {
            freq[word]++;
        }

        // Step 2: Custom comparator — defines which element is "weakest"
        // and therefore should be evicted (sit at the top of the min-heap).
        //
        // cmp(a, b) = true  →  a is STRONGER, b is weaker
        // The heap puts the element that "loses" all comparisons at the top.
        //
        // Stronger means:
        //   higher frequency, OR
        //   same frequency but lexicographically smaller (we want to KEEP smaller)
        auto cmp = [](const pair<int, string>& a,
                      const pair<int, string>& b) {
            if (a.first == b.first)
                return a.second < b.second;   // smaller lex = stronger = stays

            return a.first > b.first;         // higher freq = stronger = stays
        };

        // Step 3: Min-heap under the custom "weakest" ordering.
        // Unlike the integer variant where greater<> handled it,
        // here we need decltype(cmp) to pass the lambda type to the template.
        priority_queue
            pair<int, string>,
            vector<pair<int, string>>,
            decltype(cmp)
        > minHeap(cmp);   // lambda must also be passed to the constructor

        for (auto& [word, count] : freq) {
            minHeap.push({count, word});

            // Step 4: Evict the weakest when we exceed k.
            // "Weakest" here = lower frequency, or larger lex among equal freq.
            // That's exactly what rises to the top under our comparator.
            if (minHeap.size() > k) {
                minHeap.pop();
            }
        }

        // Step 5: Drain the heap — comes out weakest-first (lowest freq /
        // largest lex first). We want strongest-first in the answer,
        // so a single reverse() corrects the order in O(k).
        vector<string> ans;
        while (!minHeap.empty()) {
            ans.push_back(minHeap.top().second);
            minHeap.pop();
        }

        reverse(ans.begin(), ans.end());
        return ans;
    }
};