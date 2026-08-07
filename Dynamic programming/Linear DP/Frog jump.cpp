#include <bits/stdc++.h>
using namespace std;

class Solution
{
public:
    bool canCross(vector<int> &stones)
    {
        int n = stones.size();
        unordered_map<int, int> pos;
        for (int i = 0; i < n; i++)
        {
            pos[stones[i]] = i;
        }
        vector<unordered_set<int>> dp(n);
        if (pos.count(1) == 0)
            return false;
        dp[pos[1]].insert(1);
        if (pos[1] == n - 1)
            return true;
        for (int i = 0; i < n; i++)
        {
            for (int jump : dp[i])
            {
                for (int nextJump = jump - 1; nextJump <= jump + 1; nextJump++)
                {
                    if (nextJump <= 0)
                        continue;
                    int nextPos = stones[i] + nextJump;
                    if (pos.count(nextPos))
                    {
                        int nextIndex = pos[nextPos];
                        dp[nextIndex].insert(nextJump);
                        if (nextIndex == n - 1)
                            return true;
                    }
                }
            }
        }
        return false;
    }
};
