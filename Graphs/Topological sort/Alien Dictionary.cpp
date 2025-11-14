// @ https://www.geeksforgeeks.org/problems/alien-dictionary/1

class Solution {
    public:
      string findOrder(vector<string> &words) {
          // Adjacency list representation of the graph
          vector<vector<int>> graph(26); 
          
          // Indegree array to track incoming edges for each node
          vector<int> indegree(26, 0);    
          
          // Set to store unique characters present in the words
          unordered_set<char> unique_chars; 
      
          // Step 1: Extract all unique characters from words
          for (string word : words) {
              for (char ch : word) {
                  unique_chars.insert(ch);
              }
          }
  
          // Step 2: Build the Directed Graph (Adjacency List)
          for (int i = 0; i < words.size() - 1; i++) {
              string w1 = words[i];      // Current word
              string w2 = words[i + 1];  // Next word
              
              int minLength = min(w1.size(), w2.size()); // Compare up to the smaller word's length
              bool found = false; 
          
              // Compare characters of both words
              for (int j = 0; j < minLength; j++) {
                  if (w1[j] != w2[j]) {
                      // Create a directed edge from w1[j] → w2[j] (order relation)
                      graph[w1[j] - 'a'].push_back(w2[j] - 'a');
                      
                      // Increase the indegree of w2[j] (since it has an incoming edge)
                      indegree[w2[j] - 'a']++;
                      
                      found = true; // Found the first difference, so break
                      break;  
                  }
              }
  
              // If no different character was found and w1 is longer than w2, ordering is invalid
              if (!found && w1.size() > w2.size()) {
                  return "";
              }
          }
  
          // Step 3: Topological Sorting using Kahn's Algorithm (BFS)
          string ans;
          queue<int> q;
      
          // Push all characters with indegree 0 into the queue
          for (char ch : unique_chars) {
              if (indegree[ch - 'a'] == 0) {
                  q.push(ch - 'a');
              }
          }
      
          // Process nodes in topological order
          while (!q.empty()) {
              int node = q.front();
              q.pop();
              
              // Append the character to the answer
              ans += char(node + 'a');
          
              // Reduce the indegree of all adjacent nodes
              for (int neighbour : graph[node]) {
                  indegree[neighbour]--;
                  
                  // If indegree becomes 0, push into the queue
                  if (indegree[neighbour] == 0) {
                      q.push(neighbour);
                  }
              }
          }
          
          // Step 4: Validate the result
          // If all unique characters are used, return answer; else, return "" (cycle detected)
          if (ans.size() == unique_chars.size()) {
              return ans;
          } else {
              return "";
          }
      }
  };

    