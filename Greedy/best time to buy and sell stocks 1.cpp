class Solution {
public:
    int maxProfit(vector<int>& prices) {

        // Track the lowest price seen so far (best day to buy)
        int minprice = prices[0];

        // Track the maximum profit found so far
        int maxi = 0;

        // Start from day 1 (day 0 is already set as initial minprice)
        for (int i = 1; i < prices.size(); i++) {

            if (prices[i] < minprice) {
                // Found a cheaper price → update the best buy day
                minprice = prices[i];
            }
            else {
                // Current price is higher than minprice → potential sell day
                // Calculate profit if we sold today
                int current = prices[i] - minprice;

                // Update max profit if today's profit beats the best so far
                if (current > maxi) {
                    maxi = current;
                }
            }
        }

        // Return best profit found (0 if no profitable transaction exists)
        return maxi;
    }
};