// @https://leetcode.com/problems/subarray-sum-equals-k/description/

/*
PROBLEM DESCRIPTION:
Given an array of integers nums and an integer k, return the total number of
continuous subarrays whose sum equals to k.

Example 1:
Input: nums = [1,1,1], k = 2
Output: 2
Explanation: There are 2 subarrays with sum = 2: [1,1] and [1,1]

Example 2:
Input: nums = [1,2,3], k = 3
Output: 2
Explanation: There are 2 subarrays with sum = 3: [1,2] and [3]

APPROACH - PREFIX SUM + HASH MAP:
The prefixCount map is storing how many times each prefixSum has occurred — not the index at which it occurred.

If any prefixSum - k is found in the map, it means:
We have found a subarray (from the starting point of that prefix to the current index) that sums to k.
There are prefixCount[prefixSum - k] different starting points (prefixes) that lead to the current subarray summing to k.

If prefixSum[j] - k has occurred x times,
then there are x subarrays ending at j whose sum is k.

TIME COMPLEXITY: O(n) - single pass through array
SPACE COMPLEXITY: O(n) - for hashmap storing prefix sums
*/

// https:chatgpt.com/share/6896443e-9f9c-8011-a025-e3e5dbdd9a57

        class Solution
{
public:
    int subarraySum(vector<int> &nums, int k)
    {
        unordered_map<int, int> pcount;
        pcount[0] = 1;
        // ✅ Base case: This handles subarrays that start from index 0
        // Intuition: If prefixSum - k == 0, then we found a subarray [0..i] whose sum is exactly k
        // So we assume a "prefix sum = 0" exists before we start the array

        int sum = 0;   // running prefix sum
        int count = 0; // total number of valid subarrays

        for (int num : nums)
        {
            sum += num;

            // 💡 KEY IDEA:
            // We want to find the number of previous prefix sums that were (sum - k)
            // Because if sum[j] - sum[i] = k → sum[i] = sum[j] - k
            // This means there's a subarray ending here with sum = k

            if (pcount.find(sum - k) != pcount.end())
            {
                count += pcount[sum - k];
                // 🔁 We found one or more subarrays ending at current index
                // whose sum is k → count them
            }

            // 🧠 Update map after the check:
            // We're recording that we've now seen this prefix sum once more.
            // We might find it useful for future subarrays
            pcount[sum]++;
        }

        return count;
    }
};
