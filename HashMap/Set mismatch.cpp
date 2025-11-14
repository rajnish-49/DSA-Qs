// @https://leetcode.com/problems/set-mismatch/description/

#include <bits/stdc++.h>
using namespace std;

/*
  Frequency-array approach:
  - Build a frequency array for numbers 1..n
  - The value with freq == 2 is the duplicate
  - The value with freq == 0 is the missing number
*/
vector<int> findErrorNums_freq(const vector<int>& nums) {
    int n = nums.size();
    // freq[0] unused, indices 1..n used
    vector<int> freq(n + 1, 0);

    // Count occurrences
    for (int x : nums) {
        // x is guaranteed in [1..n] per problem statement
        freq[x]++;
    }

    int dup = -1, miss = -1;
    // Scan 1..n to find duplicate and missing
    for (int i = 1; i <= n; ++i) {
        if (freq[i] == 2) dup = i;    // occurred twice -> duplicate
        else if (freq[i] == 0) miss = i; // not present -> missing
    }

    return {dup, miss};
}
