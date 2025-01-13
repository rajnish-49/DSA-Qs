class Solution {
    private:
     
    void solve (string digits , vector<string>& ans ,string output , int index , string keyboard []){

        // base case 
        if ( index >= digits.length()){
            ans.push_back(output);
            return;
        }

    else{
        int number = digits[index] - '0';
        string value = keyboard [number];

        for ( char  ch : value ){
            output.push_back(ch);
            solve ( digits , ans , output , index + 1  , keyboard );
            output.pop_back();
        }
    }
}

public:
    vector <string> letterCombinations(string digits) {

        vector<string> ans;

        if (digits.empty()) return ans;

        int index = 0 ;
        string output;
        string keyboard[10]={"", "" ,"abc" ,"def" , "ghi", "jkl" , "mno" , "pqrs" , "tuv" ,"wxyz"};

        solve ( digits , ans , output , index , keyboard );
        return ans;
    }
};