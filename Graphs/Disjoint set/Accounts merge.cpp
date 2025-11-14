#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

/*
 * ═══════════════════════════════════════════════════════════════════════════════
 * PROBLEM: ACCOUNTS MERGE (LeetCode 721)
 * ═══════════════════════════════════════════════════════════════════════════════
 *
 * PROBLEM STATEMENT:
 * Given a list of accounts where each element accounts[i] is a list of strings,
 * where the first element accounts[i][0] is a name, and the rest are emails
 * representing emails of the account.
 *
 * Two accounts definitely belong to the same person if there is some common email
 * to both accounts. Note that even if two accounts have the same name, they may
 * belong to different people (same name, different people).
 *
 * People can have any number of accounts initially, but all of their accounts
 * definitely have the same name.
 *
 * After merging the accounts, return the accounts in the following format:
 * - First element is the name
 * - Rest of elements are emails in sorted order
 *
 * EXAMPLE 1:
 * Input: accounts = [
 *   ["John","johnsmith@mail.com","john_newyork@mail.com"],
 *   ["John","johnsmith@mail.com","john00@mail.com"],
 *   ["Mary","mary@mail.com"],
 *   ["John","johnnybravo@mail.com"]
 * ]
 *
 * Output: [
 *   ["John","john00@mail.com","john_newyork@mail.com","johnsmith@mail.com"],
 *   ["Mary","mary@mail.com"],
 *   ["John","johnnybravo@mail.com"]
 * ]
 *
 * EXPLANATION:
 * Account 0: ["John","johnsmith@mail.com","john_newyork@mail.com"]
 * Account 1: ["John","johnsmith@mail.com","john00@mail.com"]
 * Account 3: ["John","johnnybravo@mail.com"]
 *
 * Analysis:
 * - Account 0 and Account 1 share "johnsmith@mail.com" → SAME PERSON!
 * - Merge them: All emails = {johnsmith@, john_newyork@, john00@}
 * - Account 2 (Mary) has no common emails with others → Separate person
 * - Account 3 (John) has no common emails with others → Different John!
 *
 * Visual representation of connections:
 *
 *   Account 0 (John)                Account 2 (Mary)      Account 3 (John)
 *   ┌─────────────┐                 ┌──────────┐          ┌──────────┐
 *   │ johnsmith@  │                 │  mary@   │          │ johnnyb@ │
 *   │ john_ny@    │                 └──────────┘          └──────────┘
 *   └─────────────┘                                       (Different
 *        │                                                  person!)
 *        │ (shares johnsmith@)
 *        │
 *   Account 1 (John)
 *   ┌─────────────┐
 *   │ johnsmith@  │ ← Common email! Must be same person
 *   │ john00@     │
 *   └─────────────┘
 *
 * Result: Accounts 0 and 1 merge together.
 *
 * ═══════════════════════════════════════════════════════════════════════════════
 * CORE INTUITION: WHY IS THIS A DSU PROBLEM?
 * ═══════════════════════════════════════════════════════════════════════════════
 *
 * KEY INSIGHT #1: This is about finding connected components!
 * ────────────────────────────────────────────────────────────────
 * Think of accounts as nodes in a graph:
 * - If two accounts share an email → they're connected (same person)
 * - We need to find all groups of connected accounts
 * - Each group = one person's merged account
 *
 * KEY INSIGHT #2: Emails are the "bridges" connecting accounts!
 * ────────────────────────────────────────────────────────────────
 * When we see the same email in multiple accounts, it's like finding an edge
 * in a graph connecting those accounts.
 *
 * Example visualization as a graph:
 *
 *   Account 0 ←──── johnsmith@ ────→ Account 1
 *      ↓                                ↓
 *   john_ny@                         john00@
 *
 * Both accounts share "johnsmith@" → They're connected → Same person!
 *
 * KEY INSIGHT #3: We can map emails to account indices!
 * ────────────────────────────────────────────────────────────────
 * Strategy:
 * - First time we see an email: Remember which account it belongs to
 * - Second time we see same email: AHA! Connect that account with the first one!
 * - This is exactly what DSU union operation does!
 *
 * ═══════════════════════════════════════════════════════════════════════════════
 * WHY DSU IS PERFECT FOR THIS PROBLEM?
 * ═══════════════════════════════════════════════════════════════════════════════
 *
 * DSU ADVANTAGES:
 * ✓ Efficiently groups accounts that share emails (union operation)
 * ✓ Handles transitive relationships automatically
 *   (If A connects to B, and B connects to C, DSU knows A, B, C are all connected)
 * ✓ Fast O(α(n)) per operation (practically constant)
 * ✓ Natural representation: each group has one root (representative account)
 *
 * TRANSITIVE CONNECTION EXAMPLE:
 * Account 0: [email1, email2]
 * Account 1: [email2, email3]
 * Account 2: [email3, email4]
 *
 * When we process:
 * - email2: Union(0, 1) → Account 0 and 1 are connected
 * - email3: Union(1, 2) → Account 1 and 2 are connected
 * DSU automatically knows: 0, 1, 2 are ALL in the same group!
 * (We don't need to manually check 0 and 2 are connected)
 *
 * ALTERNATIVE APPROACHES:
 * 1. DFS/BFS:
 *    - Build graph of account connections, then traverse
 *    - Time: O(n + e), Space: O(n + e)
 *    - More code, less elegant
 *
 * 2. Manually track connected groups:
 *    - Keep list of sets, merge sets when finding common emails
 *    - Complex to implement, slower
 *
 * DSU wins for elegance, efficiency, and automatic transitive grouping!
 *
 * ═══════════════════════════════════════════════════════════════════════════════
 * ALGORITHM OVERVIEW (Before diving into code)
 * ═══════════════════════════════════════════════════════════════════════════════
 *
 * STEP 1: Initialize DSU with n accounts (each account starts separate)
 *
 * STEP 2: Process emails and build connections
 *         Use HashMap: email → first account that has this email
 *         When we see an email again: Union current account with first account
 *
 * STEP 3: Group emails by their representative account
 *         For each email, find which group (root) it belongs to
 *         Add email to that group's list
 *
 * STEP 4: Build final answer
 *         For each group: [name] + [sorted emails]
 *
 * TIME COMPLEXITY: O(n × k × α(n) + n × k × log(k))
 * - n = number of accounts
 * - k = average number of emails per account
 * - n × k × α(n): Processing all emails with DSU operations
 * - n × k × log(k): Sorting emails (dominant factor)
 * - Overall: O(nk log(nk)) due to sorting
 *
 * SPACE COMPLEXITY: O(n × k)
 * - DSU: O(n)
 * - HashMap: O(n × k) for all emails
 * - Output: O(n × k)
 */

/*
 * DISJOINT SET UNION (DSU) Data Structure
 *
 * PURPOSE: Group accounts that belong to the same person
 * - Initially: each account is its own group (n separate sets)
 * - As we find shared emails: merge accounts into larger groups
 * - Finally: each group represents one person's complete email list
 */
class DSU
{
public:
    vector<int> parent; // parent[i] = parent of account i
    vector<int> rank;   // rank[i] = approximate tree height at account i

    /*
     * CONSTRUCTOR: Initialize n accounts as separate entities
     * Each account starts as its own representative
     */
    DSU(int n)
    {
        parent.resize(n);
        rank.resize(n, 0);
        // Each account is its own parent initially
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }

    /*
     * FIND OPERATION: Find which group (person) an account belongs to
     * Returns the root/representative account of the group
     * Uses path compression for efficiency
     */
    int findParent(int x)
    {
        if (parent[x] == x)
            return x; // x is the representative account
        // Path compression: flatten tree structure
        return parent[x] = findParent(parent[x]);
    }

    /*
     * UNION OPERATION: Merge two accounts into same group
     * This is called when we discover two accounts share an email
     * Uses union by rank to keep tree balanced
     */
    void unionSet(int a, int b)
    {
        // Find representatives of both accounts
        a = findParent(a);
        b = findParent(b);

        // If already in same group, nothing to do
        if (a != b)
        {
            // Attach smaller tree under larger tree
            if (rank[a] < rank[b])
                parent[a] = b;
            else if (rank[b] < rank[a])
                parent[b] = a;
            else
            {
                parent[b] = a;
                rank[a]++;
            }
        }
    }
};

class Solution
{
public:
    vector<vector<string>> accountsMerge(vector<vector<string>> &accounts)
    {

        /*
         * ═══════════════════════════════════════════════════════════════════
         * STEP 0: SETUP
         * ═══════════════════════════════════════════════════════════════════
         */

        int n = accounts.size(); // Number of accounts

        /*
         * Initialize DSU with n accounts
         * Initially, each account is considered a separate person
         * As we find shared emails, we'll merge accounts together
         */
        DSU ds(n);

        /*
         * ═══════════════════════════════════════════════════════════════════
         * STEP 1: BUILD EMAIL-TO-ACCOUNT MAPPING & CONNECT ACCOUNTS
         * ═══════════════════════════════════════════════════════════════════
         *
         * CORE STRATEGY: Track which account we first saw each email in
         *
         * mailnode: HashMap storing email → account index
         *
         * WHY THIS WORKS:
         * - First time we see an email: Store "this email belongs to account i"
         * - Second time we see same email in account j: AHA! Accounts i and j
         *   share this email → they must be the same person → Union(i, j)!
         *
         * EXAMPLE:
         * Account 0: ["John", "a@", "b@"]
         * Account 1: ["John", "b@", "c@"]
         *
         * Processing:
         * - See "a@" in account 0: mailnode["a@"] = 0 (first time, just store)
         * - See "b@" in account 0: mailnode["b@"] = 0 (first time, just store)
         * - See "b@" in account 1: Already in map! mailnode["b@"] = 0
         *   → Union(1, 0) → Accounts 0 and 1 are now connected!
         * - See "c@" in account 1: mailnode["c@"] = 1 (first time, just store)
         *
         * After this step, DSU knows accounts 0 and 1 belong to same person!
         */
        unordered_map<string, int> mailnode;

        // Process each account
        for (int i = 0; i < n; i++)
        {
            // j starts from 1 because accounts[i][0] is the name, not an email
            for (int j = 1; j < accounts[i].size(); j++)
            {
                string mail = accounts[i][j];

                /*
                 * Check if we've seen this email before
                 *
                 * Case 1: Email NOT in map (first time seeing it)
                 * → Just record: "This email belongs to account i"
                 *
                 * Case 2: Email IS in map (seen before in another account!)
                 * → This is the KEY moment! Two accounts share an email!
                 * → They must be the same person!
                 * → Union current account i with the account that had this email
                 */
                if (mailnode.find(mail) == mailnode.end())
                {
                    // First time seeing this email
                    // Remember that account i has this email
                    mailnode[mail] = i;
                }
                else
                {
                    // WE'VE SEEN THIS EMAIL BEFORE!
                    // mailnode[mail] = the account index that had this email before
                    // Current account i also has this email
                    // → They're the same person! Connect them!
                    ds.unionSet(i, mailnode[mail]);

                    /*
                     * WHY UNION AND NOT UPDATE?
                     * We DON'T update mailnode[mail] = i
                     * We keep it pointing to the original account
                     * The DSU handles the connection - we just need one link!
                     *
                     * Example: If accounts 0, 3, 7 all share email "x@"
                     * - mailnode["x@"] stays 0 (first account that had it)
                     * - Union(3, 0) connects account 3
                     * - Union(7, 0) connects account 7
                     * - DSU automatically knows 0, 3, 7 are all connected!
                     */
                }
            }
        }

        /*
         * ═══════════════════════════════════════════════════════════════════
         * STEP 2: GROUP EMAILS BY THEIR REPRESENTATIVE ACCOUNT
         * ═══════════════════════════════════════════════════════════════════
         *
         * GOAL: For each group (person), collect all their emails
         *
         * STRATEGY:
         * - mergedmail[i] = list of all emails belonging to the group
         *   represented by account i
         *
         * WHY WE DO THIS:
         * Right now, mailnode has email → some account index
         * But that account might not be the root (representative)!
         * We need to find the ROOT of each account's group and put all
         * emails of that group together.
         *
         * EXAMPLE:
         * After Step 1, DSU state: parent = [0, 0, 2, 0]
         * - Accounts 0, 1, 3 are connected (root = 0)
         * - Account 2 is separate (root = 2)
         *
         * mailnode might have:
         * "a@" → 0, "b@" → 1, "c@" → 3, "d@" → 2
         *
         * We want:
         * mergedmail[0] = ["a@", "b@", "c@"] (all emails of group with root 0)
         * mergedmail[2] = ["d@"] (all emails of group with root 2)
         *
         * HOW:
         * For each email, find the ROOT of the account it maps to,
         * then add that email to the root's email list.
         */
        vector<vector<string>> mergedmail(n);

        // Process each email in our map
        for (auto it : mailnode)
        {
            string mail = it.first;     // The email
            int accountIdx = it.second; // The account index we stored

            /*
             * CRITICAL STEP: Find the ROOT (representative) account
             *
             * WHY?
             * The account index stored in mailnode might not be the root!
             * If accounts 0, 1, 2 are all connected with root 0:
             * - mailnode might have "email1" → 0, "email2" → 1, "email3" → 2
             * - But they all belong to the SAME person (root 0)!
             * - We need to group ALL emails under the root account (0)
             *
             * findParent(accountIdx) gives us the root of the group
             * that this account belongs to.
             */
            int root = ds.findParent(accountIdx);

            /*
             * Add this email to the root account's email list
             *
             * All emails of accounts in the same group will be added
             * to the same root's list, effectively merging them!
             */
            mergedmail[root].push_back(mail);
        }

        /*
         * ═══════════════════════════════════════════════════════════════════
         * STEP 3: BUILD FINAL ANSWER
         * ═══════════════════════════════════════════════════════════════════
         *
         * GOAL: Format the output as required:
         * Each merged account = [Name, email1, email2, ..., emailN] (sorted)
         *
         * PROCESS:
         * - For each account index i:
         *   - If mergedmail[i] is empty: skip (not a root, or no emails)
         *   - If mergedmail[i] has emails: this is a root account!
         *     → Sort the emails
         *     → Create result: [name] + [sorted emails]
         */
        vector<vector<string>> ans;

        for (int i = 0; i < n; i++)
        {
            /*
             * WHY CHECK IF EMPTY?
             * Only root accounts will have emails in mergedmail[i]
             * Non-root accounts will have empty lists
             *
             * Example: If accounts 0, 1, 2 are connected with root 0
             * - mergedmail[0] = all emails (has content)
             * - mergedmail[1] = empty (not a root)
             * - mergedmail[2] = empty (not a root)
             *
             * We only process account 0 (the root)
             */
            if (mergedmail[i].size() == 0)
            {
                continue; // Not a root or no emails, skip
            }

            /*
             * This is a root account with emails!
             * Sort emails as required by problem (lexicographic order)
             */
            sort(mergedmail[i].begin(), mergedmail[i].end());

            /*
             * Build the result for this person
             * Format: [Name, email1, email2, ..., emailN]
             */
            vector<string> temp;

            /*
             * Add the name first
             * accounts[i][0] is the name of account i
             * Since i is the root, we use its name
             *
             * NOTE: All accounts in this group have the same name
             * (problem guarantee), so using the root's name is fine
             */
            temp.push_back(accounts[i][0]);

            /*
             * Add all sorted emails
             */
            for (auto it : mergedmail[i])
            {
                temp.push_back(it);
            }

            /*
             * Add this person's merged account to final answer
             */
            ans.push_back(temp);
        }

        return ans;

        /*
         * ═══════════════════════════════════════════════════════════════════
         * COMPLEXITY SUMMARY
         * ═══════════════════════════════════════════════════════════════════
         *
         * TIME COMPLEXITY: O(N × K × log(N × K))
         * Where N = number of accounts, K = average emails per account
         *
         * Breakdown:
         * - Step 1 (Union operations): O(N × K × α(N)) ≈ O(N × K)
         *   - Process each email: O(N × K)
         *   - Each union/find: O(α(N)) ≈ O(1)
         * - Step 2 (Grouping): O(N × K × α(N)) ≈ O(N × K)
         * - Step 3 (Sorting): O(N × K × log(K)) (dominant factor)
         * - Total: O(N × K × log(K))
         *
         * SPACE COMPLEXITY: O(N × K)
         * - DSU: O(N)
         * - mailnode HashMap: O(N × K) for all unique emails
         * - mergedmail: O(N × K)
         * - Output: O(N × K)
         * - Total: O(N × K)
         *
         * ═══════════════════════════════════════════════════════════════════
         * KEY TAKEAWAYS FOR REVISION
         * ═══════════════════════════════════════════════════════════════════
         *
         * 1. EMAIL AS BRIDGE: Shared emails connect accounts (same person)
         *
         * 2. FIRST-SEEN TRACKING: HashMap tracks first account for each email
         *    - First occurrence: Just store
         *    - Second occurrence: UNION! (found a connection)
         *
         * 3. TRANSITIVE MAGIC: DSU handles chains automatically
         *    - If A↔B and B↔C, DSU knows A↔B↔C are all connected
         *
         * 4. ROOT GROUPING: Only root accounts collect emails
         *    - Find root of each email's account
         *    - Add email to root's list
         *
         * 5. SORT & FORMAT: Final step is just formatting
         *    - Sort emails
         *    - Prepend name
         *    - Done!
         */
    }
};