/*
 * ============================================================================
 * COMBINATION FUNCTION: C(n, k) = "n choose k"
 * ============================================================================
 * 
 * WHAT IT COMPUTES:
 * The number of ways to choose k items from n items (order doesn't matter)
 * 
 * MATHEMATICAL FORMULA:
 * C(n, k) = n! / (k! * (n-k)!)
 * 
 * Example: C(5, 2) = "Choose 2 items from 5"
 *   = 5! / (2! * 3!)
 *   = (5 × 4 × 3 × 2 × 1) / ((2 × 1) × (3 × 2 × 1))
 *   = 120 / (2 × 6)
 *   = 10 ways
 * 
 * WHY NOT COMPUTE FACTORIALS DIRECTLY?
 * Because factorials grow EXPLOSIVELY (100! is astronomically large).
 * Computing huge factorials and then dividing causes:
 *   1. Integer overflow
 *   2. Unnecessary computations (we cancel most terms anyway)
 * 
 * SMART INSIGHT:
 * Most factorial terms CANCEL OUT in the division!
 * 
 * Example: C(5, 2) = 5!/(2! × 3!)
 *   = (5 × 4 × 3 × 2 × 1) / ((2 × 1) × (3 × 2 × 1))
 *   = (5 × 4 × [3 × 2 × 1]) / ((2 × 1) × [3 × 2 × 1])
 *                └─────────┘              └─────────┘
 *                These cancel out!
 *   = (5 × 4) / (2 × 1)
 *   = 20 / 2 = 10
 * 
 * PATTERN: C(n, k) = (n × (n-1) × (n-2) × ... × (n-k+1)) / (k × (k-1) × ... × 1)
 *                     └──────── k terms ─────────┘          └─── k terms ───┘
 * ============================================================================
 */

long long nChoosek(long long n, long long k)
{
    // Edge case: Can't choose more items than we have
    // C(5, 7) = 0 (can't pick 7 things from 5)
    if (k > n) return 0;
    
    /* ========================================================================
     * OPTIMIZATION 1: Use Symmetry Property
     * ========================================================================
     * 
     * Key Insight: C(n, k) = C(n, n-k)
     * 
     * Why? Choosing k items to INCLUDE is the same as choosing (n-k) items 
     * to EXCLUDE.
     * 
     * Example: C(10, 8) = C(10, 2)
     *   Choosing 8 people to form a committee = Choosing 2 people to exclude
     * 
     * Benefit: Always compute the SMALLER of k or (n-k)
     *   C(100, 98) becomes C(100, 2) → only 2 multiplications instead of 98!
     */
    if (k * 2 > n) k = n - k;
    
    // Base case: Choosing 0 items = 1 way (the empty set)
    // C(n, 0) = 1 for any n
    if (k == 0) return 1;

    /* ========================================================================
     * CORE ALGORITHM: Iterative Multiplication and Division
     * ========================================================================
     * 
     * We're computing: (n × (n-1) × ... × (n-k+1)) / (1 × 2 × ... × k)
     * 
     * CRUCIAL: We interleave multiplication and division to prevent overflow
     * 
     * Why start result = n (not 1)?
     * Because the first term in the numerator is n, and we've already 
     * "used" the first denominator term (which is 1, so dividing by 1 = no-op)
     * 
     * Loop from i=2 to i=k:
     *   - Multiply by (n-i+1) → adds next numerator term
     *   - Divide by i → adds next denominator term
     * 
     * Example Walkthrough: C(5, 2)
     *   Target: (5 × 4) / (1 × 2)
     *   
     *   Initial: result = 5 (we start with n, implicitly divided by 1)
     *   
     *   i=2:
     *     result *= (n-i+1) = (5-2+1) = 4  → result = 5 × 4 = 20
     *     result /= i = 2                   → result = 20 / 2 = 10
     *   
     *   Done! Result = 10 ✓
     * 
     * WHY THIS ORDER (multiply then divide each iteration)?
     * To keep intermediate results as SMALL as possible, minimizing overflow risk.
     * 
     * Mathematical Guarantee: After each division, result is ALWAYS an integer
     * because C(n,k) is always a whole number (you can't have 3.5 ways to 
     * choose items). The formula ensures divisibility at every step.
     */
    long long result = n;
    
    for(int i = 2; i <= k; ++i) {
        // Add next numerator term: (n-i+1)
        // When i=2: (n-1), when i=3: (n-2), ..., when i=k: (n-k+1)
        result *= (n - i + 1);
        
        // Add next denominator term: i
        // Interleaving division keeps numbers manageable
        result /= i;
    }
    
    return result;
}

/*
 * ============================================================================
 * WHY (n-i+1) GENERATES THE CORRECT NUMERATOR SEQUENCE?
 * ============================================================================
 * 
 * We want: n, (n-1), (n-2), ..., (n-k+1)
 * 
 * Starting with result = n handles the first term.
 * 
 * For i from 2 to k:
 *   i=2: (n-2+1) = n-1  ✓
 *   i=3: (n-3+1) = n-2  ✓
 *   i=4: (n-4+1) = n-3  ✓
 *   ...
 *   i=k: (n-k+1)        ✓  (the last term we need)
 * 
 * Perfect! The formula (n-i+1) counts down from (n-1) to (n-k+1).
 * 
 * ============================================================================
 * COMPLEXITY ANALYSIS
 * ============================================================================
 * 
 * Time Complexity: O(min(k, n-k))
 *   We optimized k to always be ≤ n/2, so at most O(n/2) = O(n)
 * 
 * Space Complexity: O(1)
 *   Only using a single variable
 * 
 * Overflow Safety: Good for moderate values
 *   For very large n,k, might need BigInteger or modular arithmetic
 * ============================================================================
 */