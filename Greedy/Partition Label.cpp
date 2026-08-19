/*
Problem: 763. Partition Labels
Link: https://leetcode.com/problems/partition-labels/

Description:
You are given a string s.

We need to divide it into as many parts as possible such that
each letter appears in at most one part.

Return the size of each partition.

Example:
s = "ababcbacadefegdehijhklij"

Output:
[9,7,8]

Approach: Greedy

Key idea:
If a character appears in the current partition, then that partition
must extend at least up to the LAST occurrence of that character.

So first store the last index of every character.

Then traverse the string and maintain:

    end = farthest last occurrence of any character
          seen in the current partition

For each index i:

    end = max(end, last[s[i]])

This means the current partition must continue at least until 'end'.

When:

    i == end

all characters seen in the current partition have their last occurrence
inside this partition.

So we can safely end the partition here.

Then:
    partition size = end - start + 1

and the next partition starts from i + 1.

Why this is greedy:
We close a partition at the earliest possible position where every
character inside it is completely contained in that partition.

This gives the maximum possible number of partitions.

Time Complexity: O(n)
Space Complexity: O(1), because there are only 26 lowercase letters.
*/

class Solution {
public:
    vector<int> partitionLabels(string s) {

        // Store the last occurrence of every character.
        vector<int> last(26);

        for (int i = 0; i < s.size(); i++) {
            last[s[i] - 'a'] = i;
        }

        vector<int> ans;

        int start = 0;
        int end = 0;

        for (int i = 0; i < s.size(); i++) {

            // If the current character appears later,
            // the current partition must extend at least that far.
            end = max(end, last[s[i] - 'a']);

            // When i reaches end, every character seen from
            // start to i has its last occurrence within this range.
            //
            // So this partition can safely end here.
            if (i == end) {

                ans.push_back(end - start + 1);

                // Start the next partition after this index.
                start = i + 1;
            }
        }

        return ans;
    }
};