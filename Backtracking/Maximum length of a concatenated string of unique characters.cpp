/*
 * ============================================================
 * PROBLEM: Maximum Length of Concatenated String with Unique Characters
 * ============================================================
 *
 * PROBLEM STATEMENT (in simple terms):
 * -------------------------------------
 * You are given an array of strings. You need to pick a subsequence
 * (any subset, in order) of these strings and concatenate them.
 * The concatenated result must have ALL UNIQUE characters — no character
 * should appear more than once across the entire concatenation.
 * Return the maximum possible length of such a concatenation.
 *
 * INPUT:
 *   - arr: a vector of strings, each containing only lowercase letters.
 *   - arr.length is at most 16, each string length is at most 26.
 *
 * OUTPUT:
 *   - A single integer: the length of the longest valid concatenation.
 *
 * EXAMPLE:
 *   arr = ["un", "iq", "ue"]
 *   "un" + "iq" = "uniq" → 4 unique chars ✓
 *   "un" + "ue" → 'u' repeats ✗
 *   Answer: 4
 *
 * ============================================================
 * APPROACH: WHY BACKTRACKING?
 * ============================================================
 *
 * OBSERVATION 1 — It's a subset problem:
 *   We need to find the best subset of arr to concatenate. With arr.length
 *   <= 16, there are at most 2^16 = 65536 subsets — small enough to explore
 *   all of them explicitly.
 *
 * OBSERVATION 2 — Not all subsets are valid:
 *   A subset is valid only if the combined characters are all unique.
 *   So we need to explore subsets while pruning invalid ones early.
 *
 * OBSERVATION 3 — Strings with internal duplicates are always useless:
 *   If a string like "aab" has a repeated character inside itself, it can
 *   NEVER be part of any valid concatenation regardless of what else we pick.
 *   Pre-filtering these out shrinks the search space before we even start.
 *
 * OBSERVATION 4 — Greedy doesn't work:
 *   Picking the longest valid string first doesn't guarantee the best answer
 *   because a shorter string might combine with more others. We must explore
 *   all possibilities.
 *
 * CHOSEN APPROACH — Backtracking (include/exclude decision tree):
 *   At each index, we make a binary decision:
 *     - EXCLUDE arr[idx]: move to next index, current string unchanged.
 *     - INCLUDE arr[idx]: append it to current (if no conflict), recurse,
 *       then undo the append (backtrack).
 *   This explores all 2^n subsets while pruning branches where a string
 *   conflicts with what we've already built.
 *
 * RECURSIVE THINKING:
 *   solve(idx, current) = "given that I've already built `current` using
 *   strings before idx, what's the best I can do with strings from idx onward?"
 *
 *   Base case: idx == arr.size() → no more strings to consider, update answer.
 *   Recursive case: try excluding arr[idx], then try including it if valid.
 *
 * TIME COMPLEXITY: O(2^n * L) where n = arr.size(), L = avg string length.
 * ============================================================
 */

class Solution {
public:
    int ans = 0;

    /*
     * isValid(s):
     * -----------
     * Before backtracking, we pre-filter strings that have duplicate
     * characters WITHIN THEMSELVES. Such strings can never contribute
     * to a valid concatenation — even alone they violate the uniqueness rule.
     *
     * HOW IT WORKS:
     * We use a freq array of size 26. For each character, we check if it
     * was already seen. The post-increment `freq[c-'a']++` is key here:
     *   - It reads the OLD value for the if-check (0 = not seen = false → continue)
     *   - Then increments it to 1 (marking as seen)
     * So if freq was already 1 when we read it, the if fires and we return false.
     *
     * This lets us combine the "check" and "mark" into a single expression.
     */
    bool isValid(string &s) {
        int freq[26] = {0};
        for (char c : s) {
            // If freq[c] is non-zero before incrementing, 'c' is a duplicate
            // Post-increment: reads old value first (for check), then increments
            if (freq[c - 'a']++) return false;
        }
        return true;
    }

    /*
     * canAdd(current, s):
     * --------------------
     * Before including string `s` into our current concatenation, we need
     * to verify that no character in `s` already exists in `current`.
     *
     * At this point, `s` is guaranteed to have no internal duplicates
     * (because we pre-filtered in maxLength). So we only need to check
     * conflict between `s` and `current`.
     *
     * WHY freq[c] = 1 instead of freq[c]++?
     * Because `current` itself is already guaranteed unique (our invariant —
     * we only ever append conflict-free strings). So we just need presence/absence,
     * not counts. Setting to 1 (marking presence) is sufficient and clearer.
     */
    bool canAdd(string &current, string &s) {
        int freq[26] = {0};

        // Mark all characters already present in the current concatenation
        for (char c : current) freq[c - 'a'] = 1;

        // Check if any character of s is already marked — if so, adding s
        // would create a duplicate, making the concatenation invalid
        for (char c : s) {
            if (freq[c - 'a']) return false;
        }
        return true;
    }

    /*
     * solve(idx, arr, current):
     * --------------------------
     * Core backtracking function. At each call, we are deciding what to do
     * with arr[idx] — include it or skip it.
     *
     * `idx`     — which string in filtered arr we are currently deciding on
     * `arr`     — the pre-filtered array (all strings here have unique chars internally)
     * `current` — the concatenation built so far from previous decisions
     *             (passed by reference to avoid copies; we undo changes manually)
     *
     * INVARIANT maintained throughout: `current` always has all unique characters.
     * This is guaranteed because we only append via canAdd() which enforces it.
     */
    void solve(int idx, vector<string>& arr, string &current) {

        /*
         * BASE CASE: We've considered every string in the array.
         * `current` holds a complete valid concatenation for this path.
         * Update the global answer with its length.
         * Then return — no more decisions to make.
         */
        if (idx == arr.size()) {
            ans = max(ans, (int)current.size());
            return;
        }

        /*
         * DECISION 1 — EXCLUDE arr[idx]:
         * Simply skip this string and move to the next index.
         * `current` is not modified — we carry forward what we've built so far.
         * This represents the branch where arr[idx] is NOT part of our subsequence.
         */
        solve(idx + 1, arr, current);

        /*
         * DECISION 2 — INCLUDE arr[idx] (only if it doesn't conflict):
         * canAdd checks that no character in arr[idx] already exists in `current`.
         * If safe, we append arr[idx] to current, recurse deeper, then UNDO
         * the append so that `current` is restored for the caller.
         *
         * This undo step is what makes it backtracking — we try a choice,
         * explore all futures from it, then retract it as if it never happened.
         */
        if (canAdd(current, arr[idx])) {
            // CHOOSE: extend current with arr[idx]
            current += arr[idx];

            // EXPLORE: recurse with arr[idx] now part of our concatenation
            solve(idx + 1, arr, current);

            // UNDO: remove arr[idx] from the end of current so the caller
            // sees current in its original state (backtracking step)
            current.erase(current.size() - arr[idx].size());
        }
    }

    /*
     * maxLength(arr):
     * ----------------
     * Entry point. Two things happen here before backtracking begins:
     *
     * 1. PRE-FILTERING:
     *    Any string with internal duplicate characters is removed upfront.
     *    This is a one-time O(n * L) pass that shrinks the search space and
     *    simplifies canAdd() — which no longer needs to worry about whether
     *    `s` itself is internally valid, only whether it conflicts with `current`.
     *
     * 2. BACKTRACKING LAUNCH:
     *    We start with an empty `current` and idx=0, exploring all valid
     *    subsequences of the filtered array.
     */
    int maxLength(vector<string>& arr) {
        vector<string> filtered;

        /*
         * Pre-filter: keep only strings that are internally valid.
         * A string like "aab" can NEVER appear in a valid concatenation,
         * so there is no point exploring any subset that includes it.
         * Removing such strings before backtracking avoids wasted branches.
         */
        for (auto &s : arr) {
            if (isValid(s)) {
                filtered.push_back(s);
            }
        }

        // Start backtracking with an empty concatenation from index 0
        string current = "";
        solve(0, filtered, current);

        return ans;
    }
};