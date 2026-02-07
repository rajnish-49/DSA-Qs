class Solution {
public:
    int n;
    vector<vector<int>> dp;

    /*
    ============================================================================
    PROBLEM STATEMENT: Target Sum (LeetCode 494)
    ============================================================================
    Given an array of integers 'nums' and a target integer 'target':
    - You can assign '+' or '-' sign to each number in nums
    - Goal: Find how many different expressions evaluate to 'target'
    
    Example: nums = [1,1,1,1,1], target = 3
    Possible ways: +1+1+1+1-1 = 3, +1+1+1-1+1 = 3, etc.
    Answer: 5 ways
    
    ============================================================================
    CORE INSIGHT: Transform into Subset Sum Problem
    ============================================================================
    
    KEY MATHEMATICAL TRANSFORMATION:
    
    Let's divide nums into two groups:
    - P (positive): numbers we assign '+' sign to, sum = sum_P
    - N (negative): numbers we assign '-' sign to, sum = sum_N
    
    We want: sum_P - sum_N = target  ... (equation 1)
    We know: sum_P + sum_N = total_sum (sum of all elements) ... (equation 2)
    
    Adding equation 1 and 2:
        (sum_P - sum_N) + (sum_P + sum_N) = target + total_sum
        2 * sum_P = target + total_sum
        sum_P = (target + total_sum) / 2
    
    CRUCIAL REALIZATION:
    Instead of tracking +/- signs (2^n possibilities), we just need to find:
    "How many subsets of nums have sum = (target + total_sum) / 2?"
    
    This is a CLASSIC SUBSET SUM COUNT problem!
    
    WHY THIS WORKS:
    - If we find a subset with sum_P, the remaining elements automatically 
      form the negative group with sum_N
    - Each valid subset corresponds to exactly one way to assign signs
    
    VALIDITY CHECKS NEEDED:
    1. (target + total_sum) must be even (otherwise sum_P would be fractional)
    2. target <= total_sum (can't exceed maximum possible sum)
    3. target >= -total_sum (can't go below minimum possible sum)
       Combined: abs(target) <= total_sum
    
    ============================================================================
    */

    int solve(int idx, vector<int>& nums, int target) {
        /*
        RECURSIVE FUNCTION: Count subsets with exact sum = target
        
        PARAMETERS:
        - idx: current index we're deciding on (0 to n-1)
        - nums: the input array
        - target: remaining sum we need to achieve
        
        BASE CASE REASONING:
        When idx == n (processed all elements):
        - If target == 0: We successfully formed a subset with exact sum ✓
          Return 1 (this is one valid way)
        - If target != 0: We either exceeded or fell short ✗
          Return 0 (this path doesn't work)
        */
        if (idx == n) {
            return (target == 0) ? 1 : 0;
        }

        /*
        MEMOIZATION CHECK:
        Before computing, check if we've already solved this subproblem
        
        WHY MEMOIZATION WORKS HERE:
        - Same (idx, target) will have same answer regardless of how we got there
        - State space: n indices × (val+1) possible targets = O(n × val)
        - Without memo: exponential time complexity O(2^n)
        - With memo: polynomial time complexity O(n × val)
        */
        if (dp[idx][target] != -1)
            return dp[idx][target];

        /*
        DECISION 1: SKIP the current element
        
        WHY CONSIDER SKIPPING:
        This element belongs to the "negative" group (assigned '-' sign)
        We don't include it in our subset for sum_P
        
        Move to next index with same target requirement
        */
        int skip = solve(idx + 1, nums, target);
        
        /*
        DECISION 2: TAKE the current element
        
        WHY CONSIDER TAKING:
        This element belongs to the "positive" group (assigned '+' sign)
        We include it in our subset for sum_P
        
        CONSTRAINT CHECK: nums[idx] <= target
        - Only take if it doesn't make us exceed the target
        - If nums[idx] > target, taking it would overshoot
        - This pruning prevents unnecessary recursive calls
        
        After taking: reduce target by nums[idx] (we've "used up" that much sum)
        */
        int take = 0;
        if (nums[idx] <= target) {
            take = solve(idx + 1, nums, target - nums[idx]);
        }

        /*
        COMBINATION PRINCIPLE:
        Total ways = ways_without_current_element + ways_with_current_element
        
        WHY ADD (not multiply):
        - These are INDEPENDENT, MUTUALLY EXCLUSIVE choices
        - "skip" paths and "take" paths don't overlap
        - We want count of ALL possible ways (union of disjoint sets)
        
        Store in dp table for future use (memoization)
        */
        return dp[idx][target] = skip + take;
    }

    int findTargetSumWays(vector<int>& nums, int target) {
        n = nums.size();
        
        /*
        STEP 1: Calculate total sum of all elements
        This represents the maximum possible value we can achieve
        (when all elements have '+' sign)
        */
        int s = 0;
        for (auto i : nums)
            s += i;

        /*
        VALIDITY CHECK 1: Is the target mathematically achievable?
        
        s < abs(target):
        - If target > s: Even with all '+', we can't reach target
        - If target < -s: Even with all '-', we can't reach target
        - Example: nums=[1,2], target=5 → impossible (max is 3)
        
        (target + s) % 2 != 0:
        - From our derivation: sum_P = (target + s) / 2
        - If (target + s) is odd, sum_P would be a fraction
        - But sum_P must be an integer (sum of integers)
        - Example: nums=[1,1], s=2, target=1 → (1+2)/2 = 1.5 ✗
        
        If either condition fails, return 0 (no valid ways exist)
        */
        if (s < abs(target) || (target + s) % 2 != 0)
            return 0;

        /*
        STEP 2: Transform the problem
        
        Calculate val = (target + s) / 2
        This is the sum we need for the "positive subset"
        
        NEW PROBLEM: Count subsets of nums with sum exactly equal to 'val'
        
        Example walkthrough:
        nums = [1,1,1,1,1], target = 3
        s = 5
        val = (3 + 5) / 2 = 4
        
        Question becomes: "How many subsets sum to 4?"
        Answer: {1,1,1,1} is one subset (leaving one 1 for negative)
        This corresponds to: +1+1+1+1-1 = 3 ✓
        */
        int val = (target + s) / 2;

        /*
        STEP 3: Initialize DP table
        
        Dimensions: n × (val + 1)
        - n rows: one for each element index (0 to n-1)
        - val+1 columns: for target sums from 0 to val (inclusive)
        
        Initialize with -1: indicates "not yet computed"
        (0 and positive values are valid answers, so -1 is sentinel)
        */
        dp.assign(n, vector<int>(val + 1, -1));

        /*
        STEP 4: Solve the transformed subset sum problem
        Start from index 0, need to achieve sum 'val'
        
        The recursion will explore all 2^n combinations (with memoization)
        and count only those that sum exactly to 'val'
        */
        return solve(0, nums, val);
    }
};

/*
============================================================================
TIME & SPACE COMPLEXITY ANALYSIS
============================================================================

TIME COMPLEXITY: O(n × val) where val = (target + sum) / 2
- Without memoization: O(2^n) - exponential (try all subsets)
- With memoization: O(n × val) - we compute each state exactly once
- Each state computation is O(1)
- Total states: n indices × val possible sums

SPACE COMPLEXITY: O(n × val) + O(n)
- DP table: O(n × val)
- Recursion stack depth: O(n) in worst case
- Total: O(n × val)

============================================================================
EXAMPLE TRACE
============================================================================

Input: nums = [1, 1, 2], target = 0

Step 1: Calculate sum
s = 1 + 1 + 2 = 4

Step 2: Validity check
abs(0) <= 4 ✓
(0 + 4) % 2 == 0 ✓

Step 3: Transform
val = (0 + 4) / 2 = 2

Step 4: Find subsets with sum = 2
Possible subsets:
- {1, 1} → sum = 2 ✓ → corresponds to +1+1-2 = 0
- {2} → sum = 2 ✓ → corresponds to -1-1+2 = 0

Answer: 2 ways

============================================================================
*/