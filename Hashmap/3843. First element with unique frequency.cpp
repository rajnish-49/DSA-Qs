class Solution {
public:
    int firstUniqueFreq(vector<int>& nums) {
        
        /* PROBLEM UNDERSTANDING:
         * We need to find the FIRST element (left to right) whose frequency is UNIQUE.
         * "Unique frequency" means no other element appears that many times.
         * 
         * Example: [20,10,30,30]
         * - 20 appears 1 time
         * - 10 appears 1 time  
         * - 30 appears 2 times
         * Here, only 30 has a unique frequency (2), because both 20 and 10 share frequency 1.
         * So we return 30.
         */
        
        /* CORE INSIGHT - TWO-LEVEL FREQUENCY ANALYSIS:
         * 
         * Level 1: What is the frequency of each element?
         *          (How many times does each number appear?)
         * 
         * Level 2: What is the frequency of each frequency?
         *          (How many elements share the same frequency count?)
         * 
         * An element has "unique frequency" when its frequency appears only once
         * in the frequency distribution (i.e., no other element shares that count).
         */
        
        // STEP 1: Build frequency map for each element
        // Key: element value, Value: how many times it appears
        unordered_map<int, int> mp; 
        
        for(auto i : nums) {
            mp[i]++;
        }
        /* After this loop:
         * Example: [20,10,30,30]
         * mp = {20: 1, 10: 1, 30: 2}
         */
        
        
        // STEP 2: Build frequency-of-frequencies map (meta-frequency map)
        // Key: a frequency count, Value: how many elements have that frequency
        // This tells us which frequencies are unique (appear only once)
        unordered_map<int, int> ctfreq;
        
        for(auto j : mp) {
            ctfreq[j.second]++;  // j.second is the frequency of an element
        }
        /* After this loop:
         * Example: [20,10,30,30]
         * ctfreq = {1: 2, 2: 1}
         * Interpretation:
         * - Frequency 1 appears 2 times (both 20 and 10 have frequency 1)
         * - Frequency 2 appears 1 time (only 30 has frequency 2)
         * 
         * This means frequency 2 is UNIQUE, but frequency 1 is NOT unique.
         */
        
        
        /* STEP 3: Scan original array from left to right
         * WHY scan the original array instead of the map?
         * Because we need the FIRST element in the ORIGINAL ORDER.
         * Maps don't preserve insertion order, so we must scan nums[] to maintain
         * the left-to-right sequence.
         * 
         * For each element, check if its frequency is unique:
         * - Get the frequency of current element: mp[i]
         * - Check if this frequency appears only once: ctfreq[mp[i]] == 1
         * - If yes, this element has a unique frequency → return it immediately
         */
        for(auto i : nums) {
            // mp[i] gives us the frequency of element i
            // ctfreq[mp[i]] tells us how many elements share this frequency
            // If ctfreq[mp[i]] == 1, then no other element has this frequency
            if(ctfreq[mp[i]] == 1) {
                return i;  // Found the first element with unique frequency
            }
        }
        
        /* Example walkthrough: [20,10,30,30]
         * Iteration 1: i=20, mp[20]=1, ctfreq[1]=2 → NOT unique (skip)
         * Iteration 2: i=10, mp[10]=1, ctfreq[1]=2 → NOT unique (skip)
         * Iteration 3: i=30, mp[30]=2, ctfreq[2]=1 → UNIQUE! Return 30
         */
        
        // If no element with unique frequency exists, return -1
        return -1;
        
        /* TIME COMPLEXITY: O(n)
         * - Building mp: O(n)
         * - Building ctfreq: O(unique elements) ≤ O(n)
         * - Final scan: O(n)
         * Total: O(n)
         * 
         * SPACE COMPLEXITY: O(n)
         * - mp can have at most n entries (all unique elements)
         * - ctfreq can have at most n entries (all different frequencies)
         */
    }
};