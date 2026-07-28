/*
 * ============================================================
 * PROBLEM: Sum of Subarray Minimums (LC 907)
 * ============================================================
 *
 * WHAT THE PROBLEM IS ASKING:
 * ----------------------------
 * Given array arr, consider every possible contiguous subarray.
 * Each subarray has a minimum element. Sum all those minimums.
 * Return the sum modulo 10^9 + 7.
 *
 * BRUTE FORCE PICTURE:
 * For arr = [3,1,2,4], enumerate all subarrays:
 *   [3]→3, [1]→1, [2]→2, [4]→4,
 *   [3,1]→1, [1,2]→1, [2,4]→2,
 *   [3,1,2]→1, [1,2,4]→1,
 *   [3,1,2,4]→1
 *   Sum = 17
 *
 * CONSTRAINTS & IMPLICATIONS:
 * ---------------------------
 * - n up to 3*10^4 → O(n^2) brute force (~9*10^8 ops) is too slow.
 *   We need O(n log n) or O(n).
 * - Values up to 3*10^4, so products arr[i]*left*right fit in long long.
 * - Modulo 10^9+7 needed because the sum can be astronomically large.
 * - Values are POSITIVE (1 <= arr[i]), which simplifies some reasoning
 *   but doesn't change the core algorithm.
 *
 * EDGE CASES:
 * -----------
 * - Duplicate values: need careful handling to avoid double-counting
 *   subarrays (handled by using strict inequality on one side).
 * - Single element array: that element is the only subarray's min.
 * - Strictly increasing array: each element is only the min of
 *   subarrays starting at itself.
 * - Strictly decreasing array: the last element is min of all subarrays
 *   ending at it.
 *
 * ============================================================
 * THINKING FROM FIRST PRINCIPLES
 * ============================================================
 *
 * THE CONTRIBUTION TECHNIQUE:
 * ----------------------------
 * Instead of asking "what is the min of each subarray?", flip the
 * question to: "how much does each element contribute to the total sum?"
 *
 * Element arr[i] contributes arr[i] to the sum for every subarray
 * where arr[i] IS the minimum. So:
 *
 *   total_sum = Σ arr[i] * (number of subarrays where arr[i] is the min)
 *
 * This transforms the problem: for each i, count how many subarrays
 * have arr[i] as their minimum.
 *
 * WHEN IS arr[i] THE MINIMUM OF A SUBARRAY?
 * -------------------------------------------
 * A subarray [l..r] has arr[i] as its minimum (where l <= i <= r) if
 * and only if arr[i] is smaller than every other element in [l..r].
 *
 * This means:
 *   - Extending LEFT from i: how far can we go before hitting something
 *     strictly smaller than arr[i]? Let that distance be L.
 *     i.e., arr[i] is the minimum of arr[i-L+1..i], arr[i-L+2..i], ..., arr[i..i]
 *     → L choices for the left endpoint.
 *
 *   - Extending RIGHT from i: how far can we go before hitting something
 *     smaller OR EQUAL to arr[i]? Let that distance be R.
 *     → R choices for the right endpoint.
 *
 * Number of subarrays where arr[i] is the min = L * R.
 *
 * WHY STRICT ON LEFT, NON-STRICT ON RIGHT (for duplicates)?
 * ----------------------------------------------------------
 * If arr = [2, 2] and we ask "how many subarrays have their min = 2":
 * The subarray [2,2] should be counted ONCE, not twice (once for each 2).
 *
 * Convention: for the LEFT boundary, we stop at elements STRICTLY LESS
 * (arr[j] < arr[i]), meaning arr[i] wins ties on the left.
 * For the RIGHT boundary, we stop at elements LESS OR EQUAL
 * (arr[j] <= arr[i]), meaning a duplicate to the right "beats" arr[i].
 *
 * This ensures every subarray is assigned to exactly one "responsible"
 * minimum element — the leftmost occurrence in case of ties.
 *
 * DEFINING left[i] AND right[i]:
 * --------------------------------
 * left[i]  = number of consecutive elements ending at i (inclusive)
 *             where arr[i] is the minimum.
 *           = distance from i to the nearest index j < i where arr[j] < arr[i],
 *             exclusive of j itself.
 *           = i - (index of Previous Smaller Element) if it exists,
 *             else i + 1 (all elements from 0..i are valid left endpoints).
 *
 * right[i] = number of consecutive elements starting at i (inclusive)
 *             where arr[i] is the minimum.
 *           = distance from i to the nearest index k > i where arr[k] <= arr[i],
 *             exclusive of k itself.
 *           = (index of Next Smaller or Equal Element) - i if it exists,
 *             else n - i (all elements from i..n-1 are valid right endpoints).
 *
 * Then: contribution of arr[i] = arr[i] * left[i] * right[i]
 *
 * COMPUTING left[] AND right[] EFFICIENTLY:
 * ------------------------------------------
 * Use a monotonic stack (Previous Smaller Element pattern for left[],
 * Next Smaller-or-Equal Element pattern for right[]).
 * Each element is pushed/popped at most once → O(n) total.
 *
 * ============================================================
 */

class Solution {
public:
    int sumSubarrayMins(vector<int>& arr) {
        int n = arr.size();
        const int MOD = 1e9 + 7;

        // left[i]: how many subarrays ending at i have arr[i] as minimum.
        // Equivalently: (i - index_of_previous_smaller + 1), or i+1 if none.
        // This counts left endpoint choices: from (prev_smaller+1) to i inclusive.
        vector<int> left(n), right(n);

        stack<int> st; // stores INDICES into arr

        // -------------------------------------------------------
        // PASS 1: Compute left[i] — Previous Strictly Smaller Element
        // -------------------------------------------------------
        // We traverse LEFT TO RIGHT.
        // Stack invariant: indices in increasing order bottom to top,
        // with arr values strictly increasing bottom to top.
        // (Top has the most recently seen smaller element.)
        //
        // We use STRICT inequality (arr[st.top()] >= arr[i] → pop)
        // meaning we pop elements that are >= arr[i]. So the element
        // that remains after popping is the first one STRICTLY LESS
        // than arr[i]. This gives arr[i] ownership of tie-breaking on the left.
        for (int i = 0; i < n; i++) {
            // Pop indices whose values are >= arr[i].
            // WHY >=: we want the previous STRICTLY smaller element.
            // If arr[st.top()] == arr[i], that duplicate does NOT block
            // arr[i] from being the responsible minimum (we let the
            // leftmost duplicate be responsible for ties).
            while (!st.empty() && arr[st.top()] >= arr[i])
                st.pop();

            // After popping, st.top() (if exists) is the index of the
            // nearest element strictly less than arr[i] to the left.
            // left[i] = number of valid left endpoints for subarrays
            //           ending at i where arr[i] is minimum.
            //
            // If stack empty: no smaller element to the left at all.
            //   All indices 0..i are valid left endpoints → left[i] = i+1.
            // If stack non-empty: left endpoints valid from st.top()+1 to i.
            //   Count = i - st.top().
            left[i] = st.empty() ? i + 1 : i - st.top();

            st.push(i); // push current index as a candidate for future elements
        }

        // Clear the stack for the second pass.
        while (!st.empty()) st.pop();

        // -------------------------------------------------------
        // PASS 2: Compute right[i] — Next Smaller or Equal Element
        // -------------------------------------------------------
        // We traverse RIGHT TO LEFT.
        // Stack invariant: indices increasing bottom to top,
        // arr values strictly increasing bottom to top.
        //
        // We use STRICT inequality for popping (arr[st.top()] > arr[i] → pop),
        // meaning we keep elements that are <= arr[i] on the stack.
        // So the element remaining after popping is the first one
        // SMALLER OR EQUAL to arr[i] to the right.
        // This is the NON-STRICT side: a duplicate to the right stops arr[i].
        // Ensures no subarray is double-counted between two equal elements.
        for (int i = n - 1; i >= 0; i--) {
            // Pop indices whose values are STRICTLY GREATER than arr[i].
            // WHY strictly greater (not >=): we want to stop at elements
            // that are <= arr[i]. If arr[st.top()] == arr[i], we stop —
            // that duplicate to the right will "own" subarrays beyond it.
            while (!st.empty() && arr[st.top()] > arr[i])
                st.pop();

            // st.top() (if exists) is the index of the nearest element
            // <= arr[i] to the right.
            //
            // If stack empty: no smaller-or-equal element to the right.
            //   All indices i..n-1 are valid right endpoints → right[i] = n-i.
            // If stack non-empty: valid right endpoints from i to st.top()-1.
            //   Count = st.top() - i.
            right[i] = st.empty() ? n - i : st.top() - i;

            st.push(i);
        }

        // -------------------------------------------------------
        // ACCUMULATE CONTRIBUTIONS
        // -------------------------------------------------------
        // For each index i, arr[i] is the minimum of exactly
        // left[i] * right[i] subarrays.
        // Multiply and add, taking modulo to prevent overflow.
        //
        // WHY long long: arr[i] up to 3*10^4, left[i] and right[i]
        // up to n = 3*10^4 each. Product up to ~2.7*10^13, which
        // overflows int (max ~2.1*10^9) but fits in long long (max ~9.2*10^18).
        long long ans = 0;
        for (int i = 0; i < n; i++) {
            ans = (ans + (long long)arr[i] * left[i] * right[i]) % MOD;
        }

        return ans;
    }
};

/*
 * ============================================================
 * DRY RUN
 * ============================================================
 *
 * arr = [3, 1, 2, 4],  n = 4
 *
 * --- PASS 1: Compute left[] (Previous Strictly Smaller) ---
 *
 * i=0, arr[0]=3:
 *   Stack empty → left[0] = 0+1 = 1
 *   Push 0. Stack: [0]  (values: [3])
 *
 * i=1, arr[1]=1:
 *   arr[0]=3 >= 1 → pop. Stack: []
 *   Stack empty → left[1] = 1+1 = 2
 *   Push 1. Stack: [1]  (values: [1])
 *
 * i=2, arr[2]=2:
 *   arr[1]=1 >= 2? No (1 < 2) → stop.
 *   left[2] = 2 - 1 = 1
 *   Push 2. Stack: [1,2]  (values: [1,2])
 *
 * i=3, arr[3]=4:
 *   arr[2]=2 >= 4? No → stop.
 *   left[3] = 3 - 2 = 1
 *   Push 3. Stack: [1,2,3]  (values: [1,2,4])
 *
 * left = [1, 2, 1, 1]
 *
 * Interpretation:
 *   left[0]=1: arr[0]=3 is min of 1 subarray ending at 0 → [3]
 *   left[1]=2: arr[1]=1 is min of 2 subarrays ending at 1 → [1],[3,1]
 *   left[2]=1: arr[2]=2 is min of 1 subarray ending at 2 → [2]
 *   left[3]=1: arr[3]=4 is min of 1 subarray ending at 3 → [4]
 *
 * --- PASS 2: Compute right[] (Next Smaller or Equal) ---
 *
 * i=3, arr[3]=4:
 *   Stack empty → right[3] = 4-3 = 1
 *   Push 3. Stack: [3]  (values: [4])
 *
 * i=2, arr[2]=2:
 *   arr[3]=4 > 2 → pop. Stack: []
 *   Stack empty → right[2] = 4-2 = 2
 *   Push 2. Stack: [2]  (values: [2])
 *
 * i=1, arr[1]=1:
 *   arr[2]=2 > 1 → pop. Stack: []
 *   Stack empty → right[1] = 4-1 = 3
 *   Push 1. Stack: [1]  (values: [1])
 *
 * i=0, arr[0]=3:
 *   arr[1]=1 > 3? No (1 <= 3) → stop.
 *   right[0] = 1 - 0 = 1
 *   Push 0. Stack: [0,1]  (values: [3,1])
 *
 * right = [1, 3, 2, 1]
 *
 * Interpretation:
 *   right[0]=1: arr[0]=3 is min of subarrays starting at 0, extending 1 step → [3]
 *   right[1]=3: arr[1]=1 is min starting at 1, extending 3 steps → [1],[1,2],[1,2,4]
 *   right[2]=2: arr[2]=2 is min starting at 2, extending 2 steps → [2],[2,4]
 *   right[3]=1: arr[3]=4 is min starting at 3, extending 1 step → [4]
 *
 * --- CONTRIBUTION CALCULATION ---
 *
 *  i  | arr[i] | left | right | contribution
 *  0  |   3    |   1  |   1   |  3*1*1 =  3
 *  1  |   1    |   2  |   3   |  1*2*3 =  6
 *  2  |   2    |   1  |   2   |  2*1*2 =  4
 *  3  |   4    |   1  |   1   |  4*1*1 =  4
 *                                  Total = 17  ✓
 *
 * --- DUPLICATE EXAMPLE (why asymmetric inequalities matter) ---
 *
 * arr = [2, 2]
 * left:  i=0 → left[0]=1; i=1: arr[0]=2 >= 2 → pop, empty → left[1]=2
 * right: i=1 → right[1]=1; i=0: arr[1]=2 > 2? No → right[0]=1-0=1
 *
 * Contributions: arr[0]*1*1=2, arr[1]*2*1=4 → total=6
 * Manual check: [2]→2, [2]→2, [2,2]→2 → sum=6 ✓
 * Each subarray counted once. [2,2] assigned to left 2 (index 1), not both.
 *
 * ============================================================
 * COMPLEXITY ANALYSIS
 * ============================================================
 *
 * TIME COMPLEXITY: O(n)
 * ----------------------
 * Pass 1 (left[]): loop runs n times; each index pushed once, popped
 *   at most once. Total stack ops = O(n).
 * Pass 2 (right[]): same argument. Total stack ops = O(n).
 * Contribution loop: O(n).
 * Overall: O(n).
 *
 * SPACE COMPLEXITY: O(n)
 * -----------------------
 * left[] and right[] arrays: O(n) each.
 * Stack: at most n elements at any time → O(n).
 * ans is O(1).
 * Total auxiliary space: O(n).
 *
 * IS THIS OPTIMAL?
 * ----------------
 * We must read every element at least once → Ω(n) lower bound.
 * Our algorithm is O(n), matching the lower bound. Optimal.
 *
 * The contribution technique + monotonic stack is the canonical
 * O(n) approach for "sum of subarray minimums/maximums" problems.
 *
 * ============================================================
 */