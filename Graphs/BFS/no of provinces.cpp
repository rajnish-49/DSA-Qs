// Start from city i
// Push it into queue
// Visit all cities connected to it
// Then visit their connected cities
// Continue until the whole province is covered
using namespace std;

class Solution {
public:
    int findCircleNum(vector<vector<int>>& isConnected) {
        int n = isConnected.size();

        // visited[i] = false means city i is not visited yet
        // visited[i] = true means city i already belongs to some province
        vector<bool> visited(n, false);

        int count = 0;

        // Check every city
        for (int i = 0; i < n; i++) {

            // If city i is not visited,
            // it means we found a new province
            if (!visited[i]) {
                count++;

                // Start BFS from this city
                queue<int> q;
                q.push(i);
                visited[i] = true;

                // BFS will visit all cities in this province
                while (!q.empty()) {
                    int city = q.front();
                    q.pop();

                    // Check all possible cities connected to current city
                    for (int neighbour = 0; neighbour < n; neighbour++) {

                        // isConnected[city][neighbour] == 1 means
                        // there is a direct connection between city and neighbour
                        //
                        // If neighbour is not visited yet,
                        // add it to the BFS queue
                        if (isConnected[city][neighbour] == 1 && !visited[neighbour]) {
                            visited[neighbour] = true;
                            q.push(neighbour);
                        }
                    }
                }
            }
        }

        // Total number of BFS starts = total number of provinces
        return count;
    }
};