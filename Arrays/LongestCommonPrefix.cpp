string longestCommonPrefix(vector<string>& strs) {
    if (strs.empty()) return "";  // If the array is empty, return ""

    // Start by assuming the first string is the longest common prefix
    string prefix = strs[0];

    // Compare with every other string
    for (int i = 1; i < strs.size(); ++i) {
        int j = 0;
        
        // Compare the prefix with the current string
        while (j < prefix.size() && j < strs[i].size() && prefix[j] == strs[i][j]) {
            ++j;  // Keep comparing characters as long as they match
        }
        
        // Update the prefix to the part that matched
        prefix = prefix.substr(0, j);

        // If at any point, prefix becomes empty, no common prefix exists
        if (prefix.empty()) return "";
    }

    return prefix;
}