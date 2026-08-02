// @https://leetcode.com/problems/fruit-into-baskets/

#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

class Solution {
public:
    int totalFruit(vector<int>& fruits) {
        unordered_map<int, int> frequency;

        int left = 0;
        int maxFruits = 0;

        // The current window represents a continuous group of trees
        // from which we are trying to collect every fruit.
        //
        // Since we have only two baskets and each basket can hold
        // only one fruit type, a valid window can contain at most
        // two distinct fruit types.
        for (int right = 0; right < fruits.size(); right++) {

            // Include the fruit at 'right' in the current window.
            // The map stores how many fruits of each type are currently
            // present between left and right.
            frequency[fruits[right]]++;

            // If there are more than two distinct fruit types,
            // we cannot collect every fruit from this window using
            // only two baskets.
            //
            // Shrink the window from the left until one fruit type
            // completely disappears and only two types remain.
            while (frequency.size() > 2) {
                frequency[fruits[left]]--;

                // A fruit type counts as being inside the window only
                // while its frequency is greater than zero.
                //
                // Erasing it reduces the number of distinct fruit types
                // represented by frequency.size().
                if (frequency[fruits[left]] == 0) {
                    frequency.erase(fruits[left]);
                }

                left++;
            }

            // The window [left ... right] now contains at most two
            // fruit types, so we can collect every fruit in this window.
            //
            // Its size is the number of fruits collected continuously.
            maxFruits = max(maxFruits, right - left + 1);
        }

        return maxFruits;
    }
};