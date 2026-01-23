// @https://leetcode.com/problems/find-the-largest-area-of-square-inside-two-rectangles/description/?envType=daily-question&envId=2026-01-17

class Solution {
    public:
        long long largestSquareArea(vector<vector<int>>& bottomLeft,
                                    vector<vector<int>>& topRight) {
    
            // We store the maximum square SIDE found so far.
            // It must be long long because the final area can be large (side^2).
            long long maxside = 0;
    
            int n = bottomLeft.size();
    
            // We must consider every pair of rectangles.
            // Why pairs are enough:
            // Any region formed by intersection of >=2 rectangles
            // will also appear as the intersection of at least one pair.
            for (int i = 0; i < n; i++) {
                for (int j = i + 1; j < n; j++) {
    
                    /*
                     * ------------------ CORE GEOMETRIC IDEA ------------------
                     *
                     * A rectangle is NOT a drawn shape.
                     * It is a set of constraints:
                     *
                     *   x >= left   and   x <= right
                     *   y >= bottom and   y <= top
                     *
                     * The intersection of two rectangles means:
                     *   x must satisfy ALL x-constraints
                     *   y must satisfy ALL y-constraints
                     *
                     * For lower bounds, the strictest constraint wins → max()
                     * For upper bounds, the strictest constraint wins → min()
                     *
                     * This is why max/min is forced by logic, not chosen.
                     */
    
                    // Compute the ONLY possible left and right x-boundaries
                    int bottomleftx = max(bottomLeft[i][0], bottomLeft[j][0]);
                    int toprightx   = min(topRight[i][0],   topRight[j][0]);
    
                    // Compute the ONLY possible bottom and top y-boundaries
                    int bottomlefty = max(bottomLeft[i][1], bottomLeft[j][1]);
                    int toprighty   = min(topRight[i][1],   topRight[j][1]);
    
                    /*
                     * At this point we have a CANDIDATE intersection region.
                     * However, max/min alone does NOT guarantee that
                     * any area actually exists.
                     *
                     * Example of failure without check:
                     *   x-range becomes [5, 2] → impossible
                     *   or [3, 3] → zero-width (line)
                     *
                     * Therefore we MUST verify that the intersection
                     * has positive width AND positive height.
                     *
                     * Strict < is required:
                     *   == would mean line or point (zero area)
                     */
                    if (bottomleftx < toprightx && bottomlefty < toprighty) {
    
                        // Now we know a real overlapping rectangle exists.
                        int width  = toprightx - bottomleftx;
                        int height = toprighty - bottomlefty;
    
                        /*
                         * ------------------ SQUARE LOGIC ------------------
                         *
                         * Inside this intersection rectangle, the largest
                         * axis-aligned square that can fit is limited by
                         * the smaller dimension.
                         *
                         * Why:
                         * A square must satisfy BOTH width and height.
                         * If side > width  → spills horizontally
                         * If side > height → spills vertically
                         *
                         * Therefore:
                         *   max square side = min(width, height)
                         */
                        int side = min(width, height);
    
                        /*
                         * We only care about the largest side globally.
                         * Invalid intersections never reach here.
                         *
                         * Explicit cast is required because:
                         *   maxside is long long
                         *   side is int
                         */
                        maxside = max(maxside, (long long) side);
                    }
                }
            }
    
            /*
             * We stored the maximum SIDE.
             * The problem asks for AREA.
             *
             * Area = side * side
             * long long is required to avoid overflow.
             */
            return maxside * maxside;
        }
    };
    