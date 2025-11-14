// @ https://leetcode.com/problems/remove-all-occurrences-of-a-substring/description/

class Solution {
    public:
        string removeOccurrences(string s, string part) {
            
            // Keep removing 'part' from 's' as long as it exists in 's'
            while (s.find(part) != string::npos) {
                // Find the first occurrence of 'part' and erase it from 's'
                s.erase(s.find(part), part.length());
            }
    
            // Return the modified string after all occurrences are removed
            return s;
        }
    };

    
    class Solution {
        public:
            string removeOccurrences(string s, string part) {
                string result;  // Acts as a stack
                
                for (char c : s) {
                    result.push_back(c);  // Add character to result
                    
                    // Check if the last part of result matches 'part'
                    if (result.size() >= part.size() && result.substr(result.size() - part.size()) == part) {
                        result.erase(result.size() - part.size());  // Remove 'part'
                    }
                }
                
                return result;
            }
        };
        