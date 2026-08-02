#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int maxArea(vector<int>& height) {
        int left = 0;
        int right = height.size() - 1;
        int maxWater = 0;

        // Start with the two farthest lines because they give us
        // the maximum possible width.
        while (left < right) {

            // Distance between the two selected lines.
            int width = right - left;

            // Water cannot rise above the shorter line,
            // otherwise it would overflow from that side.
            int containerHeight = min(height[left], height[right]);

            int currentArea = width * containerHeight;
            maxWater = max(maxWater, currentArea);

            // After moving a pointer, the width will always decrease.
            //
            // Therefore, to possibly get a larger area, we must try
            // to increase the container's height.
            //
            // The shorter line is currently limiting the height,
            // so we move that pointer and look for a taller line.
            //
            // Moving the taller line is useless because the shorter
            // line would still limit the height while width decreases.
            if (height[left] < height[right]) {
                left++;
            } else {
                right--;
            }
        }

        return maxWater;
    }
};