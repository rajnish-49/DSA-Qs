// ============================================================
// PROBLEM UNDERSTANDING
// ============================================================
// Given string s and integer k.
// Two equal characters are "close" if their index distance <= k.
// When close, RIGHT merges into LEFT (right one disappears, left stays).
// Rule: always merge the pair with smallest left index first.
//       if tie on left index, pick smallest right index.
// Keep merging until no more merges possible. Return final string.
//
// Example: s="abca", k=3
// 'a' at index 0 and 'a' at index 3. Distance = 3 <= k=3. Close!
// Right 'a' (index 3) disappears. Result = "abc". Done.
//
// Example: s="aabca", k=2
// 'a' at index 0 and 'a' at index 1. Distance = 1 <= k=2. Close!
// Right 'a' (index 1) disappears. String becomes "abca".
// Now 'a' at index 0 and 'a' at index 3. Distance = 3 > k=2. Not close.
// No more merges. Result = "abca".
//
// CONSTRAINTS: s.length <= 100, k <= s.length
// Small constraints -> even O(n^3) brute force is perfectly fine here.

// ============================================================
// APPROACH 1: SIMULATION (Brute Force)
// ============================================================
//
// INTUITION FROM FIRST PRINCIPLES:
// The problem literally tells us what to do — find close pairs, merge them,
// repeat. So just do exactly that. Simulate the process step by step.
//
// THE RESTART PROBLEM — why we can't just do one left-to-right pass:
// After every single merge (deletion), the string changes. Indices shift.
// Characters that were previously TOO FAR apart might now be within distance k.
//
// Concrete example of why restart is necessary:
// s = "aXXXa", k = 3
// 'a' at index 0, 'a' at index 4. Distance = 4 > k=3. NOT close yet.
// But say 'X' at index 1 gets merged/deleted by some other rule first.
// String becomes "aXXa". Now 'a' at index 0, 'a' at index 3. Distance = 3 = k. CLOSE!
// Without restarting, we would have missed this merge entirely.
//
// So after EVERY single merge, we must restart scanning from the beginning.
// This guarantees we never miss a merge that became possible due to a deletion.
//
// THE MERGE ORDER RULE is naturally satisfied by scanning left to right:
// - Outer loop i goes left to right -> smallest left index is always found first
// - Inner loop j goes left to right from i -> smallest right index found first
// - We stop and restart the moment we find ANY merge -> ensures priority order
//
// COMPLEXITY:
// Each merge removes one character. At most n merges total.
// Each pass to find a merge scans O(n) for i, O(k) for j -> O(n*k) per pass.
// Total: O(n^2 * k). With n=100, k=100 -> 1 million ops. Totally fine.

class Solution
{
public:
    string mergeCloseCharacters(string s, int k)
    {

        // This flag answers: "did we successfully perform a merge in this pass?"
        // Start as true to enter the while loop for the first time.
        // If we complete an entire pass over the string without finding any
        // valid merge, merged stays false and we exit -> string is fully stable.
        bool merged = true;

        while (merged)
        {
            merged = false; // Reset: assume this pass will find no merges.
                            // If we find one, we set it back to true and restart.

            // Scan left to right to find the LEFTMOST valid merge pair.
            // The '&& !merged' condition is critical:
            // The moment we find and perform a merge, we must STOP this scan
            // immediately and restart from the beginning of the string.
            // Why? Because the deletion shifted all indices after j.
            // Continuing the scan with stale indices would be wrong.
            // Also, the problem wants smallest left index — restarting ensures
            // we never process a later merge before an earlier one that
            // might have become valid due to this deletion.
            for (int i = 0; i < s.size() && !merged; i++)
            {

                // For the character at position i, scan ahead up to k positions.
                // Distance between i and j is (j - i).
                // For them to be "close", we need j - i <= k, so j <= i + k.
                // j starts at i+1 because distance must be > 0 (different indices).
                // Among all valid j for this i, we want the smallest j first
                // (problem rule) — left-to-right scan gives us this naturally.
                for (int j = i + 1; j <= i + k && j < s.size(); j++)
                {

                    if (s[i] == s[j])
                    {
                        // Found the highest priority merge available:
                        // - i is the smallest possible left index (outer loop)
                        // - j is the smallest possible right index for this i
                        //
                        // Perform the merge: RIGHT character disappears.
                        // s.erase(j, 1) removes 1 character at position j.
                        // The left character at i stays unchanged.
                        // All characters after j shift one position left.
                        s.erase(j, 1);

                        // Signal that a merge happened this pass.
                        // The '!merged' in the for loop condition will now
                        // break us out of both for loops simultaneously.
                        merged = true;
                        break; // Exit inner j loop. Outer i loop exits via !merged.
                               // While loop condition (merged=true) triggers restart.
                    }
                }
            }
            // If we reach here with merged=false, we scanned the entire string
            // and found zero valid merges. String is in its final stable state.
        }

        return s;
    }
};



class Solution
{
public:
    string mergeCharacters(string s, int k)
    {
        /*
         * PROBLEM:
         * Two equal characters merge if their distance <= k. Right one disappears.
         * Always process leftmost merge first. Repeat until no merges possible.
         *
         * BRUTE FORCE ISSUE:
         * Simulating actual deletions and re-scanning is O(n^2) because every
         * merge shifts indices and we'd need to rescan from the beginning.
         *
         * KEY OBSERVATION (why one pass works):
         * Since we always process the leftmost merge first, merges naturally
         * cascade left to right. A character either:
         *   - Gets absorbed by a same-character to its left within distance k → skip it
         *   - Has no same-character within k positions to its left → it survives
         *
         * So we can decide each character's fate as we go, left to right,
         * without ever going back. We just need to know: "does my character
         * already exist in the last k positions of the OUTPUT string?"
         *
         * WHY OUTPUT STRING AND NOT INPUT:
         * After merges, deleted characters no longer exist — they don't count
         * toward distance. So distances must be measured on the evolving result,
         * not the original string. Our result IS that evolving string.
         *
         * THE SLIDING WINDOW:
         * We keep a window = last k characters of `result`.
         * cnt[26] mirrors exactly which characters live inside this window.
         *
         * When we process a character c from s:
         *   - cnt[c] > 0 → c exists within last k positions of result
         *                → distance would be <= k → it gets merged → SKIP
         *   - cnt[c] == 0 → no same character in range → SURVIVES → append to result
         *
         * After appending, if result grew beyond k characters, the character
         * at index (result.size() - k - 1) is now k+1 positions behind the
         * latest character → it's outside the merge window → evict it from cnt.
         *
         * WINDOW INVARIANT: cnt always reflects exactly the last k chars of result.
         *
         * ── FULL TRACE: s = "yybyzybz", k = 2 ──────────────────────────────
         *
         * Process 'y' (s[0]):
         *   cnt[y]=0 → SURVIVE → result="y", cnt[y]++ → cnt={y:1}
         *   size=1 not > 2 → no eviction | Window=['y']
         *
         * Process 'y' (s[1]):
         *   cnt[y]=1 > 0 → SKIP (absorbed by result[0])
         *   result="y", cnt={y:1} | Window=['y']
         *
         * Process 'b' (s[2]):
         *   cnt[b]=0 → SURVIVE → result="yb", cnt[b]++ → cnt={y:1,b:1}
         *   size=2 not > 2 → no eviction | Window=['y','b']
         *
         * Process 'y' (s[3]):
         *   cnt[y]=1 > 0 → SKIP (result[0]='y' is within distance 2)
         *   result="yb", cnt={y:1,b:1} | Window=['y','b']
         *
         * Process 'z' (s[4]):
         *   cnt[z]=0 → SURVIVE → result="ybz", cnt[z]++ → cnt={y:1,b:1,z:1}
         *   size=3 > 2 → EVICT result[3-2-1]=result[0]='y'
         *   cnt[y]-- → cnt={y:0,b:1,z:1} | Window=['b','z']
         *
         * Process 'y' (s[5]):
         *   cnt[y]=0 → SURVIVE → result="ybzy", cnt[y]++ → cnt={y:1,b:1,z:1}
         *   size=4 > 2 → EVICT result[4-2-1]=result[1]='b'
         *   cnt[b]-- → cnt={y:1,b:0,z:1} | Window=['z','y']
         *
         * Process 'b' (s[6]):
         *   cnt[b]=0 → SURVIVE → result="ybzyb", cnt[b]++ → cnt={y:1,b:1,z:1}
         *   size=5 > 2 → EVICT result[5-2-1]=result[2]='z'
         *   cnt[z]-- → cnt={y:1,b:1,z:0} | Window=['y','b']
         *
         * Process 'z' (s[7]):
         *   cnt[z]=0 → SURVIVE → result="ybzybz", cnt[z]++ → cnt={y:1,b:1,z:1}
         *   size=6 > 2 → EVICT result[6-2-1]=result[3]='y'
         *   cnt[y]-- → cnt={y:0,b:1,z:1} | Window=['b','z']
         *
         * Final result = "ybzybz" ✓
         * ────────────────────────────────────────────────────────────────────
         *
         * TIME:  O(n) — single pass through s
         * SPACE: O(n) for result, O(26) for cnt
         */

        string result;
result.reserve(s.size()); // pre-allocate to avoid repeated reallocation, result will never exceed s.size()
int cnt[26] = {0};        // cnt[i] = how many times character 'a'+i appears in the current window (last k chars of result)

for (char c : s)
{
    int idx = c - 'a';

    // If cnt[idx] > 0, this character already exists somewhere in the last k
    // positions of result. That means if we were to add c now, it would be
    // within merge distance (<=k) of that existing copy → it gets absorbed → don't add it.
    if (cnt[idx] > 0)
        continue;

    // cnt[idx] == 0 means no copy of c exists in the last k positions of result.
    // This character has no same-character neighbor close enough to merge with → it survives.
    result.push_back(c);
    cnt[idx]++; // c is now the rightmost character in result, it enters the window → track it

    // After appending, check if the window has grown too large.
    // The window must always contain exactly the last k characters of result.
    // If result.size() > k, then result[result.size()-k-1] is now k+1 positions
    // behind the latest character → it has slid OUT of the window → stop tracking it.
    // Example: result="ybz", k=2 → window should be ['b','z'], so 'y' at index 0
    // (which is 3-2-1=0) must be evicted since it's no longer within k distance.
    if (result.size() > k)
    {
        char drop = result[result.size() - k - 1];
        cnt[drop - 'a']--; // decrement its count → it no longer blocks future same characters
    }
}

return result;