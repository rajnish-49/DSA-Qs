// @https://www.geeksforgeeks.org/problems/allocate-minimum-number-of-pages0937/1

// Problem: Allocate minimum number of pages to students such that no student reads more than a given number of pages.
// Given an array of integers representing the number of pages in each book and an integer k representing the number of students, the task is to find the minimum number of pages that can be allocated to a student such that no student reads more than that number of pages.
class Solution {
public:

    // Helper function to check if it's possible to allocate books such that
    // no student gets more than 'mid' pages
    bool possible(vector<int> arr, int mid, int k) {
        int current = 0;        // Sum of pages currently assigned to a student
        int studentcnt = 1;     // Start by assigning books to the first student

        for (auto i : arr) {
            // If adding current book doesn't exceed the limit, assign it
            if (current + i <= mid) {
                current += i;
            }
            // Otherwise, assign this book to the next student
            else {
                studentcnt++;   // Increment the student count
                current = i;    // Start new allocation from this book
            }

            // If number of students exceeds the allowed number, return false
            if (studentcnt > k) {
                return false;
            }
        }

        // All books allocated successfully under the constraint
        return true;
    }

    int findPages(vector<int> &arr, int k) {
        // Edge case: If students are more than books, allocation is not possible
        if (k > arr.size()) {
            return -1;
        }

        int ans = 0;

        // Minimum possible answer is the maximum pages in a single book
        int low = *max_element(arr.begin(), arr.end());

        // Maximum possible answer is the sum of all pages (1 student reads all books)
        int high = accumulate(arr.begin(), arr.end(), 0);

        // Binary Search on the answer (minimum possible max pages)
        while (low <= high) {
            int mid = low + (high - low) / 2;

            // Check if current mid is a valid max page limit per student
            if (possible(arr, mid, k)) {
                // If yes, store the answer and try for smaller max pages
                ans = mid;
                high = mid - 1;
            } else {
                // If not possible, try with higher page limit
                low = mid + 1;
            }
        }

        return ans;  // Minimum possible maximum pages per student
    }
};
