class Solution {
public:
    vector<int> findOrder(int numCourses,
                          vector<vector<int>>& prerequisites) {

        vector<vector<int>> adj(numCourses);
        vector<int> indegree(numCourses, 0);

        /*
            prerequisites[i] = [course, prerequisite]

            To take 'course', we must first complete 'prerequisite'.

            Therefore, the directed edge is:

                prerequisite -> course
        */
        for (auto& requirement : prerequisites) {

            int course = requirement[0];
            int prerequisite = requirement[1];

            adj[prerequisite].push_back(course);

            // 'course' has one more prerequisite.
            indegree[course]++;
        }

        queue<int> q;

        /*
            Courses with indegree 0 have no prerequisites,
            so they can be completed immediately.
        */
        for (int course = 0; course < numCourses; course++) {
            if (indegree[course] == 0) {
                q.push(course);
            }
        }

        vector<int> order;

        while (!q.empty()) {

            int course = q.front();
            q.pop();

            // Add the currently available course to the answer.
            order.push_back(course);

            /*
                After completing this course, remove its dependency
                from every course that depends on it.
            */
            for (int nextCourse : adj[course]) {

                indegree[nextCourse]--;

                /*
                    If indegree becomes 0, all prerequisites of
                    nextCourse have now been completed.
                */
                if (indegree[nextCourse] == 0) {
                    q.push(nextCourse);
                }
            }
        }

        /*
            If all courses were added, 'order' is a valid
            topological ordering.

            If some courses were not added, a cycle exists,
            so no valid ordering is possible.
        */
        if (order.size() != numCourses) {
            return {};
        }

        return order;
    }
};