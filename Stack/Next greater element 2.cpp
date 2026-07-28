/*
 * ============================================================
 * PROBLEM: Next Greater Element II (LC 503)
 * ============================================================
 *
 * WHAT THE PROBLEM IS ASKING:
 * ----------------------------
 * Given a CIRCULAR array nums, for every element find the first
 * element that is strictly greater than it, searching to the right
 * and wrapping around if needed.
 * If no greater element exists even after a full circle, return -1.
 *
 * CIRCULAR means: after nums[n-1], the next element is nums[0].
 * So every element gets to "see" the entire array (except itself)
 * as potential candidates for its NGE.
 *
 * DIFFERENCE FROM LC 496 (NGE I):
 * --------------------------------
 * In LC 496, the array was linear. Here, an element near the end
 * can wrap around and find its NGE near the beginning.
 * Example: [1,2,1] → last element 1 wraps around and sees 2.
 *
 * IMPORTANT DEFINITIONS:
 * ----------------------
 * - NGE in circular context: first strictly greater element when
 *   traversing rightward with wraparound, up to n-1 steps away.
 * - "First" means nearest in traversal order, not global maximum.
 *
 * CONSTRAINTS & IMPLICATIONS:
 * ---------------------------
 * - Values can be negative (down to -10^9) → we cannot use -1 as
 *   a sentinel value in the stack. We store actual values, so fine.
 * - n up to 10^4 → O(n) solution needed; O(n^2) brute force works
 *   too but is inelegant.
 * - Unlike LC 496, values are NOT necessarily unique, so multiple
 *   elements may have the same value (the logic still holds because
 *   we are comparing values, not identities).
 *
 * EDGE CASES:
 * -----------
 * - Strictly decreasing array like [4,3,2,1]: every element can see
 *   the array's maximum (4) by wrapping, except 4 itself → ans = [-1,4,4,4]... 
 *   wait: 4 is the max so ans[0]=-1, others find 4 by wrapping.
 * - All elements equal: no element has a strictly greater neighbor → all -1.
 * - Single element: no other element exists → -1.
 *
 * ============================================================
 * THINKING FROM FIRST PRINCIPLES
 * ============================================================
 *
 * START FROM THE LINEAR CASE:
 * ----------------------------
 * For a linear array, we process right-to-left with a monotonic
 * stack (as in LC 496). This gives NGE for each position in O(n).
 *
 * THE CIRCULAR PROBLEM:
 * ----------------------
 * Circularity means an element at index i can have its NGE at any
 * index j where j > i OR j < i (wrapping around).
 * In the worst case, we need to scan almost the full array.
 *
 * KEY INSIGHT — THE DOUBLING TRICK:
 * -----------------------------------
 * Imagine "unrolling" the circular array into a linear array of
 * length 2n by conceptually concatenating nums with itself:
 *
 *   [1, 2, 1,  |  1, 2, 1]
 *    original     duplicate
 *
 * Now the NGE problem on this 2n array is purely linear.
 * For index i in [0, n-1], its NGE in the circular sense is exactly
 * its NGE in this doubled array, because the second copy simulates
 * the wraparound.
 *
 * WHY 2n IS ENOUGH AND NOT 3n?
 * ------------------------------
 * An element at index i can wrap around at most once. The farthest
 * useful candidate for i is at index i-1 (one full circle minus self).
 * In the doubled array, index i can look as far as index i + n - 1,
 * which is within the 2n window. So 2n suffices.
 *
 * WE DON'T ACTUALLY BUILD THE 2n ARRAY:
 * ----------------------------------------
 * Allocating 2n space is wasteful. Instead, we iterate i from
 * 2n-1 down to 0 and use i % n to index into the original nums.
 * This simulates the doubled array with O(1) extra space (beyond stack).
 *
 * WHY RIGHT TO LEFT (2n-1 DOWN TO 0)?
 * -------------------------------------
 * Same reason as LC 496: when processing index i, we need its
 * right-side candidates already loaded in the stack. Processing
 * right-to-left ensures that.
 *
 * CRITICAL SUBTLETY — WHEN TO WRITE ANS:
 * -----------------------------------------
 * We only write to ans[idx] for indices in the FIRST copy (i in [0, n-1],
 * or equivalently, we overwrite ans[idx] every time we encounter it).
 * The second copy (i in [n, 2n-1]) is processed first (we go right to left),
 * so it "seeds" the stack with wraparound candidates. When we reach the
 * first copy (smaller i values), those seeds are already in the stack.
 *
 * Since ans is initialized to -1, and we only set ans[idx] = st.top()
 * when the stack is non-empty, indices with no NGE naturally remain -1.
 * When i is in [0, n-1], if we set ans[idx], it overwrites any earlier
 * write from the second copy pass — and that's correct because the
 * first-copy pass has more (and closer) right-side context visible.
 *
 * INVARIANT:
 * ----------
 * After processing virtual index i (going right to left), the stack
 * contains values from virtual positions [i, 2n-1] that are strictly
 * increasing from top to bottom, representing surviving NGE candidates.
 *
 * ============================================================
 */

class Solution {
public:
    vector<int> nextGreaterElements(vector<int>& nums) {
        int n = nums.size();

        // ans[i] holds the NGE for nums[i] in the circular sense.
        // Initialized to -1 because if we never find a greater element,
        // -1 is the correct answer by definition.
        vector<int> ans(n, -1);

        // Monotonic stack storing VALUES (not indices).
        // Invariant: strictly increasing from top to bottom.
        // Top is the smallest surviving candidate seen so far
        // (from the right), bottom is the largest.
        stack<int> st;

        // Simulate traversal of the doubled array [0, 2n-1] right to left.
        // We use i % n to map virtual indices back to real indices.
        //
        // WHY START AT 2n-1?
        // The rightmost virtual index is 2n-1 (maps to n-1 in original).
        // Starting here means the second copy is processed before the first,
        // seeding the stack with wraparound candidates for the first copy.
        for (int i = 2 * n - 1; i >= 0; i--) {
            int idx = i % n;  // real index into nums

            // Pop all stack elements <= nums[idx].
            // WHY: Any value <= nums[idx] cannot be the NGE of nums[idx]
            // OR of anything to the left of idx (in the virtual array),
            // because nums[idx] itself is a closer and at-least-as-large
            // candidate. They are permanently useless → discard them.
            while (!st.empty() && st.top() <= nums[idx]) {
                st.pop();
            }

            // If stack is non-empty, its top is the first value strictly
            // greater than nums[idx] in the rightward (circular) direction.
            // Write this as the NGE for position idx.
            //
            // NOTE: We write ans[idx] for BOTH passes (second copy i in [n,2n-1]
            // and first copy i in [0,n-1]). When i is in the first copy,
            // the stack already contains seeds from the second copy (the
            // wraparound candidates). So the first-copy write is the final,
            // correct answer. The second-copy write may be overwritten later,
            // but that's fine — the second copy exists only to populate the stack.
            if (!st.empty()) {
                ans[idx] = st.top();
            }

            // Push current value onto the stack as a candidate for elements
            // that will be processed later (to the left in the virtual array).
            st.push(nums[idx]);
        }

        return ans;
    }
};

/*
 * ============================================================
 * DRY RUN
 * ============================================================
 *
 * nums = [1, 2, 1],  n = 3
 * ans initialized to [-1, -1, -1]
 *
 * Virtual doubled array (conceptual): [1, 2, 1, 1, 2, 1]
 *                                       0  1  2  3  4  5  (virtual idx)
 *                                       0  1  2  0  1  2  (real idx = virtual % 3)
 *
 * Process i from 5 down to 0:
 *
 * i=5, idx=2, nums[2]=1:
 *   Stack: [] → nothing to pop.
 *   Stack empty → ans[2] stays -1.
 *   Push 1.  Stack (top→bottom): [1]
 *
 * i=4, idx=1, nums[1]=2:
 *   Stack: [1] → 1 <= 2, pop. Stack: []
 *   Stack empty → ans[1] stays -1.
 *   Push 2.  Stack (top→bottom): [2]
 *
 * i=3, idx=0, nums[0]=1:
 *   Stack: [2] → 2 > 1, don't pop.
 *   Stack non-empty → ans[0] = 2.
 *   Push 1.  Stack (top→bottom): [1, 2]
 *
 *   (Second copy done. Stack seeded. Now first copy:)
 *
 * i=2, idx=2, nums[2]=1:
 *   Stack: [1, 2] → 1 <= 1, pop. Stack: [2]
 *                   2 > 1, stop.
 *   Stack non-empty → ans[2] = 2.   ← overwrites -1 with 2 ✓
 *   Push 1.  Stack (top→bottom): [1, 2]
 *
 * i=1, idx=1, nums[1]=2:
 *   Stack: [1, 2] → 1 <= 2, pop. Stack: [2]
 *                   2 <= 2, pop. Stack: []
 *   Stack empty → ans[1] stays -1.  ✓ (2 has no NGE, correct)
 *   Push 2.  Stack (top→bottom): [2]
 *
 * i=0, idx=0, nums[0]=1:
 *   Stack: [2] → 2 > 1, don't pop.
 *   Stack non-empty → ans[0] = 2.   ← overwrites 2 with 2 (same, correct)
 *   Push 1.  Stack (top→bottom): [1, 2]
 *
 * Final ans = [2, -1, 2]  ✓
 *
 * ============================================================
 * COMPLEXITY ANALYSIS
 * ============================================================
 *
 * TIME COMPLEXITY: O(n)
 * ----------------------
 * The loop runs 2n iterations.
 * Across ALL 2n iterations, each value is pushed at most twice
 * (once in each copy) and popped at most twice.
 * Total push + pop operations = O(2n) = O(n).
 * Each iteration also does O(1) work outside the while loop.
 * Total: O(n).
 *
 * SPACE COMPLEXITY: O(n)
 * -----------------------
 * - The stack holds at most n elements at any time (it never needs
 *   to hold more than n distinct positions worth of candidates).
 * - The ans array is O(n) but is the required output.
 * - No extra array of size 2n is allocated; i % n gives us the
 *   doubled traversal for free.
 * - Auxiliary space: O(n) for the stack.
 *
 * IS THIS OPTIMAL?
 * ----------------
 * Yes. We must examine every element at least once → Ω(n) lower bound.
 * Our algorithm is O(n), so it is asymptotically optimal.
 * The doubling trick avoids actually building the 2n array, keeping
 * space tight at O(n).
 *
 * ============================================================
 */