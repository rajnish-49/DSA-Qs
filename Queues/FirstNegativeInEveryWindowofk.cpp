class Solution {
  public:
    // Function to find the first negative integer in every window of size 'k'
    vector<int> FirstNegativeInteger(vector<int>& arr, int k) {
        // deque to store indices of negative elements
        deque<int> dq;
        vector<int> ans; //ans to store answer

        // Process the first window of size k
        for (int i = 0; i < k; i++) {
            // If the current element is negative, store its index in deque
            if (arr[i] < 0) {
                dq.push_back(i);
            }
        }

        // If there are negative elements in the first window, take the first one
        if (dq.size() > 0) {
            ans.push_back(arr[dq.front()]);
        } else {
            // If no negative elements, append 0
            ans.push_back(0);
        }

        // Process the rest of the windows
        for (int i = k; i < arr.size(); i++) {
            // Remove indices that are out of the current window
            if (!dq.empty() && i - dq.front() >= k) {
                dq.pop_front();
            }

            // If the current element is negative, store its index in deque
            if (arr[i] < 0) {
                dq.push_back(i);
            }

            // If there are any negative elements in the window, take the first one
            if (dq.size() > 0) {
                ans.push_back(arr[dq.front()]);
            } else {
                // If no negative elements, append 0
                ans.push_back(0);
            }
        }
        // Return the result vector
        return ans;
    }
};