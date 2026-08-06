// @https://leetcode.com/problems/maximum-length-of-pair-chain/ 

class Solution {
public:

    /*
        solve(idx, p):

        idx → the current index we are deciding on
        p   → index of the previously chosen pair
              p == -1 means no pair has been chosen yet

        The function returns:
        "The maximum chain length we can form
         using pairs from idx to end,
         given that the last chosen pair is p"
    */
    int solve(vector<vector<int>>& pairs, vector<vector<int>>& dp, int idx, int p) {

        int n = pairs.size();

        /*
            Base case:
            If we have considered all pairs, no more can be added to the chain.
        */
        if (idx >= n) {
            return 0;
        }

        /*
            Memoization check:

            If p != -1, then (idx, p) represents a valid DP state.
            If we have already computed the answer for this state,
            reuse it instead of recomputing.

            This is where exponential recursion collapses into DP.
        */
        if (p != -1 && dp[idx][p] != -1) {
            return dp[idx][p];
        }

        /*
            Option 1: TAKE the current pair at index idx.

            We are allowed to take it if:
            - no previous pair has been chosen yet (p == -1), OR
            - the current pair can follow the previous one
              (pairs[idx][0] > pairs[p][1])

            If we take it:
            - chain length increases by 1
            - current pair becomes the "previous" one for future decisions
        */
        int take = 0;
        if (p == -1 || pairs[idx][0] > pairs[p][1]) {
            take = 1 + solve(pairs, dp, idx + 1, idx);
        }

        /*
            Option 2: SKIP the current pair.

            We move to the next index,
            but the previously chosen pair (p) remains unchanged.

            This represents the choice:
            "Do not include pairs[idx] in the chain"
        */
        int skip = solve(pairs, dp, idx + 1, p);

        /*
            Store the result in DP table.

            Important subtlety:
            - We only memoize when p != -1
            - p == -1 is the starting condition and does not represent
              a fixed previous constraint in the table

            dp[idx][p] stores:
            "Best chain length possible from idx onward,
             given that p is the previously chosen pair"
        */
        if (p != -1) {
            dp[idx][p] = max(take, skip);
        }

        /*
            Return the best outcome from this state:
            either we take the current pair or we skip it.
        */
        return max(take, skip);
    }


    int findLongestChain(vector<vector<int>>& pairs) {

        /*
            Sort pairs by their ending value (right endpoint).

            This ensures:
            - Pairs that finish earlier are considered first
            - Once we move forward in idx, we never need to go backward
            - The DP recursion forms an acyclic state graph

            Sorting is not DP itself, but it makes the DP valid and simpler.
        */
        sort(pairs.begin(), pairs.end(),
             [](const vector<int>& a, const vector<int>& b) {
                 return a[1] < b[1];
             });
        
        int n = pairs.size();

        /*
            DP table:
            dp[idx][p] = cached answer for state (idx, p)

            Size is (n x n) because:
            - idx can range from 0 to n-1
            - p can range from 0 to n-1
        */
        vector<vector<int>> dp(n + 1, vector<int>(n + 1, -1));

        /*
            Start recursion:
            - idx = 0 → start considering from first pair
            - p = -1  → no previous pair chosen yet
        */
        return solve(pairs, dp, 0, -1);
    }
};


class Solution {
public:
    int findLongestChain(vector<vector<int>>& pairs) {

        /*
            First, sort pairs by their ending value (right endpoint).

            Why this matters for DP:
            - After sorting, if j < i, then pairs[j] ends no later than pairs[i]
            - This gives a natural left-to-right order
            - It ensures we never need to "go back" once we move forward
        */
        sort(pairs.begin(), pairs.end(),
             [](const vector<int>& a, const vector<int>& b) {
                 return a[1] < b[1];
             });

        int n = pairs.size();

        /*
            dp[i] = length of the longest valid chain
                    that ENDS EXACTLY at pair i

            This invariant is critical:
            - dp[i] represents a real, valid chain
            - The last pair of that chain is pairs[i]
        */
        vector<int> dp(n, 1);

        /*
            Any single pair can form a chain of length 1 by itself.
            So dp[i] starts from 1 for all i.
        */
        int maxi = 1;

        /*
            Outer loop fixes the ENDING pair of the chain.

            Question we are answering for each i:
            "If the chain must end at pair i,
             what is the longest chain I can form?"
        */
        for (int i = 0; i < n; i++) {

            /*
                Inner loop tries all possible previous pairs j < i.

                We are checking:
                "Can pair j come immediately before pair i in the chain?"
            */
            for (int j = 0; j < i; j++) {

                /*
                    Validity condition for chaining:
                    pairs[j][1] < pairs[i][0]

                    Meaning:
                    - pair j finishes before pair i starts
                    - so pair i can legally follow pair j
                */
                if (pairs[j][1] < pairs[i][0]) {

                    /*
                        Transition:

                        dp[j] = best chain ending at j
                        + 1   = add pair i to that chain

                        We take the maximum because multiple j's
                        might be valid predecessors, and we want
                        the best one.
                    */
                    dp[i] = max(dp[i], dp[j] + 1);
                }
            }

            /*
                Track the global maximum chain length.

                Important:
                - The longest chain does NOT have to end at the last index
                - So we must track the maximum over all dp[i]
            */
            maxi = max(maxi, dp[i]);
        }

        /*
            maxi represents the length of the longest valid chain
            across all possible ending pairs.
        */
        return maxi;
    }
};
