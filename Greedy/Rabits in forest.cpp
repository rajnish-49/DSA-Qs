//@https://leetcode.com/problems/rabbits-in-forest/?envType=daily-question&envId=2025-04-20

class Solution {
    public:
        int numRabbits(vector<int>& answers) {
    
            unordered_map<int, int> mp; // Map to store frequency of each answer
            int n = answers.size();
            int rab = 0; // Total minimum number of rabbits in the forest
    
            // Step 1: Count how many times each answer appears
            for (auto i : answers) {
                mp[i]++; // mp[answer] = count of rabbits that gave this answer
            }
    
            // Step 2: For each unique answer, calculate how many rabbits are required
            for (auto i : mp) {
                int ans = i.first;   // The answer value (i.e., how many *other* rabbits of the same color)
                int count = i.second; // Number of rabbits that gave this answer
    
                int groupsize = (ans + 1); 
                // A rabbit saying "x" means there must be x+1 rabbits of that color including itself
    
                int groups = ceil((double)count / groupsize);
                // We divide the total count of such rabbits into groups of size (x+1)
                // ceil is used because if count is not exactly divisible, we still need a full group
    
                rab += groups * groupsize;
                // Add the total rabbits for all such groups
            }
    
            return rab; // Final answer: Minimum number of rabbits possible
        }
    };
    