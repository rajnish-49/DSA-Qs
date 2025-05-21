// @https://leetcode.com/problems/search-a-2d-matrix/description/

class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {

        int r = matrix.size();
        int c= matrix[0].size();

        int low =0;
        int high= (r*c)-1;

        while (low <= high) {
        int mid = (low + high) / 2;
        int mid_value = matrix[mid / c][mid % c];

        if (mid_value == target) {
            return true;
        } else if (mid_value < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return false;
}


        
        
    
};

