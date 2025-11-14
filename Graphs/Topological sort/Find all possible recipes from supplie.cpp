// @https://leetcode.com/problems/find-all-possible-recipes-from-given-supplies/description/

class Solution {
    public:
        vector<string> findAllRecipes(vector<string>& recipes,
                                      vector<vector<string>>& ingredients,
                                      vector<string>& supplies) {
    
            // Adjacency list (Graph representation)
            // Key: Ingredient | Value: List of recipes that require this ingredient
            unordered_map<string, vector<string>> adj;
    
            // In-degree map to store how many ingredients each recipe still needs
            unordered_map<string, int> inDegree;
    
            // A set to store all initially available supplies for quick lookup
            unordered_set<string> available(supplies.begin(), supplies.end());
    
            // Result vector to store the list of recipes that can be made
            vector<string> ans;
    
            /*** Step 1: Construct the graph and in-degree map ***/
            for (int i = 0; i < recipes.size(); i++) {
                string recipe = recipes[i];
    
                // Set the initial in-degree (number of missing ingredients)
                inDegree[recipe] = ingredients[i].size();
    
                // Iterate through the ingredients required for this recipe
                for (string& ingredient : ingredients[i]) {
                    // Create the adjacency list: this ingredient helps make this recipe
                    adj[ingredient].push_back(recipe);
                }
            }
    
            /*** Step 2: Initialize queue with all available supplies ***/
            queue<string> q;
    
            // Push all initial supplies into the queue (they are available from the start)
            for (auto& supply : supplies) {
                q.push(supply);
            }
    
            /* Step 3: Process ingredients and update dependent recipes */
            while (!q.empty()) {
                string ingredient = q.front(); // Get the current ingredient from the queue
                q.pop(); // Remove it from the queue
    
                // If the ingredient is used in any recipes, process them
                for (string& recipe : adj[ingredient]) {
                    inDegree[recipe]--; // Reduce the count of missing ingredients for this recipe
    
                    // If all required ingredients are now available (inDegree == 0)
                    if (inDegree[recipe] == 0) {
                        q.push(recipe);  // Add this recipe to the queue, as it can now be made
                        ans.push_back(recipe);  // Store the recipe in the result
                    }
                }
            }
    
            /* Step 4: Return the list of recipes that can be made */
            return ans;
        }
    };
    