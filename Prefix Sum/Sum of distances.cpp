/*
 * ============================================================
 * PROBLEM: Sum of Distances (LeetCode 2615)
 * ============================================================
 *
 * PROBLEM STATEMENT (simple terms):
 * Given an array `nums`, for each index i, compute the sum of
 * absolute differences |i - j| for every j where nums[j] == nums[i].
 * In other words, for each element, find all other positions
 * carrying the same value and sum their distances to i.
 *
 * INPUT : nums = [1, 3, 1, 1, 2]
 * OUTPUT: arr  = [5, 0, 3, 4, 0]
 *
 * ============================================================
 * INTUITION & APPROACH
 * ============================================================
 *
 * BRUTE FORCE (why it fails):
 *   For every i, scan all j and accumulate |i-j| when nums[j]==nums[i].
 *   That is O(n²) — too slow for n = 10^5.
 *
 * KEY OBSERVATION — removing the absolute value:
 *   Suppose index i has k same-valued neighbours at positions
 *   p[0], p[1], ..., p[k-1] (sorted). Split them into:
 *     LEFT  = those with index < i  → they contribute  (i - p[j]),  positive
 *     RIGHT = those with index > i  → they contribute  (p[j] - i),  positive
 *
 *   For the LEFT side (L neighbours before i):
 *     sum of (i - p[j]) = L*i - (p[0]+p[1]+...+p[L-1])
 *                       = L*i - prefixSum[left of i]
 *
 *   For the RIGHT side (R neighbours after i):
 *     sum of (p[j] - i) = (p[L]+...+p[k-1]) - R*i
 *                       = suffixSum[right of i] - R*i
 *
 *   So arr[i] = L*i - leftSum + rightSum - R*i   — all O(1) per index!
 *
 * PLAN:
 *   1. Group indices by value using a hash map.
 *   2. For each group, do ONE left-to-right pass maintaining a
 *      running prefix sum and left-count to fill arr[i] for the left part.
 *   3. Do ONE right-to-left pass for the right part, accumulating into arr[i].
 *
 * EXAMPLE TRACE — group for value 1, indices = [0, 2, 3]:
 *
 *   LEFT PASS (i goes 0 → 2 → 3):
 *     i=0: leftCnt=0, leftSum=0  → leftContrib = 0*0 - 0 = 0  → arr[0]=0
 *          then leftCnt=1, leftSum=0
 *     i=2: leftContrib = 1*2 - 0 = 2                          → arr[2]=2
 *          then leftCnt=2, leftSum=2
 *     i=3: leftContrib = 2*3 - 2 = 4                          → arr[3]=4
 *
 *   RIGHT PASS (i goes 3 → 2 → 0):
 *     i=3: rightCnt=0, rightSum=0 → rightContrib = 0 - 0*3 = 0 → arr[3]+=0 → 4
 *          then rightCnt=1, rightSum=3
 *     i=2: rightContrib = 3 - 1*2 = 1                          → arr[2]+=1 → 3
 *          then rightCnt=2, rightSum=5
 *     i=0: rightContrib = 5 - 2*0 = 5                          → arr[0]+=5 → 5
 *
 *   Final: arr[0]=5, arr[2]=3, arr[3]=4  ✓
 * ============================================================
 */

class Solution {
public:
    vector<long long> distance(vector<int>& nums) {
        int n = nums.size();
        vector<long long> arr(n, 0);

        // Group indices by their value.
        // Each key = a number in nums; value = sorted list of positions
        // carrying that number. Sorting is free because we iterate left→right.
        unordered_map<int, vector<int>> indexGroup;
        for (int i = 0; i < n; i++)
            indexGroup[nums[i]].push_back(i);

        for (auto& [val, indices] : indexGroup) {
            // Single-element groups contribute 0 everywhere — skip.
            if (indices.size() == 1) continue;

            // ── LEFT PASS ──────────────────────────────────────────
            // Walk left→right through this group's indices.
            // Before processing position indices[i], we know:
            //   leftCnt = how many group members are strictly to the left
            //   leftSum = sum of those members' positions
            // Contribution from the left = leftCnt*pos - leftSum
            // (each left neighbour p satisfies pos > p, so |pos-p| = pos-p)
            long long leftCnt = 0, leftSum = 0;
            for (int pos : indices) {
                arr[pos] += leftCnt * pos - leftSum;
                // Update running state so the NEXT position in the group
                // correctly counts the current position as a left neighbour.
                leftCnt++;
                leftSum += pos;
            }

            // ── RIGHT PASS ─────────────────────────────────────────
            // Mirror of the left pass, but right→left.
            // rightCnt / rightSum track neighbours strictly to the right.
            // Contribution from the right = rightSum - rightCnt*pos
            long long rightCnt = 0, rightSum = 0;
            for (int i = (int)indices.size() - 1; i >= 0; i--) {
                int pos = indices[i];
                arr[pos] += rightSum - rightCnt * pos;
                rightCnt++;
                rightSum += pos;
            }
        }

        return arr;
    }
};
/*
 * TIME : O(n)  — each index is visited twice (one pass per direction per group)
 * SPACE: O(n)  — the hash map stores every index exactly once
 */