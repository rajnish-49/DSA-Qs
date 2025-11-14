// @https://leetcode.com/problems/design-browser-history/

class BrowserHistory {
private:
    // Stores the full browsing history in order.
    // Example: ["leetcode.com", "google.com", "facebook.com"]
    vector<string> history; 
    
    // Points to the *current* page inside the history vector.
    // Example: if currentIndex = 1, that means we are on history[1].
    int currentIndex;      

public:
    // Constructor: initializes the browser with the homepage
    BrowserHistory(string homepage) {
        // Add the homepage as the very first entry in history
        history.push_back(homepage); 
        
        // Since homepage is at position 0 in the vector,
        // the current index starts at 0.
        currentIndex = 0;          
    }

    // Visits a new URL from the current page.
    // Rule: if we had any "forward history", it gets erased.
    void visit(string url) {
        // 1. Erase all pages *after* the current page
        //    (they represent the forward history).
        // Example: history = ["leetcode", "google", "facebook"], currentIndex = 1 ("google")
        // Forward pages = ["facebook"], which must be removed.
        history.erase(history.begin() + currentIndex + 1, history.end());

        // 2. Add the new URL at the end of the history.
        history.push_back(url);

        // 3. Move currentIndex one step forward
        //    to point to this newly visited page.
        currentIndex++;
    }

    // Move backwards in history by 'steps'.
    // Cannot go past the very first page (index 0).
    string back(int steps) {
        // Reduce currentIndex by 'steps',
        // but clamp it at 0 so we don't go out of bounds.
        currentIndex = max(0, currentIndex - steps);

        // Return the URL at the new current position.
        return history[currentIndex];
    }

    // Move forwards in history by 'steps'.
    // Cannot go past the most recently visited page.
    string forward(int steps) {
        // Increase currentIndex by 'steps',
        // but clamp it at history.size() - 1
        // so we don't go beyond the last valid page.
        currentIndex = min((int)history.size() - 1, currentIndex + steps);

        // Return the URL at the new current position.
        return history[currentIndex];
    }
};
