/*

================================================================================
PROBLEM: Burst Balloons (LeetCode 312)
================================================================================

WHAT THE PROBLEM IS ASKING:
You are given `n` balloons, each with an integer value painted on it, represented by an array `nums`.
You are asked to burst all the balloons.
When you burst the `i-th` balloon, you get `nums[i-1] * nums[i] * nums[i+1]` coins.
If `i-1` or `i+1` goes out of bounds of the array, treat it as if there is a balloon with a value of `1`.
The objective is to find the MAXIMUM possible coins you can collect by strategically choosing the order 
in which you burst the balloons.

DEFINITIONS OF IMPORTANT TERMS:
- Bursting: Removing an element from the sequence, which causes its left and right neighbors to become adjacent.
- Padding: Adding conceptual `1`s at the boundaries (-1 and n) so boundary bursts don't require special logic.

CONSTRAINTS AND THEIR IMPLICATIONS:
- `n <= 300`: This is a massive hint. O(N!) permutations to try every bursting order is impossible. 
  Even an O(N^4) algorithm would struggle. O(N^3) is strongly implied, which is the classic signature 
  of "Interval Dynamic Programming" (Matrix Chain Multiplication type problems).
- `nums[i] <= 100`: Coins can get large, but the max possible value (100 * 100 * 100 * 300) easily 
  fits within a standard 32-bit signed integer.

EDGE CASES:
- `nums` is empty: Return 0.
- `nums` has 1 element: It bursts and multiplies with the out-of-bounds 1s (1 * nums[0] * 1).
- Zeros in the array: Bursting a 0 yields 0 and eliminates a balloon, which might actually be optimal 
  to get it out of the way so larger adjacent numbers can multiply together.

====================================================================================================
THINKING FROM FIRST PRINCIPLES
====================================================================================================
START FROM SCRATCH:
Imagine an array [A, B, C, D]. 
If we think chronologically (forward), we might decide to burst B first. 
Coins gained = A * B * C.
The new array becomes [A, C, D]. 
Now, A and C are adjacent. If we burst C next, we multiply A * C * D.
Notice how bursting B changed the neighbors of A and C. The left subproblem and right subproblem 
interfere with each other. Because adjacency changes dynamically, we cannot cleanly break the array 
into independent subproblems. Standard Divide & Conquer fails.

BUILD INTUITION STEP BY STEP:
Since forward chronological order causes dependencies, what if we reverse time? 
Instead of picking the FIRST balloon to burst, what if we pick the LAST balloon to burst?

EXPLAIN HOW THE KEY OBSERVATION IS DISCOVERED:
Assume the original array is padded: [1, A, B, C, D, 1].
Let's guess that 'C' is the ABSOLUTE LAST balloon to burst.
If 'C' is the last to burst, it means just before 'C' bursts, the array looks like this: [1, C, 1].
This implies that ALL balloons to the left of 'C' (which are A and B) were already burst.
And ALL balloons to the right of 'C' (which is D) were already burst.
Crucially: While we were figuring out how to burst the left side (A, B), 'C' and the left boundary '1' 
were standing there like solid, immovable walls. 
Similarly, while bursting the right side (D), 'C' and the right boundary '1' were solid walls.
By fixing the LAST balloon, we have successfully decoupled the left subarray from the right subarray!

EXPLAIN THE CORE CONCEPT DEEPLY:
Interval DP relies on defining a range and choosing a pivot that splits the range into independent parts.
By making the pivot `k` the LAST balloon to burst in the interval `(i, j)`, we can safely state that 
the cost to burst `k` at the very end will be `arr[i] * arr[k] * arr[j]`.

CLEARLY DEFINE INVARIANTS / STATES / TRANSITIONS:
- State `dp[i][j]`: The maximum coins we can get by bursting ALL balloons STRICTLY BETWEEN index `i` and index `j`.
  (Note: Balloons at `i` and `j` are NOT burst in this subproblem; they act as the permanent walls for the interval).
- Invariant: When solving `dp[i][j]`, `arr[i]` and `arr[j]` are guaranteed to exist and have not been burst yet.
- Transition: To find `dp[i][j]`, we iterate every possible balloon `k` (where `i < k < j`) and pretend `k` 
  is the LAST balloon to burst in this interval.
  The total coins if `k` is last = (Max coins to burst everything between i and k) 
                                 + (Max coins to burst everything between k and j) 
                                 + (Coins from bursting k itself, which is arr[i] * arr[k] * arr[j]).
  Therefore: `dp[i][j] = max(dp[i][j], dp[i][k] + dp[k][j] + (arr[i] * arr[k] * arr[j]))`

====================================================================================================
WHY THIS SPECIFIC ALGORITHM WORKS
====================================================================================================
WHY THIS DATA STRUCTURE IS CHOSEN:
A 2D vector (matrix) `dp` is chosen because our state is uniquely identified by two variables: 
the left boundary `i` and the right boundary `j`. The table caches these overlapping subproblems.

WHY CORRECTNESS HOLDS:
It exhaustively searches all possible "last balloons to burst" for every possible sub-segment of 
the array, building up from the smallest segments (length 2, meaning 0 balloons between them) up to 
the full length of the padded array. Since subproblems are proven to be independent, the Bellman 
Equation for Optimality holds.

WHY OPTIMAL COMPLEXITY IS ACHIEVED:
Instead of trying O(N!) permutations, we only evaluate O(N^2) intervals. For each interval, we try 
O(N) pivots. This guarantees an upper bound of O(N^3) operations, which perfectly fits the 300 constraint.
*/

#include <vector>
#include <algorithm>
#include <iostream>

using namespace std; // Applied to avoid std prefixes in C++ code

// =================================================================================================
// APPROACH 1: TOP-DOWN MEMOIZATION (Recursive)
// =================================================================================================
class SolutionTopDown {
public:
    // Memoization table to cache results of subproblems. 
    // dp[i][j] holds the answer for the open interval (i, j).
    vector<vector<int>> dp;
    
    // Padded array to hold the original numbers plus the boundary 1s.
    vector<int> arr;

    // Recursive function to solve for the open interval (i, j)
    int solve(int i, int j) {
        // BASE CASE: 
        // If there are no balloons STRICTLY between i and j, we cannot burst anything.
        // What it breaks if removed: Infinite recursion, attempting to burst non-existent balloons.
        // Invariant maintained: The interval (i, j) must contain at least one burstable balloon (j - i > 1).
        if (j - i <= 1)
            return 0;

        // MEMOIZATION CHECK:
        // If the state has already been computed (not -1), return the cached maximum.
        // What logical role it plays: Pruning the recursive tree from O(N!) to O(N^3).
        if (dp[i][j] != -1)
            return dp[i][j];

        // Variable to track the maximum coins achievable for the current interval (i, j)
        int ans = 0;

        // Iterate through every possible balloon 'k' strictly between 'i' and 'j'
        // Conceptual meaning: We are guessing that balloon 'k' will be the VERY LAST balloon 
        // to burst in this entire interval.
        for (int k = i + 1; k < j; k++) {
            // Because 'k' is the LAST to burst, all balloons between 'i' and 'k' must burst first.
            // And all balloons between 'k' and 'j' must burst first.
            // While they are bursting, 'i', 'k', and 'j' act as indestructible walls.
            int coins = 
                solve(i, k) +               // Max coins from recursively bursting left sub-segment (i, k)
                solve(k, j) +               // Max coins from recursively bursting right sub-segment (k, j)
                arr[i] * arr[k] * arr[j];   // Finally, bursting 'k' itself when only i, k, and j are left.

            // Update the interval's maximum possible answer
            ans = max(ans, coins);
        }

        // Cache the computed answer for this state before returning
        return dp[i][j] = ans;
    }

    int maxCoins(vector<int>& nums) {
        // PADDING THE ARRAY:
        // Pushing 1 to the beginning to handle the left-most out-of-bounds rule.
        arr.push_back(1);

        // Copying the actual balloons into our working array.
        for (int x : nums)
            arr.push_back(x);

        // Pushing 1 to the end to handle the right-most out-of-bounds rule.
        arr.push_back(1);

        int n = arr.size();

        // INITIALIZE DP TABLE:
        // Size n x n, filled with -1 to signify uncomputed states.
        dp.assign(n, vector<int>(n, -1));

        // Start the recursion requesting the max coins for bursting everything 
        // strictly between index 0 and index n-1.
        return solve(0, n - 1);
    }
};


// =================================================================================================
// APPROACH 2: BOTTOM-UP TABULATION (Iterative)
// =================================================================================================
class SolutionBottomUp {
public:
    int maxCoins(vector<int>& nums) {
        // STEP 1: PADDING
        // We create a new array and pad it with 1s at both ends.
        // What logical role it plays: Unifies the logic. We no longer need 'if out of bounds' checks 
        // when multiplying adjacent balloon values.
        vector<int> arr;
        arr.push_back(1);
        for (int x : nums)
            arr.push_back(x);
        arr.push_back(1);

        int n = arr.size();

        // STEP 2: DP TABLE INITIALIZATION
        // dp[i][j] represents max coins obtained by bursting all balloons strictly between index i and j.
        // We initialize with 0. Ranges of length 0 or 1 naturally have 0 coins, which is correct.
        vector<vector<int>> dp(n, vector<int>(n, 0));

        // STEP 3: FILLING THE DP TABLE BASED ON INTERVAL LENGTH
        // We must solve smaller subproblems before larger ones. The smallest interval where we can 
        // actually burst a balloon has a length of 2 (e.g., boundaries i=0, j=2, burst k=1).
        // What would break if removed/changed: Iterating by `i` then `j` normally would look up 
        // states (like dp[k][j]) that haven't been computed yet. Length-based iteration ensures dependencies are met.
        for (int len = 2; len < n; len++) {

            // Slide a window of size `len` across the array. 
            // `i` is the left boundary (exclusive wall).
            for (int i = 0; i + len < n; i++) {
                
                // `j` is the right boundary (exclusive wall).
                int j = i + len;

                // `k` represents the index of the balloon we are choosing to burst LAST 
                // in the current interval strictly between i and j.
                for (int k = i + 1; k < j; k++) {
                    
                    // The core transition. 
                    // To burst 'k' last, we take the optimal solutions of the left part (i to k) 
                    // and right part (k to j) - which were already calculated in previous, shorter `len` iterations -
                    // and add the final burst value of 'k'.
                    dp[i][j] = max(
                        dp[i][j], 
                        dp[i][k] + dp[k][j] + arr[i] * arr[k] * arr[j]
                    );
                }
            }
        }

        // The answer to the entire problem is the max coins gained by bursting everything 
        // strictly between the 0th padded element and the (n-1)th padded element.
        return dp[0][n - 1];
    }
};

/*
====================================================================================================
DRY RUN WITH A SMALL EXAMPLE
====================================================================================================
Input: nums = [3, 1, 5]
Padded arr = [1, 3, 1, 5, 1]
n = 5. Indices: 0, 1, 2, 3, 4. Values: 1, 3, 1, 5, 1.

DP Initialization: All dp[i][j] = 0.

ITERATION 1: len = 2 (1 balloon strictly between i and j)
- i=0, j=2. k=1. dp[0][2] = max(0, dp[0][1] + dp[1][2] + arr[0]*arr[1]*arr[2]) = 0 + 0 + (1*3*1) = 3
- i=1, j=3. k=2. dp[1][3] = max(0, dp[1][2] + dp[2][3] + arr[1]*arr[2]*arr[3]) = 0 + 0 + (3*1*5) = 15
- i=2, j=4. k=3. dp[2][4] = max(0, dp[2][3] + dp[3][4] + arr[2]*arr[3]*arr[4]) = 0 + 0 + (1*5*1) = 5

ITERATION 2: len = 3 (2 balloons strictly between i and j)
- i=0, j=3. Possible 'k' = 1 or 2.
  - k=1 (burst 1st balloon LAST): dp[0][1] + dp[1][3] + arr[0]*arr[1]*arr[3] = 0 + 15 + (1*3*5) = 30
  - k=2 (burst 2nd balloon LAST): dp[0][2] + dp[2][3] + arr[0]*arr[2]*arr[3] = 3 + 0 + (1*1*5) = 8
  - dp[0][3] = max(30, 8) = 30
- i=1, j=4. Possible 'k' = 2 or 3.
  - k=2 (burst 2nd LAST): dp[1][2] + dp[2][4] + arr[1]*arr[2]*arr[4] = 0 + 5 + (3*1*1) = 8
  - k=3 (burst 3rd LAST): dp[1][3] + dp[3][4] + arr[1]*arr[3]*arr[4] = 15 + 0 + (3*5*1) = 30
  - dp[1][4] = max(8, 30) = 30

ITERATION 3: len = 4 (3 balloons strictly between i and j) -> The full problem!
- i=0, j=4. Possible 'k' = 1, 2, or 3.
  - k=1: dp[0][1] + dp[1][4] + arr[0]*arr[1]*arr[4] = 0 + 30 + (1*3*1) = 33
  - k=2: dp[0][2] + dp[2][4] + arr[0]*arr[2]*arr[4] = 3 + 5 + (1*1*1) = 9
  - k=3: dp[0][3] + dp[3][4] + arr[0]*arr[3]*arr[4] = 30 + 0 + (1*5*1) = 35
  - dp[0][4] = max(33, 9, 35) = 35.

Final Answer: dp[0][4] = 35.

====================================================================================================
COMPLEXITY DERIVATION
====================================================================================================
TIME COMPLEXITY: O(N^3)
- We have three nested loops in the tabulation approach (and equivalently, 3 degrees of freedom in the memoized version).
- Outer loop (`len`): iterates from 2 to N. ~ O(N) iterations.
- Middle loop (`i`): iterates from 0 to N - len. ~ O(N) iterations.
- Inner loop (`k`): iterates from i + 1 to j - 1. In the worst case, this is roughly `len` iterations, which scales with O(N).
- Total operations: sum of (len) over all valid start positions ~ N^3 / 6 operations.
- Therefore, the time complexity firmly drops to O(N^3).

SPACE COMPLEXITY: O(N^2)
- The algorithm defines a 2D matrix (vector of vectors) `dp` of size N x N.
- In the tabulation approach, this is exactly N^2 integers.
- In the memoized approach, it is N^2 integers plus O(N) call stack space for the recursion.
- Therefore, overall space complexity is O(N^2).

WHY THIS IS OPTIMAL:
Any problem that involves searching for the optimal grouping or parenthesization of contiguous elements 
(like Matrix Chain Multiplication, Optimal Binary Search Trees, or bursting contiguous segments of balloons) 
requires evaluating the optimal split point (pivot) for every possible sub-segment. 
Since there are mathematically O(N^2) possible intervals, and evaluating each interval requires testing 
every potential pivot O(N) inside it, the lower bound for this specific dynamic programming state formulation 
is intrinsically O(N^3). There are no redundant state calculations.
*/