// @https://leetcode.com/problems/smallest-pair-with-different-frequencies/description/

class Solution {
public:
    vector<int> smallestPair(vector<int>& nums) {
        map<int,int> freq;
        for(int n : nums) freq[n]++;
        
        vector<pair<int,int>> vals(freq.begin(), freq.end());
        
        for(int i = 0; i < vals.size(); i++){
            for(int j = i+1; j < vals.size(); j++){
                if(vals[i].second != vals[j].second)
                    return {vals[i].first, vals[j].first};
            }
        }
        return {-1,-1};
    }
};



class Solution {
public:
    vector<int> smallestPair(vector<int>& nums) {
        int freq[101] = {};
        for(int n : nums) freq[n]++;
        
        for(int i = 1; i <= 100; i++){
            if(!freq[i]) continue;
            for(int j = i+1; j <= 100; j++){
                if(!freq[j]) continue;
                if(freq[i] != freq[j])
                    return {i, j};
            }
        }
        return {-1,-1};
    }
};