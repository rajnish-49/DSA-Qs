// @https://leetcode.com/problems/split-linked-list-in-parts/

class Solution {
public:
    vector<ListNode*> splitListToParts(ListNode* head, int k) {

        // Step 1: Count total nodes in the linked list
        // Why? Because we need to know how many nodes are available
        // to distribute across 'k' parts. This is like knowing total candies
        // before dividing them among kids.
        int pos = 0 ;
        ListNode* temp = head;

        while (temp != nullptr ) {
            temp = temp->next;
            pos++;
        }

        // Result container: each element is head of one part
        vector<ListNode*> ans;

        // Step 2: Figure out "base size" of each part
        // Every part will get at least 'parts' nodes.
        // Extra nodes = remainder when total nodes can't be evenly divided.
        // The first 'extra' parts will be one node bigger than others
        // (distribute surplus one by one).
        int parts = pos / k;
        int extra = pos % k;

        // Start from head, will keep cutting list into chunks
        ListNode * curr = head;

        // Step 3: Construct each of the k parts
        for (int i = 0; i < k; i++) {
            
            // Determine how many nodes this part should have.
            // Intuition: 
            // - Each part must get 'parts' nodes 
            // - First 'extra' parts get 1 additional node
            int currSize = parts + (i < extra ? 1 : 0);
                    
            // If no nodes left for this part (happens when k > total nodes),
            // push null to maintain exactly k parts.
            if (currSize == 0) { 
                ans.push_back(nullptr); 
                continue;
            }

            // Mark the starting node of this part
            ListNode* currHead = curr;

            // Move forward (currSize - 1) steps so that
            // 'curr' points to the last node of current part.
            for (int j = 0; j < currSize - 1; j++) {
                curr = curr->next;
            }

            // Step 4: Cut the connection to isolate this part
            // Store the head of next part before breaking the link.
            ListNode* nextPart = curr->next;
            curr->next = nullptr; // disconnect
            curr = nextPart;      // move to next part head

            // Add this chunk's head to result
            ans.push_back(currHead);
        }

        return ans;
    }
};

class Solution {
public:
    vector<ListNode*> splitListToParts(ListNode* head, int k) {
        /*
         Problem summary (one-line):
           Split the linked list into k consecutive parts as evenly as possible.
           Each part should get either floor(n/k) or floor(n/k)+1 nodes; the
           extra nodes go to the first parts.

         High-level intuition (good revision memory):
           Think of the nodes as "candies" and parts as "kids". Each kid gets
           floor(total/k) candies. The first (total % k) kids get one more candy.
           Because a linked list is sequential, we must walk and cut to isolate parts.
         */

        // -----------------------
        // Step 1: Count total nodes
        // -----------------------
        // We need the length to determine how many nodes each part should receive.
        int length = 0;
        ListNode* temp = head;
        while (temp) {
            temp = temp->next;
            length++;
        }

        // ------------------------------
        // Step 2: Compute base / extras
        // ------------------------------
        // partSize = minimum nodes each part receives
        // extra = number of parts that should receive one additional node
        // (these are the first 'extra' parts).
        int partSize = length / k;   // floor(length / k)
        int extra = length % k;      // remainder — distribute to first 'extra' parts

        // Prepare return vector with exactly k elements (default nullptr).
        // Important: we must return exactly k pointers even when length < k.
        vector<ListNode*> ans(k, nullptr);

        // node: the current pointer we use to walk through the original list.
        ListNode* node = head;

        // ---------------------------------------------------------------
        // Step 3: For each part (0..k-1), if there are nodes left, isolate it
        // ---------------------------------------------------------------
        // Loop invariant: when we start iteration i (and node != nullptr),
        //   - 'node' points to the first node that has not yet been assigned to a part.
        //   - we will create a part whose head is 'headPart' and length 'currSize'.
        //
        // We use `for (i < k && node)` so:
        //   - If there are fewer parts with nodes than k, the remaining ans[] stay nullptr.
        //   - If head == nullptr initially, the loop body is skipped (all nullptr).
        for (int i = 0; i < k && node; i++) {
            // headPart: start of current chunk — this is what we store in ans[i].
            ListNode* headPart = node;

            // Determine how many nodes this part should have:
            // base partSize plus one more if i < extra (first 'extra' parts).
            int currSize = partSize + (i < extra ? 1 : 0);

            // Sanity / invariant: because node != nullptr when entering the loop,
            // currSize must always be >= 1. (If length < k, extra==length so
            // first 'length' parts will have currSize==1; later parts aren't iterated.)
            //
            // Walk to the end (last node) of this chunk.
            // Note: when currSize == 1, the loop body doesn't execute and 'node'
            // stays at the single-node chunk head.
            for (int j = 1; j < currSize; j++) {
                // After this inner loop finishes, 'node' will point to the
                // last node of the current part.
                node = node->next;
            }

            // Save the beginning of the remainder (next part) before we sever the link.
            ListNode* nextPart = node->next;

            // Cut: make current part end here so it becomes independent.
            // This is the key step that turns one long list into separate lists.
            node->next = nullptr;

            // Save head of this part into result
            ans[i] = headPart;

            // Advance 'node' to the start of the next unprocessed segment
            node = nextPart;
        }

        // At this point:
        // - If length >= k, all k ans[] will be non-null (but some parts may be tiny).
        // - If length < k, first 'length' ans[] entries are non-null and remaining are nullptr.
        //
        // Complexity:
        // - Time: O(n) to count nodes + O(n) to split = O(n) overall (n = length)
        // - Space: O(k) additional for the returned vector (ignoring output storage).
        //
        return ans;
    }
};

