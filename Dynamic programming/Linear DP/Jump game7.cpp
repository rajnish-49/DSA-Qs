// @https://leetcode.com/problems/jump-game-vii/description/

/*
    Jump Game VII

    Start at index 0 and reach index n - 1.

    From a reachable index j, we can jump to index i when:

        minJump <= i - j <= maxJump

    We can land only on positions containing '0'.

    State:
        dp[i] = true if index i is reachable

    For index i, valid previous indices lie in:

        [i - maxJump, i - minJump]

    Instead of checking this entire range for every index,
    'possible' stores the number of reachable indices currently
    present inside this range.
*/

class Solution
{
public:
    bool canReach(string s, int minJump, int maxJump)
    {
        int n = s.size();

        // dp[i] tells whether index i can be reached.
        vector<bool> dp(n, false);
        dp[0] = true;

        /*
            Number of reachable indices inside the valid predecessor
            window for the current index.
        */
        int possible = 0;

        for (int i = 1; i < n; i++)
        {
            /*
                Index i - minJump has just entered the valid window.

                If it is reachable, it becomes a possible starting
                index for a jump to i.
            */
            if (i - minJump >= 0 && dp[i - minJump])
            {
                possible++;
            }

            /*
                Index i - maxJump - 1 has moved outside the valid window,
                so remove its contribution.
            */
            if (i - maxJump - 1 >= 0 && dp[i - maxJump - 1])
            {
                possible--;
            }

            /*
                Index i is reachable only when:
                - landing is allowed at i
                - at least one reachable index exists in its valid window
            */
            if (s[i] == '0' && possible > 0)
            {
                dp[i] = true;
            }
        }

        return dp[n - 1];
    }
};

/*
    Time:  O(n)
    Space: O(n)
*/