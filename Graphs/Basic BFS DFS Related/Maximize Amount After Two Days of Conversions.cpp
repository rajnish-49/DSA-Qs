// @https://leetcode.com/problems/maximize-amount-after-two-days-of-conversions/

/*
====================================================================================
PROBLEM: Maximize Amount After Two Days of Conversions
====================================================================================

You start with 1.0 of initialCurrency.
Day 1: Convert using pairs1 and rates1
Day 2: Convert using pairs2 and rates2
Goal: Return maximum amount of initialCurrency after both days

====================================================================================
CORE INSIGHT: Two-Phase Optimization with Stopover Currency
====================================================================================

This is about finding the BEST "STOPOVER CURRENCY" - the currency you hold at the
end of Day 1 / start of Day 2.

Some currencies are:
- Easy to acquire on Day 1 but hard to sell back on Day 2
- Hard to acquire on Day 1 but great to sell back on Day 2

You need to TEST ALL POSSIBLE STOPOVERS to find which gives maximum final value!

MENTAL MODEL: Airport Layovers
Imagine flying from NYC to London with a layover:
- Route 1: NYC → Paris → London
- Route 2: NYC → Dubai → London

You can't decide the best route by only looking at Day 1 OR Day 2 prices.
You must evaluate COMPLETE ROUND-TRIP cost through each stopover city!

====================================================================================
ALGORITHM STRATEGY
====================================================================================

PHASE 1 (Day 1): "How much of EACH currency can I accumulate?"
- Start with 1.0 of initialCurrency
- Find MAX amount of EVERY currency reachable on Day 1

PHASE 2 (Day 2): "From EACH currency, how much initialCurrency can I get back?"
- For each currency, find MAX conversion back to initialCurrency on Day 2

PHASE 3 (Combine): "Which stopover gives the best round-trip?"
For each currency X:
  totalValue = (amount of X from 1.0 initialCurrency) × (amount of initialCurrency from 1.0 X)

Pick the X that maximizes totalValue!

TIME COMPLEXITY: O(V + E) for each BFS
SPACE COMPLEXITY: O(V + E) for graphs
*/

class Solution
{
public:
    /*
    ====================================================================================
    BFS HELPER: Find Maximum Conversion Rates
    ====================================================================================

    PURPOSE:
    Given a graph and starting currency, find the MAXIMUM amount of EVERY other 
    currency reachable through conversion chains.

    KEY DIFFERENCE FROM STANDARD BFS:
    - Standard BFS: Use visited set, find if reachable
    - Our BFS: Allow revisiting if we found a BETTER conversion rate

    WHY ALLOW REVISITING?
    Consider: A → B (rate 2.0), A → C → B (rate 3.0)
    First time we reach B: value = 2.0
    Second time we reach B: value = 3.0 (BETTER!)
    We MUST update B's value and explore its neighbors again!

    WHY NO INFINITE LOOP?
    - We only push if newAmount > old value (strictly better)
    - Conversion rates are positive and finite
    - Eventually, no better paths exist → queue becomes empty naturally
    */
    unordered_map<string, double> bfsMaxConversions(
        unordered_map<string, vector<pair<string, double>>> &graph,
        string startCurrency)
    {
        unordered_map<string, double> maxRates;
        queue<pair<string, double>> q;

        q.push({startCurrency, 1.0});
        maxRates[startCurrency] = 1.0;

        while (!q.empty())
        {
            auto [curr, amount] = q.front();
            q.pop();

            /*
            curr: Current currency we're exploring from
            amount: Accumulated value (how much of curr we have)
            
            Think of it like a ripple effect:
            - Start with 1.0 EUR
            - EUR → USD at 2.0 → USD gets value 2.0
            - USD → JPY at 3.0 → JPY gets value 6.0
            Each ripple carries ACCUMULATED multiplication!
            */

            for (auto &[neighbor, rate] : graph[curr])
            {
                double newAmount = amount * rate;

                /*
                CRITICAL DECISION:
                Update neighbor if:
                1. Never reached before, OR
                2. Found a BETTER path (newAmount > old value)

                WHY THIS CREATES CASCADING UPDATES:
                If we improve currency B, its neighbors might also improve!
                
                EXAMPLE:
                EUR → USD → JPY (path 1): JPY = 6.0
                EUR → GBP → JPY (path 2): JPY = 8.0 (better!)
                When we update JPY to 8.0, we re-explore JPY's neighbors
                because they might benefit from this better rate!
                */

                if (maxRates.find(neighbor) == maxRates.end() ||
                    maxRates[neighbor] < newAmount)
                {
                    maxRates[neighbor] = newAmount;
                    q.push({neighbor, newAmount});
                }
            }
        }

        return maxRates;
    }

    double maxAmount(string initialCurrency,
                     vector<vector<string>> &pairs1,
                     vector<double> &rates1,
                     vector<vector<string>> &pairs2,
                     vector<double> &rates2)
    {
        /*
        ====================================================================================
        STEP 1: Build TWO Separate Graphs
        ====================================================================================

        WHY SEPARATE?
        Day 1 and Day 2 have COMPLETELY DIFFERENT conversion rates!

        WHY BIDIRECTIONAL?
        If EUR → USD at rate 2.0, then USD → EUR at rate 1/2.0 = 0.5
        This allows exploring ALL possible conversion paths.
        */

        unordered_map<string, vector<pair<string, double>>> graph1;

        for (int i = 0; i < pairs1.size(); i++)
        {
            string from = pairs1[i][0];
            string to = pairs1[i][1];
            double rate = rates1[i];

            graph1[from].push_back({to, rate});
            graph1[to].push_back({from, 1.0 / rate});
        }

        unordered_map<string, vector<pair<string, double>>> graph2;

        for (int i = 0; i < pairs2.size(); i++)
        {
            string from = pairs2[i][0];
            string to = pairs2[i][1];
            double rate = rates2[i];

            graph2[from].push_back({to, rate});
            graph2[to].push_back({from, 1.0 / rate});
        }

        /*
        ====================================================================================
        STEP 2: Day 1 Analysis - "What can I accumulate?"
        ====================================================================================

        QUESTION:
        "Starting with 1.0 initialCurrency, what's the MAXIMUM amount of EACH 
        currency I can get by end of Day 1?"

        WHY THIS MATTERS:
        We need to know ALL possible currencies we could hold after Day 1.
        We don't know yet which will be best for Day 2!

        EXAMPLE OUTPUT:
        day1Rates = {
            "EUR": 1.0,    // Starting currency
            "USD": 2.0,    // Can get 2.0 USD from 1.0 EUR
            "JPY": 6.0     // Can get 6.0 JPY from 1.0 EUR (via USD)
        }
        */

        unordered_map<string, double> day1Rates =
            bfsMaxConversions(graph1, initialCurrency);

        /*
        ====================================================================================
        STEP 3: Day 2 Analysis - "What can I convert back to?"
        ====================================================================================

        QUESTION:
        "If I START Day 2 with 1.0 of ANY currency, what's the MAXIMUM initialCurrency 
        I can get back?"

        KEY INSIGHT - WHY BFS FROM INITIALCURRENCY WORKS:
        The graph is BIDIRECTIONAL!
        A path EUR → JPY with product 120.0 means:
        - Going EUR → JPY multiplies by 120.0
        - Going JPY → EUR also multiplies by 120.0 (same edges, opposite direction!)

        EXAMPLE OUTPUT:
        day2Rates = {
            "EUR": 1.0,     // Already in initialCurrency
            "USD": 30.0,    // 1.0 USD → 30.0 EUR on Day 2
            "JPY": 120.0    // 1.0 JPY → 120.0 EUR on Day 2
        }

        INTERPRETATION:
        day2Rates["JPY"] = 120.0 means:
        "If I hold 1.0 JPY at start of Day 2, I can convert to 120.0 EUR"
        */

        unordered_map<string, double> day2Rates =
            bfsMaxConversions(graph2, initialCurrency);

        /*
        ====================================================================================
        STEP 4: Combine Results - "Which stopover is best?"
        ====================================================================================

        FOR EACH CURRENCY X:
        Round-trip value = day1Rates[X] × day2Rates[X]

        WHY THIS MULTIPLICATION?

        CONCRETE EXAMPLE with JPY:
        day1Rates["JPY"] = 6.0  → "I can get 6.0 JPY from 1.0 EUR"
        day2Rates["JPY"] = 120.0 → "Each 1.0 JPY converts to 120.0 EUR"

        JOURNEY:
        Start:  1.0 EUR
        Day 1:  1.0 EUR → 6.0 JPY
        Day 2:  6.0 JPY → 6.0 × 120.0 = 720.0 EUR

        FORMULA: 1.0 × day1Rates["JPY"] × day2Rates["JPY"] = 720.0

        WHY TEST ALL CURRENCIES?
        The best Day 1 currency might not have the best Day 2 conversion!
        Must evaluate COMPLETE ROUND TRIP for each option.
        */

        double maxResult = 1.0;  // Base case: don't convert (stay in initialCurrency)

        unordered_set<string> allCurrencies;
        for (auto &[currency, _] : day1Rates)
        {
            allCurrencies.insert(currency);
        }
        for (auto &[currency, _] : day2Rates)
        {
            allCurrencies.insert(currency);
        }

        /*
        WHY COLLECT ALL CURRENCIES?
        Only currencies that appear in BOTH day1Rates AND day2Rates are valid stopovers.
        Some might only be reachable on Day 1 (can't convert back on Day 2).
        Some might only be reachable on Day 2 (can't acquire on Day 1).
        */

        for (const string &currency : allCurrencies)
        {
            if (day1Rates.find(currency) != day1Rates.end() &&
                day2Rates.find(currency) != day2Rates.end())
            {
                double roundTrip = day1Rates[currency] * day2Rates[currency];

                /*
                WHAT THIS REPRESENTS:
                Total multiplier for round trip through 'currency'
                
                VISUALIZATION:
                1.0 initialCurrency
                   ↓ (multiply by day1Rates[currency])
                X units of currency
                   ↓ (multiply by day2Rates[currency])
                Final amount of initialCurrency
                */

                maxResult = max(maxResult, roundTrip);
            }
        }

        return maxResult;
    }
};

/*
====================================================================================
COMPLETE WALKTHROUGH EXAMPLE
====================================================================================

Input:
initialCurrency = "EUR"
pairs1 = [["EUR","USD"],["USD","JPY"]], rates1 = [2.0, 3.0]
pairs2 = [["JPY","USD"],["USD","CHF"],["CHF","EUR"]], rates2 = [4.0, 5.0, 6.0]

STEP 1: Build Graphs
--------------------
Graph 1 (Day 1):
  EUR ↔ USD (2.0, 0.5)
  USD ↔ JPY (3.0, 0.33)

Graph 2 (Day 2):
  JPY ↔ USD (4.0, 0.25)
  USD ↔ CHF (5.0, 0.2)
  CHF ↔ EUR (6.0, 0.17)

STEP 2: Day 1 BFS from EUR with 1.0
------------------------------------
Process EUR (1.0):
  → USD: 1.0 × 2.0 = 2.0
  maxRates: {EUR: 1.0, USD: 2.0}

Process USD (2.0):
  → JPY: 2.0 × 3.0 = 6.0
  maxRates: {EUR: 1.0, USD: 2.0, JPY: 6.0}

RESULT: day1Rates = {EUR: 1.0, USD: 2.0, JPY: 6.0}

STEP 3: Day 2 BFS from EUR with 1.0
------------------------------------
Process EUR (1.0):
  → CHF: 1.0 × 6.0 = 6.0

Process CHF (6.0):
  → USD: 6.0 × 5.0 = 30.0

Process USD (30.0):
  → JPY: 30.0 × 4.0 = 120.0

RESULT: day2Rates = {EUR: 1.0, CHF: 6.0, USD: 30.0, JPY: 120.0}

STEP 4: Test All Stopovers
---------------------------
- EUR: 1.0 × 1.0 = 1.0
- USD: 2.0 × 30.0 = 60.0
- JPY: 6.0 × 120.0 = 720.0 ✅ MAXIMUM!

ANSWER: 720.0

THE WINNING PATH:
Day 1: 1.0 EUR → 2.0 USD → 6.0 JPY
Day 2: 6.0 JPY → 24.0 USD → 120.0 CHF → 720.0 EUR

WHY JPY WAS OPTIMAL:
- Good acquisition rate on Day 1 (6.0)
- Excellent conversion back on Day 2 (120.0)
- Combined: 6.0 × 120.0 = 720.0 beats all other options!
*/