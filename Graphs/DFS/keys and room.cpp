class Solution {
public:
    void dfs(int room, vector<vector<int>>& rooms, vector<bool>& visited) {
        // Mark current room as visited.
        // This means we were able to enter this room.
        visited[room] = true;

        // rooms[room] contains all keys available inside current room.
        // Each key represents another room we can enter.
        for (int key : rooms[room]) {

            // If the room unlocked by this key is not visited yet,
            // enter that room using DFS.
            if (!visited[key]) {
                dfs(key, rooms, visited);
            }
        }
    }

    bool canVisitAllRooms(vector<vector<int>>& rooms) {
        int n = rooms.size();

        // visited[i] = false means room i has not been entered yet.
        // visited[i] = true means room i is reachable from room 0.
        vector<bool> visited(n, false);

        // Only room 0 is unlocked initially,
        // so DFS must start from room 0.
        dfs(0, rooms, visited);

        // After DFS, check whether every room was visited.
        // If any room is still unvisited, we cannot visit all rooms.
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                return false;
            }
        }

        // All rooms are reachable from room 0.
        return true;
    }
};