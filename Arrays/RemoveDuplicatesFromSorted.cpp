// @ https://leetcode.com/problems/remove-duplicates-from-sorted-array/description/

class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        // Initialize a pointer i to keep track of the position of the unique elements.
        int i = 0;

        // Start a loop from the second element (index 1) to the end of the vector.
        for (int j = 1; j < nums.size(); j++) {
            // Check if the current element is different from the last unique element.
            if (nums[j] != nums[i]) {
                // Move the pointer i to the next position for a unique element.
                i++;
                // Copy the current unique element to the position pointed by i.
                nums[i] = nums[j];
            }
        }

        // Return the number of unique elements, which is i + 1 (1-based index).
        return i + 1;
    }
};
