// BRUTE FORCE - GENERATE ALL THE SUBARRAYS 

/*Outer loop: i goes from 0 to n-1 → start index of subarray.
Middle loop: j goes from i to n-1 → end index of subarray.
Innermost loop: k goes from i to j → sums elements from a[i] to a[j].
*/

for (int i = 0; i < n; i++) {
    for (int j = i; j < n; j++) {
        int s = 0;
        for (int k = i; k <= j; k++) {
            s += a[k];
        }
        if (s == K) len = max(len, j - i + 1);
    }
}
/*Goal: For each subarray, if its sum equals K, update len to the maximum length of such a subarray.
Final result: len stores the length of the longest subarray with sum equal to K.
*/

/* BETTER APPROACH - HASHING

Keep track of the cumulative (prefix) sum as we iterate through the array.
Store the first index where each prefix sum occurs in a hashmap.
For each index i, check if prefix_sum - K has appeared before.

If yes, it means the subarray between that previous index + 1 and i sums to K.

✅ Why it Works
Let prefix_sum at index i be x.
If there is a previous index j where prefix_sum = x - K, then:

sum of elements between (j+1) and i = prefix_sum[i] - prefix_sum[j] = K
This allows you to find the subarray in O(1) time for each element.
*/

https://chatgpt.com/share/6896443e-9f9c-8011-a025-e3e5dbdd9a57

int longestSubarrayWithSumK(const vector<int>& a, int K) {
    // HashMap: prefix_sum → first_index_where_this_sum_occurred
    unordered_map<int, int> prefix_sum_map;
    
    int prefix_sum = 0;    // Running cumulative sum from index 0 to current index
    int max_length = 0;    // Length of longest subarray found so far

    for (int i = 0; i < a.size(); i++) {
        // Update running prefix sum
        prefix_sum += a[i];

        // 🎯 CASE 1: Check if entire prefix from start has sum K
        // This handles subarrays that start from index 0
        if (prefix_sum == K) {
            max_length = i + 1;  // Length = current_index + 1
            // Example: [2,3], K=5 → at i=1, prefix_sum=5, length=2
        }

        // 🎯 CASE 2: Check if removing some prefix gives us sum K
        // We need: current_prefix_sum - some_previous_prefix_sum = K
        // Rearranging: some_previous_prefix_sum = current_prefix_sum - K
        if (prefix_sum_map.find(prefix_sum - K) != prefix_sum_map.end()) {
            // Found a previous prefix sum that, when removed, leaves us with sum K
            int previous_index = prefix_sum_map[prefix_sum - K];
            int current_subarray_length = i - previous_index;
            
            max_length = max(max_length, current_subarray_length);
            
            // 💡 Why this works:
            // prefix_sum[i] - prefix_sum[previous_index] = K
            // So subarray from (previous_index + 1) to i has sum K
            // Length = i - previous_index
        }

        // 🔑 CRITICAL: Store only the FIRST occurrence of each prefix sum
        // This ensures maximum subarray length for future calculations
        if (prefix_sum_map.find(prefix_sum) == prefix_sum_map.end()) {
            prefix_sum_map[prefix_sum] = i;
            
            // ⚠️ WHY NOT UPDATE? Consider this example:
            // Array: [1, 2, 1, 2], K = 3
            // At i=1: prefix_sum=3, store map[3]=1
            // At i=3: prefix_sum=6, look for 6-3=3, found at index 1
            // Subarray [1,2] from index 2-3 has length 2
            // 
            // If we had updated map[3]=3 instead of keeping map[3]=1,
            // we would lose this longer subarray opportunity!
        }
    }

    return max_length;
}

/*
Key Insight
When you calculate prefix_sum at index i:

You need to know if (prefix_sum - K) occurred before.

This lets you find a subarray between the first occurrence of (prefix_sum - K) + 1 and the current index i with sum K.

The earliest occurrence gives the longest possible subarray ending at index i, because:

A prefix sum earlier in the array means the subarray spanning from that point will be longer.

⚠️ Why Not Update Existing Prefix Sum Index?
DO NOT replace existing prefix sum indices in the hashmap.

If you replace them with a later occurrence, you miss longer subarrays starting from the first time that sum occurred.

✅ Always store only the first time a prefix sum appears:

cpp
*/