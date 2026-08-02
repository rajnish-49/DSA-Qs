class Solution {
public:
    vector<int> resultsArray(vector<int>& nums, int k) {
        int n = nums.size();
        vector<int> answer;

        int left = 0;

        // Counts adjacent pairs inside the current window
        // that are not increasing by exactly 1.
        int brokenPairs = 0;

        for (int right = 0; right < n; right++) {

            // When nums[right] enters the window, it creates one new
            // adjacent pair with nums[right - 1].
            if (right > 0 && nums[right] != nums[right - 1] + 1) {
                brokenPairs++;
            }

            // Keep the window size at most k.
            if (right - left + 1 > k) {

                // The pair nums[left] -> nums[left + 1] leaves the window.
                // If it was broken, remove its contribution.
                if (nums[left + 1] != nums[left] + 1) {
                    brokenPairs--;
                }

                left++;
            }

            // Process every window of exactly size k.
            if (right - left + 1 == k) {

                // No broken adjacent pair means all elements are
                // consecutive and increasing, so the power is nums[right].
                if (brokenPairs == 0) {
                    answer.push_back(nums[right]);
                } else {
                    answer.push_back(-1);
                }
            }
        }

        return answer;
    }
};