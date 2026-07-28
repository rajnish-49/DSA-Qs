/*
 * ============================================================
 * PROBLEM: Next Greater Element I (LC 496)
 * ============================================================
 *
 * WHAT THE PROBLEM IS ASKING:
 * ----------------------------
 * Given two arrays nums1 and nums2 (nums1 is a subset of nums2),
 * for each element in nums1, find its "Next Greater Element" (NGE)
 * as it appears in nums2.
 *
 * The NGE of an element x in nums2 is the first element to the RIGHT
 * of x in nums2 that is strictly greater than x.
 * If no such element exists, the answer is -1.
 *
 * IMPORTANT DEFINITIONS:
 * ----------------------
 * - Next Greater Element (NGE): First element strictly to the right
 *   that is larger. "First" means closest, not global maximum.
 * - nums1 is a subset of nums2: every value in nums1 exists somewhere
 *   in nums2. All values are unique across both arrays.
 *
 * CONSTRAINTS & IMPLICATIONS:
 * ---------------------------
 * - All integers are unique → no duplicate handling needed;
 *   a value maps to exactly one position in nums2.
 * - 1 <= nums1.length <= nums2.length <= 1000 → brute force O(n*m)
 *   is technically acceptable, but the follow-up demands O(n+m).
 * - 0 <= values <= 10^4 → values fit comfortably as map keys.
 *
 * EDGE CASES:
 * -----------
 * - The last element of nums2 always has NGE = -1 (nothing to its right).
 * - An element larger than everything to its right → NGE = -1.
 * - nums1 has one element → still works, just one lookup.
 *
 * ============================================================
 * THINKING FROM FIRST PRINCIPLES
 * ============================================================
 *
 * NAIVE APPROACH (O(n * m)):
 * For each element in nums1, locate it in nums2, then scan rightward
 * to find the first larger element. This is correct but slow.
 *
 * KEY OBSERVATION:
 * ----------------
 * We're computing NGE for every element in nums2, not just nums1.
 * Once we have a precomputed map of {value → its NGE in nums2},
 * answering queries for nums1 is just O(1) lookups.
 *
 * So the real sub-problem is:
 *   "Efficiently compute NGE for every element in nums2."
 *
 * HOW TO EFFICIENTLY COMPUTE NGE FOR ALL ELEMENTS:
 * -------------------------------------------------
 * Consider processing nums2 from RIGHT to LEFT.
 *
 * When we're at index i, we want the first element to its right
 * that is greater. If we maintain a stack of "candidates" — elements
 * that haven't yet found something that beats them — we can query
 * the stack for the NGE at each step.
 *
 * WHY A STACK (MONOTONIC STACK)?
 * --------------------------------
 * The stack stores elements in strictly increasing order from bottom
 * to top (a decreasing-from-bottom-to-top stack, or "monotonic
 * decreasing" when viewed top-to-bottom).
 *
 * Here's the intuition:
 * - We process right to left.
 * - For element curr at index i, we want the nearest greater element
 *   to its right, which is already on the stack.
 * - Any element on the stack that is <= curr is USELESS for curr
 *   and for any future element to the left of curr, because curr
 *   itself would be that element's NGE or curr is bigger.
 *   So we pop them.
 * - After popping, the stack top (if it exists) is the first element
 *   to the right of curr that is strictly greater → that IS the NGE.
 * - We then push curr onto the stack so future left-side elements
 *   can use it as a candidate.
 *
 * INVARIANT MAINTAINED:
 * ----------------------
 * After processing index i, the stack contains elements from
 * nums2[i..n-1] in strictly increasing order from top to bottom.
 * These are the "surviving candidates" — elements not yet dominated
 * by something to their left.
 *
 * WHY CORRECTNESS HOLDS:
 * ----------------------
 * When we pop st.top() because st.top() <= curr, we're saying:
 * curr sits to the LEFT of st.top() in nums2. So for any future
 * element (further left), curr is closer and at least as large as
 * st.top(). Hence st.top() can NEVER be the NGE for any element
 * to curr's left → safe to discard from future consideration.
 *
 * WHY OPTIMAL O(n + m) COMPLEXITY:
 * ----------------------------------
 * Each element of nums2 is pushed onto the stack exactly once and
 * popped at most once → total stack work is O(n).
 * Then we answer m queries from nums1 in O(m) via hashmap lookups.
 * Total: O(n + m).
 *
 * ============================================================
 */

class Solution {
public:
    vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {

        // nge: maps each value in nums2 → its Next Greater Element.
        // We precompute this for ALL of nums2 so that answering
        // each nums1 query is just a O(1) hashmap lookup.
        unordered_map<int, int> nge;

        // Monotonic stack: stores values (not indices) from nums2.
        // Invariant: stack is strictly increasing from top to bottom,
        // i.e., decreasing top-to-bottom. This means the top is always
        // the "nearest greater" candidate for the current element.
        stack<int> st;

        // Process nums2 from RIGHT to LEFT.
        // Why right to left? Because the NGE of curr lies to its right,
        // and we want those elements already loaded in the stack when
        // we reach curr.
        for (int i = nums2.size() - 1; i >= 0; i--) {
            int curr = nums2[i];

            // Pop all elements from the stack that are <= curr.
            // WHY: These elements are to the right of curr in nums2.
            // If they are <= curr, then curr itself blocks them from
            // being the "first greater" for anything to curr's left.
            // They are permanently useless as NGE candidates for any
            // element left of curr, so we discard them.
            // If we did NOT pop: we'd incorrectly report a smaller
            // element as the NGE instead of skipping to the true one.
            while (!st.empty() && st.top() <= curr) {
                st.pop();
            }

            // After popping, check what's left on the stack.
            // The top of the stack (if any) is:
            //   - To the RIGHT of curr in nums2 (processed before curr)
            //   - Strictly GREATER than curr (survived the popping)
            //   - The NEAREST such element (stack's top-to-bottom order
            //     preserves the right-to-left discovery order)
            // → This is exactly the definition of NGE.
            if (st.empty()) {
                // No element to the right is greater → NGE is -1.
                nge[curr] = -1;
            } else {
                // Stack top is the first greater element to the right.
                nge[curr] = st.top();
            }

            // Push curr onto the stack so elements to its left can use
            // it as a candidate for their own NGE.
            // curr is now a valid right-side candidate for all future
            // (leftward) iterations.
            st.push(curr);
        }

        // Build the answer for nums1 using the precomputed NGE map.
        // Since nums1 is a subset of nums2, every nums1[i] is guaranteed
        // to be a key in nge. This loop is O(nums1.length).
        vector<int> ans;
        for (int x : nums1) {
            ans.push_back(nge[x]);
        }

        return ans;
    }
};

/*
 * ============================================================
 * DRY RUN
 * ============================================================
 *
 * nums1 = [4, 1, 2]
 * nums2 = [1, 3, 4, 2]
 *
 * We process nums2 right to left:
 *
 * i=3, curr=2:
 *   Stack: []  → empty, nothing to pop.
 *   Stack empty → nge[2] = -1
 *   Push 2.  Stack (top→bottom): [2]
 *
 * i=2, curr=4:
 *   Stack: [2] → 2 <= 4, pop 2. Stack: []
 *   Stack empty → nge[4] = -1
 *   Push 4.  Stack (top→bottom): [4]
 *
 * i=1, curr=3:
 *   Stack: [4] → 4 > 3, do NOT pop.
 *   Stack not empty → nge[3] = 4
 *   Push 3.  Stack (top→bottom): [3, 4]
 *
 * i=0, curr=1:
 *   Stack: [3, 4] → 3 > 1, do NOT pop.
 *   Stack not empty → nge[1] = 3
 *   Push 1.  Stack (top→bottom): [1, 3, 4]
 *
 * nge map: { 2→-1, 4→-1, 3→4, 1→3 }
 *
 * Now answer nums1 = [4, 1, 2]:
 *   nge[4] = -1
 *   nge[1] = 3
 *   nge[2] = -1
 *
 * Output: [-1, 3, -1]  ✓
 *
 * ============================================================
 * COMPLEXITY ANALYSIS
 * ============================================================
 *
 * Let n = nums2.length, m = nums1.length.
 *
 * TIME COMPLEXITY: O(n + m)
 * -------------------------
 * - The right-to-left loop runs n times.
 * - Each element of nums2 is pushed exactly once and popped at most once.
 *   Total push + pop operations across all iterations = O(n).
 *   (Amortized: even though the inner while loop can run multiple times
 *    per iteration, the TOTAL pops across all iterations ≤ n, because
 *    you can't pop more than you push.)
 * - Building the answer: m iterations, each O(1) hashmap lookup → O(m).
 * - Total: O(n + m).
 *
 * SPACE COMPLEXITY: O(n)
 * ----------------------
 * - The stack holds at most n elements at any point in time → O(n).
 * - The hashmap stores exactly n entries (one per element of nums2) → O(n).
 * - The output array is O(m), but that's the required output space.
 * - Total auxiliary space: O(n).
 *
 * IS THIS OPTIMAL?
 * ----------------
 * Yes. We must read every element of nums2 at least once to compute NGEs,
 * which is Ω(n). We must read every element of nums1 to produce the output,
 * which is Ω(m). So Ω(n + m) is a lower bound, and our algorithm matches it.
 *
 * ============================================================
 */