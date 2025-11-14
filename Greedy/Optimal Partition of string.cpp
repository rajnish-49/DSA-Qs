// @https://leetcode.com/problems/optimal-partition-of-string/description/

class Solution {
    public:
        int partitionString(string s) {
    
            set<char> st;  // Set to store unique characters in the current substring
            int count = 1; // Initialize partition count to 1 (we need at least one partition)
    
            // Traverse the string character by character
            for (int i = 0; i < s.size(); i++) {
                char current = s[i]; // Get the current character
                
                // If the character is already in the set, it means a duplicate is found
                if (st.find(current) != st.end()) {
                    st.clear();  // Clear the set to start a new partition
                    count++;      // Increase the partition count
                }
    
                // Insert the current character into the set (for tracking unique characters)
                st.insert(current);
            }
    
            return count; // Return the total number of partitions
        }
    };
    