/**
 * LeetCode 543: Diameter of Binary Tree
 * https://leetcode.com/problems/diameter-of-binary-tree/
 *
 * Problem:
 * Given the root of a binary tree, return the length of its diameter.
 *
 * The diameter is the longest path between any two nodes.
 * The path may or may not pass through the root.
 *
 * Important:
 * The answer is measured in NUMBER OF EDGES, not nodes.
 *
 * Example:
 *
 *          1
 *         / \
 *        2   3
 *       / \
 *      4   5
 *
 * The longest path is:
 *
 *      4 -> 2 -> 1 -> 3
 *
 * It contains 3 edges, so the answer is 3.
 */


// ============================================================
// Approach 1: Calculate height separately
// ============================================================

class Solution {

private:

    /*
     * Returns the height of the subtree rooted at 'root',
     * measured in number of NODES.
     *
     * Why nodes and not edges?
     * Because when a parent adds left + right, the node counts
     * cancel out and give the correct edge count automatically.
     *
     * Example:
     *
     *       1         height = 3 (nodes: 1, 2, 3)
     *      /
     *     2
     *    /
     *   3
     *
     * If left height = 2 and right height = 0,
     * then left + right = 2, which is the number of edges.
     * That works out correctly because each "node count"
     * represents one edge to its parent.
     */
    int solve(TreeNode* root) {

        // Null node contributes nothing — height 0.
        if(root == NULL)
            return 0;

        // Recurse into both subtrees to find their heights.
        // We need these before we can compute our own height.
        int left = solve(root->left);
        int right = solve(root->right);

        // Our height = 1 (this node) + the taller of the two sides.
        // We pick max because height is the longest downward path.
        return 1 + max(left, right);
    }

public:

    int diameterOfBinaryTree(TreeNode* root) {

        // Empty tree has no nodes, so diameter is 0.
        if(root == NULL)
            return 0;

        // Get how deep we can go on each side from this node.
        int left = solve(root->left);
        int right = solve(root->right);

        /*
         * If the diameter passes THROUGH this node, it goes:
         *
         *      deepest node in left subtree
         *                  |
         *             current node
         *                  |
         *      deepest node in right subtree
         *
         * left and right are node-counts, and their sum gives
         * the number of edges in this path.
         *
         * Why? Each node-count represents one edge upward.
         * So left edges + right edges = left + right.
         */
        int count = left + right;

        /*
         * But the diameter might not pass through this node at all.
         * It could live entirely inside the left subtree.
         * Recurse to check that possibility.
         */
        count = max(count, diameterOfBinaryTree(root->left));

        /*
         * Or it could live entirely inside the right subtree.
         * Recurse to check that too.
         */
        count = max(count, diameterOfBinaryTree(root->right));

        return count;
    }
};


// Time: O(n²) — for every node, solve() reruns on the whole subtree
// Space: O(h)


// ============================================================
// Approach 2: Calculate height and diameter together
// ============================================================

class Solution {

private:

    /*
     * Global answer, updated at every node during the traversal.
     *
     * Why global? Because the diameter can peak at any node in
     * the tree. We can't just return it up — we're already using
     * the return value to pass height upward. So we track the
     * best diameter seen so far in a separate variable.
     */
    int ans = 0;

    /*
     * Does two things in one pass:
     *
     *   1. Returns the HEIGHT of this subtree to the parent,
     *      so the parent can compute its own diameter.
     *
     *   2. Updates 'ans' with the diameter of the longest path
     *      whose peak is this node.
     *
     * This eliminates the redundant height recomputations
     * that made Approach 1 O(n²).
     */
    int solve(TreeNode* root) {

        // Null node — no height, no path.
        if(root == NULL)
            return 0;

        /*
         * Postorder: process children before the current node.
         *
         * We cannot compute the diameter through this node until
         * we know how deep each subtree goes. So children first.
         */
        int left = solve(root->left);
        int right = solve(root->right);

        /*
         * The longest path with THIS node as the peak spans
         * both sides:
         *
         *      deepest left node
         *             \
         *           this node
         *             /
         *      deepest right node
         *
         * left + right = number of edges in this path.
         *
         * We update ans here because this node is the peak —
         * once we return upward, we can only offer one side.
         */
        ans = max(ans, left + right);

        /*
         * Return height to the parent.
         *
         * The parent can extend through us, but only in ONE
         * direction — it cannot take both our left and right
         * arms, because that would make the path branch.
         *
         * So we give the parent our best single-sided depth,
         * plus 1 for the edge between us and the parent.
         */
        return 1 + max(left, right);
    }

public:

    int diameterOfBinaryTree(TreeNode* root) {

        /*
         * One full DFS traversal.
         * At every node, height is computed bottom-up and
         * diameter is updated as we go.
         * After this call, 'ans' holds the final answer.
         */
        solve(root);

        return ans;
    }
};


// Time: O(n) — every node visited exactly once
// Space: O(h)