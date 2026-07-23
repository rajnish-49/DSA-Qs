class Solution {
public:
    vector<int> eventualSafeNodes(vector<vector<int>>& graph) {

        int n = graph.size();

        /*
            reverseGraph[x] contains all nodes that point to x
            in the original graph.

            Original edge:
                u -> v

            Reversed edge:
                v -> u

            Why reverse?

            We start from terminal nodes because they are definitely safe.

            After confirming that a node is safe, we need to move backward
            and check which nodes were pointing to it.

            The original adjacency list cannot move backward,
            so we build a reversed graph.
        */
        vector<vector<int>> reverseGraph(n);

        /*
            remainingOutgoing[node] tells us:

            How many outgoing neighbours of this node have not yet
            been confirmed safe?

            Initially, this is simply the original outdegree.
        */
        vector<int> remainingOutgoing(n, 0);

        for (int node = 0; node < n; node++) {

            remainingOutgoing[node] = graph[node].size();

            for (int neighbour : graph[node]) {

                // Original: node -> neighbour
                // Reversed: neighbour -> node
                reverseGraph[neighbour].push_back(node);
            }
        }

        queue<int> q;

        /*
            A node with original outdegree 0 is a terminal node.

            A terminal node cannot enter a cycle because it has no
            outgoing edge at all.

            Therefore, every terminal node is safe and becomes
            the starting point of the BFS.
        */
        for (int node = 0; node < n; node++) {
            if (remainingOutgoing[node] == 0) {
                q.push(node);
            }
        }

        vector<int> safeNodes;

        while (!q.empty()) {

            int safeNode = q.front();
            q.pop();

            // This node has now been proved safe.
            safeNodes.push_back(safeNode);

            /*
                reverseGraph[safeNode] contains all nodes that point
                to safeNode in the original graph.

                For every such previousNode:

                    previousNode -> safeNode

                Since safeNode is confirmed safe, one outgoing edge of
                previousNode is now resolved as safe.
            */
            for (int previousNode : reverseGraph[safeNode]) {

                remainingOutgoing[previousNode]--;

                /*
                    If this becomes 0, every outgoing neighbour of
                    previousNode has already been confirmed safe.

                    Therefore, every path starting from previousNode
                    eventually reaches a terminal node.

                    So previousNode is also safe.
                */
                if (remainingOutgoing[previousNode] == 0) {
                    q.push(previousNode);
                }
            }
        }

        /*
            Nodes are discovered in BFS order, not necessarily
            increasing numerical order.

            The problem asks for the safe nodes in sorted order.
        */
        sort(safeNodes.begin(), safeNodes.end());

        return safeNodes;
    }
};