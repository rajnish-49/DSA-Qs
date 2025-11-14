// https://leetcode.com/problems/reverse-words-in-a-string/

string reverseWords(string &s) {
    int n = s.size();
    int i = 0, j = 0;

    // Step 1: Remove leading spaces
    while (i < n && s[i] == ' ') i++;

    bool spacePending = false; // Flag to track if a space needs to be inserted before next word

    // Step 2: Remove extra spaces and compress spaces in-place
    while (i < n) {
        if (s[i] != ' ') {
            // If we encountered spaces before this word, insert a single space before the word
            if (spacePending) s[j++] = ' ';
            
            // Copy the non-space character (part of the word) to position j
            s[j++] = s[i];

            // Reset the spacePending flag since we just processed a word character
            spacePending = false;
        } else {
            // Mark that we've encountered at least one space → next word should get ONE space
            spacePending = true;
        }
        i++;
    }

    // Step 3: Resize the string to the new valid length (removes trailing garbage if any)
    s.resize(j);

    // Step 4: Remove trailing space if present (if last char is space, pop it)
    if (!s.empty() && s.back() == ' ') s.pop_back();

    // Step 5: Reverse the entire string to reverse the order of the words
    reverse(s.begin(), s.end());

    // Step 6: Reverse each word individually to restore the letters of each word
    int start = 0;
    for (int end = 0; end <= s.size(); ++end) {
        if (end == s.size() || s[end] == ' ') {
            // Reverse characters from [start, end-1] to correct the word
            reverse(s.begin() + start, s.begin() + end);
            // Move start to the beginning of the next word
            start = end + 1;
        }
    }

    return s;
}
