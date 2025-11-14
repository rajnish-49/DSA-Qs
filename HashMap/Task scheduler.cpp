#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {
        // Imagine each task as a letter with some frequency.
        // The key challenge: identical tasks (say, multiple 'A's)
        // must be spaced at least 'n' intervals apart.
        // Our goal: find the shortest possible total time (including idle gaps).

        // Step 1: Count frequency of each task (A-Z)
        // Why? Because the task that appears the most
        // will "stretch" the schedule — it determines
        // the minimum possible time due to cooldown requirements.
        vector<int> freq(26, 0);
        for (char c : tasks)
            freq[c - 'A']++;

        // Step 2: Find the maximum frequency (M)
        // This represents the "bottleneck" task — the one repeated the most.
        // Example: If A appears 6 times, no matter what else we do,
        // there must be enough time to fit all 6 As with n gaps between them.
        int M = *max_element(freq.begin(), freq.end());

        // Step 3: Find how many tasks have that same max frequency (k)
        // This matters because if multiple tasks are equally frequent,
        // they will all occupy the final "row" of our conceptual grid.
        int k = 0;
        for (int f : freq)
            if (f == M)
                k++;

        // Step 4: Think of the schedule as divided into "blocks" or "chunks"
        // Each block represents (n + 1) intervals:
        //     one slot for a high-frequency task + n cooldown spaces.
        // There are (M - 1) such full blocks before the final appearance
        // of the most frequent tasks.
        //
        // Example: If M=3, n=2 → we have:
        //   A _ _ | A _ _ | A
        // That’s (M - 1) = 2 full chunks, each of length (n + 1) = 3,
        // plus the last row where the final A(s) go.
        int part1 = (M - 1) * (n + 1) + k;

        // Step 5: Compare with total tasks.
        // Why? Because if there are *enough other tasks*,
        // they can fill the cooldown slots (the underscores above),
        // meaning no idles are required.
        //
        // So, the schedule length cannot be *shorter* than total tasks,
        // but it might be *longer* if idle gaps are forced by cooldowns.
        // Taking the maximum of both covers both cases.
        return max((int)tasks.size(), part1);
    }
};
