// @ https://leetcode.com/problems/design-a-number-container-system/

#include <iostream>
#include <unordered_map>
#include <map>
#include <set>

using namespace std;

class NumberContainers {
private:
    // Maps an index to the number stored at that index
    unordered_map<int, int> indexMap;

    // Maps a number to a set of indices where it appears (sorted order)
    map<int, set<int>> numberMap;

public:
    // Constructor: Initializes the NumberContainers system (empty initially)
    NumberContainers() {}

    // Function to insert or update a number at a given index
    void change(int index, int number) {
        // Step 1: Check if the index is already present in indexMap
        if (indexMap.find(index) != indexMap.end()) {
            // Retrieve the old number stored at this index
            int oldNumber = indexMap[index];

            // Remove the index from the old number's set in numberMap
            numberMap[oldNumber].erase(index);

            // If the old number has no more indices left, remove it from numberMap
            if (numberMap[oldNumber].empty()) {
                numberMap.erase(oldNumber);
            }
        }

        // Step 2: Update indexMap to store the new number at the given index
        indexMap[index] = number;

        // Step 3: Insert the index into the set for the new number in numberMap
        numberMap[number].insert(index);
    }

    // Function to find the smallest index containing a given number
    int find(int number) {
        // Check if the number exists in numberMap and has at least one index
        if (numberMap.find(number) != numberMap.end() && !numberMap[number].empty()) {
            return *numberMap[number].begin(); // Get the smallest index (first element in set)
        }
        return -1; // If the number is not present, return -1
    }
};

/**
 * Example Usage:
 *
 * NumberContainers nc;
 * cout << nc.find(10) << endl;  // Output: -1 (10 is not in the system)
 * nc.change(2, 10);  
 * nc.change(1, 10);  
 * nc.change(3, 10);  
 * nc.change(5, 10);  
 * cout << nc.find(10) << endl;  // Output: 1 (smallest index with number 10)
 * nc.change(1, 20);  
 * cout << nc.find(10) << endl;  // Output: 2 (since index 1 is now 20)
 *
 * Complexity Analysis:
 * - change(): O(log N) due to insertions/deletions in ordered map and set
 * - find(): O(log N) since map lookup and set access are logarithmic
 */
