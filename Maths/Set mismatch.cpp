// @https://leetcode.com/problems/set-mismatch/description/

#include <bits/stdc++.h>
using namespace std;

/*
  Math approach (O(1) extra space):
  - Compute S  = sum(nums)
  - Compute S2 = sum(nums^2)
  - Compute expected E and E2 for 1..n
  - diff = S - E = D - M
  - sqdiff = S2 - E2 = D^2 - M^2 = diff * (D + M)
  - sumDM = (D + M) = sqdiff / diff
  - D = (diff + sumDM) / 2
  - M = D - diff

  Note: we use long long to avoid overflow (sum of squares can be large).
*/
vector<int> findErrorNums_math(const vector<int>& nums) {
    int n = nums.size();

    long long S = 0;   // sum of nums
    long long S2 = 0;  // sum of nums^2

    for (int x : nums) {
        S += x;
        S2 += 1LL * x * x; // promote to long long before multiply
    }

    // expected sums for 1..n (use long long)
    long long E  = 1LL * n * (n + 1) / 2;                       // sum
    long long E2 = 1LL * n * (n + 1) * (2LL * n + 1) / 6;       // sum of squares

    long long diff = S - E;       // D - M
    long long sqdiff = S2 - E2;   // D^2 - M^2 = (D - M)(D + M)

    // Guard: in valid input diff should not be zero because D != M.
    // If something unexpected happens, fallback to frequency approach.
    if (diff == 0) {
        return findErrorNums_freq(nums); // safe fallback
    }

    // (D + M) = sqdiff / diff  (must divide evenly)
    long long sumDM = sqdiff / diff;

    // D = (diff + sumDM) / 2
    long long D = (diff + sumDM) / 2;
    long long M = D - diff;

    return { (int)D, (int)M };
}
