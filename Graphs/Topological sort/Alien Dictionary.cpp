class Solution {
public:
    string findOrder(vector<string>& words, int K) {

        /*
            Each character is treated as a graph node.

            Character mapping:
                'a' -> 0
                'b' -> 1
                ...
        */
        vector<unordered_set<int>> adj(K);
        vector<int> indegree(K, 0);

        /*
            Compare only adjacent words.

            Since the dictionary is already sorted, the first different
            character between two adjacent words tells us their order.

            Example:
                "baa"
                "abcd"

            First different characters:
                'b' and 'a'

            Therefore:
                b comes before a
                edge: b -> a
        */
        for (int i = 0; i < words.size() - 1; i++) {

            string& first = words[i];
            string& second = words[i + 1];

            int minLength = min(first.size(), second.size());
            int index = 0;

            /*
                Skip the common prefix.

                Example:
                    "abcd"
                    "abca"

                a, b and c are equal.
                The first difference is:
                    d != a
            */
            while (index < minLength &&
                   first[index] == second[index]) {
                index++;
            }

            /*
                Invalid prefix case:

                    first  = "abc"
                    second = "ab"

                A longer word cannot come before its own prefix
                in a valid dictionary.

                Correct order would have been:
                    "ab"
                    "abc"
            */
            if (index == minLength) {
                if (first.size() > second.size()) {
                    return "";
                }

                // No new character ordering can be obtained.
                continue;
            }

            int u = first[index] - 'a';
            int v = second[index] - 'a';

            /*
                first comes before second in the dictionary.

                Therefore:
                    first[index] must come before second[index]

                Add directed edge:
                    u -> v

                unordered_set prevents duplicate edges.

                This is important because a duplicate edge should not
                increase the indegree multiple times.
            */
            if (adj[u].insert(v).second) {
                indegree[v]++;
            }
        }

        queue<int> q;

        /*
            A character with indegree 0 has no character that must
            necessarily come before it.

            Therefore, it can be placed next in the alien alphabet.
        */
        for (int character = 0; character < K; character++) {
            if (indegree[character] == 0) {
                q.push(character);
            }
        }

        string order;

        /*
            Apply Kahn's algorithm for topological sorting.
        */
        while (!q.empty()) {

            int current = q.front();
            q.pop();

            // Add the current character to the alien alphabet order.
            order.push_back(current + 'a');

            /*
                Remove the current character's outgoing edges.

                After placing current in the order, every neighbour
                has one fewer unresolved prerequisite.
            */
            for (int neighbour : adj[current]) {

                indegree[neighbour]--;

                /*
                    When indegree becomes 0, all characters that must
                    come before this neighbour have been processed.
                */
                if (indegree[neighbour] == 0) {
                    q.push(neighbour);
                }
            }
        }

        /*
            If all K characters were not processed, the graph contains
            a directed cycle.

            Example:
                a -> b
                b -> a

            Such an alphabet order is impossible.
        */
        if (order.size() != K) {
            return "";
        }

        return order;
    }
};