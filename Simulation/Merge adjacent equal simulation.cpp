class Solution {
public:
    vector<long long> mergeAdjacent(vector<int>& nums) {
        /*
        ═══════════════════════════════════════════════════════════════════════════
        PROBLEM STATEMENT:
        ═══════════════════════════════════════════════════════════════════════════
        Repeatedly merge adjacent equal elements by replacing them with their sum.
        Always choose the LEFTMOST pair of equal adjacent elements.
        Continue until NO adjacent equal elements remain.
        
        Example 1: [3,1,1,2]
        Step 1: [3, 1, 1, 2] → Find leftmost equal pair (1,1) → [3, 2, 2]
        Step 2: [3, 2, 2]    → Find leftmost equal pair (2,2) → [3, 4]
        Step 3: [3, 4]       → No equal pairs → DONE
        Answer: [3, 4]
        
        Example 2: [2,2,4] - demonstrates CASCADING MERGES
        Step 1: [2, 2, 4] → Merge (2,2) → [4, 4]
        Step 2: [4, 4]    → Merge (4,4) → [8]    ← New pair formed!
        Answer: [8]
        
        ═══════════════════════════════════════════════════════════════════════════
        THE CRITICAL INSIGHT - CASCADING MERGES:
        ═══════════════════════════════════════════════════════════════════════════
        When we merge two elements, the RESULTING SUM might equal the element
        that came BEFORE them, creating a NEW merge opportunity!
        
        Visual Example: [1, 1, 2, 4]
        
        Process 1st element (1):
            Stack: [1]
        
        Process 2nd element (1):
            Current = 1
            Stack top = 1 (EQUAL!)
            Merge: 1 + 1 = 2
            Stack: [2]  ← New element!
        
        Process 3rd element (2):
            Current = 2
            Stack top = 2 (EQUAL!)  ← The 2 we just created!
            Merge: 2 + 2 = 4
            Stack: [4]  ← Another new element!
        
        Process 4th element (4):
            Current = 4
            Stack top = 4 (EQUAL!)  ← The 4 we just created!
            Merge: 4 + 4 = 8
            Stack: [8]  ← Cascading continues!
        
        Final: [8]
        
        This cascading behavior is WHY we need a WHILE loop, not just an IF!
        
        ═══════════════════════════════════════════════════════════════════════════
        WHY STACK? THE FUNDAMENTAL PATTERN:
        ═══════════════════════════════════════════════════════════════════════════
        
        This is a MONOTONIC STACK problem pattern where:
        1. We process elements LEFT TO RIGHT (maintaining order)
        2. Each new element can AFFECT previously processed elements
        3. We need to check BACKWARDS to see if merging is possible
        
        The stack stores "finalized elements that haven't merged yet".
        When a new element arrives, we check if it can merge with the TOP.
        If yes, we keep merging BACKWARDS until no more merges possible.
        
        Think of it like Tetris blocks falling and combining!
        
        ═══════════════════════════════════════════════════════════════════════════
        BRUTE FORCE vs STACK APPROACH:
        ═══════════════════════════════════════════════════════════════════════════
        
        BRUTE FORCE (O(n²)):
        - Scan entire array repeatedly
        - Find first equal pair, merge them
        - Restart from beginning
        - Repeat until no changes
        Problem: After each merge, we rescan everything!
        
        STACK APPROACH (O(n)) - THIS SOLUTION:
        - Process each element ONCE from left to right
        - Use stack to remember "what we've built so far"
        - When new element arrives, check if it can merge with TOP
        - Keep merging BACKWARDS in stack until no more matches
        - Each element pushed once, popped at most once → O(n) total
        
        ═══════════════════════════════════════════════════════════════════════════
        WHY "current" VARIABLE INSTEAD OF DIRECT STACK MANIPULATION?
        ═══════════════════════════════════════════════════════════════════════════
        
        The "current" variable accumulates all the merges for this position:
        
        Example: Stack = [8], processing element 2
        - current = 2 initially
        - Can't merge with 8, so push 2
        - Stack = [8, 2]
        
        Example: Stack = [2, 2], processing element 2
        - current = 2 initially
        - Merge with top: current = 2 + 2 = 4, pop stack → Stack = [2]
        - Merge with top: current = 4 + 2 = 6, pop stack → Stack = []
        - Push 6
        - Stack = [6]
        
        The "current" acts as an ACCUMULATOR that gathers all cascading merges
        before we commit the final value to the stack.
        
        ═══════════════════════════════════════════════════════════════════════════
        TIME COMPLEXITY: O(n)
        ═══════════════════════════════════════════════════════════════════════════
        Why O(n) when we have a WHILE loop inside a FOR loop?
        
        KEY INSIGHT: Amortized Analysis
        - Each element is PUSHED to stack exactly ONCE (in ans.push_back)
        - Each element is POPPED from stack at most ONCE (in while loop)
        - Total operations: n pushes + at most n pops = 2n = O(n)
        
        Even though while loop runs multiple times for some elements,
        ACROSS ALL iterations of the for loop, total while loop iterations ≤ n
        
        Example trace for [1,1,2,4]:
        Element 1: 0 pops, 1 push
        Element 1: 1 pop,  1 push  (total pops so far: 1)
        Element 2: 1 pop,  1 push  (total pops so far: 2)
        Element 4: 1 pop,  1 push  (total pops so far: 3)
        Total: 3 pops, 4 pushes = 7 operations for 4 elements
        
        ═══════════════════════════════════════════════════════════════════════════
        SPACE COMPLEXITY: O(n)
        ═══════════════════════════════════════════════════════════════════════════
        In worst case (no merges possible), stack contains all n elements.
        Example: [1, 2, 3, 4, 5] → no adjacent equals → stack = [1,2,3,4,5]
        
        ═══════════════════════════════════════════════════════════════════════════
        */
        
        vector<long long> ans;  // Stack to store our result
                                // Using long long because merging can create large sums
                                // Example: [100000, 100000] → 200000 (still fits)
                                // But repeated merges can overflow int!
       
        // Process each element from LEFT to RIGHT (maintaining order requirement)
        for(auto i : nums){
            
            // ACCUMULATOR PATTERN:
            // "current" will hold the value after all possible merges
            // It starts as the element itself, but might grow if merges happen
            long long current = i;
            
            // CRITICAL WHILE LOOP - handles CASCADING MERGES
            // 
            // WHY WHILE and not IF?
            // ═══════════════════════════════════════════════════════════════
            // After one merge, the NEW SUM might equal the PREVIOUS stack top!
            // We need to keep checking BACKWARDS until no more merges possible.
            //
            // Example: Stack = [2, 2], current = 2
            // 1st iteration: current=2, top=2 → merge to 4, stack=[2]
            // 2nd iteration: current=4, top=2 → no match, exit
            //
            // Example: Stack = [4], current = 4
            // 1st iteration: current=4, top=4 → merge to 8, stack=[]
            // 2nd iteration: stack empty, exit
            //
            // This continues until either:
            // a) Stack becomes empty (all elements merged into current)
            // b) Top of stack ≠ current (no more merges possible)
            while(!ans.empty() && ans.back() == current){
                
                // MERGE OPERATION:
                // Add the top of stack to our current accumulator
                // This represents: "combine current element with previous element"
                current += ans.back();
                
                // REMOVE the merged element from stack
                // It's now been "consumed" into current
                ans.pop_back();
                
                // Loop continues to check if the NEW current value
                // can merge with what's now on top of the stack
                // This is the CASCADING effect!
            }
            
            // After all possible merges, push the FINAL accumulated value
            // This value is "stable" - it doesn't equal anything below it in stack
            // So it sits in the stack waiting for future elements to potentially merge with it
            ans.push_back(current);
        }

        // The stack now contains the final array after all merges
        // Elements in stack are guaranteed to have NO adjacent equals
        // because we merged all equal pairs as we went
        return ans;
    }
};

/*
═══════════════════════════════════════════════════════════════════════════
DETAILED EXECUTION TRACE - EXAMPLE: [2,2,4]
═══════════════════════════════════════════════════════════════════════════

Initial state:
    nums = [2, 2, 4]
    ans = []

───────────────────────────────────────────────────────────────────────────
ITERATION 1: Processing nums[0] = 2
───────────────────────────────────────────────────────────────────────────
    current = 2
    
    Check WHILE condition: ans.empty()? YES
        → WHILE loop doesn't run
    
    ans.push_back(2)
    ans = [2]

───────────────────────────────────────────────────────────────────────────
ITERATION 2: Processing nums[1] = 2
───────────────────────────────────────────────────────────────────────────
    current = 2
    
    Check WHILE condition:
        ans.empty()? NO
        ans.back() = 2, current = 2, EQUAL? YES
        → Enter WHILE loop
    
    WHILE iteration 1:
        current += ans.back()  → current = 2 + 2 = 4
        ans.pop_back()         → ans = []
    
    Check WHILE condition again:
        ans.empty()? YES
        → Exit WHILE loop
    
    ans.push_back(4)
    ans = [4]
    
    ⚡ KEY MOMENT: The merge created a 4, which will match the next element!

───────────────────────────────────────────────────────────────────────────
ITERATION 3: Processing nums[2] = 4
───────────────────────────────────────────────────────────────────────────
    current = 4
    
    Check WHILE condition:
        ans.empty()? NO
        ans.back() = 4, current = 4, EQUAL? YES
        → Enter WHILE loop
    
    WHILE iteration 1:
        current += ans.back()  → current = 4 + 4 = 8
        ans.pop_back()         → ans = []
    
    Check WHILE condition again:
        ans.empty()? YES
        → Exit WHILE loop
    
    ans.push_back(8)
    ans = [8]
    
    ⚡ CASCADING MERGE: The original merge (2+2=4) enabled this merge (4+4=8)!

───────────────────────────────────────────────────────────────────────────
FINAL RESULT: ans = [8]
───────────────────────────────────────────────────────────────────────────

═══════════════════════════════════════════════════════════════════════════
REVISION CHECKLIST:
═══════════════════════════════════════════════════════════════════════════
✓ Understand WHY we need a WHILE loop (cascading merges)
✓ Know the "current" accumulator pattern
✓ Remember: Each element pushed once, popped at most once → O(n)
✓ Stack stores "finalized, non-mergeable elements so far"
✓ Process left to right to maintain "leftmost" requirement
✓ Use long long to prevent overflow from repeated merges
✓ Compare with brute force: repeated scans vs single pass

PATTERN: "Process left to right, but previous elements can be affected by current"
═══════════════════════════════════════════════════════════════════════════
*/