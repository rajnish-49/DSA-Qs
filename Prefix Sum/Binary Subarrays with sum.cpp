//@https://leetcode.com/problems/binary-subarrays-with-sum/description/

/*Writing code to find exactly k ones in a subarray is tricky.
It requires checking every subarray, counting how many 1s it has → very slow (O(n²)).

✅ The Trick to Simplify:
Count how many subarrays have at most k ones, and
Count how many subarrays have at most k-1 ones

Then subtract:

exactly(k) = atMost(k) - atMost(k - 1)

You are starting with a big group (at most 2 ones),
and removing the ones with fewer than 2 (at most 1 one),
leaving behind only the exact 2 ones group.

This is why it works:

[All with ≤ 2 ones] - [All with ≤ 1 ones] = [Exactly 2 ones]
*/

