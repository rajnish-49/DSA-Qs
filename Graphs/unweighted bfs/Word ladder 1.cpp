class Solution {
public:
    int ladderLength(
        string beginWord,
        string endWord,
        vector<string>& wordList
    ) {
        // Words are nodes, and a valid one-character change is an edge.
        // Since every transformation costs 1, BFS finds the shortest sequence.
        unordered_set<string> words(wordList.begin(), wordList.end());

        // The final transformed word must be present in the dictionary.
        if (!words.count(endWord)) {
            return 0;
        }

        // Store {word, number of words in its transformation sequence}.
        // beginWord itself makes the initial sequence length 1.
        queue<pair<string, int>> q;
        q.push({beginWord, 1});

        // The set contains only valid words that have not been visited yet.
        words.erase(beginWord);

        while (!q.empty()) {
            string currentWord = q.front().first;
            int sequenceLength = q.front().second;
            q.pop();

            // BFS reaches words in increasing sequence length,
            // so the first arrival at endWord gives the shortest sequence.
            if (currentWord == endWord) {
                return sequenceLength;
            }

            // Generate graph neighbours directly instead of comparing
            // currentWord with every word in wordList.
            for (int i = 0; i < currentWord.size(); i++) {
                char originalCharacter = currentWord[i];

                for (char ch = 'a'; ch <= 'z'; ch++) {
                    if (ch == originalCharacter) {
                        continue;
                    }

                    currentWord[i] = ch;

                    // Presence in the set means the generated word is:
                    // 1. valid according to wordList
                    // 2. not visited before
                    if (words.count(currentWord)) {
                        q.push({currentWord, sequenceLength + 1});

                        // Mark visited immediately when discovered.
                        // Otherwise, different words in the same BFS level
                        // could insert this word into the queue repeatedly.
                        words.erase(currentWord);
                    }
                }

                // Each index must be modified from the original word,
                // not from a word changed during the previous iteration.
                currentWord[i] = originalCharacter;
            }
        }

        return 0;
    }
};