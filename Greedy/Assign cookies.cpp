// @https://leetcode.com/problems/determine-if-two-events-have-conflict/description/

class Solution {
public:
    int findContentChildren(vector<int>& g, vector<int>& s) {
        // g = greed factors of children
        // s = sizes of cookies

        // STEP 1: Sort both arrays
        // Sorting ensures we try to satisfy children with the smallest greed first
        // and use the smallest available cookie that can satisfy them.
        sort(g.begin(), g.end());
        sort(s.begin(), s.end());

        int child = 0;   // pointer for children
        int cookie = 0;  // pointer for cookies

        // STEP 2: Iterate while we still have children & cookies
        while (child < g.size() && cookie < s.size()) {
            // If current cookie can satisfy current child's greed
            if (s[cookie] >= g[child]) {
                child++;  // child is satisfied → move to next child
            }
            // In any case, move to next cookie (used or too small)
            cookie++;
        }

        // STEP 3: 'child' now represents how many children got satisfied
        return child;
    }
};
