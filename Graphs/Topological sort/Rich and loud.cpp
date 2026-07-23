class Solution {
public:
    vector<int> loudAndRich(vector<vector<int>>& richer,
                            vector<int>& quiet) {

        int n = quiet.size();

        /*
            Build the graph as:

                richer person -> poorer person

            For richer[i] = [a, b]:
                a is richer than b

            So we add:
                a -> b

            This direction allows richer people to pass their
            best quiet candidate to poorer people.
        */
        vector<vector<int>> adj(n);
        vector<int> indegree(n, 0);

        for (auto& relation : richer) {
            int rich = relation[0];
            int poor = relation[1];

            adj[rich].push_back(poor);
            indegree[poor]++;
        }

        /*
            answer[i] stores the quietest person currently known
            among:

                person i
                +
                everyone definitely richer than i

            Initially, each person is the only known valid candidate
            for themself.
        */
        vector<int> answer(n);

        for (int person = 0; person < n; person++) {
            answer[person] = person;
        }

        queue<int> q;

        /*
            A person with indegree 0 has nobody directly known
            to be richer than them.

            These people are processed first in Kahn's algorithm.
        */
        for (int person = 0; person < n; person++) {
            if (indegree[person] == 0) {
                q.push(person);
            }
        }

        /*
            Process people from richer to poorer.

            By the time a person is removed from the queue,
            all people richer than them have already been processed.

            Therefore, answer[person] is already the quietest person
            among that person and everyone richer than them.
        */
        while (!q.empty()) {

            int person = q.front();
            q.pop();

            for (int poorerPerson : adj[person]) {

                /*
                    answer[person] is a valid candidate for poorerPerson.

                    Why?

                    If answer[person] is at least as rich as person,
                    and person is richer than poorerPerson,
                    then answer[person] is also richer than poorerPerson.

                    Keep whichever candidate has the smaller quiet value.
                */
                if (quiet[answer[person]] <
                    quiet[answer[poorerPerson]]) {

                    answer[poorerPerson] = answer[person];
                }

                /*
                    Normal Kahn's algorithm step.

                    One richer dependency of poorerPerson
                    has now been processed.
                */
                indegree[poorerPerson]--;

                /*
                    When indegree becomes 0, all richer people pointing
                    to poorerPerson have already passed their best answer.

                    So answer[poorerPerson] is now complete.
                */
                if (indegree[poorerPerson] == 0) {
                    q.push(poorerPerson);
                }
            }
        }

        return answer;
    }
};