// @https://leetcode.com/problems/candy/description/

class Solution {
public:
    int candy(vector<int>& ratings) {

        int n = ratings.size();

        // Step 1: Give each child 1 candy initially
        // This ensures that every child has at least one candy
        vector<int> candies(n, 1);

        // Step 2: Left to right pass
        // If a child has a higher rating than the one before,
        // give them more candies than the previous child
        for (int i = 1; i < n; i++) {
            if (ratings[i] > ratings[i - 1]) {
                candies[i] = candies[i - 1] + 1;
            }
        }

        // Step 3: Right to left pass
        // If a child has a higher rating than the one after,
        // and doesn't already have more candies,
        // increase their candies to one more than the right neighbor
        for (int i = n - 1; i > 0; i--) {
            if (ratings[i - 1] > ratings[i]) {
                candies[i - 1] = max(candies[i] + 1, candies[i - 1]);
            }
        }

        // Step 4: Calculate the total candies required
        int cnt = 0;
        for (auto i : candies) {
            cnt += i;
        }

        // Return the total minimum candies needed
        return cnt;
    }
};
