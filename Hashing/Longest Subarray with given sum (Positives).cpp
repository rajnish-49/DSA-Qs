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

int longestsubarraywithsumK(const vector<int>& a, int K) {
    unordered_map<int, int> prefix_sum_map;
    int prefix_sum = 0;
    int max_length = 0;

    for (int i = 0; i < a.size(); i++) {
        prefix_sum += a[i];

        if (prefix_sum == K) {
            max_length = i + 1;
        }

        if (prefix_sum_map.find(prefix_sum - K) != prefix_sum_map.end()) {
            max_length = max(max_length, i - prefix_sum_map[prefix_sum - K]);
        }

        if (prefix_sum_map.find(prefix_sum) == prefix_sum_map.end()) {
            prefix_sum_map[prefix_sum] = i;
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