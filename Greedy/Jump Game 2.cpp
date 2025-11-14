class Solution {
public:
    int jump(vector<int>& nums) {
        int n = nums.size();

        // 'jumps' will store the minimum number of jumps needed
        int jumps = 0;

        // 'currentEnd' marks the end of the current jump's range.
        // When we reach this index, it means we have to make another jump.
        int currentEnd = 0;

        // 'farthest' keeps track of the farthest index we can reach
        // while we are still inside the current jump's range.
        int farthest = 0;

        // We stop at n-2 because:
        // - Once we are able to reach or cross the last index (n-1), we are done.
        // - No need to check further since reaching the end is guaranteed.
        for (int i = 0; i < n - 1; i++) {
            // From the current index i, we can jump up to i + nums[i].
            // Keep updating the farthest point we can reach.
            farthest = max(farthest, i + nums[i]);

            // If we have reached the end of the current jump's range:
            // - It means we have used up this jump.
            // - We must increment our jump count.
            // - And we extend 'currentEnd' to 'farthest' (the best we can reach next).
            if (i == currentEnd) {
                jumps++;
                currentEnd = farthest;
            }
        }

        // The total jumps made will be our answer.
        return jumps;
    }
};
