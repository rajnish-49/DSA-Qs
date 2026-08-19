/*
Problem: 406. Queue Reconstruction by Height
Link: https://leetcode.com/problems/queue-reconstruction-by-height/

Description:
Each person is represented as [height, k], where k tells us how many
people standing in front of this person must have height >= their height.

We need to rearrange all people so that this condition is satisfied
for every person.

Approach: Greedy + Sorting

1. Sort people by height in descending order.
2. If two people have the same height, sort smaller k first.
3. Process people in this order and insert each person at index k.

Why does this work?
When we process a person [h, k], all people already present in the
answer have height >= h.

Therefore, inserting this person at index k puts exactly k
taller-or-equal people before them.

Any shorter people inserted later do not affect this person's k value.

Time Complexity: O(n^2)
- Sorting takes O(n log n)
- Each insertion in vector can take O(n)

Space Complexity: O(n)
*/

class Solution {
public:
    vector<vector<int>> reconstructQueue(vector<vector<int>>& people) {

        // Taller people should come first.
        // For the same height, smaller k should come first.
        sort(people.begin(), people.end(),
             [](const vector<int>& a, const vector<int>& b) {

                 if (a[0] == b[0])
                     return a[1] < b[1];

                 return a[0] > b[0];
             });

        vector<vector<int>> ans;

        for (auto& person : people) {

            int k = person[1];

            // At this point, every person already in ans has
            // height >= current person's height.
            //
            // So putting current person at index k means there
            // will be exactly k taller-or-equal people before them.
            ans.insert(ans.begin() + k, person);
        }

        return ans;
    }
};