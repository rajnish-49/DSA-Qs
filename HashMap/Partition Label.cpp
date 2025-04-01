class Solution {
    public:
        vector<int> partitionLabels(string s) {
    
            // Create an unordered_map to store the last occurrence index of each character
            unordered_map<char, int> mp;
    
            // Traverse the string and update the last occurrence of each character
            for (int i = 0; i < s.size(); i++) {
                mp[s[i]] = i;  // Store the last index of character s[i]
            }
    
            int start = 0;  // Start index of the current partition
            int end = 0;    // End index of the current partition
            vector<int> ans; // Vector to store the sizes of partitions
    
            // Traverse the string again to determine partitions
            for (int i = 0; i < s.size(); i++) {
                end = max(end, mp[s[i]]); // Update the end of the partition
    
                // If the current index reaches the end of a partition
                if (i == end) {
                    ans.push_back(end - start + 1); // Store the partition size
                    start = end + 1; // Move to the next partition
                }
            }
    
            return ans; // Return the list of partition sizes
        }
    };
    