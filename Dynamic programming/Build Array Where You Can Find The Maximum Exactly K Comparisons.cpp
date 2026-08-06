/*
    Build Array Where You Can Find the Maximum Exactly K Comparisons

    Build arrays of length n using values from 1 to m.

    While scanning an array from left to right, the search cost increases
    whenever the current value is greater than every previously seen value.

    Return the number of arrays having exactly k such new maximums.
    Ex: [2,3,1,3,4] -> search cost 3 (new maxes at idx 0,1,4)
*/


// ============================================================
// MEMOIZATION
// ============================================================

class MemoizationSolution {
public:
    static const int MOD = 1e9 + 7;

    int n, m, requiredCost;
    int dp[51][51][101];

    /*
        State:
        solve(index, cost, currentMax)

        index      = position currently being filled
        cost       = number of new maximums found so far
        currentMax = maximum value used in the current prefix
    */
    int solve(int index, int cost, int currentMax) {

        // Once cost exceeds k, this path can never become valid.
        if (cost > requiredCost) {
            return 0;
        }

        // After filling the complete array, accept only exact cost k.
        if (index == n) {
            return cost == requiredCost;
        }

        int &answer = dp[index][cost][currentMax];

        if (answer != -1) {
            return answer;
        }

        long long ways = 0;

        for (int value = 1; value <= m; value++) {

            if (value > currentMax) {
                /*
                    A new maximum is created:
                    - search cost increases
                    - current maximum becomes value
                */
                ways += solve(index + 1, cost + 1, value);
            } else {
                /*
                    The value does not exceed the current maximum,
                    so neither cost nor currentMax changes.
                */
                ways += solve(index + 1, cost, currentMax);
            }

            ways %= MOD;
        }

        return answer = ways;
    }

    int numOfArrays(int n, int m, int k) {
        this->n = n;
        this->m = m;
        requiredCost = k;

        memset(dp, -1, sizeof(dp));

        /*
            currentMax starts at 0 because array values begin from 1.
            Therefore, the first selected value always creates the first maximum.
        */
        return solve(0, 0, 0);
    }
};


/*
    Memoization Complexity

    States: n * k * m
    Work per state: try m possible values

    Time:  O(n * k * m²)
    Space: O(n * k * m)
*/


// ============================================================
// TABULATION
// ============================================================

class TabulationSolution {
public:
    static const int MOD = 1e9 + 7;

    int numOfArrays(int n, int m, int k) {

        /*
            State:
            dp[filled][cost][currentMax]

            Number of ways to build a prefix where:
            - exactly 'filled' positions have been filled
            - 'cost' new maximums have appeared
            - the maximum value in the prefix is currentMax
        */
        vector<vector<vector<long long>>> dp(
            n + 1,
            vector<vector<long long>>(
                k + 1,
                vector<long long>(m + 1, 0)
            )
        );

        /*
            Empty prefix:
            - zero positions filled
            - zero search cost
            - no maximum yet, represented by 0
        */
        dp[0][0][0] = 1;

        for (int filled = 0; filled < n; filled++) {

            for (int cost = 0; cost <= k; cost++) {

                for (int currentMax = 0; currentMax <= m; currentMax++) {

                    long long currentWays =
                        dp[filled][cost][currentMax];

                    // This state cannot be reached.
                    if (currentWays == 0) {
                        continue;
                    }

                    // Try every possible value at the next position.
                    for (int value = 1; value <= m; value++) {

                        if (value > currentMax) {

                            /*
                                A new maximum is created.

                                Transition:
                                dp[filled][cost][currentMax]
                                    ->
                                dp[filled + 1][cost + 1][value]
                            */
                            if (cost + 1 <= k) {
                                dp[filled + 1][cost + 1][value] =
                                    (
                                        dp[filled + 1][cost + 1][value]
                                        + currentWays
                                    ) % MOD;
                            }

                        } else {

                            /*
                                No new maximum is created.

                                Transition:
                                dp[filled][cost][currentMax]
                                    ->
                                dp[filled + 1][cost][currentMax]
                            */
                            dp[filled + 1][cost][currentMax] =
                                (
                                    dp[filled + 1][cost][currentMax]
                                    + currentWays
                                ) % MOD;
                        }
                    }
                }
            }
        }

        long long answer = 0;

        /*
            We need exactly k new maximums, but the final maximum
            can be any value from 1 to m.
        */
        for (int finalMax = 1; finalMax <= m; finalMax++) {
            answer = (
                answer + dp[n][k][finalMax]
            ) % MOD;
        }

        return answer;
    }
};


/*
    Tabulation Complexity

    States: n * k * m
    Work per state: try m possible values

    Time:  O(n * k * m²)
    Space: O(n * k * m)
*/