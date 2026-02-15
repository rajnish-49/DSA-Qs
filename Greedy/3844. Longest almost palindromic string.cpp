class Solution {
public:
    int almostPalindromic(string s) {
        /*
         * ═══════════════════════════════════════════════════════════════════════════
         * PROBLEM STATEMENT - WHAT ARE WE SOLVING?
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * We need to find the LENGTH of the LONGEST almost-palindromic SUBSTRING.
         * 
         * DEFINITIONS:
         * 
         * 1. SUBSTRING: A contiguous sequence of characters within a string.
         *    Example: In "abca", substrings include: "a", "ab", "abc", "abca", "b", "bc", etc.
         * 
         * 2. PALINDROME: A string that reads the same forwards and backwards.
         *    Examples: "aba", "abba", "racecar", "noon"
         *    Non-examples: "abc", "abca"
         * 
         * 3. ALMOST-PALINDROMIC: A string that becomes a palindrome after removing 
         *    EXACTLY ONE character from it.
         *    
         *    Example 1: "abca"
         *      - Remove 'a' at index 0 → "bca" (not palindrome)
         *      - Remove 'b' at index 1 → "aca" (not palindrome)
         *      - Remove 'c' at index 2 → "aba" (palindrome!) ✓
         *      - Remove 'a' at index 3 → "abc" (not palindrome)
         *      Since we CAN remove one character ('c') to make it palindrome, "abca" is almost-palindromic.
         * 
         *    Example 2: "abc"
         *      - Remove 'a' → "bc" (not palindrome)
         *      - Remove 'b' → "ac" (not palindrome)
         *      - Remove 'c' → "ab" (not palindrome)
         *      "abc" is NOT almost-palindromic.
         * 
         * IMPORTANT INSIGHT: A perfect palindrome IS also almost-palindromic!
         *    Example: "aba"
         *      - Remove middle 'b' → "aa" (palindrome!) ✓
         *      So "aba" qualifies as almost-palindromic.
         * 
         * OUR TASK: Find the longest substring that is almost-palindromic.
         * 
         * ═══════════════════════════════════════════════════════════════════════════
         * SOLUTION APPROACH - EXPAND AROUND CENTER
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * KEY OBSERVATION: Every palindrome has a CENTER point.
         * 
         * There are TWO types of centers:
         * 
         * TYPE 1 - ODD-LENGTH PALINDROME (single character center):
         *    "racecar" → center is 'e' at index 3
         *     r a c e c a r
         *     0 1 2 3 4 5 6
         *           ↑
         *         center
         *    We expand: compare (2,4), then (1,5), then (0,6)
         * 
         * TYPE 2 - EVEN-LENGTH PALINDROME (gap between two characters):
         *    "abba" → center is between the two 'b's (between index 1 and 2)
         *     a b b a
         *     0 1 2 3
         *       ↑ ↑
         *       gap
         *    We expand: compare (1,2), then (0,3)
         * 
         * OUR STRATEGY:
         * 
         * For EVERY possible center position in the string:
         * 
         *   STEP 1: Expand outward (left--, right++) while characters match
         *           → This finds the LONGEST PERFECT PALINDROME at this center
         * 
         *   STEP 2: When we hit a mismatch (s[left] != s[right]):
         *           → Try SKIPPING the left character and continue expanding
         *           → Try SKIPPING the right character and continue expanding
         *           → This finds almost-palindromes (perfect palindrome + 1 extra char)
         * 
         * WHY THIS WORKS:
         * 
         * An almost-palindromic string has ONE of these structures:
         * 
         * Structure A: Perfect palindrome + 1 extra character inserted somewhere
         *    Example: "abca" = "aba" (palindrome) + 'c' inserted
         *    Visual:  a b [c] a
         *             ↑       ↑
         *           These match when we skip [c]
         * 
         * Structure B: Two strings that would be palindrome if one char was removed
         *    Example: "xabax" = if we remove one 'x', we get "abax" or "xaba"... 
         *             Actually, if we remove first 'x', we get "abax" (not palindrome)
         *             But the substring "abax" itself: remove 'x' → "aba" (palindrome!)
         * 
         * By expanding around every center and trying to skip one mismatched character,
         * we explore ALL possible almost-palindromic substrings.
         * 
         * ═══════════════════════════════════════════════════════════════════════════
         */
        
        int n = s.size();
        
        // EDGE CASE: If string has less than 2 characters, return its length
        // Why? A single character is trivially almost-palindromic (remove it, get empty string which is palindrome)
        // Though based on constraints, n >= 2, so this is just defensive programming
        if (n < 2) return n;
        
        // This will store the maximum length of almost-palindromic substring found
        // Initialize to 1 because any single character is almost-palindromic
        int maxLen = 1;
        
        /*
         * ═══════════════════════════════════════════════════════════════════════════
         * MAIN LOOP: TRY EVERY POSSIBLE CENTER POSITION
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * We iterate through each index i from 0 to n-1.
         * At each index i, we will:
         *   1. Try treating i as the center of an ODD-length palindrome
         *   2. Try treating the gap between i and i+1 as center of an EVEN-length palindrome
         * 
         * Why check every position?
         * - We don't know WHERE the longest almost-palindrome is located
         * - It could be centered anywhere in the string
         * - By checking all positions, we guarantee finding the longest one
         * 
         * Example: s = "abcdefg"
         * We'll try centers at:
         *   - Odd: 0, 1, 2, 3, 4, 5, 6
         *   - Even: (0,1), (1,2), (2,3), (3,4), (4,5), (5,6)
         */
        for (int i = 0; i < n; i++) {
            
            /*
             * ───────────────────────────────────────────────────────────────────────
             * SECTION 1: ODD-LENGTH PALINDROME CENTERED AT INDEX i
             * ───────────────────────────────────────────────────────────────────────
             * 
             * CONCEPT: The center is the single character at index i.
             * We expand outward symmetrically: (i-1, i+1), (i-2, i+2), (i-3, i+3), ...
             * 
             * Example: s = "racecar", i = 3 (center at 'e')
             *   r a c e c a r
             *   0 1 2 3 4 5 6
             *         ↑
             *       center
             * 
             * Expansion steps:
             *   1. left=3, right=3: s[3]='e' == s[3]='e' ✓ (length 1)
             *   2. left=2, right=4: s[2]='c' == s[4]='c' ✓ (length 3)
             *   3. left=1, right=5: s[1]='a' == s[5]='a' ✓ (length 5)
             *   4. left=0, right=6: s[0]='r' == s[6]='r' ✓ (length 7)
             *   5. left=-1, right=7: out of bounds, stop
             * 
             * We found a perfect palindrome "racecar" of length 7.
             */
            
            // Initialize both pointers at the center character
            int left = i;
            int right = i;
            
            /*
             * ─────────────────────────────────────────────────────────────────
             * PHASE 1: FIND THE LONGEST PERFECT PALINDROME AT THIS CENTER
             * ─────────────────────────────────────────────────────────────────
             * 
             * Loop invariant: s[left+1...right-1] is a palindrome
             * We keep expanding while:
             *   1. left is within bounds (left >= 0)
             *   2. right is within bounds (right < n)
             *   3. Characters at left and right match (s[left] == s[right])
             * 
             * Why all three conditions?
             *   - If left < 0 or right >= n: we've gone outside the string
             *   - If s[left] != s[right]: we've found a mismatch, palindrome ends
             */
            while (left >= 0 && right < n && s[left] == s[right]) {
                // Current substring from left to right is a perfect palindrome
                // Calculate its length: right - left + 1
                // Example: indices [2, 5] → length = 5 - 2 + 1 = 4
                
                // Update maxLen if this palindrome is longer than what we've found
                maxLen = max(maxLen, right - left + 1);
                
                // IMPORTANT: This perfect palindrome is ALSO almost-palindromic!
                // We can remove any character from it and (in most cases) still
                // have characters that could form a palindrome.
                // So we update maxLen here.
                
                // Expand outward for next iteration
                left--;   // Move left pointer one step to the left
                right++;  // Move right pointer one step to the right
            }
            
            /*
             * AFTER THE LOOP:
             * 
             * We've exited the while loop, which means one of these happened:
             *   1. left went below 0 (left < 0)
             *   2. right went beyond string (right >= n)
             *   3. Characters mismatched (s[left] != s[right])
             * 
             * If (1) or (2) happened AND (3) didn't:
             *   - We consumed the entire string as a palindrome
             *   - Both pointers are out of bounds
             *   - We've already recorded this palindrome in maxLen
             *   - No point trying to extend further
             * 
             * If (3) happened:
             *   - We found a mismatch at positions (left, right)
             *   - We have a perfect palindrome from (left+1) to (right-1)
             *   - Now we can try to EXTEND by skipping one mismatched character
             *   - This is where we find almost-palindromes!
             * 
             * Example: s = "abca", i = 1
             *   Initial: left=1, right=1 (center at 'b')
             *   Step 1: s[1]='b' == s[1]='b' ✓ → left=0, right=2, maxLen=1
             *   Step 2: s[0]='a' != s[2]='c' ✗ → STOP
             *   After loop: left=0, right=2, perfect palindrome is just "b"
             */
            
            /*
             * ─────────────────────────────────────────────────────────────────
             * PHASE 2: TRY TO EXTEND BY SKIPPING ONE CHARACTER
             * ─────────────────────────────────────────────────────────────────
             * 
             * CONDITION: if (left >= 0 || right < n)
             * 
             * Why this condition?
             *   - If BOTH left < 0 AND right >= n: we've consumed entire string as palindrome
             *   - If AT LEAST ONE is still in bounds: we stopped due to mismatch
             *   - Only in the mismatch case do we try to extend
             * 
             * Example where we DON'T enter (both out of bounds):
             *   s = "aba", center at i=1
             *   After expansion: left=-1, right=3 (n=3)
             *   Condition: (-1 >= 0) OR (3 < 3) → FALSE OR FALSE → FALSE
             *   We skip this block (correct, we already found "aba")
             * 
             * Example where we DO enter (mismatch):
             *   s = "abca", center at i=1
             *   After expansion: left=0, right=2
             *   Condition: (0 >= 0) OR (2 < 4) → TRUE OR TRUE → TRUE
             *   We enter this block to try extending
             */
            if (left >= 0 || right < n) {
                
                /*
                 * At this point, we have:
                 *   - A perfect palindrome from (left+1) to (right-1)
                 *   - A mismatch at positions (left, right) where s[left] != s[right]
                 *     (or one/both are out of bounds)
                 * 
                 * We have TWO options to create an almost-palindrome:
                 *   OPTION A: Skip/ignore the character at position 'left'
                 *   OPTION B: Skip/ignore the character at position 'right'
                 * 
                 * Why try both?
                 *   - We don't know which one will lead to a longer almost-palindrome
                 *   - One might extend further than the other
                 *   - We try both and keep the maximum
                 * 
                 * Visual example: s = "abca", left=0, right=2
                 *   a b c a
                 *   ↑   ↑
                 *   left right (s[0]='a' != s[2]='c')
                 * 
                 *   Option A: Skip 'a' at left, compare from (-1, 2) onwards
                 *   Option B: Skip 'c' at right, compare from (0, 3) onwards
                 */
                
                // ═════════════════════════════════════════════════════════════
                // OPTION A: SKIP THE LEFT CHARACTER
                // ═════════════════════════════════════════════════════════════
                /*
                 * Strategy: Pretend the character at 'left' doesn't exist.
                 *           Start comparing from (left-1) and (right).
                 * 
                 * Why left-1?
                 *   - We're skipping s[left]
                 *   - So we start from the character BEFORE it
                 * 
                 * Why right (not right+1)?
                 *   - We're keeping s[right]
                 *   - We want to see if it matches with s[left-1]
                 * 
                 * Example: s = "xabax", left=0, right=4 (after "aba" palindrome)
                 *   x a b a x
                 *   ↑       ↑
                 *  left    right (s[0]='x' == s[4]='x', but let's say we skip left)
                 * 
                 *   Skip left 'x': start from l1=-1, r1=4
                 *   Can't expand (l1 out of bounds)
                 */
                int l1 = left - 1;  // Start from one position BEFORE left
                int r1 = right;     // Start from right position
                
                // Expand outward while characters match
                // Same logic as Phase 1, just different starting point
                while (l1 >= 0 && r1 < n && s[l1] == s[r1]) {
                    l1--;
                    r1++;
                }
                
                /*
                 * After this loop:
                 *   - We've expanded as far as possible after skipping s[left]
                 *   - The valid substring is from (l1+1) to (r1-1)
                 * 
                 * Length calculation: (r1 - 1) - (l1 + 1) + 1 = r1 - l1 - 1
                 * 
                 * What does this substring represent?
                 *   - It includes the original perfect palindrome from (left+1) to (right-1)
                 *   - PLUS the skipped character s[left] (that's the one we "remove")
                 *   - PLUS any additional matching characters we found by expanding
                 * 
                 * This substring is almost-palindromic because:
                 *   - If we remove s[left], the remaining characters form a palindrome
                 * 
                 * Example: s = "abca", left=0, right=2
                 *   After skipping left:
                 *     l1 = -1, r1 = 2
                 *     Can't expand (l1 out of bounds)
                 *     Length = 2 - (-1) - 1 = 2
                 *     Substring from (0) to (1) is "ab"
                 *     Remove 'a' → "b" (palindrome, length 1... not great)
                 */
                maxLen = max(maxLen, r1 - l1 - 1);
                
                // ═════════════════════════════════════════════════════════════
                // OPTION B: SKIP THE RIGHT CHARACTER
                // ═════════════════════════════════════════════════════════════
                /*
                 * Strategy: Pretend the character at 'right' doesn't exist.
                 *           Start comparing from (left) and (right+1).
                 * 
                 * Why left (not left-1)?
                 *   - We're keeping s[left]
                 *   - We want to see if it matches with s[right+1]
                 * 
                 * Why right+1?
                 *   - We're skipping s[right]
                 *   - So we start from the character AFTER it
                 * 
                 * Example: s = "abca", left=0, right=2
                 *   a b c a
                 *   ↑   ↑ ↑
                 *  left skip compare with this
                 * 
                 *   Skip right 'c': start from l2=0, r2=3
                 *   s[0]='a' == s[3]='a' ✓
                 *   Expand: l2=-1, r2=4 (out of bounds, stop)
                 *   Length = 4 - (-1) - 1 = 4
                 *   Substring "abca" is almost-palindromic!
                 */
                int l2 = left;       // Start from left position
                int r2 = right + 1;  // Start from one position AFTER right
                
                // Expand outward while characters match
                while (l2 >= 0 && r2 < n && s[l2] == s[r2]) {
                    l2--;
                    r2++;
                }
                
                /*
                 * After this loop:
                 *   - We've expanded as far as possible after skipping s[right]
                 *   - The valid substring is from (l2+1) to (r2-1)
                 * 
                 * Length calculation: r2 - l2 - 1
                 * 
                 * What does this substring represent?
                 *   - It includes the original perfect palindrome from (left+1) to (right-1)
                 *   - PLUS the skipped character s[right] (that's the one we "remove")
                 *   - PLUS any additional matching characters we found by expanding
                 * 
                 * This substring is almost-palindromic because:
                 *   - If we remove s[right], the remaining characters form a palindrome
                 * 
                 * Example trace for s = "abca", left=0, right=2:
                 *   Initial: l2=0, r2=3
                 *   Iteration 1: s[0]='a' == s[3]='a' ✓
                 *                l2=-1, r2=4
                 *   Iteration 2: l2=-1 < 0, stop
                 *   
                 *   Length = 4 - (-1) - 1 = 4
                 *   Substring from index (l2+1=0) to (r2-1=3) is "abca"
                 *   
                 *   Verification: Remove s[right=2]='c' from "abca" → "aba" ✓ (palindrome!)
                 */
                maxLen = max(maxLen, r2 - l2 - 1);
            }
            
            /*
             * ───────────────────────────────────────────────────────────────────────
             * SECTION 2: EVEN-LENGTH PALINDROME CENTERED BETWEEN i AND i+1
             * ───────────────────────────────────────────────────────────────────────
             * 
             * CONCEPT: The center is the GAP between two characters.
             * We start by comparing characters at positions i and i+1.
             * Then expand: (i-1, i+2), (i-2, i+3), ...
             * 
             * Example: s = "abba", i = 1 (center between two 'b's)
             *   a b b a
             *   0 1 2 3
             *     ↑ ↑
             *     gap (center)
             * 
             * Expansion steps:
             *   1. left=1, right=2: s[1]='b' == s[2]='b' ✓ (length 2)
             *   2. left=0, right=3: s[0]='a' == s[3]='a' ✓ (length 4)
             *   3. left=-1, right=4: out of bounds, stop
             * 
             * We found a perfect palindrome "abba" of length 4.
             * 
             * WHY EVEN-LENGTH IS SEPARATE:
             * 
             * Odd-length example:  "aba" - center is 'b', we compare (0,2)
             * Even-length example: "abba" - no single center char, we compare (1,2) first
             * 
             * These are structurally different:
             *   - Odd: Start with one character (left=right=i)
             *   - Even: Start with two characters (left=i, right=i+1)
             * 
             * If we only checked odd centers, we'd miss even-length palindromes!
             */
            
            // Initialize pointers at adjacent positions
            left = i;      // Left pointer at current index
            right = i + 1; // Right pointer at next index
            
            /*
             * ─────────────────────────────────────────────────────────────────
             * PHASE 1: FIND THE LONGEST PERFECT EVEN-LENGTH PALINDROME
             * ─────────────────────────────────────────────────────────────────
             * 
             * Same expansion logic as odd-length case, just different starting point.
             * 
             * We expand while:
             *   1. Both pointers are in bounds
             *   2. Characters at both pointers match
             */
            while (left >= 0 && right < n && s[left] == s[right]) {
                // Current substring is a perfect even-length palindrome
                maxLen = max(maxLen, right - left + 1);
                
                // Expand outward
                left--;
                right++;
            }
            
            /*
             * AFTER THE LOOP:
             * 
             * Same situation as odd-length case:
             *   - Either we consumed the whole string as a palindrome (both pointers out of bounds)
             *   - Or we hit a mismatch
             * 
             * Example: s = "abba", i = 1
             *   Initial: left=1, right=2
             *   Step 1: s[1]='b' == s[2]='b' ✓ → left=0, right=3, maxLen=2
             *   Step 2: s[0]='a' == s[3]='a' ✓ → left=-1, right=4, maxLen=4
             *   Step 3: left=-1 < 0, stop
             *   After loop: both pointers out of bounds
             */
            
            /*
             * ─────────────────────────────────────────────────────────────────
             * PHASE 2: TRY TO EXTEND BY SKIPPING ONE CHARACTER
             * ─────────────────────────────────────────────────────────────────
             * 
             * EXACT SAME LOGIC as odd-length case!
             * 
             * We check if at least one pointer is still valid (meaning we hit a mismatch).
             * If so, try skipping left character or right character and continue expanding.
             */
            if (left >= 0 || right < n) {
                
                // OPTION A: Skip the left character
                int l1 = left - 1;
                int r1 = right;
                
                while (l1 >= 0 && r1 < n && s[l1] == s[r1]) {
                    l1--;
                    r1++;
                }
                
                maxLen = max(maxLen, r1 - l1 - 1);
                
                // OPTION B: Skip the right character
                int l2 = left;
                int r2 = right + 1;
                
                while (l2 >= 0 && r2 < n && s[l2] == s[r2]) {
                    l2--;
                    r2++;
                }
                
                maxLen = max(maxLen, r2 - l2 - 1);
            }
            
            /*
             * END OF ITERATION FOR INDEX i
             * 
             * We've now checked:
             *   1. All odd-length palindromes/almost-palindromes centered at i
             *   2. All even-length palindromes/almost-palindromes centered between i and i+1
             * 
             * Move to next index and repeat.
             */
        }
        
        /*
         * ═══════════════════════════════════════════════════════════════════════════
         * AFTER ALL ITERATIONS
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * We've checked every possible center position (both odd and even).
         * For each center, we:
         *   1. Found the longest perfect palindrome
         *   2. Tried extending by skipping one character
         * 
         * maxLen now contains the length of the longest almost-palindromic substring.
         * 
         * ═══════════════════════════════════════════════════════════════════════════
         * COMPLEXITY ANALYSIS
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * TIME COMPLEXITY: O(n²)
         *   - Outer loop: n iterations (one for each index)
         *   - Inner while loops: Each can expand up to n times in worst case
         *   - Total: O(n) * O(n) = O(n²)
         * 
         *   Worst case example: s = "aaaaaa" (all same characters)
         *   - Every center expands to the full string
         * 
         * SPACE COMPLEXITY: O(1)
         *   - We only use a fixed number of variables (left, right, l1, r1, l2, r2, maxLen)
         *   - No additional data structures that grow with input size
         * 
         * CAN WE DO BETTER?
         *   - For finding longest palindromic substring, Manacher's algorithm does O(n)
         *   - But for almost-palindromic, the problem is more complex
         *   - O(n²) is acceptable given the constraint: n ≤ 2500
         *   - Max operations: 2500² = 6,250,000 (easily handled)
         * 
         * ═══════════════════════════════════════════════════════════════════════════
         * CORRECTNESS ARGUMENT
         * ═══════════════════════════════════════════════════════════════════════════
         * 
         * Why does this algorithm find ALL almost-palindromic substrings?
         * 
         * CLAIM: Every almost-palindromic substring can be found by:
         *        1. Expanding from some center to get a perfect palindrome
         *        2. Skipping one character at the mismatch point
         * 
         * PROOF SKETCH:
         *   - Let S be an almost-palindromic substring
         *   - Remove one character c from S to get palindrome P
         *   - P has some center (either a character or a gap)
         *   - When we expand from this center, we'll find P
         *   - Then we'll try to extend by skipping characters
         *   - One of our "skip" attempts will skip exactly c
         *   - This recovers the original substring S
         *   - Therefore, we'll find S and record its length
         * 
         * Since we try every possible center and every possible skip,
         * we're guaranteed to find the longest almost-palindromic substring.
         * 
         * ═══════════════════════════════════════════════════════════════════════════
         */
        
        return maxLen;
    }
};

/*
 * ═══════════════════════════════════════════════════════════════════════════
 * COMPLETE EXAMPLE WALKTHROUGH: s = "abca"
 * ═══════════════════════════════════════════════════════════════════════════
 * 
 * Initial: n = 4, maxLen = 1
 * 
 * ─────────────────────────────────────────────────────────────────────────────
 * i = 0 (center at 'a')
 * ─────────────────────────────────────────────────────────────────────────────
 * 
 * ODD-LENGTH:
 *   left=0, right=0: s[0]='a' == s[0]='a' ✓, maxLen=1, expand
 *   left=-1, right=1: left out of bounds, stop
 *   Condition: (-1 >= 0 || 1 < 4) = TRUE
 *     Skip left: l1=-2, r1=1 → immediately out of bounds, length=2
 *     Skip right: l2=-1, r2=2 → immediately out of bounds, length=2
 *   maxLen = max(1, 2, 2) = 2
 * 
 * EVEN-LENGTH (between 'a' and 'b'):
 *   left=0, right=1: s[0]='a' != s[1]='b', stop immediately
 *   Condition: (0 >= 0 || 1 < 4) = TRUE
 *     Skip left: l1=-1, r1=1 → out of bounds, length=1
 *     Skip right: l2=0, r2=2: s[0]='a' != s[2]='c', stop, length=1
 *   maxLen = 2
 * 
 * ─────────────────────────────────────────────────────────────────────────────
 * i = 1 (center at 'b')
 * ─────────────────────────────────────────────────────────────────────────────
 * 
 * ODD-LENGTH:
 *   left=1, right=1: s[1]='b' == s[1]='b' ✓, maxLen=2, expand
 *   left=0, right=2: s[0]='a' != s[2]='c', stop
 *   Condition: (0 >= 0 || 2 < 4) = TRUE
 *     Skip left: l1=-1, r1=2 → out of bounds, length=2
 *     Skip right: l2=0, r2=3: s[0]='a' == s[3]='a' ✓, expand
 *                 l2=-1, r2=4: out of bounds, stop
 *                 length = 4 - (-1) - 1 = 4 ← FOUND IT!
 *   maxLen = max(2, 2, 4) = 4
 * 
 * EVEN-LENGTH (between 'b' and 'c'):
 *   left=1, right=2: s[1]='b' != s[2]='c', stop immediately
 *   (skip options won't improve)
 * 
 * ─────────────────────────────────────────────────────────────────────────────
 * i = 2 and i = 3: Won't find anything longer than 4
 * ─────────────────────────────────────────────────────────────────────────────
 * 
 * FINAL ANSWER: maxLen = 4
 * 
 * VERIFICATION: "abca" is almost-palindromic
 *   Remove 'c' at index 2 → "aba" which is a palindrome ✓
 * 
 * ═══════════════════════════════════════════════════════════════════════════
 */