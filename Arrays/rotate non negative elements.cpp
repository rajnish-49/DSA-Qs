// @https://leetcode.com/problems/rotate-non-negative-elements/

class Solution { 
public: 
    vector<int> rotateElements(vector<int>& nums, int k) { 
        /*
         * CORE CONCEPT: Three-Phase Rotation Strategy
         * ============================================
         * We can't rotate elements in-place because negative numbers act as "barriers"
         * that must stay fixed. So we use a 3-step approach:
         * 
         * Phase 1: EXTRACT - Pull out all non-negative elements (they're free to move)
         * Phase 2: ROTATE - Apply left rotation to these extracted elements
         * Phase 3: INJECT - Put rotated elements back into their non-negative slots
         * 
         * Think of it like: removing all movable pieces from a board, shuffling them,
         * then placing them back into the movable positions only.
         */
        
        int n = nums.size();
        vector<int> temp;
 
        // ========== PHASE 1: EXTRACTION ==========
        /* 
         * WHY: We need to isolate elements that can be rotated.
         * Negative elements are "anchored" - they can't move from their positions.
         * Only non-negative elements (>= 0, includes zero!) participate in rotation.
         * 
         * Example: [1, -2, 3, -4, 5]
         *          Extract: [1, 3, 5]  (these can rotate)
         *          Anchored: -2 at index 1, -4 at index 3 (these stay put)
         */
        for(int i = 0; i < n; i++){ 
            if(nums[i] >= 0){  // >= not > because 0 is non-negative!
                temp.push_back(nums[i]); 
            } 
        }
 
        int fs = temp.size();  // fs = count of elements that will rotate
        
        // Edge case: If no non-negative elements exist, nothing to rotate
        if(fs == 0){ 
            return nums;  // Return original array unchanged
        }
 
        // ========== PHASE 2: ROTATION ==========
        /*
         * LEFT ROTATION FORMULA: new[i] = old[(i + k) % size]
         * 
         * WHY THIS WORKS:
         * Left rotation by k means "start reading from position k onwards"
         * Think of the array as circular (like a clock):
         * 
         * temp = [A, B, C, D], k = 2
         * 
         * Reading normally:     A, B, C, D
         * Reading from index 2: C, D, A, B  (this is left rotation by 2!)
         * 
         * For each position i in the NEW array, we read from position (i+k) in OLD array:
         * new[0] = temp[(0+2)%4] = temp[2] = C
         * new[1] = temp[(1+2)%4] = temp[3] = D
         * new[2] = temp[(2+2)%4] = temp[0] = A (wrapped around using %)
         * new[3] = temp[(3+2)%4] = temp[1] = B (wrapped around using %)
         * 
         * The % operator handles wraparound when (i+k) exceeds array bounds
         */
        vector<int> final(fs); 
        for (int i = 0; i < fs; i++) { 
            final[i] = temp[(i + k) % fs];   
            /*
             * Why modulo? When (i + k) >= fs, we need to wrap to beginning
             * Example: i=3, k=2, fs=4 → (3+2)%4 = 5%4 = 1 (wraps to index 1)
             */
        }
 
        // ========== PHASE 3: INJECTION ==========
        /*
         * WHY: Now we have rotated elements, but they need to go back into the 
         * SAME POSITIONS where non-negative elements originally were.
         * 
         * Critical insight: We iterate through original array's positions,
         * and whenever we find a non-negative position (>= 0), we fill it
         * with the NEXT element from our rotated array.
         * 
         * Example:
         * Original: [1, -2, 3, -4, 5]
         * After Phase 1: temp = [1, 3, 5]
         * After Phase 2: final = [3, 5, 1] (rotated)
         * 
         * Now inject back:
         * Position 0: nums[0]=1 (>=0), replace with final[0]=3 → [3, -2, 3, -4, 5]
         * Position 1: nums[1]=-2 (<0), SKIP (negative stays) → [3, -2, 3, -4, 5]
         * Position 2: nums[2]=3 (>=0), replace with final[1]=5 → [3, -2, 5, -4, 5]
         * Position 3: nums[3]=-4 (<0), SKIP (negative stays) → [3, -2, 5, -4, 5]
         * Position 4: nums[4]=5 (>=0), replace with final[2]=1 → [3, -2, 5, -4, 1]
         * 
         * Result: [3, -2, 5, -4, 1] ✓
         */
        int idx = 0;  // Tracks which element from 'final' to use next
 
        for(int x = 0; x < n; x++){ 
            if(nums[x] >= 0){  
                /*
                 * CRITICAL: Must use >= 0 (same condition as extraction!)
                 * If we used > 0, zeros would be extracted but not re-injected!
                 * This would cause a position mismatch and wrong answer.
                 * 
                 * Rule: Extract condition MUST match injection condition
                 */
                nums[x] = final[idx];  // Replace with next rotated element
                idx++;  // Move to next element in rotated array
            } 
            // If nums[x] < 0, do nothing - negative elements never move!
        }
 
        return nums; 
    } 
};

/*
 * ========== COMPLEXITY ANALYSIS ==========
 * Time: O(n)
 *   - Phase 1 (extract): O(n) - scan entire array once
 *   - Phase 2 (rotate): O(m) where m = count of non-negative elements, m ≤ n
 *   - Phase 3 (inject): O(n) - scan entire array once
 *   Total: O(n) + O(m) + O(n) = O(n)
 * 
 * Space: O(m) where m = count of non-negative elements
 *   - temp array: O(m)
 *   - final array: O(m)
 *   - In worst case all elements are non-negative, so O(n)
 * 
 * ========== KEY INSIGHTS TO REMEMBER ==========
 * 1. Negative elements are "anchors" - they never move from their positions
 * 2. Rotation only affects non-negative elements in their own "subarray"
 * 3. Left rotation by k = reading from index k in circular fashion
 * 4. Extract and inject conditions MUST be identical (both >= 0)
 * 5. The % operator is crucial for handling circular wraparound
 */