// @https://leetcode.com/problems/koko-eating-bananas/

class Solution {
public:
    
    // Helper function to check if Koko can eat all bananas at speed = givenHour in <= h hours
    bool canEatAll(vector<int>& piles, int givenHour, int h) {
        int actualHour = 0;  // Total hours Koko needs at this speed

        for(int &x : piles) {
            // Add the integer part: how many full chunks of `givenHour` she can eat
            actualHour += x / givenHour;

            // If some bananas are left in the pile after division,
            // she needs one more hour to finish the remainder
            if(x % givenHour != 0)
                actualHour++;
        }

        // If total hours taken is within the allowed h hours, return true
        return actualHour <= h;
    }
    
    // Main function to find the minimum eating speed k such that Koko can eat all bananas in h hours
    int minEatingSpeed(vector<int>& piles, int h) {
        int n = piles.size();

        // Minimum possible speed is 1 banana/hour
        // Maximum possible speed is the size of the largest pile
        int l = 1, r = *max_element(begin(piles), end(piles));

        // Binary search for the smallest valid speed k
        while(l < r) {
            int mid = l + (r - l) / 2;  // Midpoint speed to test

            // If Koko can eat all bananas at speed = mid within h hours,
            // try to find a smaller valid speed (search left)
            if(canEatAll(piles, mid, h)) {
                r = mid;  // Possible answer, but check smaller speeds
            } else {
                l = mid + 1;  // Speed too slow, need to go faster (search right)
            }
        }

        // When l == r, we found the smallest valid eating speed
        return l;
    }
};
