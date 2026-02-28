class Solution {
public:
    int numSteps(string s) {

        /*
        ═══════════════════════════════════════════════════════
        UNDERSTANDING THE PROBLEM IN BINARY FIRST
        ═══════════════════════════════════════════════════════

        Two operations:
            - Number is EVEN (last bit is 0) → divide by 2
            - Number is ODD  (last bit is 1) → add 1

        In binary these operations look like this:
            - Divide by 2  = right shift = just DROP the last bit
                e.g.  1100 → 110 → 11 → ...
            - Add 1 to odd = flip the trailing 1 to 0 + carry propagates left
                e.g.  0111 + 1 = 1000

        So the game is really: keep removing the rightmost bit until only
        the leading 1 remains.

        ═══════════════════════════════════════════════════════
        COST OF EACH BIT (the fundamental observation)
        ═══════════════════════════════════════════════════════

        Think about what it takes to "get rid of" each bit position
        scanning from right to left:

        If the bit is 0:
            → It's even, just divide by 2 (drop the bit).
            → COST = 1 step.

        If the bit is 1:
            → It's odd, we must add 1 first (cost 1 step),
              which flips this bit to 0 and sends a carry left.
            → Now it's even, divide by 2 to drop it (cost 1 step).
            → COST = 2 steps. AND it generates a carry into the next bit.

        ═══════════════════════════════════════════════════════
        THE CARRY — why it matters and what it does
        ═══════════════════════════════════════════════════════

        When we add 1 to a "1" bit, it generates a carry that ripples
        leftward, just like normal binary addition.

        That carry will HIT the next bit to the left. Two cases:

        CASE A — carry hits a "0" bit:
            0 + carry(1) = 1   → the bit effectively becomes 1
            Now we have a "1" bit again → costs 2 steps + carry continues.

                Concretely: "10" with carry coming in
                effective bit = 0 + 1 = 1 (odd)
                → add 1: costs 1 step, carry goes further left
                → divide by 2: costs 1 step
                Total: 2 steps, carry still alive.

        CASE B — carry hits a "1" bit:
            1 + carry(1) = 10  → this bit becomes 0, carry continues left
            The bit is now effectively 0 → costs only 1 step (just divide).

                Concretely: "11" with carry coming in
                effective bit = 1 + 1 = 2 (even, write 0 carry 1)
                → divide by 2: costs 1 step
                Total: 1 step, carry still alive.

        Notice something beautiful here:
            A run of 1s like "0111":
            - rightmost 1: 2 steps, carry generated
            - middle  1: carry+1=2, just 1 step, carry continues
            - leftmost 1: carry+1=2, just 1 step, carry continues
            - the 0 at front gets carry → becomes 1 → 2 steps
            This perfectly mirrors what actually happens:
            0111 + 1 = 1000 (all three 1s collapse, carry pops out the top)

        ═══════════════════════════════════════════════════════
        WHY WE DON'T PROCESS s[0] IN THE LOOP
        ═══════════════════════════════════════════════════════

        s[0] is guaranteed to be '1' (the leading bit).
        We want to REACH this 1, not eliminate it — it's our target.

        However, if a carry is still alive when we finish processing s[1],
        it means that carry will turn the leading "1" into "10" (binary for 2),
        and we need one more divide-by-2 step to go from 2 → 1.
        That's why we do `return ans + carry` at the end.

        If carry is 0, the leading bit is just 1, we're already done.

        ═══════════════════════════════════════════════════════
        */

        int n = s.length();
        int ans = 0;
        int carry = 0;

        // Process every bit except the leading bit s[0], right to left.
        for (int i = n - 1; i >= 1; i--) {

            // The "effective" bit is the actual bit PLUS any carry
            // that has rippled in from the right due to previous add-1 operations.
            int cbit = (s[i] - '0') + carry;

            if (cbit % 2 == 0) {
                // Effective bit is EVEN (0 or 2).
                //
                // cbit == 0: actual bit was 0, no carry. Just divide by 2. 1 step.
                //            carry stays 0.
                //
                // cbit == 2: actual bit was 1, carry was 1. 1+1=2 in binary is "10",
                //            meaning this position becomes 0 and carry CONTINUES left.
                //            The effective bit here is 0 → just divide. 1 step.
                //            carry remains 1 (it passed through, we don't reset it).
                //
                // In BOTH sub-cases the cost is 1 step and carry is unchanged.
                ans += 1;

            } else {
                // Effective bit is ODD (1).
                //
                // cbit == 1: either actual bit=1 carry=0, OR actual bit=0 carry=1.
                //            Both mean the current effective value is odd.
                //            We must add 1 → flips to 0, generates carry left. (1 step)
                //            Then divide by 2 → drops the 0.             (1 step)
                //            Total: 2 steps. And carry is now 1 going left.
                //
                // Why does carry become 1 here regardless of what it was before?
                // Because: the only way cbit is odd is if exactly one of
                // {actual bit, incoming carry} is 1. In either sub-case,
                // adding 1 to this odd effective bit produces a carry of 1.
                ans += 2;
                carry = 1;
            }
        }

        // If carry == 1 after the loop: the leading "1" absorbs the carry,
        // turning it into "10" (binary 2). We need 1 more step (divide by 2)
        // to reach 1. If carry == 0: leading bit is already 1, we're done.
        return ans + carry;
    }
};