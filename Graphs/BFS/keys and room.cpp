class Solution {
public:
    bool canVisitAllRooms(vector<vector<int>>& rooms) {
        int n = rooms.size();

        // visited[i] = false means room i has not been entered yet
        // visited[i] = true means room i is reachable from room 0
        vector<bool> visited(n, false);

        queue<int> q;

        // Only room 0 is unlocked initially
        q.push(0);
        visited[0] = true;

        // BFS traversal starting from room 0
        while (!q.empty()) {
            int room = q.front();
            q.pop();

            // rooms[room] contains all keys available inside current room
            for (int key : rooms[room]) {

                // If the room unlocked by this key is not visited yet,
                // mark it visited and add it to the queue
                if (!visited[key]) {
                    visited[key] = true;
                    q.push(key);
                }
            }
        }

        // After BFS, check whether every room was reachable from room 0
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                return false;
            }
        }

        // All rooms were visited
        return true;
    }
};