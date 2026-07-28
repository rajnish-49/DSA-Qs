/*
 * =========================================================================
 * PROBLEM: Frequency Balance Subarray
 * =========================================================================
 *
 * You are given an array. Find the longest subarray that is "frequency balanced".
 *
 * WHAT IS FREQUENCY BALANCED:
 * Count how many times each distinct value appears in the subarray.
 * Those counts must either:
 *   - All be the same number, OR
 *   - Be exactly two numbers where one is double the other (f and 2f)
 *
 * Example: [1,2,2,1,2,3,3,3]
 *   subarray [2,1,2,3,3] → 1 appears once, 2 appears twice, 3 appears twice
 *   counts are {1, 2, 2} → distinct counts are {1, 2} → 2 == 2*1 → valid
 *
 * NOTE: The problem statement is confusingly written. It says "both f and 2f
 * must occur among distinct values" which implies all-same-frequency is invalid.
 * But then example 2 ([5,5,5,5]) is valid. They handle this with a special case
 * "if only one distinct value exists it is always valid."
 * A cleaner way to think about it: collect all frequencies into a SET.
 * Valid if |set| == 1 OR (|set| == 2 AND max == 2 * min).
 *
 * =========================================================================
 * THINKING FROM FIRST PRINCIPLES
 * =========================================================================
 *
 * STEP 1 — UNDERSTAND WHAT MAKES A SUBARRAY VALID
 *
 * Take any subarray. Count frequencies of each distinct value.
 * Example: [1,1,2,2,2,3] → freq(1)=2, freq(2)=3, freq(3)=1
 * Now look at just those frequency VALUES as a set: {2, 3, 1}
 * Three distinct frequencies → can we write them as f and 2f? No.
 * Invalid.
 *
 * Example: [1,1,2,2,3,3] → freq(1)=2, freq(2)=2, freq(3)=2
 * Frequency set: {2} → only one distinct frequency → valid.
 *
 * Example: [1,2,2,3,3] → freq(1)=1, freq(2)=2, freq(3)=2
 * Frequency set: {1, 2} → two distinct frequencies, 2 == 2*1 → valid.
 *
 * KEY OBSERVATION:
 * You don't care about WHICH value has which frequency.
 * You only care about the SET of distinct frequency values.
 * Valid iff |freqSet| == 1, OR |freqSet| == 2 AND max == 2 * min.
 * If |freqSet| >= 3, always invalid.
 *
 * STEP 2 — BRUTE FORCE IDEA
 *
 * Try every subarray [i..j].
 * For each, build a frequency map.
 * Extract distinct frequency values into a set.
 * Check the condition.
 * Track the longest valid subarray.
 *
 * With n=1000, there are O(n^2) = 10^6 subarrays.
 * For each subarray, building the freq map from scratch is O(n).
 * Total: O(n^3) → too slow conceptually, but n=1000 makes even O(n^2) fine.
 *
 * STEP 3 — MAKE IT O(n^2) BY FIXING LEFT BOUNDARY
 *
 * Fix i (left boundary). Extend j from i to n-1.
 * Instead of rebuilding the frequency map from scratch for each j,
 * MAINTAIN it incrementally — just add nums[j] at each step.
 * This makes the inner work O(1) per j instead of O(j-i).
 *
 * But we still need to check the freqSet condition at each j.
 * Extracting distinct frequencies from the map is O(k) where k = distinct values.
 * Can we do better?
 *
 * STEP 4 — THE KEY IDEA: FREQUENCY OF FREQUENCIES
 *
 * Instead of rebuilding the freqSet every time, maintain it incrementally too.
 *
 * Introduce a second map: mpp1
 * mpp1[f] = how many distinct values currently have frequency f
 *
 * Example: mpp = {1:2, 2:2, 3:1}
 * mpp1 = {2:2, 1:1}  ← two values have freq 2, one value has freq 1
 *
 * Now:
 * mpp1.size() directly gives |freqSet| in O(1).
 * When mpp1.size()==2, we just iterate 2 entries to get both frequencies.
 *
 * HOW DO WE MAINTAIN mpp1 INCREMENTALLY?
 * When nums[j] = val is added to the subarray:
 *   - val's frequency increases from f to f+1
 *   - In mpp1: one fewer value has frequency f → mpp1[f]--
 *   - In mpp1: one more value has frequency f+1 → mpp1[f+1]++
 *   - If mpp1[f] hits 0, erase it (otherwise mpp1.size() is wrong)
 *
 * This is O(1) per step. So the full algorithm is O(n^2) with O(1) per (i,j).
 *
 * =========================================================================
 * WHY CORRECTNESS HOLDS
 * =========================================================================
 *
 * mpp always reflects exact frequencies of values in [i..j] because:
 *   - We reset it for every new i
 *   - We only ever add elements (extend j rightward)
 *   - Each addition updates exactly one value's frequency
 *
 * mpp1 always mirrors mpp because:
 *   - Every time a value's frequency changes in mpp (f → f+1),
 *     we immediately reflect that in mpp1 (remove from f bucket, add to f+1 bucket)
 *   - Erasing zero-count entries keeps mpp1.size() truthful
 *
 * The validity check is correct because:
 *   - mpp1.size()==1 means all values share one frequency → valid
 *   - mpp1.size()==2 with max==2*min means exactly f and 2f exist → valid
 *   - mpp1.size()>=3 means three distinct frequencies → impossible to satisfy → skip
 *
 * =========================================================================
 */

class Solution {
public:
    int getLength(vector<int>& nums) {
        int n = nums.size();
        int ans = 0;

        // Fix the left boundary i.
        // Every subarray [i..j] for j>=i is explored by extending j rightward.
        // We reset both maps here because subarrays starting at different i
        // are completely independent — frequencies from a previous i must not
        // carry over.
        for (int i = 0; i < n; i++) {

            // mpp[val] = how many times val appears in current subarray [i..j]
            unordered_map<int, int> mpp;

            // mpp1[f] = how many distinct values currently have frequency f
            // This is the "frequency of frequencies" map.
            // Its SIZE tells us how many distinct frequency values exist in the subarray,
            // which is exactly what we need to check validity in O(1).
            unordered_map<int, int> mpp1;

            for (int j = i; j < n; j++) {
                int val = nums[j];

                // BEFORE updating val's frequency in mpp, we must remove val
                // from its CURRENT frequency bucket in mpp1.
                // Why? Because val is about to move from freq f to freq f+1.
                // mpp1 must reflect this move. If we don't remove it first,
                // mpp1 will double-count val (once under f, once under f+1),
                // making mpp1.size() wrong and breaking the validity check.
                // We only do this if val has appeared before (freq > 0).
                // If val is new (freq == 0), there's nothing to remove.
                if (mpp[val] > 0) {
                    int freq = mpp[val];    // val's current frequency before increment
                    mpp1[freq]--;           // val is leaving this frequency bucket
                    // If no value has frequency `freq` anymore, erase the entry.
                    // This is critical: if we leave mpp1[freq]=0 without erasing,
                    // mpp1.size() counts it as an existing frequency, making
                    // |freqSet| appear larger than it actually is.
                    // Example: if we don't erase, mpp1={1:0, 2:2} has size 2
                    // but the real freqSet is just {2}, which should give size 1.
                    if (mpp1[freq] == 0) mpp1.erase(freq);
                }

                // Now increment val's frequency in mpp.
                mpp[val]++;

                // Register val under its NEW frequency in mpp1.
                // val has moved from freq bucket to (freq+1) bucket.
                // mpp1 now correctly reflects the updated frequency distribution.
                mpp1[mpp[val]]++;

                // VALIDITY CHECK — CASE 1:
                // Only one distinct value exists in the subarray (mpp.size()==1)
                // AND all values share one frequency (mpp1.size()==1).
                // A single distinct value trivially satisfies the condition.
                // We check mpp1.size()==1 alongside to be safe, but mpp.size()==1
                // already guarantees mpp1.size()==1 (only one value, one frequency).
                if (mpp.size() == 1 && mpp1.size() == 1) {
                    ans = max(ans, j - i + 1);
                }
                // VALIDITY CHECK — CASE 2:
                // Exactly two distinct frequencies exist.
                // Valid only if the larger is exactly double the smaller.
                // We iterate mpp1 (guaranteed only 2 entries here) to extract
                // both frequency values and check the condition.
                else if (mpp1.size() == 2) {
                    auto it = mpp1.begin();
                    int f1 = it->first; it++;
                    int f2 = it->first;
                    int maxi = max(f1, f2), mini = min(f1, f2);
                    // maxi == 2*mini means the two frequencies are f and 2f.
                    // Both are present (they're both keys in mpp1 with count > 0).
                    // This satisfies the frequency balance condition.
                    if (maxi == 2 * mini) ans = max(ans, j - i + 1);
                }
                // CASE 3: mpp1.size() >= 3
                // Three or more distinct frequencies exist.
                // Impossible to express as f and 2f → skip, don't update ans.
            }
        }

        return ans;
    }
};

/*
 * =========================================================================
 * DRY RUN: nums = [1, 2, 2, 1]
 * =========================================================================
 *
 * Fix i=0. mpp={}, mpp1={}
 *
 * j=0, val=1:
 *   mpp[1]==0, skip removal block.
 *   mpp[1]++ → mpp={1:1}
 *   mpp1[1]++ → mpp1={1:1}
 *   mpp.size()==1 && mpp1.size()==1 → valid, ans=1
 *
 * j=1, val=2:
 *   mpp[2]==0, skip removal block.
 *   mpp[2]++ → mpp={1:1, 2:1}
 *   mpp1[1]++ → mpp1={1:2}   ← both value 1 and value 2 have freq 1
 *   mpp.size()==2, mpp1.size()==1 → valid (all same freq), ans=2
 *
 * j=2, val=2:
 *   mpp[2]==1 > 0 → freq=1
 *   mpp1[1]-- → mpp1={1:1}   ← value 2 is leaving freq bucket 1
 *   mpp1[1] != 0, no erase.
 *   mpp[2]++ → mpp={1:1, 2:2}
 *   mpp1[2]++ → mpp1={1:1, 2:1}  ← value 2 now in freq bucket 2
 *   mpp1.size()==2 → f1=1, f2=2, maxi=2, mini=1, 2==2*1 → valid, ans=3
 *
 * j=3, val=1:
 *   mpp[1]==1 > 0 → freq=1
 *   mpp1[1]-- → mpp1={1:0, 2:1}
 *   mpp1[1]==0 → erase → mpp1={2:1}
 *   mpp[1]++ → mpp={1:2, 2:2}
 *   mpp1[2]++ → mpp1={2:2}   ← both value 1 and value 2 have freq 2
 *   mpp1.size()==1 → valid (all same freq), ans=4
 *
 * Final ans=4. Correct — entire array is frequency balanced.
 *
 * =========================================================================
 * COMPLEXITY
 * =========================================================================
 *
 * TIME:
 * - Outer loop runs n times (each choice of i)
 * - Inner loop runs at most n times (each choice of j)
 * - Each (i,j) step: O(1) for mpp update, O(1) for mpp1 update,
 *   O(1) for validity check (mpp1 has at most 2 entries when we iterate)
 * - Total: O(n^2)
 *
 * SPACE:
 * - mpp holds at most O(n) entries (at most n distinct values)
 * - mpp1 holds very few entries in practice (rarely more than 3)
 *   but technically O(n) in worst case
 * - Both reset every outer iteration
 * - Total: O(n)
 *
 * IS THIS OPTIMAL?
 * For n<=1000, O(n^2) is perfectly fine (10^6 operations).
 * A true O(n) solution is non-trivial and likely requires a different
 * mathematical insight. This solution is the natural, clean approach
 * for the given constraints.
 */