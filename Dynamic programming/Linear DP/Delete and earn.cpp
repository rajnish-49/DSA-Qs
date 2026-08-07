/*
    Delete and Earn

    Choosing a value x eventually allows us to earn all occurrences of x,
    giving total points x * frequency[x].

    However, choosing x prevents us from earning from x - 1 and x + 1.

    After grouping equal values:
        points[x] = total points available from value x

    The problem becomes House Robber over the value range:
    take x  -> points[x] + answer up to x - 2
    skip x  -> answer up to x - 1
*/


// ============================================================
// MEMOIZATION
// ============================================================

class MemoizationSolution {
public:
    vector<int> points;
    vector<int> memo;

    /*
        solve(value) = maximum points obtainable by considering
        all values from 0 to 'value'.
    */
    int solve(int value) {
        if (value < 0) {
            return 0;
        }

        if (value == 0) {
            return points[0];
        }

        if (memo[value] != -1) {
            return memo[value];
        }

        /*
            Take the current value:
            We earn points[value] but must skip value - 1.

            Skip the current value:
            The answer remains the best result up to value - 1.
        */
        int take = points[value] + solve(value - 2);
        int skip = solve(value - 1);

        return memo[value] = max(take, skip);
    }

    int deleteAndEarn(vector<int>& nums) {
        int maximumValue =
            *max_element(nums.begin(), nums.end());

        points.assign(maximumValue + 1, 0);

        // Combine all occurrences of the same value into one earning.
        for (int value : nums) {
            points[value] += value;
        }

        memo.assign(maximumValue + 1, -1);

        return solve(maximumValue);
    }
};

/*
    Time:  O(n + maximumValue)
    Space: O(maximumValue) DP + O(maximumValue) recursion stack
*/


// ============================================================
// TABULATION
// ============================================================

class TabulationSolution {
public:
    int deleteAndEarn(vector<int>& nums) {
        int maximumValue =
            *max_element(nums.begin(), nums.end());

        vector<int> points(maximumValue + 1, 0);

        for (int value : nums) {
            points[value] += value;
        }

        /*
            dp[value] = maximum points obtainable by considering
            all values from 0 to 'value'.
        */
        vector<int> dp(maximumValue + 1, 0);

        dp[0] = points[0];

        if (maximumValue >= 1) {
            dp[1] = max(points[0], points[1]);
        }

        for (int value = 2; value <= maximumValue; value++) {

            /*
                Take value:
                points[value] + dp[value - 2]

                Skip value:
                dp[value - 1]
            */
            dp[value] = max(
                points[value] + dp[value - 2],
                dp[value - 1]
            );
        }

        return dp[maximumValue];
    }
};

/*
    Time:  O(n + maximumValue)
    Space: O(maximumValue)
*/


// ============================================================
// SPACE OPTIMIZATION
// ============================================================

class SpaceOptimizedSolution {
public:
    int deleteAndEarn(vector<int>& nums) {
        int maximumValue =
            *max_element(nums.begin(), nums.end());

        vector<int> points(maximumValue + 1, 0);

        for (int value : nums) {
            points[value] += value;
        }

        /*
            Only the previous two DP states are required:

            twoValuesBack = dp[value - 2]
            oneValueBack  = dp[value - 1]
        */
        int twoValuesBack = 0;
        int oneValueBack = points[0];

        for (int value = 1; value <= maximumValue; value++) {
            int current = max(
                oneValueBack,
                twoValuesBack + points[value]
            );

            twoValuesBack = oneValueBack;
            oneValueBack = current;
        }

        return oneValueBack;
    }
};

/*
    Time:  O(n + maximumValue)
    Space: O(maximumValue) for the points array
    Extra DP space: O(1)
*/