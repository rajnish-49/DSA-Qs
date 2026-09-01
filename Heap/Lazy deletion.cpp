#include <bits/stdc++.h>
using namespace std;

/*
 * LAZY DELETION IN PRIORITY QUEUE
 *
 * We do NOT immediately remove an element when delete() is called.
 *
 * Instead:
 *     1. Mark the element as deleted.
 *     2. Leave it inside the priority queue.
 *     3. When it reaches the top, remove it.
 *
 * This is called lazy deletion.
 */

class LazyPriorityQueue {

    priority_queue<int, vector<int>, greater<int>> pq;

    // Stores elements that have been logically deleted.
    unordered_map<int, int> deleted;


    /*
     * Remove all deleted elements currently sitting at the top.
     *
     * Deleted elements somewhere inside the heap are left alone.
     * We only care about them when they reach the top.
     */
    void clean() {

        while (!pq.empty() && deleted[pq.top()] > 0) {

            deleted[pq.top()]--;

            pq.pop();
        }
    }


public:

    /*
     * INSERT
     *
     * Simply insert the element normally.
     */
    void insert(int x) {
        pq.push(x);
    }


    /*
     * DELETE
     *
     * Do not search for x inside the priority queue.
     *
     * Just record that one occurrence of x should be deleted.
     */
    void remove(int x) {
        deleted[x]++;
    }


    /*
     * GET MIN
     *
     * First remove any logically deleted elements
     * that have reached the top.
     */
    int getMin() {

        clean();

        if (pq.empty())
            throw runtime_error("Priority queue is empty");

        return pq.top();
    }


    /*
     * EXTRACT MIN
     *
     * First remove all deleted elements from the top.
     * Then remove the actual minimum.
     */
    int extractMin() {

        clean();

        if (pq.empty())
            throw runtime_error("Priority queue is empty");

        int ans = pq.top();

        pq.pop();

        return ans;
    }


    bool empty() {
        clean();
        return pq.empty();
    }
};