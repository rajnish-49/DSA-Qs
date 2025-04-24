#include <iostream>
#include <vector>
#include <stack>

using namespace std;

class Solution {
public:
    void reverseString(vector<char>& str) {
        stack<char> s;
        
        // Push each character of the vector onto the stack
        for (char ch : str) {
            s.push(ch);
        }

        // Pop each character from the stack back into the vector, reversing it
        for (int i = 0; i < str.size(); i++) {
            str[i] = s.top();
            s.pop();
        }
    }
};

int main() {
    vector<char> str = {'h', 'e', 'l', 'l', 'o'};
    
    Solution solution;
    solution.reverseString(str);

    cout << "Reversed string: ";
    for (char ch : str) {
        cout << ch;
    }
    cout << endl;

    return 0;
}
