/*
Problem: 135. Candy
Link: https://leetcode.com/problems/candy/

Description:
There are n children standing in a line.

ratings[i] represents the rating of the ith child.

Rules:
1. Every child must get at least 1 candy.
2. If a child has a higher rating than an adjacent child,
   they must get more candies than that child.

Return the minimum total number of candies required.

Approach: Greedy + Two Passes

Why two passes?

Each child may have to satisfy a condition with:
- the child on the left
- the child on the right

A left-to-right pass can only correctly handle:

    ratings[i] > ratings[i - 1]

But it cannot properly handle decreasing sequences like:

    ratings = [3, 2, 1]

So we use two passes.

Pass 1: Left -> Right
If current rating is greater than the left neighbor,
give current child one more candy than the left neighbor.

Pass 2: Right -> Left
If current rating is greater than the right neighbor,
current child must have at least one more candy than the right neighbor.

We use max() because the first pass may already have assigned more
candies to satisfy the left-side condition, and we must preserve that.

Example:
ratings = [1, 2, 5, 4, 3, 2, 1]

After left pass:
candies = [1, 2, 3, 1, 1, 1, 1]

After right pass:
candies = [1, 2, 5, 4, 3, 2, 1]

Now both left and right neighbor conditions are satisfied.

Time Complexity: O(n)
Space Complexity: O(n)
*/

class Solution {
public:
    int candy(vector<int>& ratings) {

        int n = ratings.size();

        // Every child must receive at least one candy.
        vector<int> candies(n, 1);

        // Left -> Right:
        // Satisfy the condition with the left neighbor.
        //
        // If current child has a higher rating than the child
        // on the left, they need one more candy than them.
        for (int i = 1; i < n; i++) {

            if (ratings[i] > ratings[i - 1]) {
                candies[i] = candies[i - 1] + 1;
            }
        }

        // Right -> Left:
        // Satisfy the condition with the right neighbor.
        //
        // If current child has a higher rating than the child
        // on the right, they need at least one more candy.
        for (int i = n - 2; i >= 0; i--) {

            if (ratings[i] > ratings[i + 1]) {

                // max() preserves the requirement already established
                // from the left-to-right pass.
                candies[i] = max(candies[i],
                                 candies[i + 1] + 1);
            }
        }

        int total = 0;

        for (int c : candies) {
            total += c;
        }

        return total;
    }
};