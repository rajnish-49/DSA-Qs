/*
 * PROBLEM: Count Binary Substrings
 *
 * We need to count substrings where:
 *   1. Equal number of 0s and 1s
 *   2. All 0s are grouped together AND all 1s are grouped together
 *
 * Valid forms are ONLY: "0011", "01", "0001", "000111", "1100", "10", etc.
 * i.e., a block of one char followed by a block of the other char.
 *
 * KEY INSIGHT — Think in "runs":
 * Any valid substring is formed by taking some suffix of one run
 * and some prefix of the NEXT run. For example:
 *
 *   s = "0 0 0 1 1"
 *         [run=3] [run=2]
 *
 * Valid substrings here: "0011" (2 of each), "01" (1 of each)
 * Count = min(3, 2) = 2
 *
 * Why min? Because we can pair at most min(left_run, right_run) characters.
 * If left run has 3 zeros and right run has 2 ones, we can form
 * substrings of length 2 (one pair each): "0011" and "01".
 * We can't form "000111" because there aren't 3 ones available.
 *
 * CORE ALGORITHM:
 * As we scan left to right, we track:
 *   - curr: length of the current run (consecutive same characters)
 *   - prev: length of the run just before current
 *
 * Every time a run ends (character changes), we know we have
 * a (prev, curr) adjacent run pair — and min(prev, curr) new valid
 * substrings are formed between them.
 *
 * Example walkthrough for "00110011":
 *   Runs: [00][11][00][11]  →  lengths: 2, 2, 2, 2
 *   Between run1 & run2: min(2,2) = 2  → "0011", "01"
 *   Between run2 & run3: min(2,2) = 2  → "1100", "10"
 *   Between run3 & run4: min(2,2) = 2  → "0011", "01"
 *   Total = 6 ✓
 */

int countBinarySubstrings(string s) {
    int res = 0;
    int prev = 0; // length of the run BEFORE the current one
                  // starts at 0 because there's no previous run at the start
    int curr = 1; // length of the current run; starts at 1 (the first character)

    for (int i = 1; i < s.length(); i++) {
        if (s[i] == s[i - 1]) {
            // Still in the same run, just extend it
            curr++;
        } else {
            // Current run has ended; a new run begins with s[i]
            // NOW is the moment to count: we have a complete (prev, curr) pair
            // min(prev, curr) = number of valid substrings spanning these two runs
            res += min(prev, curr);

            // The run that just ended becomes "prev" for the next boundary
            prev = curr;
            // Start fresh run of length 1 for s[i]
            curr = 1;
        }
    }

    // After the loop, we've processed the last run but never hit an "else"
    // to count it against its predecessor. Handle that final boundary here.
    // This accounts for the boundary between the second-to-last and last run.
    return res + min(prev, curr);
}