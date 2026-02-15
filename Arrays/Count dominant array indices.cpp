class Solution {
public:
    int dominantIndices(vector<int>& nums) {
        /*
        ═══════════════════════════════════════════════════════════════════════════
        PROBLEM STATEMENT:
        ═══════════════════════════════════════════════════════════════════════════
        An index i is "dominant" if: nums[i] > average(all elements after index i)
        
        Example: [5, 4, 3]
        - Index 0: Is 5 > avg(4,3)? Is 5 > 3.5? YES → dominant
        - Index 1: Is 4 > avg(3)?   Is 4 > 3?   YES → dominant  
        - Index 2: No elements after it → NOT dominant (by definition)
        
        Answer: 2 dominant indices
        
        ═══════════════════════════════════════════════════════════════════════════
        KEY INSIGHT - THE CORE MATHEMATICAL TRANSFORMATION:
        ═══════════════════════════════════════════════════════════════════════════
        Instead of computing: nums[i] > (sum of right elements) / (count of right elements)
        
        We rearrange to AVOID DIVISION and floating point arithmetic:
        nums[i] > rightSum / rightCount
        ⟹ nums[i] * rightCount > rightSum    (multiply both sides by rightCount)
        
        This is CRUCIAL because:
        1. Avoids floating point precision errors
        2. Faster computation (multiplication vs division)
        3. Works with integers only
        
        ═══════════════════════════════════════════════════════════════════════════
        APPROACH: PREFIX SUM WITH TOTAL SUM TECHNIQUE
        ═══════════════════════════════════════════════════════════════════════════
        
        The brilliant insight: If we know the TOTAL sum and PREFIX sum, we can
        calculate the RIGHT sum efficiently without recalculating every time!
        
        Visual representation for array [5, 4, 3]:
        
        At index i=0:
        [5] | [4, 3]
         ↑     ↑
        cur   right side (what we need)
        
        rightSum = totalSum - prefixSum - current
                 = (5+4+3) - 0 - 5 
                 = 12 - 0 - 5 = 7
        
        At index i=1:
        [5, 4] | [3]
            ↑     ↑
           cur   right side
        
        rightSum = totalSum - prefixSum - current
                 = 12 - 5 - 4 = 3
        
        WHY THIS WORKS:
        - totalSum = prefix + current + right
        - Rearranging: right = totalSum - prefix - current
        - This gives us O(1) access to right sum at each step!
        
        ═══════════════════════════════════════════════════════════════════════════
        TIME COMPLEXITY: O(n) - two passes through array
        SPACE COMPLEXITY: O(1) - only using a few variables
        ═══════════════════════════════════════════════════════════════════════════
        */

        int n = nums.size();
        int total = 0;  // Sum of ALL elements in the array
        int ans = 0;    // Counter for dominant indices
        
        // STEP 1: Calculate total sum of all elements
        // This is our "anchor" - knowing this lets us calculate any subarray sum
        // by subtraction rather than repeated addition
        for(auto i: nums){
            total += i;
        }
        
        int psum = 0;  // Prefix sum: sum of all elements BEFORE current index
        
        // STEP 2: Iterate through each index and check dominance
        for(int i = 0 ; i < n ; i++){
            
            // CALCULATE RIGHT SUM using the mathematical relationship:
            // Array = [prefix elements] [current] [right elements]
            // total = psum + nums[i] + rsum
            // Therefore: rsum = total - psum - nums[i]
            int rsum = total - psum - nums[i];
            
            // COUNT of elements on the right side
            // If we're at index i, elements after are: i+1, i+2, ..., n-1
            // That's (n-1) - (i+1) + 1 = n - i - 1 elements
            int right = n - i - 1;
            
            // CHECK DOMINANCE CONDITION
            // Original: nums[i] > rsum / right
            // Transformed: nums[i] * right > rsum
            // 
            // EDGE CASE: When i = n-1 (last element), right = 0
            // So nums[i] * 0 = 0, and 0 > rsum will be false (correct behavior!)
            // This naturally handles the "last element is never dominant" rule
            if(nums[i] * right > rsum){
                ans++;
            }
            
            // UPDATE PREFIX SUM for next iteration
            // After processing index i, nums[i] becomes part of the prefix
            // for the next index i+1
            psum += nums[i];
        }
        
        return ans;
    }
};

/*
═══════════════════════════════════════════════════════════════════════════
ALTERNATIVE APPROACHES TO CONSIDER:
═══════════════════════════════════════════════════════════════════════════

APPROACH 1: BRUTE FORCE (Nested Loop)
────────────────────────────────────────
Time: O(n²), Space: O(1)

for i in 0 to n-2:
    sum = 0
    for j in i+1 to n-1:
        sum += nums[j]
    if nums[i] > sum / (n-i-1):
        count++

PROS: Most intuitive, easy to understand
CONS: Recalculates sum for each index (inefficient)

────────────────────────────────────────
APPROACH 2: SUFFIX SUM ARRAY
────────────────────────────────────────
Time: O(n), Space: O(n)

Build array: suffixSum[i] = sum from index i to end
Then: rightSum = suffixSum[i+1]

PROS: Very clear what each value represents
CONS: Uses extra O(n) space

────────────────────────────────────────
APPROACH 3: CURRENT SOLUTION (Best!)
────────────────────────────────────────
Time: O(n), Space: O(1)

Use: rightSum = totalSum - prefixSum - current

PROS: Optimal time and space, elegant
CONS: Requires understanding the mathematical relationship

═══════════════════════════════════════════════════════════════════════════
REVISION CHECKLIST:
═══════════════════════════════════════════════════════════════════════════
✓ Understand why we multiply instead of divide (avoid floating point)
✓ Know the formula: rightSum = total - prefix - current
✓ Remember: rightCount = n - i - 1
✓ Last element is never dominant (handled automatically by rightCount = 0)
✓ Prefix sum updates AFTER checking current index
✓ This pattern (total - prefix = suffix) is useful in many array problems!

SIMILAR PROBLEMS WHERE THIS TECHNIQUE APPLIES:
- Product of Array Except Self
- Subarray Sum Equals K (with modifications)
- Partition Array into Two Equal Sum Subarrays
═══════════════════════════════════════════════════════════════════════════
*/