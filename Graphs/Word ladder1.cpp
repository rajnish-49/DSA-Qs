// @ https://leetcode.com/problems/word-ladder/

class Solution {
    public:
        int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
            // Create an unordered_set to store all words from wordList for quick lookup
            unordered_set<string> st(wordList.begin(), wordList.end());
    
            // If endWord is not in wordList, no transformation sequence is possible
            if (st.find(endWord) == st.end()) {
                return 0;
            }
    
            // BFS queue stores pairs of (word, number of transformation steps)
            queue<pair<string, int>> q;
            q.push({beginWord, 1});  // Start BFS from beginWord with step count = 1
    
            // Remove beginWord from the set to avoid revisiting it
            st.erase(beginWord);
    
            while (!q.empty()) {
                string word = q.front().first;  // Get the current word from queue
                int steps = q.front().second;   // Get the step count for this word
                q.pop();  // Remove the processed word from queue
    
                // If we reach the endWord, return the step count
                if (word == endWord) {
                    return steps;
                }
    
                // Try changing each character of the current word one at a time
                for (int i = 0; i < word.size(); i++) {
                    char originalChar = word[i];  // Store original character
    
                    // Replace with every possible lowercase letter 'a' to 'z'
                    for (char ch = 'a'; ch <= 'z'; ch++) {
                        word[i] = ch;  // Modify the character
    
                        // If the modified word exists in the set, it's a valid transformation
                        if (st.find(word) != st.end()) {
                            st.erase(word);  // Remove the word from set to avoid revisiting
                            q.push({word, steps + 1});  // Push new word with incremented step count
                        }
                    }
    
                    // Restore original character before moving to next position
                    word[i] = originalChar;
                }
            }
    
            // If no valid transformation sequence found, return 0
            return 0;
        }
    };
    