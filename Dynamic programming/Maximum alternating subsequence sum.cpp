// @ https://leetcode.com/problems/maximum-alternating-subsequence-sum/

class Solution {
public:
    #define ll long long

    /*
        CORE IDEA (read this first):

        We are building a subsequence.
        After reindexing, the subsequence always follows this rule:

            1st picked element → +
            2nd picked element → -
            3rd picked element → +
            4th picked element → -
            ...

        IMPORTANT:
        The sign of a number does NOT depend on:
        - its original index in nums
        - the value itself

        The sign depends ONLY on:
        - how many elements have already been picked before it

        If we have picked:
            even count → next pick is +
            odd count  → next pick is -

        We encode this using a boolean `flag`:
            flag == true  → next pick contributes +value
            flag == false → next pick contributes -value
    */


    /*
        solve(index, nums, flag, dp)

        INTUITIVE MEANING (this is the DP state):

        "Starting from position `index`,
         and assuming that the NEXT element we decide to pick
         will use the sign described by `flag`,
         what is the maximum alternating sum we can still obtain?"

        Key points:
        - We are NOT storing the sum so far.
        - We are storing the BEST POSSIBLE FUTURE GAIN from this state.
        - Same (index, flag) situation can appear many times → memoize it.
    */
    ll solve(int index, vector<int>& nums, bool flag, vector<array<ll, 2>>& dp) {

        ll n = nums.size();

        /*
            Base case:

            If index has moved past the end of the array,
            there are no elements left to consider.

            So regardless of the flag,
            the maximum additional alternating sum from here is 0.
        */
        if (index >= n) {
            return 0;
        }

        /*
            Memoization check:

            If we have already solved this exact situation before
            (same index, same expected sign for the next pick),
            we can reuse the stored result.

            dp[index][flag] already represents:
            "maximum alternating sum achievable from this state".
        */
        if (dp[index][flag] != -1) {
            return dp[index][flag];
        }

        /*
            OPTION 1: SKIP the current element nums[index]

            Meaning:
            - We choose NOT to include nums[index] in our subsequence.
            - Since we did not pick anything,
              the number of picked elements does NOT change.
            - Therefore, the expected sign for the next pick (flag)
              remains the same.

            So we simply move to index + 1 with the same flag.
        */
        ll skip = solve(index + 1, nums, flag, dp);

        /*
            OPTION 2: TAKE the current element nums[index]

            If we take it, two things happen:
            1. It contributes either +nums[index] or -nums[index]
               depending on `flag`.
            2. Since we picked an element, the parity of picked elements
               changes, so the sign for the NEXT pick flips.
        */

        // Start with the raw value
        ll val = nums[index];

        /*
            If flag == false, this means:
            "The next picked element should be subtracted."

            So we negate the value to reflect its contribution.
        */
        if (flag == false) {
            val = -val;
        }

        /*
            After taking nums[index]:
            - We add its signed contribution (val)
            - We move to index + 1
            - We flip the flag because we have picked one element
              (even → odd or odd → even count)
        */
        ll take = solve(index + 1, nums, !flag, dp) + val;

        /*
            Final decision at this state:

            From the same (index, flag), we have two valid choices:
            - skip the element
            - take the element

            We choose whichever gives the larger alternating sum.

            This value is:
            - the correct answer for this state
            - stored in dp[index][flag] for future reuse
            - returned to the caller
        */
        return dp[index][flag] = max(skip, take);
    }


    /*
        Entry function:

        We start from index = 0
        and we have picked NOTHING yet.

        Since we have picked 0 elements (even count),
        the FIRST element we pick must be treated as '+'.

        Therefore, we start with flag = true.
    */
    long long maxAlternatingSum(vector<int>& nums) {

        ll n = nums.size();

        /*
            dp[index][flag] memo table:

            index → current position in nums
            flag  → expected sign of the NEXT pick

            dp[i][0] → best result from index i if next pick is '-'
            dp[i][1] → best result from index i if next pick is '+'

            Initialized to -1 to indicate "not yet computed".
        */
        vector<array<ll, 2>> dp(n, { -1, -1 });

        // Compute the answer starting from the initial state
        return solve(0, nums, true, dp);
    }
};

class Solution {
public:
    #define ll long long

    long long maxAlternatingSum(vector<int>& nums) {

        int n = nums.size();

        /*
            DP STATE DEFINITION (this is the backbone — read this carefully):

            dp[i][0]:
                - We are choosing a subsequence from the FIRST i+1 elements
                  (i.e., nums[0] ... nums[i])
                - The number of picked elements is EVEN (length is even NOW)
                - Since length is even:
                    * last picked index in subsequence = (even - 1) = odd
                    * odd index → SUBTRACTED
                - dp[i][0] stores the MAX alternating sum under these conditions

            dp[i][1]:
                - We are choosing a subsequence from the FIRST i+1 elements
                - The number of picked elements is ODD (length is odd NOW)
                - Since length is odd:
                    * last picked index in subsequence = (odd - 1) = even
                    * even index → ADDED
                - dp[i][1] stores the MAX alternating sum under these conditions

            IMPORTANT:
            - The DP tracks what the subsequence IS NOW, not what happens next
            - Signs (+ / -) are a CONSEQUENCE of length parity, not a choice
        */

        vector<array<ll, 2>> dp(n, { -1, -1 });

        /*
            BASE CASE (i = 0, only nums[0] available):

            Two possibilities:

            1) Pick NOTHING:
               - subsequence = []
               - length = 0 (even)
               - alternating sum = 0
               → dp[0][0] = 0

            2) Pick nums[0]:
               - subsequence = [nums[0]]
               - length = 1 (odd)
               - first element is always ADDED
               → dp[0][1] = nums[0]

            This matches the alternating sum definition exactly.
        */
        dp[0][0] = 0;
        dp[0][1] = nums[0];

        /*
            MAIN DP LOOP

            At each index i, we decide:
            - either SKIP nums[i]
            - or TAKE nums[i]

            Skipping:
            - does NOT change subsequence length
            - parity remains the same

            Taking:
            - increases subsequence length by 1
            - parity FLIPS
            - sign (+ / -) depends on the PREVIOUS length
        */
        for (int i = 1; i < n; i++) {

            /*
                Transition for dp[i][0] (EVEN length NOW):

                How can we end up with EVEN length after index i?

                Case 1: SKIP nums[i]
                    - previous length was already even
                    - dp[i-1][0]

                Case 2: TAKE nums[i]
                    - previous length must have been odd
                    - odd length ⇒ last index was even ⇒ next index is odd
                    - odd index ⇒ SUBTRACT nums[i]
                    - dp[i-1][1] - nums[i]

                We take the maximum of these two possibilities.
            */
            dp[i][0] = max(
                dp[i-1][0],
                dp[i-1][1] - nums[i]
            );

            /*
                Transition for dp[i][1] (ODD length NOW):

                How can we end up with ODD length after index i?

                Case 1: SKIP nums[i]
                    - previous length was already odd
                    - dp[i-1][1]

                Case 2: TAKE nums[i]
                    - previous length must have been even
                    - even length ⇒ next index is even
                    - even index ⇒ ADD nums[i]
                    - dp[i-1][0] + nums[i]

                Again, choose the better option.
            */
            dp[i][1] = max(
                dp[i-1][1],
                dp[i-1][0] + nums[i]
            );
        }

        /*
            FINAL ANSWER:

            After processing all elements:
            - The subsequence may end with EVEN length or ODD length
            - The problem does NOT restrict parity of final length

            So we take the maximum of both possibilities.
        */
        return max(dp[n-1][0], dp[n-1][1]);
    }
};

