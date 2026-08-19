/*
Problem: 860. Lemonade Change
Link: https://leetcode.com/problems/lemonade-change/

Description:
Each lemonade costs $5.

Customers pay using bills of:
- $5
- $10
- $20

Customers come one by one, and we must give correct change
using only the bills received from previous customers.

Return true if we can give correct change to every customer,
otherwise return false.

Approach: Greedy

Keep track of how many $5 and $10 bills we currently have.

For each customer:

1. If they pay with $5:
   No change is needed.
   Store the $5 bill.

2. If they pay with $10:
   We need to return $5.
   So we must use one $5 bill.

3. If they pay with $20:
   We need to return $15.

   There are two possible ways:
       $10 + $5
       $5 + $5 + $5

   Greedy choice:
   Prefer giving $10 + $5 whenever possible.

Why?
$5 bills are more useful because they are required for giving
change to customers who pay with $10.

A $10 bill cannot replace a $5 bill in that situation.

So while giving $15 change, we try to preserve as many $5 bills
as possible.

Time Complexity: O(n)
Space Complexity: O(1)
*/

class Solution {
public:
    bool lemonadeChange(vector<int>& bills) {

        int five = 0;
        int ten = 0;

        for (int bill : bills) {

            // Customer pays exact amount.
            if (bill == 5) {
                five++;
            }

            // Need to return $5 change.
            else if (bill == 10) {

                if (five == 0) {
                    return false;
                }

                five--;
                ten++;
            }

            // Customer pays $20, so we need to return $15.
            else {

                // Prefer $10 + $5.
                // This preserves more $5 bills for future customers.
                if (ten > 0 && five > 0) {
                    ten--;
                    five--;
                }

                // Otherwise, try three $5 bills.
                else if (five >= 3) {
                    five -= 3;
                }

                // Cannot make $15 change.
                else {
                    return false;
                }
            }
        }

        return true;
    }
};