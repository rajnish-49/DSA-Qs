#include <iostream>
#include <vector>
#include <climits> // For INT_MAX
using namespace std;

int minSubArrayLen(int target, vector<int>& arr) {
    int left = 0, sum = 0, minLength = INT_MAX;

    for (int right = 0; right < arr.size(); right++) {
        sum += arr[right];  // Expand the window

        while (sum >= target) {  // Shrink the window
            minLength = min(minLength, right - left + 1);
            sum -= arr[left];  
            left++;
        }
    }

    return (minLength == INT_MAX) ? 0 : minLength;
}

int main() {
    vector<int> arr = {2, 3, 1, 2, 4, 3};
    int target = 7;
    cout << "Minimum subarray length: " << minSubArrayLen(target, arr) << endl;
    return 0;
}
