// @ https://leetcode.com/problems/course-schedule/

class Solution {
    public:
        bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
    
            int V  = numCourses; // Total number of courses (vertices in the graph)
            vector<vector<int>> graph(V); // Adjacency list to represent the graph
    
            // Step 1: Build the adjacency list representation of the graph
            for(auto i: prerequisites){
                graph[i[1]].push_back(i[0]); // Directed edge: prerequisite (i[1]) → course (i[0])
            }
    
            // Step 2: Create an in-degree array to track prerequisites count for each course
            vector<int> indeg(V, 0); // indeg[i] represents the number of prerequisites for course i
    
            // Step 3: Populate the in-degree array by counting incoming edges for each course
            for (int i = 0; i < V; i++) {
                for (auto it : graph[i]) { 
                    indeg[it]++; // Increase in-degree of course 'it' since 'i' is its prerequisite
                }
            }
    
            vector<int> topo; // Stores the topological order of courses
    
            queue<int> q; // Queue to process courses with 0 prerequisites
    
            // Step 4: Add all courses with 0 in-degree (no prerequisites) to the queue
            for (int i = 0; i < V; i++) {
                if (indeg[i] == 0) { 
                    q.push(i);
                }
            }
    
            // Step 5: Process the courses in topological order using Kahn's Algorithm (BFS)
            while(!q.empty()){
                int node = q.front(); // Get the course with no remaining prerequisites
                q.pop();
                topo.push_back(node); // Add the course to topological order
    
                // Step 6: Reduce the in-degree of dependent courses and push if in-degree becomes 0
                for(auto it: graph[node]){
                    indeg[it]--; // Remove dependency (prerequisite completed)
                    if(indeg[it] == 0 ){ // If a course has no more prerequisites, add to queue
                        q.push(it);
                    }
                }
            }
    
            // Step 7: Check if we were able to process all courses
            if(topo.size() == V){ // If all courses are included in topological order, no cycle exists
                return true; // Possible to finish all courses
            }
            else{
                return false; // A cycle exists (some courses couldn't be completed)
            }
        }
    };
    

    // USING DFS 

    class Solution {
        public:
            bool dfs(int node, vector<vector<int>>& graph, vector<int>& visited) {
                if (visited[node] == 1) return true;  // Cycle detected
                if (visited[node] == 2) return false; // Already processed (safe)
        
                visited[node] = 1;  // Mark node as "processing"
        
                for (int neighbor : graph[node]) {  
                    if (dfs(neighbor, graph, visited)) {
                        return true; // If a cycle is found, return true
                    }
                }
        
                visited[node] = 2;  // Mark node as "processed"
                return false;
            }
        
            bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
                vector<vector<int>> graph(numCourses); // Adjacency list
                for (auto i : prerequisites) {
                    graph[i[1]].push_back(i[0]); // Directed edge: prerequisite → course
                }
        
                vector<int> visited(numCourses, 0); // Track node states: 0 = unvisited, 1 = processing, 2 = processed
        
                for (int i = 0; i < numCourses; i++) {
                    if (visited[i] == 0) { // If not yet visited, perform DFS
                        if (dfs(i, graph, visited)) {
                            return false; // Cycle detected, cannot finish all courses
                        }
                    }
                }
        
                return true; // No cycle detected, all courses can be completed
            }
        };
        