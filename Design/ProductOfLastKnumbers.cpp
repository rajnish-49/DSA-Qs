// @ https://leetcode.com/problems/product-of-the-last-k-numbers/description/

#include <vector>
using namespace std;

class ProductOfNumbers {
    vector<int> prefix; // Stores prefix products
    int lastZeroIndex = -1; // Tracks the last occurrence of zero

public:
    // Function to add a new number to the stream
    void add(int num) {
        if (num == 0) {
            // If 0 appears, reset the prefix product list
            prefix.clear();
            lastZeroIndex = prefix.size(); // Track position where reset happened
        } else {
            if (prefix.empty()) {
                // If the prefix list is empty, store the number directly
                prefix.push_back(num);
            } else {
                // Multiply the last prefix product with the new number
                prefix.push_back(prefix.back() * num);
            }
        }
    }

    // Function to get the product of the last k numbers
    int getProduct(int k) {
        int n = prefix.size();

        // If k exceeds available numbers, return 0 (meaning we encountered a zero)
        if (k > n) return 0;

        // If k is exactly the size of prefix array, return the last product
        if (k == n) return prefix.back();

        // Otherwise, compute product using division
        return prefix.back() / prefix[n - k - 1];
    }
};
