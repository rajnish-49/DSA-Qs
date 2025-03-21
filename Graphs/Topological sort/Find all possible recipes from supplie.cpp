// @ https://leetcode.com/problems/find-all-possible-recipes-from-given-supplies/description/?envType=daily-question&envId=2025-03-21

vector<string> findAllRecipes(vector<string>& recipes, vector<vector<string>>& ingredients, vector<string>& supplies) {
    int n = recipes.size();  

    // Step 1: Store initial supplies in an unordered set for quick lookup
    unordered_set<string> st(begin(supplies), end(supplies));

    // Step 2: Create an adjacency list and in-degree array
    unordered_map<string, vector<int>> adj;  // Maps ingredients to recipes that depend on them
    vector<int> indegree(n, 0);  // Stores the number of missing ingredients for each recipe

    // Step 3: Build adjacency list and in-degree array
    for (int i = 0; i < n; i++) {  
        for (string& ing : ingredients[i]) {  
            // If the ingredient is not in the initial supplies
            if (!st.count(ing)) {  
                adj[ing].push_back(i);  // This ingredient contributes to making recipe[i]
                indegree[i]++;  // Increase in-degree as recipe[i] needs this ingredient
            }
        }
    }

    // Step 4: Initialize the queue with recipes that have zero missing ingredients
    queue<int> que;
    for (int i = 0; i < n; i++) {  
        if (indegree[i] == 0) {  // If a recipe has all required ingredients
            que.push(i);  // Add it to the queue for processing
        }
    }

    vector<string> result;  // Stores the recipes that can be made

    // Step 5: Process recipes in the queue (Topological Sorting using BFS)
    while (!que.empty()) {  
        int i = que.front();  // Get the front element (recipe index)
        que.pop();  // Remove it from the queue

        string recipe = recipes[i];  // Get the recipe name
        result.push_back(recipe);  // Add the recipe to the result as it can be made

        // Step 6: Reduce the in-degree of recipes dependent on this recipe
        for (int& idx : adj[recipe]) {  // For each recipe that depends on this recipe
            indegree[idx]--;  // Reduce its in-degree (one less missing ingredient)
            if (indegree[idx] == 0) {  // If it now has all required ingredients
                que.push(idx);  // Add it to the queue for processing
            }
        }
    }

    return result;  // Return the list of recipes that can be made
}
