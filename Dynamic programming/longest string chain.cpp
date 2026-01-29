// @https://leetcode.com/problems/longest-string-chain/

class Solution {
public:

    int n;
    vector<vector<int>> dp;

    /*
        ispred(shorter, longer):

        This function captures the ENTIRE definition of "predecessor"
        from the problem statement.

        Key intuition:
        - longer must be exactly one character longer than shorter
        - shorter must appear as a subsequence of longer
        - only ONE extra character is allowed in longer

        Conceptually:
        Instead of "inserting one character into shorter",
        we reverse the thinking:
        "Can we delete exactly one character from longer to get shorter?"
    */
    bool ispred(const string& shorter, const string& longer) {
        if (longer.length() != shorter.length() + 1)
            return false;

        int i = 0, j = 0;
        bool skipped = false;

        while (i < shorter.length() && j < longer.length()) {
            if (shorter[i] == longer[j]) {
                i++;
                j++;
            } else {
                if (skipped)
                    return false;
                skipped = true;
                j++;
            }
        }

        return true;
    }

    /*
        solve(pr, curr):

        pr   → index of the previously chosen word
               pr == -1 means no word has been chosen yet
        curr → index of the current word we are deciding on

        Returns:
        The maximum chain length that can be formed
        considering words from index curr onward,
        given that the last chosen word is pr.

        This is a classic TAKE / SKIP DP state.
    */
    int solve(vector<string>& words, int pr, int curr) {

        /*
            Base case:
            Once we have considered all words,
            no further extension is possible.
        */
        if (curr >= n) {
            return 0;
        }

        /*
            Memoization check:

            The state (pr, curr) fully captures the constraint:
            - where we are
            - what the previous word was

            If this state has already been solved,
            reuse the stored answer to avoid recomputation.
        */
        if (pr != -1 && dp[pr][curr] != -1) {
            return dp[pr][curr];
        }

        int take = 0, skip = 0;

        /*
            TAKE decision:

            We are allowed to take words[curr] if:
            - no word has been chosen yet (pr == -1), OR
            - words[pr] is a valid predecessor of words[curr]

            If we take it:
            - chain length increases by 1
            - current word becomes the new "previous" word
        */
        if (pr == -1 || ispred(words[pr], words[curr])) {
            take = 1 + solve(words, curr, curr + 1);
        }

        /*
            SKIP decision:

            We ignore words[curr] and move forward,
            keeping the previous word unchanged.
        */
        skip = solve(words, pr, curr + 1);

        /*
            Store result for this state.

            dp[pr][curr] represents:
            "Best chain length possible starting from curr,
             assuming pr is the previously chosen word."
        */
        if (pr != -1) {
            dp[pr][curr] = max(skip, take);
        }

        return max(skip, take);
    }

    int longestStrChain(vector<string>& words) {

        /*
            Sort words by length (ascending).

            This is a CRITICAL step:
            - A predecessor must be shorter by exactly one character
            - Sorting guarantees all possible predecessors appear BEFORE
              the current word in the recursion

            This eliminates cycles and makes DP valid.
        */
        sort(words.begin(), words.end(),
             [](const string& a, const string& b) {
                 return a.length() < b.length();
             });

        n = words.size();

        /*
            DP table:
            dp[pr][curr]

            Dimensions:
            - pr   ranges from 0 to n-1
            - curr ranges from 0 to n-1

            dp[pr][curr] stores the best result for that state.
        */
        dp.assign(n, vector<int>(n, -1));

        /*
            Start recursion:
            - curr = 0 → start from the first word
            - pr   = -1 → no previous word chosen yet
        */
        return solve(words, -1, 0);
    }
};


class Solution {
public:
    int longestStrChain(vector<string>& words) {

        /*
            STEP 1: Sort words by length (ascending)

            Intuition:
            - A word can only be formed by inserting one character
              into a strictly shorter word.
            - Therefore, any valid predecessor of a word MUST appear
              before it in this order.
            - This sorting gives us a valid left-to-right DP order.
        */
        sort(words.begin(), words.end(),
             [](const string& a, const string& b) {
                 return a.length() < b.length();
             });

        int n = words.size();

        /*
            STEP 2: Define DP state

            dp[i] = length of the longest valid word chain
                    that ENDS EXACTLY at words[i]

            This invariant is crucial:
            - dp[i] always represents a REAL chain
            - The last word of that chain is words[i]
            - No global best is mixed into dp[i]
        */
        vector<int> dp(n, 1);

        /*
            Base intuition:
            - Any single word is a valid chain of length 1 by itself
            - Hence dp[i] starts from 1 for all i
        */
        int maxi = 1;

        /*
            STEP 3: Build DP bottom-up

            Outer loop:
            - Fixes the ending word of the chain (words[i])
            - We are asking:
              "If the chain MUST end at words[i], what is the best we can do?"
        */
        for (int i = 0; i < n; i++) {

            /*
                Inner loop:
                - Try all possible earlier words as predecessors
                - Only words before i can be predecessors due to sorting
            */
            for (int j = 0; j < i; j++) {

                /*
                    Predecessor check intuition:

                    words[j] can precede words[i] if:
                    - words[i] is exactly one character longer
                    - words[j] appears as a subsequence of words[i]

                    If this holds, then any chain ending at j
                    can be extended by words[i].
                */
                if (isPredecessor(words[j], words[i])) {

                    /*
                        Transition:

                        dp[j] = best chain ending at words[j]
                        + 1   = append words[i] to that chain

                        We take max because multiple j's may be valid
                        predecessors, and we want the best one.
                    */
                    dp[i] = max(dp[i], dp[j] + 1);
                }
            }

            /*
                STEP 4: Track global answer

                Important insight:
                - The longest chain does NOT have to end at the last word
                - So we track the maximum across all dp[i]
            */
            maxi = max(maxi, dp[i]);
        }

        /*
            maxi now represents the length of the longest possible word chain.
        */
        return maxi;
    }

private:
    /*
        Helper used only for predecessor validation.

        Intuition:
        - longer must be exactly one character longer than shorter
        - shorter must match longer with exactly one skipped character
    */
    bool isPredecessor(const string& shorter, const string& longer) {
        if (longer.length() != shorter.length() + 1)
            return false;

        int i = 0, j = 0;
        bool skipped = false;

        while (i < shorter.length() && j < longer.length()) {
            if (shorter[i] == longer[j]) {
                i++;
                j++;
            } else {
                if (skipped)
                    return false;
                skipped = true;
                j++;
            }
        }
        return true;
    }
};
