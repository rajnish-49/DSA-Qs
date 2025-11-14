// @ 

class Solution {
public:
    string removeDuplicateLetters(string s) {
        // Step 1: Precompute the last occurrence of every character.
        // Why? Because this tells us how long we can "wait" before we MUST include a character.
        // If the last occurrence of 'b' is at index 6, then until i == 6, we can still safely
        // drop earlier 'b's knowing another chance will come later.
        vector<int> last(26, -1);
        for (int i = 0; i < s.size(); i++) {
            last[s[i] - 'a'] = i;
        }
        
        // Step 2: Track which characters are already in our "result stack".
        // Why? Because we only want each character ONCE. If 'c' is already sitting in the stack,
        // we skip any future 'c' until we pop it explicitly.
        vector<bool> inStack(26, false);

        // Step 3: The stack itself — but implemented as a string for simplicity.
        // We'll push characters as candidates, and sometimes pop them if we later
        // realize a smaller lexicographic order is possible without losing uniqueness.
        string st;

        // Step 4: Walk through the string left to right, making greedy decisions.
        for (int i = 0; i < s.size(); i++) {
            char c = s[i];

            // Case A: If this character is already included in the stack/result,
            // we skip it. Why? Because adding it again would break the "distinct characters" rule.
            if (inStack[c - 'a']) continue;

            // Case B: If not already included, we may try to push it.
            // BUT first, check whether the top of the stack should be removed.
            // Imagine the stack currently ends with 'd', and now we see 'b'.
            // If 'd' > 'b' lexicographically, the string could be smaller if 'd' moves behind 'b'.
            // BUT: we can only pop 'd' if it reappears later, otherwise we’d lose 'd' entirely.
            while (!st.empty() && st.back() > c && last[st.back() - 'a'] > i) {
                // The while-loop embodies the greedy choice:
                //  - "st.back() > c" → we found a lexicographically better option by swapping.
                //  - "last[...] > i" → safe to drop, because 'st.back()' will show up again later.
                inStack[st.back() - 'a'] = false; // Mark the popped char as no longer "in stack".
                st.pop_back();
            }

            // Case C: Now it's safe (and beneficial) to push the current character.
            st.push_back(c);
            inStack[c - 'a'] = true; // Mark that this character is now locked in the stack.
        }

        // By the end, the stack holds the smallest subsequence of distinct characters.
        return st;
    }
};
