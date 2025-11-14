// @https://leetcode.com/problems/bag-of-tokens/

class Solution {
public:
    int bagOfTokensScore(vector<int>& tokens, int power) {
        // Sort tokens so we can use the smallest for score
        // and the largest for regaining power
        sort(tokens.begin(), tokens.end());

        int l = 0, r = tokens.size() - 1;
        int score = 0, maxScore = 0;

        while (l <= r) {
            if (tokens[l] <= power) {
                // Case 1: If we have enough power, play the smallest token face-up
                // → lose tokens[l] power, gain +1 score
                power -= tokens[l];
                score++;
                maxScore = max(maxScore, score); // track best score achieved
                l++;
            } 
            else if (score > 0 && l < r) {
                // Case 2: If we are out of power but have score,
                // play the largest token face-down
                // → gain tokens[r] power, lose -1 score
                power += tokens[r];
                score--;
                r--;
            } 
            else {
                // Case 3: No beneficial moves left
                // → break early to avoid unnecessary plays
                break;
            }
        }

        return maxScore; // maximum score reached during the process
    }
};
