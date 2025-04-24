// @ https://leetcode.com/problems/find-first-and-last-position-of-element-in-sorted-array/description/

class Solution {
    public:
        /**
         * Finds the first (leftmost) occurrence of the target value in a sorted array.
         * Uses binary search with a modification to continue searching leftward even after
         * finding a match, to ensure we get the first occurrence.
         */
        int findfirst(vector<int>& nums, int target) {
            int l = 0;                         // Left boundary of search range
            int r = nums.size() - 1;           // Right boundary of search range
            int result = -1;                   // Default result if target is not found
            
            while (l <= r) {                   // Continue until search range is valid
                int mid = l + (r - l) / 2;     // Calculate middle index (avoids overflow)
                
                if (nums[mid] == target) {
                    result = mid;              // Store this index as a potential result
                    r = mid - 1;               // Continue searching in the left half for earlier occurrences
                } else if (nums[mid] > target) {
                    r = mid - 1;               // Target is smaller, search in left half
                } else {
                    l = mid + 1;               // Target is larger, search in right half
                }
            }
            
            return result;  // Return the leftmost index of target, or -1 if not found
        }
        
        /**
         * Finds the last (rightmost) occurrence of the target value in a sorted array.
         
         * Uses binary search with a modification to continue searching rightward even after
         * finding a match, to ensure we get the last occurrence.
         */
        int findlast(vector<int>& nums, int target) {
            int l = 0;                         // Left boundary of search range
            int r = nums.size() - 1;           // Right boundary of search range
            int result = -1;                   // Default result if target is not found
            
            while (l <= r) {                   // Continue until search range is valid
                int mid = l + (r - l) / 2;     // Calculate middle index (avoids overflow)
                
                if (nums[mid] == target) {
                    result = mid;              // Store this index as a potential result
                    l = mid + 1;               // Continue searching in the right half for later occurrences
                } else if (nums[mid] > target) {
                    r = mid - 1;               // Target is smaller, search in left half
                } else {
                    l = mid + 1;               // Target is larger, search in right half
                }
            }
            
            return result;  // Return the rightmost index of target, or -1 if not found
        }
        
        vector<int> searchRange(vector<int>& nums, int target) {
            int l = findfirst(nums, target);   // Find the first occurrence of target
            int r = findlast(nums, target);    // Find the last occurrence of target
            
            return {l, r};  // Return range as [first_occurrence, last_occurrence]
        }
    };

    // STL APPROACH 

    class Solution {
        public:
            vector<int> searchRange(vector<int>& nums, int target) {
                // Use STL lower_bound and upper_bound functions
                auto first = lower_bound(nums.begin(), nums.end(), target);  // First position ≥ target
                auto last = upper_bound(nums.begin(), nums.end(), target);   // First position > target
        
                // Check if target is actually present
                if (first == nums.end() || *first != target) {
                    return {-1, -1};  // Target not found
                }
        
                // Convert iterators to indices
                int start = first - nums.begin();         // Index of first occurrence
                int end = last - nums.begin() - 1;        // Index of last occurrence (subtract 1)
        
                return {start, end};
            }
        };
        