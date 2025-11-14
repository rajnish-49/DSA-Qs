class Solution {
  public:
    double fractionalKnapsack(vector<int>& val, vector<int>& wt, int capacity) {
        int n = val.size();
        
        // Store items as (value, weight, ratio)
        vector<pair<double, pair<int,int>>> items; 
        for (int i = 0; i < n; i++) {
            double ratio = (double)val[i] / wt[i];
            items.push_back({ratio, {val[i], wt[i]}});
        }
        
        // Sort by ratio descending
        sort(items.rbegin(), items.rend());
        
        double totalValue = 0.0;
        
        for (int i = 0; i < n; i++) {
            int weight = items[i].second.second;
            int value  = items[i].second.first;
            
            if (capacity >= weight) {
                // Take whole item
                totalValue += value;
                capacity -= weight;
            } else {
                // Take fraction of item
                totalValue += items[i].first * capacity; 
                break;
            }
        }
        
        return totalValue;
    }
};
