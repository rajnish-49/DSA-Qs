class Solution {
public:
    // Function to check if two trees are identical
    bool isIdentical(Node* r1, Node* r2) {
        // Base case 1: Both nodes are null
        // If both r1 and r2 are null, it means we are at the end of both trees, and they match up to this point.
        if (r1 == nullptr && r2 == nullptr) {
            return true; // Both trees are empty, so they are identical
        }

        // Base case 2: One node is null and the other is not
        // If only one of the nodes is null, the trees are not identical because their structures differ.
        if (r1 == nullptr || r2 == nullptr) {
            return false; // One tree is empty, and the other is not
        }

        // Recursive case:
        // Step 1: Recursively check if the left subtrees of r1 and r2 are identical
        bool left = isIdentical(r1->left, r2->left);

        // Step 2: Recursively check if the right subtrees of r1 and r2 are identical
        bool right = isIdentical(r1->right, r2->right);

        // Step 3: Compare the values of the current nodes
        bool value = (r1->data == r2->data);

        // Combine results:
        // The trees are identical only if:
        // - The left subtrees are identical (`left == true`), and
        // - The right subtrees are identical (`right == true`), and
        // - The values of the current nodes are equal (`value == true`).
        return left && right && value;
    }
};
