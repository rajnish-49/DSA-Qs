class Solution {
public:
    vector<int> countDistinct(vector<int>& nums, int k) {
        vector<int> answer;
        unordered_map<int, int> frequency;

        int left = 0;

        // Maintain a fixed-size window [left ... right] of size k.
        // The frequency map stores how many times each value appears
        // inside the current window.
        for (int right = 0; right < nums.size(); right++) {

            // Add the new element entering from the right.
            frequency[nums[right]]++;

            // If the window becomes larger than k, remove the element
            // that is leaving from the left side.
            if (right - left + 1 > k) {
                int leavingValue = nums[left];

                frequency[leavingValue]--;

                // When the frequency becomes zero, that value is no longer
                // present anywhere in the current window.
                //
                // Erasing it ensures frequency.size() represents the exact
                // number of distinct elements in the window.
                if (frequency[leavingValue] == 0) {
                    frequency.erase(leavingValue);
                }

                left++;
            }

            // Once the window contains exactly k elements, the number
            // of keys in the map equals the number of distinct values.
            if (right - left + 1 == k) {
                answer.push_back(frequency.size());
            }
        }

        return answer;
    }
};