// @https://leetcode.com/problems/water-bottles/description/

/*
PROBLEM: Water Bottles
You have numBottles full water bottles. You can exchange numExchange empty bottles
for 1 full water bottle. Return the maximum number of water bottles you can drink.

EXAMPLE:
numBottles = 9, numExchange = 3
- Drink 9 bottles → 9 empty bottles
- Exchange 9 empty for 3 full (9/3 = 3) → Drink 3, have 3 empty
- Exchange 3 empty for 1 full (3/3 = 1) → Drink 1, have 1 empty
- Can't exchange 1 bottle (need 3)
Total drunk: 9 + 3 + 1 = 13

APPROACH: Simulation
Keep exchanging empty bottles for new ones until we don't have enough empties.
Track both the bottles we drink and the empties we accumulate.

KEY INSIGHT: After each exchange round:
- New full bottles = empty_bottles / numExchange (integer division)
- Remaining empties = (new full bottles) + (leftover empties from division)

TIME COMPLEXITY: O(log n) - bottles reduce exponentially each round
SPACE COMPLEXITY: O(1) - only using variables
*/

class Solution
{
public:
    int numWaterBottles(int numBottles, int numExchange)
    {

        int ans = 0; // Total bottles we can drink

        // 🍼 PHASE 1: Drink all initial full bottles
        ans += numBottles;

        // After drinking, we have 'numBottles' empty bottles
        // Calculate first exchange:
        int extra = numBottles / numExchange; // New full bottles from exchange
        int empty = numBottles % numExchange; // Leftover empties we can't exchange yet
        int total = extra + empty;            // Total empty bottles after drinking 'extra'

        ans += extra; // Drink the newly exchanged bottles

        // 💡 WHY THIS WORKS:
        // If we have 9 empties and numExchange=3:
        // - Exchange 9/3 = 3 full bottles (drink these immediately)
        // - Left with 9%3 = 0 empties from previous + 3 new empties from drinking
        // - Total empties = 3, ready for next round

        // 🔁 PHASE 2: Keep exchanging until we can't anymore
        // Continue while we have enough empties to make at least one exchange
        while (total >= numExchange)
        {

            // Calculate new bottles from current empties
            int extra = total / numExchange; // New full bottles we get
            ans += extra;                    // Drink them immediately

            // 🧠 CRITICAL UPDATE: Calculate new total empties
            // = (empties from drinking 'extra' new bottles) + (leftover empties we couldn't exchange)
            total = extra + (total % numExchange);

            // EXAMPLE: total=5, numExchange=3
            // extra = 5/3 = 1 (get 1 new bottle)
            // After drinking: 1 empty + 2 leftover = 3 total empties
            // Next round: can exchange these 3 again!
        }

        // Loop ends when total < numExchange (not enough empties to exchange)

        return ans;
    }
};
