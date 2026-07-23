class Solution {
public:
    // Map stores relation between original node and cloned node
    // original node pointer -> cloned node pointer
    unordered_map<Node*, Node*> mp;

    Node* cloneGraph(Node* node) {
        // If graph is empty, there is nothing to clone
        if (node == nullptr) {
            return nullptr;
        }

        // If this node was already cloned earlier,
        // directly return its clone.
        //
        // This prevents infinite recursion in cyclic graphs
        // and also ensures each node is cloned only once.
        if (mp.find(node) != mp.end()) {
            return mp[node];
        }

        // Create a clone of the current node with the same value.
        //
        // At this moment, its neighbours are still empty.
        Node* cloneNode = new Node(node->val);

        // Store it immediately in the map before cloning neighbours.
        //
        // This is important because if there is a cycle,
        // another DFS path may come back to this same node.
        mp[node] = cloneNode;

        // Now clone all neighbours of the original node.
        for (Node* neighbour : node->neighbors) {

            // cloneGraph(neighbour) returns the cloned copy
            // of this neighbour.
            Node* clonedNeighbour = cloneGraph(neighbour);

            // Add cloned neighbour to current cloned node's neighbour list.
            cloneNode->neighbors.push_back(clonedNeighbour);
        }

        // Return cloned copy of current node.
        return cloneNode;
    }
};