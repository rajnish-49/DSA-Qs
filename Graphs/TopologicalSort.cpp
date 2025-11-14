class Solution {
    private:
    
    // Depth-First Search (DFS) function to process nodes for topological sorting
    void dfs(int node, vector<int> &vis, stack<int> &st, vector<vector<int>>& adj) {
        
        // Mark the current node as visited
        vis[node] = 1;
  
        // Iterate through all adjacent (neighbor) nodes of the current node
        for(auto it : adj[node]) {
            // If the neighbor node is not visited, call DFS recursively
            if(!vis[it]) {
                dfs(it, vis, st, adj);
            }
        }
  
        // After visiting all its neighbors, push the current node onto the stack
        // This ensures that nodes with dependencies are processed first
        st.push(node);
    }
       
    public:
      // Function to return a list containing vertices in Topological order.
      vector<int> topologicalSort(vector<vector<int>>& adj) {
          
          int V = adj.size();  // Number of vertices
  
          vector<int> vis(V, 0);  // Visited array to track visited nodes
          stack<int> st;          // Stack to store the topological order
          vector<int> ans;        // Final answer vector
  
          // Loop through all nodes (0 to V-1) to start DFS
          // This ensures that disconnected components are also processed
          for(int i = 0; i < V; i++) {
              if(!vis[i]) {  // If node is unvisited, call DFS
                  dfs(i, vis, st, adj);
              }
          }
  
          // Pop elements from the stack to get the topological order
          while(!st.empty()) {
              ans.push_back(st.top());  // Add top element of stack to answer
              st.pop();                 // Remove element from stack
          }
  
          return ans;  // Return the final topological ordering
      }
  };
  