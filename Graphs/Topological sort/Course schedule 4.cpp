class Solution {
public:
    vector<bool> checkIfPrerequisite(
        int numCourses,
        vector<vector<int>>& prerequisites,
        vector<vector<int>>& queries
    ) {
        /*
            Build a directed graph.

            prerequisites[i] = [u, v] means:
                u must be completed before v

            So the directed edge is:
                u -> v
        */
        vector<vector<int>> adj(numCourses);
        vector<int> indegree(numCourses, 0);

        for (auto& edge : prerequisites) {
            int u = edge[0];
            int v = edge[1];

            adj[u].push_back(v);
            indegree[v]++;
        }

        /*
            ancestors[course] stores all direct and indirect
            prerequisites of that course.

            We use set because:
            1. The same prerequisite may reach a course through
               multiple different paths.
            2. set automatically removes duplicates.
        */
        vector<set<int>> ancestors(numCourses);

        queue<int> q;

        /*
            Courses with indegree 0 have no prerequisites.

            These courses are the starting points of
            Kahn's topological sort.
        */
        for (int course = 0; course < numCourses; course++) {
            if (indegree[course] == 0) {
                q.push(course);
            }
        }

        /*
            Process courses in topological order.

            Important property:

            When a course is removed from the queue,
            all its prerequisites have already been processed.

            Therefore, ancestors[course] is complete and can be
            passed to every course that depends on it.
        */
        while (!q.empty()) {
            int course = q.front();
            q.pop();

            for (int nextCourse : adj[course]) {

                /*
                    Since:
                        course -> nextCourse

                    course is a direct prerequisite of nextCourse.
                */
                ancestors[nextCourse].insert(course);

                /*
                    Every prerequisite of course is also
                    a prerequisite of nextCourse.

                    If:
                        x -> ... -> course -> nextCourse

                    then x is also a prerequisite of nextCourse.
                */
                for (int prerequisite : ancestors[course]) {
                    ancestors[nextCourse].insert(prerequisite);
                }

                /*
                    Normal Kahn's algorithm step.

                    One incoming prerequisite edge of nextCourse
                    has now been processed.
                */
                indegree[nextCourse]--;

                /*
                    When indegree becomes 0, all direct prerequisites
                    of nextCourse have been processed.

                    Hence, its complete ancestor set has been collected.
                */
                if (indegree[nextCourse] == 0) {
                    q.push(nextCourse);
                }
            }
        }

        vector<bool> answer;

        /*
            For query [u, v], check whether u exists in
            the prerequisite set of v.

            If yes:
                u is a direct or indirect prerequisite of v.
        */
        for (auto& query : queries) {
            int u = query[0];
            int v = query[1];

            answer.push_back(ancestors[v].count(u) > 0);
        }

        return answer;
    }
};