// @ https://leetcode.com/problems/find-elements-in-a-contaminated-binary-tree/

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class FindElements {

    unordered_set <int> s;

    void dfs (TreeNode* root , int corrected){

    if ( root == NULL){
        return;
    }

    root -> val = corrected;
    s.insert(root->val);

    if( root -> left ){
        dfs ( root -> left  , 2*(root->val) + 1 );
    }

     if( root -> right ){
        dfs ( root -> right  , 2*(root->val)+ 2 );
    }
    }

public:
  
    FindElements(TreeNode* root) {

    if ( root != NULL ){
       dfs(root , 0 ); 
    }
    }

   bool find(int target) {
    return s.find(target) != s.end(); 
}

};

/**
 * Your FindElements object will be instantiated and called as such:
 * FindElements* obj = new FindElements(root);
 * bool param_1 = obj->find(target);
 */