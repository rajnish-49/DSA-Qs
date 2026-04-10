/*
    PROBLEM:
    We may increase any nums[i] any number of times.
    An index i is special if nums[i] > nums[i-1] and nums[i] > nums[i+1].

    Goal:
    1. Maximize the number of special indices.
    2. Among all ways to achieve that maximum, minimize total increments.

    ----------------------------------------------------------------------
    FIRST PRINCIPLES / OBSERVATIONS
    ----------------------------------------------------------------------

    1. Only interior indices can ever become special.
       So the only candidate positions are 1 ... n-2.

    2. Two adjacent positions can never both be special.
       If i is special then nums[i] > nums[i+1].
       If i+1 is special then nums[i+1] > nums[i].
       Both cannot hold together.

       So among candidate positions, we want to choose as many
       non-adjacent indices as possible.

    3. Cost of forcing one index i to become special:
       nums[i] must become strictly greater than both neighbors,
       so the cheapest target is:

           max(nums[i-1], nums[i+1]) + 1

       Hence:

           cost[i] = max(0, max(nums[i-1], nums[i+1]) + 1 - nums[i])

    4. To maximize the number of chosen non-adjacent positions,
       the pattern must be as dense as possible, i.e. alternating.

       - If n is odd, then the candidate count (n-2) is odd.
         There is only one maximum pattern:
             1, 3, 5, ...

       - If n is even, then the candidate count (n-2) is even.
         Maximum-count solutions are obtained by starting with the odd pattern
         and switching once to the even pattern, or vice versa.
         So instead of exploring arbitrary subsets, we only need to try
         these alternating maximum layouts.

    Approach 1:
    Build the answer explicitly as
        odd-pattern prefix + even-pattern suffix

    Approach 2:
    Same idea, but compressed into a rolling "parity shift" transition.
*/

// Approach 1: Explicit prefix/suffix style
class Solution {
public:
    using ll = long long;

    long long minIncrease(vector<int>& nums) {
        int n = nums.size();
        vector<ll> cost(n, 0);

        // Precompute the independent cost of making each interior index a peak.
        // This turns the original array problem into:
        // "Choose a maximum number of non-adjacent candidate positions
        //  with minimum total chosen cost."
        for (int i = 1; i <= n - 2; i++) {
            cost[i] = max(0LL, 1LL * max(nums[i - 1], nums[i + 1]) + 1 - nums[i]);
        }

        // Odd n -> candidate count (n-2) is odd.
        // There is only one densest alternating layout:
        // 1,3,5,...
        // Since this is the only way to achieve the maximum number of peaks,
        // we do not need to compare anything else.
        if (n % 2 == 1) {
            ll ans = 0;
            for (int i = 1; i <= n - 2; i += 2) {
                ans += cost[i];
            }
            return ans;
        }

        // Even n is the interesting case.
        // Now multiple maximum-count alternating layouts are possible.
        //
        // Every optimal layout can be viewed as:
        //   take odd positions on the left,
        //   then switch once,
        //   then take even positions on the right.
        //
        // So we precompute:
        //   prefix[k] = cost of taking all odd indices up to k
        //   suffix[k] = cost of taking all even indices from k to n-2
        //
        // Then every valid maximum-count arrangement is obtained by choosing
        // a split point and combining:
        //   odd-prefix + even-suffix
        vector<ll> prefix(n, 0), suffix(n + 2, 0);

        for (int i = 1; i <= n - 2; i += 2) {
            prefix[i] = cost[i] + (i >= 2 ? prefix[i - 2] : 0);
        }

        for (int i = n - 2; i >= 2; i -= 2) {
            suffix[i] = cost[i] + ((i + 2 <= n - 2) ? suffix[i + 2] : 0);
        }

        // Split before the first odd position => pure even pattern.
        ll ans = suffix[2];

        // Try every split:
        // - left side follows the odd pattern up to i
        // - right side follows the even pattern starting from i+3
        //
        // Why i+3?
        // If we end the odd side at i, then i+1 is the "gap" index.
        // The next valid even-parity candidate on the right is i+3.
        //
        // This loop is checking all maximum-count alternating layouts.
        for (int i = 1; i <= n - 2; i += 2) {
            ll left = prefix[i];
            ll right = (i + 3 <= n - 2 ? suffix[i + 3] : 0);
            ans = min(ans, left + right);
        }

        return ans;
    }
};



// Approach 2: Rolling parity-shift version
class Solution {
public:
    using ll = long long;

    long long minIncrease(vector<int>& nums) {
        int n = nums.size();
        vector<ll> cost(n, 0);

        // Same preprocessing as Approach 1:
        // convert each candidate position into an independent "cost to choose".
        for (int i = 1; i <= n - 2; i++) {
            cost[i] = max(0LL, 1LL * max(nums[i - 1], nums[i + 1]) + 1 - nums[i]);
        }

        // Odd n -> only one densest alternating layout exists.
        if (n % 2 == 1) {
            ll ans = 0;
            for (int i = 1; i <= n - 2; i += 2) {
                ans += cost[i];
            }
            return ans;
        }

        // For even n, instead of storing explicit prefix/suffix arrays,
        // we simulate the same idea with one running sum.
        //
        // Start from the pure odd pattern:
        // 1,3,5,...
        //
        // Then, from right to left, gradually replace odd positions by their
        // neighboring even positions:
        //
        //   1,3,5,...
        //   1,3,...,even on the far right
        //   1,...,even,even on the right
        //   ...
        //   2,4,6,...
        //
        // Each replacement keeps the total number of chosen peaks maximum,
        // while exploring a different valid alternating layout.
        ll cur = 0;
        for (int i = 1; i <= n - 2; i += 2) {
            cur += cost[i];
        }

        ll ans = cur;

        // Replace the pair (i-1) -> i from right to left.
        // This is the rolling version of trying every split point.
        for (int i = n - 2; i >= 2; i -= 2) {
            cur += cost[i];      // include even index
            cur -= cost[i - 1];  // remove previous odd index
            ans = min(ans, cur);
        }

        return ans;
    }
};
