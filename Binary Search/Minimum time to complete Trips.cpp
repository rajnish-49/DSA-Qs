// @https://leetcode.com/problems/minimum-time-to-complete-trips/

class Solution {
public:

    // Helper function to check if it's possible to complete at least 'totalTrips'
    // within 'giventime' using all buses
    bool possible(vector<int>& time, long long giventime, int totalTrips) {
        long long actualtrips = 0;

        // For each bus, calculate how many trips it can complete in 'giventime'
        for (int &t : time) {
            actualtrips += giventime / t;
            // giventime / t gives number of trips current bus can do in that time
        }

        // If total trips from all buses >= required trips, it's possible
        return actualtrips >= totalTrips;
    }

    long long minimumTime(vector<int>& time, int totalTrips) {

        int n = time.size();

        // Lower bound of time (minimum possible time) is 1
        long long l = 1;

        // Upper bound:
        // If the fastest bus does all the trips, this is the maximum time required
        // Use 1LL to promote multiplication to long long and avoid overflow
        long long r = 1LL * (*min_element(time.begin(), time.end())) * totalTrips;

        // Binary Search between l and r
        while (l < r) {
            long long mid = l + (r - l) / 2;

            // If it's possible to complete totalTrips in 'mid' time,
            // try to find a smaller value (search left half)
            if (possible(time, mid, totalTrips)) {
                r = mid;
            }
            // Otherwise, search in the right half
            else {
                l = mid + 1;
            }
        }

        // At the end, l == r and it's the minimum time to complete at least totalTrips
        return l;
    }
};
