// @ https://www.geeksforgeeks.org/problems/subset-sums2234/1&selectedLang=python3

//{ Driver Code Starts
#include <bits/stdc++.h>
using namespace std;

// } Driver Code Ends
class Solution {
  public:
  
  // Helper function to solve the problem recursively
  void solve(int index, vector<int>& arr, int &track, vector<int>& ans) {
      // Base case: when the index reaches the end of the array
      if (index == arr.size()) {
          // Push the current value of track (which represents the subset sum) into the result
          ans.push_back(track);
          return;
      }

      // Store the current element of the array
      int curr = arr[index];

      // Include the current element in the subset
      track += curr;
      
      // Recursively call solve, moving to the next element with the current element included
      solve(index + 1, arr, track, ans);

      // After returning from the recursive call, undo the inclusion of the current element
      track -= curr;
      
      // Recursively call solve, moving to the next element without including the current element
      solve(index + 1, arr, track, ans);
  }
  
  // Function to return the subset sums
  vector<int> subsetSums(vector<int>& arr) {
      // Vector to store the final answers (subset sums)
      vector<int> ans;
      
      // Variable to keep track of the current subset sum
      int track = 0;
      
      // Start the recursive function call from the 0th index with track initialized to 0
      solve(0, arr, track, ans);
      
      // Return the final answer with all subset sums
      return ans;
  }
};