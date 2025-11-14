// @https://leetcode.com/problems/maximum-points-you-can-obtain-from-cards/

/*
Problem: You can take exactly k cards from either the beginning or end of the array.
Find the maximum sum of points you can obtain.

Approach: Sliding Window (Reverse Thinking)
Instead of finding the best k cards to take, we find the worst (n-k) cards to leave.
The remaining cards will be our answer.

Key Insight:
- We must take exactly k cards
- We can only take from front or back
- This means we must leave exactly (n-k) consecutive cards in the middle
- We want to leave the minimum sum of (n-k) consecutive cards
- Answer = Total sum - Minimum sum of (n-k) consecutive cards
*/

class Solution
{
public:
    int maxScore(vector<int> &cardPoints, int k)
    {
        int n = cardPoints.size();

        // Calculate total sum of all cards
        int total = 0;
        for (auto i : cardPoints)
        {
            total += i;
        }

        // If we can take all cards, return total sum
        if (k >= n)
        {
            return total;
        }

        // Size of the window we want to leave (the cards we don't take)
        int windowSize = n - k;

        // Calculate sum of first window (first windowSize elements)
        int currentSum = 0;
        for (int i = 0; i < windowSize; i++)
        {
            currentSum += cardPoints[i];
        }

        // Track minimum sum of any window of size windowSize
        int minWindowSum = currentSum;

        // Slide the window and find minimum sum
        // For each position, add new element and remove oldest element
        for (int i = windowSize; i < n; i++)
        {
            // Add new element to window
            currentSum += cardPoints[i];
            // Remove oldest element from window
            currentSum -= cardPoints[i - windowSize];

            // Update minimum window sum
            minWindowSum = min(minWindowSum, currentSum);
        }

        // Return total sum minus the minimum window sum
        // This gives us the maximum sum of k cards we can take
        return total - minWindowSum;
    }
};

/*
Example Walkthrough:
cardPoints = [1, 2, 3, 4, 5, 6, 1], k = 3
n = 7, windowSize = 7 - 3 = 4

Total sum = 1+2+3+4+5+6+1 = 22

We need to find minimum sum of 4 consecutive cards:

Window 1: [1,2,3,4] = 10
Window 2: [2,3,4,5] = 14
Window 3: [3,4,5,6] = 18
Window 4: [4,5,6,1] = 16

Minimum window sum = 10

Answer = 22 - 10 = 12

This means we take cards [5,6,1] (from end) = 12 points
*/
