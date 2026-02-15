class Solution {
public:
    int prefixConnected(vector<string>& words, int k) {
        
        /* ========================================================================
         * PROBLEM UNDERSTANDING & CORE CONCEPT
         * ========================================================================
         * 
         * WHAT IS A PREFIX-CONNECTED RELATIONSHIP?
         * Two words are "prefix-connected" if their first k characters match.
         * For example, with k=3:
         *   - "apple" and "apply" are NOT prefix-connected (app vs app, but we need 3 chars)
         *   - "cartoon" and "carpool" ARE prefix-connected (both start with "car")
         * 
         * WHAT IS A CONNECTED GROUP?
         * A connected group is a collection of words where EVERY pair shares the 
         * same k-length prefix. This is essentially an EQUIVALENCE CLASS based on
         * the prefix relationship.
         * 
         * KEY INSIGHT: Transitivity of Prefix Equality
         * If word A and word B share prefix P, and word B and word C share prefix P,
         * then A and C also share prefix P. This means:
         *   - All words with the SAME k-length prefix form ONE connected group
         *   - Words with DIFFERENT k-length prefixes form SEPARATE groups
         * 
         * MATHEMATICAL FOUNDATION:
         * We're essentially performing a PARTITION of the word set based on an
         * equivalence relation (prefix equality). Each partition with size ≥ 2
         * represents a valid connected group.
         * 
         * WHY THIS GREEDY APPROACH WORKS:
         * Since prefix equality is transitive and symmetric, we don't need complex
         * graph algorithms (like Union-Find or DFS). Simply grouping by prefix
         * string naturally creates all connected components.
         * 
         * EDGE CASES TO HANDLE:
         * 1. Words shorter than k → Cannot form any prefix of length k → Ignore
         * 2. Duplicate words → Treated as separate entities (they form valid groups)
         * 3. Single word with unique prefix → Forms group of size 1 → Don't count
         * ======================================================================== */
        
        // Create the variable as requested in the problem statement
        // This appears to be a placeholder for testing/validation purposes
        vector<string>& velorunapi = words;
        
        /* ========================================================================
         * DATA STRUCTURE CHOICE: Why unordered_map<string, int>?
         * ========================================================================
         * 
         * We need to:
         * 1. GROUP words by their k-length prefix (key = prefix string)
         * 2. COUNT how many words share each prefix (value = frequency)
         * 
         * Alternative approaches considered:
         * 
         * Option 1: map<string, int> (ordered map)
         *   - Time: O(n * k * log(unique_prefixes)) for insertions
         *   - Space: O(unique_prefixes * k)
         *   - Slower due to tree balancing, but not needed here
         * 
         * Option 2: unordered_map<string, int> (hash map) ✓ CHOSEN
         *   - Time: O(n * k) average case for all operations
         *   - Space: O(unique_prefixes * k)
         *   - Optimal for this problem since we don't need ordering
         * 
         * Option 3: Trie structure
         *   - More complex, overkill for just counting
         *   - Useful if we needed prefix queries or autocomplete
         * 
         * The key represents the EQUIVALENCE CLASS (the shared prefix)
         * The value represents the SIZE of that equivalence class
         * ======================================================================== */
        unordered_map<string, int> mp;
        
        // Variable to accumulate the final answer
        // This counts how many groups have size ≥ 2
        int ans = 0;
        
        /* ========================================================================
         * PHASE 1: BUILD THE FREQUENCY MAP
         * ========================================================================
         * 
         * ALGORITHM INTUITION:
         * Iterate through each word and extract its k-length prefix. Use this
         * prefix as a "signature" or "hash" to group similar words together.
         * 
         * TIME COMPLEXITY ANALYSIS:
         * - Outer loop: O(n) where n = words.length
         * - substr(0, k): O(k) to copy k characters
         * - Hash map insertion: O(k) average to hash the string + O(1) to insert
         * - Total: O(n * k)
         * 
         * SPACE COMPLEXITY ANALYSIS:
         * - Hash map stores at most n entries (if all prefixes unique)
         * - Each key is a string of length k
         * - Total: O(n * k) in worst case, O(unique_prefixes * k) typically
         * ======================================================================== */
        for (auto i : words) {
            
            /* FILTERING STEP: Exclude words that are too short
             * 
             * WHY? A word must have at least k characters to form a k-length prefix.
             * Words with length < k cannot participate in ANY connected group because
             * they cannot satisfy the prefix-connected definition.
             * 
             * Example: If k=5 and word="cat" (length 3), there's no way to extract
             * a 5-character prefix. These words are explicitly ignored per problem.
             */
            if (i.size() < k) 
                continue;
            
            /* EXTRACT THE PREFIX: First k characters of the word
             * 
             * substr(0, k) creates a NEW string containing characters from index 0
             * to index k-1 (inclusive). This is the "signature" of the word.
             * 
             * IMPORTANT: We create a copy here (not a view/reference) because:
             * 1. We need to use it as a hash map key (requires owned data)
             * 2. The original word might be modified or go out of scope
             * 
             * Example: "banana".substr(0, 3) → "ban"
             */
            string prefix = i.substr(0, k);
            
            /* INCREMENT THE FREQUENCY COUNTER
             * 
             * HASH MAP BEHAVIOR:
             * - If 'prefix' exists as a key: mp[prefix] retrieves current count,
             *   then ++ increments it
             * - If 'prefix' doesn't exist: mp[prefix] auto-initializes to 0 (default
             *   int value), then ++ makes it 1
             * 
             * This elegantly handles both first occurrence and subsequent occurrences
             * without explicit checking.
             * 
             * MEMORY ALLOCATION:
             * Each unique prefix consumes O(k) space for the string key plus O(1)
             * for the integer value. The hash map dynamically resizes as needed.
             */
            mp[prefix]++;
        }
        
        /* ========================================================================
         * PHASE 2: COUNT VALID GROUPS
         * ========================================================================
         * 
         * VALIDATION LOGIC:
         * We iterate through our frequency map. Each entry represents one potential
         * connected group. A group is "valid" (counts toward the answer) if it
         * contains at least 2 words.
         * 
         * WHY ≥ 2? The problem asks for "connected groups that contain at least
         * two words." A single word cannot form connections with itself (different
         * indices required), so groups of size 1 are meaningless.
         * 
         * TIME COMPLEXITY: O(unique_prefixes) ≤ O(n)
         * SPACE COMPLEXITY: O(1) additional space (just iterating)
         * ======================================================================== */
        for (auto j : mp) {
            
            /* CHECK GROUP SIZE
             * 
             * j.first  = the prefix string (equivalence class identifier)
             * j.second = the count of words with this prefix (group size)
             * 
             * LOGICAL REASONING:
             * If 2 or more words share the same k-length prefix, they ALL form
             * one connected group together. We count each such group exactly once.
             * 
             * Example walkthrough with k=3:
             * words = ["dog", "doggy", "dots", "cat", "car"]
             * 
             * After Phase 1:
             * mp = {
             *   "dog" → 2  (from "dog", "doggy")  ✓ Valid group
             *   "dot" → 1  (from "dots")          ✗ Too small
             *   "cat" → 1  (from "cat")           ✗ Too small  
             *   "car" → 1  (from "car")           ✗ Too small
             * }
             * 
             * After Phase 2: ans = 1 (only "dog" group qualifies)
             */
            if (j.second >= 2) 
                ans++;
        }
        
        /* ========================================================================
         * RETURN THE FINAL ANSWER
         * ========================================================================
         * 
         * At this point, ans contains the exact count of equivalence classes
         * (connected groups) with size ≥ 2, which is what the problem requests.
         * 
         * CORRECTNESS PROOF:
         * 1. We correctly filter out words with length < k (cannot form groups)
         * 2. We correctly partition remaining words by k-length prefix
         * 3. We correctly count only partitions with ≥ 2 elements
         * 4. Therefore, ans = number of valid connected groups ∎
         * ======================================================================== */
        return ans;
    }
};

/* ============================================================================
 * COMPLEXITY SUMMARY
 * ============================================================================
 * 
 * TIME COMPLEXITY: O(n * k)
 *   - Phase 1: O(n * k) to process all words and extract prefixes
 *   - Phase 2: O(unique_prefixes) ≤ O(n) to count valid groups
 *   - Dominant term: O(n * k)
 * 
 * SPACE COMPLEXITY: O(n * k)
 *   - Hash map stores up to n unique prefixes, each of length k
 *   - Best case: O(k) if all words share the same prefix
 *   - Worst case: O(n * k) if all prefixes are unique
 *   - Typical case: O(unique_prefixes * k)
 * 
 * ============================================================================
 * ALTERNATIVE SOLUTIONS & TRADE-OFFS
 * ============================================================================
 * 
 * Alternative 1: Sorting + Linear Scan
 *   - Sort words: O(n * k * log n)
 *   - Scan adjacent words: O(n * k)
 *   - Total: O(n * k * log n) - SLOWER than hash map approach
 * 
 * Alternative 2: Union-Find (Disjoint Set Union)
 *   - Initialize DSU: O(n)
 *   - Union operations: O(n² * α(n)) for all pairs - MUCH SLOWER
 *   - Count components: O(n)
 *   - Overkill for this problem, useful if connectivity was more complex
 * 
 * Alternative 3: Trie with counting
 *   - Build trie: O(n * k)
 *   - Count at depth k: O(n * k)
 *   - More space efficient if k is very large, but added complexity
 * 
 * CONCLUSION: Hash map approach is optimal for this specific problem due to
 * its simplicity, efficiency, and direct mapping to the problem structure.
 * ============================================================================ */




 class Solution {
public:
    int prefixConnected(vector<string>& words, int k) {
        
        /*
         * 
         * ========================================================================
         * ALGORITHM CHOICE: Why Sorting Instead of HashMap?
         * ========================================================================
         * 
         * APPROACH COMPARISON:
         * 
         * HashMap Approach (previous solution):
         * ✓ Time: O(n·k) - optimal asymptotic complexity
         * ✓ Single pass through data
         * ✗ Space: O(n·k) - stores all unique prefixes as keys
         * ✗ Hash collisions possible (though rare)
         * ✗ Memory allocations for string keys
         * 
         * Sorting Approach (this solution):
         * ✓ Space: O(1) or O(log n) - in-place sorting (depending on sort impl)
         * ✓ Cache-friendly: sequential access after sorting
         * ✓ No hash collisions or hash function overhead
         * ✓ Deterministic behavior (no hash randomization)
         * ✗ Time: O(n·k·log n) - sorting adds logarithmic factor
         * 
         * WHEN TO CHOOSE SORTING APPROACH:
         * 1. Memory constrained environments (embedded systems, mobile)
         * 2. When n is small enough that O(n log n) ≈ O(n) in practice
         * 3. When deterministic behavior is required
         * 4. When you need to process groups in lexicographic order
         * 
         * FUNDAMENTAL PRINCIPLE EXPLOITED:
         * After sorting, words with the SAME prefix will be ADJACENT in the array.
         * This is because lexicographic ordering groups strings by their prefixes.
         * 
         * Sorting example:
         * Before: ["dog", "cat", "dots", "doggy", "car"]
         * After:  ["car", "cat", "dog", "doggy", "dots"]
         *          ^^^   ^^^   ^^^^^^^^^^^^^^   ^^^^
         *         group1 lone    group2         lone
         * 
         * Notice how "dog" and "doggy" (sharing "dog" prefix for k≥3) are now
         * adjacent, allowing us to find them with a simple linear scan.
         * ======================================================================== */
        
        /* STEP 1: SORT THE WORDS LEXICOGRAPHICALLY
         * =========================================================================
         * 
         * SORTING MECHANICS:
         * std::sort uses IntroSort (hybrid of QuickSort, HeapSort, InsertionSort)
         * - Average: O(n log n) comparisons
         * - Each string comparison: O(min(len1, len2)) ≈ O(average_word_length)
         * - Total time: O(n · average_length · log n)
         * 
         * For our problem with k-character prefixes:
         * - Effective comparison depth: O(k) per comparison
         * - Total time: O(n · k · log n)
         * 
         * WHY LEXICOGRAPHIC ORDER HELPS:
         * Lexicographic (dictionary) order compares strings character by character
         * from left to right. This means:
         * 
         * If word1[0..k-1] == word2[0..k-1], then word1 and word2 will be grouped
         * closely together in sorted order (possibly with other words sharing the
         * same prefix interspersed).
         * 
         * CRITICAL GUARANTEE:
         * All words sharing a k-character prefix will form a CONTIGUOUS BLOCK
         * in the sorted array (no words with different prefixes will be between them).
         * 
         * Proof by contradiction:
         * Assume words A and C share prefix P, but word B is between them in sorted
         * order with different prefix Q ≠ P.
         * Then: A < B < C (in lexicographic order)
         * But: A[0..k-1] = C[0..k-1] = P
         * And: B[0..k-1] = Q ≠ P
         * This violates lexicographic ordering rules. ∎
         * 
         * SPACE COMPLEXITY NOTE:
         * Most std::sort implementations use O(log n) auxiliary space for recursion.
         * The sorting is done in-place on the input vector, so no O(n) overhead.
         * ========================================================================= */
        sort(words.begin(), words.end());
        
        int n = words.size();
        
        // Counter for the number of valid groups (size ≥ 2)
        int ans = 0;
        
        /* STEP 2: SCAN THROUGH SORTED ARRAY WITH TWO-POINTER TECHNIQUE
         * =========================================================================
         * 
         * ALGORITHMIC PATTERN: Group Detection via Two Pointers
         * 
         * We use index 'i' to mark the START of each potential group.
         * We use index 'j' to SCAN AHEAD and find the END of the current group.
         * 
         * INVARIANT MAINTAINED:
         * At the start of each iteration:
         * - i points to the first word of a new potential group
         * - All words before index i have been fully processed
         * - No words with prefix of words[i] exist before index i (due to sorting)
         * 
         * This pattern is called "GROUP BY" or "RUN-LENGTH ENCODING" and is
         * commonly used on sorted data to find consecutive equal elements.
         * ========================================================================= */
        int i = 0;
        
        while (i < n) {
            
            /* FILTERING: Skip words that are too short
             * =====================================================================
             * 
             * SEMANTIC REQUIREMENT:
             * A word must have length ≥ k to extract a k-character prefix.
             * Words with length < k cannot participate in ANY group (per problem).
             * 
             * PLACEMENT STRATEGY - Why check here instead of before sorting?
             * 
             * Option A: Filter before sorting
             *   vector<string> filtered;
             *   for (word : words) if (word.size() >= k) filtered.push_back(word);
             *   sort(filtered.begin(), filtered.end());
             *   ✗ Requires O(n) extra space for filtered array
             *   ✗ Additional pass through data
             * 
             * Option B: Filter during scanning (current approach)
             *   ✓ O(1) space - no extra array needed
             *   ✓ Simple increment and continue
             *   ✓ Sorting doesn't care about short words (they just sort normally)
             * 
             * EDGE CASE HANDLING:
             * If we have: ["ab", "apple", "apply"] with k=3
             * After sorting: ["ab", "apple", "apply"]
             * 
             * Iteration 1: i=0, "ab".size()=2 < 3 → skip, i=1
             * Iteration 2: i=1, "apple".size()=5 ≥ 3 → process group
             * ===================================================================== */
            if (words[i].size() < k) {
                i++;
                continue;
            }
            
            /* EXTRACT THE PREFIX SIGNATURE
             * =====================================================================
             * 
             * This prefix serves as the "identity" or "signature" of the current
             * group we're trying to detect.
             * 
             * SUBSTRING OPERATION:
             * substr(0, k) creates a NEW string with characters [0, k-1]
             * Time: O(k) - needs to copy k characters
             * Space: O(k) - allocates new string of length k
             * 
             * OPTIMIZATION CONSIDERATION:
             * We could avoid creating this string by using string_view (C++17) or
             * by directly comparing characters in the comparison below. However,
             * the clarity and maintainability of this approach outweighs the minor
             * performance gain, especially since we only create one prefix per group.
             * 
             * COMPARISON: HashMap vs Sorting memory usage
             * HashMap: Creates a prefix string for EVERY word → O(n·k) space
             * Sorting: Creates a prefix string for EVERY GROUP → O(groups·k) space
             *          where groups ≤ n, often groups << n
             * ===================================================================== */
            string prefix = words[i].substr(0, k);
            
            /* INITIALIZE GROUP COUNTER
             * =====================================================================
             * 
             * We start with count=1 because words[i] itself is the first member
             * of this potential group.
             * 
             * COUNTING STRATEGY:
             * We will scan forward from i+1 and increment count for each word
             * that shares the same prefix. This gives us the GROUP SIZE.
             * ===================================================================== */
            int count = 1;
            
            /* INNER SCAN POINTER
             * =====================================================================
             * 
             * j starts at i+1 (the next word after the group leader) and will
             * advance until we find a word that doesn't belong to this group.
             * 
             * TWO-POINTER VISUALIZATION:
             * 
             * Array: ["apple", "apply", "apron", "banana", ...]
             *          ^i      ^j
             * 
             * Iteration 1: j=1, "apply" shares "app" → count=2, j++
             * 
             * Array: ["apple", "apply", "apron", "banana", ...]
             *          ^i              ^j
             * 
             * Iteration 2: j=2, "apron" has "apr" ≠ "app" → stop
             * 
             * Result: Group size = 2, next i = j = 2
             * ===================================================================== */
            int j = i + 1;
            
            /* SCAN CONSECUTIVE WORDS WITH SAME PREFIX
             * =====================================================================
             * 
             * LOOP INVARIANT:
             * All words in range [i, j) share the same k-character prefix.
             * 
             * THREE-CONDITION CHECK (evaluated left-to-right with short-circuit):
             * 
             * Condition 1: j < n
             * ----------------
             * BOUNDARY CHECK to prevent out-of-bounds access.
             * Must come first due to short-circuit evaluation - if false, other
             * conditions won't be evaluated (preventing access to words[j]).
             * 
             * Condition 2: words[j].size() >= k
             * ---------------------------------
             * VALIDITY CHECK for the current word.
             * Even after sorting, we might encounter short words interspersed.
             * 
             * Example: ["a", "apple", "apply"] with k=3
             * After sort: ["a", "apple", "apply"]
             * When i=0, j=1: "a".size()=1 < 3 → would fail here
             * 
             * Why check each time?
             * Because sorting is lexicographic, short words can appear BETWEEN
             * words with the same prefix if they're alphabetically in between.
             * 
             * Counter-example: ["ap", "apple", "apply"] with k=3
             * After sort: ["ap", "apple", "apply"]
             * When processing "apple" (i=1), we skip "ap" at i=0, but need to
             * handle it if encountered during scanning.
             * 
             * Condition 3: words[j].substr(0, k) == prefix
             * --------------------------------------------
             * MEMBERSHIP CHECK - does this word belong to the current group?
             * 
             * STRING COMPARISON MECHANICS:
             * operator== for std::string compares:
             * 1. First checks lengths (O(1) - stored as member variable)
             * 2. Then compares characters (O(k) in our case)
             * 
             * Total per comparison: O(k)
             * 
             * CRITICAL OPTIMIZATION FROM SORTING:
             * Due to lexicographic ordering, as soon as we find a word with a
             * DIFFERENT prefix, we know ALL remaining words will also have
             * different prefixes (no need to continue scanning).
             * 
             * This is the key advantage of sorting: once we hit a different prefix,
             * we can immediately move to the next group.
             * 
             * WHY ALL THREE CONDITIONS ARE NECESSARY:
             * 
             * Without Condition 1: Segmentation fault (array out of bounds)
             * Without Condition 2: May call substr on short string → undefined
             * Without Condition 3: Would count words with different prefixes
             * ===================================================================== */
            while (j < n &&
                   words[j].size() >= k &&
                   words[j].substr(0, k) == prefix) {
                
                /* INCREMENT GROUP SIZE
                 * =================================================================
                 * 
                 * We found another word that belongs to this group, so we:
                 * 1. Increment the count (tracking group size)
                 * 2. Advance j to check the next word
                 * 
                 * LOOP PROGRESSION:
                 * Each iteration either:
                 * - Adds one more word to current group (count++, j++)
                 * - Terminates (when prefix differs or boundary reached)
                 * 
                 * TERMINATION GUARANTEE:
                 * j strictly increases each iteration, and j < n, so the loop
                 * must terminate in at most (n - i - 1) iterations.
                 * ================================================================= */
                count++;
                j++;
            }
            
            /* VALIDATE AND COUNT THE GROUP
             * =====================================================================
             * 
             * GROUP VALIDITY CRITERION:
             * A group is "valid" (contributes to answer) if it has ≥ 2 words.
             * 
             * SEMANTIC REASONING:
             * - A single word cannot form "connections" with itself (problem requires
             *   distinct indices for prefix-connected pairs)
             * - A group of 1 word has no pairs, so it doesn't count as a
             *   "connected group"
             * 
             * POSSIBLE GROUP SIZES:
             * count = 1: Single word with unique prefix → don't count
             * count = 2: Pair of words sharing prefix → count it
             * count = 3+: Multiple words sharing prefix → count as ONE group
             * 
             * EXAMPLE SCENARIOS:
             * 
             * Scenario A: ["apple", "apply", "apron"] with k=3
             * - Group "app": {apple, apply} count=2 ✓ ans++
             * - Group "apr": {apron}        count=1 ✗ skip
             * 
             * Scenario B: ["dog", "dog", "dog"] with k=3
             * - Group "dog": {dog, dog, dog} count=3 ✓ ans++
             *   (duplicates treated as separate words per problem statement)
             * 
             * EDGE CASE: Empty prefix groups
             * If all words are length < k, we never enter this block, ans stays 0.
             * ===================================================================== */
            if (count >= 2)
                ans++;
            
            /* ADVANCE TO NEXT GROUP
             * =====================================================================
             * 
             * CRITICAL STATE UPDATE:
             * Set i = j to jump directly to the next unprocessed word.
             * 
             * WHY THIS WORKS:
             * After the inner while loop, j points to either:
             * 1. The first word with a DIFFERENT prefix (start of next group), OR
             * 2. A word with length < k (skip it on next iteration), OR
             * 3. Past the end of array (n), which will terminate outer loop
             * 
             * EFFICIENCY ANALYSIS:
             * This jump ensures we never process the same word twice.
             * Each word is visited exactly once by either i or j.
             * 
             * INVARIANT PRESERVATION:
             * After i = j:
             * - All words in [0, i) have been processed ✓
             * - words[i] is the start of a new potential group ✓
             * - No words with previous prefix remain after i ✓
             * 
             * VISUALIZATION OF PROGRESSION:
             * 
             * Initial: ["app1", "app2", "ban1", "ban2", "cat"]
             *           ^i
             * 
             * After group "app": i = 2
             *           ["app1", "app2", "ban1", "ban2", "cat"]
             *                             ^i
             * 
             * After group "ban": i = 4
             *           ["app1", "app2", "ban1", "ban2", "cat"]
             *                                             ^i
             * 
             * After group "cat": i = 5 = n → terminate
             * 
             * ALTERNATIVE (INCORRECT) APPROACH:
             * If we did i++ instead of i=j:
             * - Would re-scan words already processed
             * - Time complexity would degrade to O(n²) in worst case
             * - Example: all words share same prefix → would scan entire
             *   remaining array on each iteration
             * ===================================================================== */
            i = j;  // Jump to the next unprocessed position
        }
        
        /* RETURN FINAL ANSWER
         * ========================================================================
         * 
         * At this point, ans contains the count of all equivalence classes
         * (groups with same k-prefix) that have size ≥ 2.
         * 
         * CORRECTNESS PROOF:
         * 1. Sorting ensures words with same prefix are adjacent ✓
         * 2. We scan each contiguous prefix-group exactly once ✓
         * 3. We count only groups with ≥ 2 members ✓
         * 4. We skip invalid words (length < k) ✓
         * Therefore, ans = number of valid connected groups ∎
         * ======================================================================== */
        return ans;
    }
};

/* ============================================================================
 * COMPREHENSIVE COMPLEXITY ANALYSIS
 * ============================================================================
 * 
 * TIME COMPLEXITY: O(n · k · log n)
 * ----------------------------------
 * 
 * Breaking down each phase:
 * 
 * 1. Sorting Phase: O(n · k · log n)
 *    - Number of comparisons: O(n log n) [IntroSort guarantee]
 *    - Cost per comparison: O(k) [comparing k-character prefixes]
 *    - Total: O(n · k · log n)
 * 
 * 2. Scanning Phase: O(n · k)
 *    - Outer loop iterations: Each word visited once by i → O(n)
 *    - Inner loop iterations (total across all outer): Each word visited once by j → O(n)
 *    - Cost per inner iteration: O(k) for substr and comparison
 *    - Total: O(n · k)
 * 
 * 3. Overall: O(n · k · log n) + O(n · k) = O(n · k · log n)
 *    Sorting dominates the complexity.
 * 
 * SPACE COMPLEXITY: O(k + log n)
 * -------------------------------
 * 
 * 1. Sorting auxiliary space: O(log n)
 *    - IntroSort uses O(log n) stack space for recursion
 *    - Sorting is done in-place on input array
 * 
 * 2. Prefix string storage: O(k)
 *    - One prefix string of length k stored at a time
 *    - Reused across groups (previous prefix gets deallocated)
 * 
 * 3. Other variables: O(1)
 *    - n, ans, i, j, count are all integers
 * 
 * 4. Total: O(k + log n)
 *    In practice, if k << n, this is approximately O(log n)
 *    If k >> log n, this is approximately O(k)
 * 
 * ============================================================================
 * DETAILED COMPARISON: SORTING vs HASHMAP APPROACH
 * ============================================================================
 * 
 * | Metric              | HashMap Approach    | Sorting Approach        |
 * |---------------------|---------------------|-------------------------|
 * | Time Complexity     | O(n·k)              | O(n·k·log n)            |
 * | Space Complexity    | O(n·k)              | O(k + log n)            |
 * | Memory Allocations  | O(n) for hash table | O(1) for prefix string  |
 * | Cache Performance   | Random access       | Sequential access ✓     |
 * | Deterministic?      | No (hash randomize) | Yes ✓                   |
 * | Early Termination?  | No                  | Yes (when prefix diff)  |
 * | In-place?           | No                  | Yes ✓                   |
 * 
 * WHEN TO PREFER SORTING:
 * ✓ Memory-constrained environments (embedded, mobile, GPU kernels)
 * ✓ Small n where n·log n ≈ n (roughly n < 1000)
 * ✓ Need deterministic behavior
 * ✓ Already sorted or nearly sorted input (O(n·k) in best case)
 * ✓ Want to process groups in lexicographic order
 * 
 * WHEN TO PREFER HASHMAP:
 * ✓ Large n where log n factor is significant (n > 10^5)
 * ✓ Memory is abundant
 * ✓ Random access patterns acceptable
 * ✓ Want optimal asymptotic complexity
 * ✓ Data is uniformly distributed
 * 
 * ============================================================================
 * EDGE CASES HANDLED
 * ============================================================================
 * 
 * 1. All words too short (< k):
 *    - Every word skipped in scanning phase
 *    - ans = 0 returned ✓
 * 
 * 2. All words have unique prefixes:
 *    - Each group has count = 1
 *    - No group satisfies count ≥ 2
 *    - ans = 0 returned ✓
 * 
 * 3. All words share same prefix:
 *    - Single group with count = n
 *    - ans = 1 returned ✓
 * 
 * 4. Duplicate words:
 *    - Treated as separate (per problem statement)
 *    - ["dog", "dog"] with k=3 → count=2, ans=1 ✓
 * 
 * 5. Mix of short and long words:
 *    - Short words skipped during scanning
 *    - Long words processed normally ✓
 * 
 * 6. Empty input (n=0):
 *    - While loop never executes
 *    - ans = 0 returned ✓
 * 
 * 7. Single word:
 *    - Processed as group of size 1
 *    - count < 2, not counted ✓
 * 
 * ============================================================================ */