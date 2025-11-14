/*
PROBLEM: Partition Labels
Partition a string into as many parts as possible so that each letter appears
in at most one part. Return the sizes of these parts.

GREEDY STRATEGY:
For each partition, we must include ALL occurrences of every character in it.
The key insight: Once we start a partition with character 'x', we MUST extend
it to include the LAST occurrence of 'x'. But that might introduce new characters,
whose last occurrences we must also include. This creates a chain effect.

APPROACH:
1. Find the last occurrence of each character (preprocessing)
2. Scan through string, extending partition boundary as needed
3. When current position reaches partition boundary, we can safely cut

*/

class Solution
{
public:
    vector<int> partitionLabels(string s)
    {

        // 📍 PHASE 1: Preprocessing - Map each character to its LAST occurrence
        // WHY? We need to know how far we MUST extend each partition
        unordered_map<char, int> lastOccurrence;

        for (int i = 0; i < s.size(); i++)
        {
            lastOccurrence[s[i]] = i;
            // Overwrites previous index, keeping only the LAST occurrence
            // This is the farthest point we need to reach if this char is in partition
        }

        // 🎯 PHASE 2: Build partitions using greedy approach
        int start = 0;   // Start index of current partition
        int end = 0;     // End boundary of current partition (farthest we must go)
        vector<int> ans; // Store sizes of each partition

        // Scan through string, dynamically extending partition as needed
        for (int i = 0; i < s.size(); i++)
        {
            // 🔑 KEY INSIGHT: Update partition boundary
            // Current character s[i] last appears at lastOccurrence[s[i]]
            // Our partition MUST extend at least that far to include all occurrences
            end = max(end, lastOccurrence[s[i]]);

            // 💡 WHY MAX?
            // We might have already committed to going further due to previous characters
            // Example: If partition contains 'a' (last at 8) and 'b' (last at 5)
            // We must go to index 8, not stop at 5

            // ✂️ CUT POINT: Can we partition here?
            if (i == end)
            {
                // We've reached the farthest point any character in this partition needs
                // All characters [start...end] are now "contained" - safe to cut!
                ans.push_back(end - start + 1); // Record partition size
                start = end + 1;                // Next partition starts right after current

                // 🧠 WHY THIS WORKS:
                // - No character in [start...end] appears after 'end'
                // - No character after 'end' appears in [start...end]
                // Perfect partition achieved!
            }
            // If i < end, we haven't finished current partition yet, keep scanning
        }

        return ans;
    }
};


