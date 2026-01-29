#include <bits/stdc++.h>
using namespace std;

/*
╔════════════════════════════════════════════════════════════════════════════════╗
║                          NINJA'S TRAINING PROBLEM                              ║
╚════════════════════════════════════════════════════════════════════════════════╝

PROBLEM ESSENCE:
----------------
Ninja trains for N days. Each day he can do one of 3 activities (0, 1, 2).
Each activity gives different merit points on different days.
CONSTRAINT: He cannot do the SAME activity on consecutive days.
GOAL: Maximize total merit points over N days.

CORE INSIGHT - Why DP?
-----------------------
1. We have CHOICES at each step (which activity to do)
2. Current choice DEPENDS on previous choice (can't repeat activity)
3. We want OPTIMAL solution (maximum points)
4. Problem has OVERLAPPING SUBPROBLEMS (same states visited multiple times)
5. Problem has OPTIMAL SUBSTRUCTURE (optimal solution contains optimal subsolutions)

FUNDAMENTAL APPROACH:
---------------------
Think of it as building a path through days where:
- At each day, you stand at one of 3 activities
- You can only move to a DIFFERENT activity next day
- Each move gives you points
- Find the path with maximum total points

WHY TABULATION (Bottom-Up)?
----------------------------
We build the solution from day 0 onwards. By the time we reach day i,
we already know the best way to reach each activity on day i-1.
This eliminates redundant calculations and gives us O(N) time complexity.

DP STATE DEFINITION:
--------------------
dp[i][j] = Maximum merit points achievable from day 0 to day i,
           when activity j is performed on day i

Why track which activity? Because it affects what we can do NEXT day!

RECURRENCE RELATION:
--------------------
dp[i][j] = points[i][j] + max(dp[i-1][k]) where k ≠ j
           ↑                ↑
           |                |
      Points today    Best points till yesterday
                      (from different activity)

INTUITION: To get max points by doing activity j today, we need:
1. Points we get for activity j today
2. PLUS the best we could have done till yesterday
   BUT only from activities that are NOT j (due to constraint)
*/

int ninjaTraining(int n, vector<vector<int>> &points) {
    
    /*
    ═══════════════════════════════════════════════════════════════
                            DP TABLE SETUP
    ═══════════════════════════════════════════════════════════════
    
    Create a 2D table: dp[days][activities]
    
    Dimensions: n × 3
    - n rows: one for each day (0 to n-1)
    - 3 columns: one for each activity (0, 1, 2)
    
    Visual representation of what we're building:
    
             Activity 0   Activity 1   Activity 2
    Day 0  [  points[0][0], points[0][1], points[0][2]  ]
    Day 1  [   dp[1][0]   ,  dp[1][1]  ,   dp[1][2]    ]
    Day 2  [   dp[2][0]   ,  dp[2][1]  ,   dp[2][2]    ]
    ...
    Day n-1[  dp[n-1][0]  , dp[n-1][1] ,  dp[n-1][2]  ]
    
    Each cell answers: "What's the max points till this day if I do this activity today?"
    */
    vector<vector<int>> dp(n, vector<int>(3, 0));
    
    
    /*
    ═══════════════════════════════════════════════════════════════
                            BASE CASE (Day 0)
    ═══════════════════════════════════════════════════════════════
    
    On the first day, there's NO previous day, so NO constraint!
    Ninja can freely choose any activity.
    
    The maximum points for doing activity j on day 0 is simply
    the points that activity gives on day 0.
    
    INTUITION: Starting positions - we just take what's offered.
    No optimization needed yet since there's no history to consider.
    */
    dp[0][0] = points[0][0];  // Max points if we do activity 0 on day 0
    dp[0][1] = points[0][1];  // Max points if we do activity 1 on day 0
    dp[0][2] = points[0][2];  // Max points if we do activity 2 on day 0
    

    /*
    ═══════════════════════════════════════════════════════════════
                    FILL TABLE (Days 1 to n-1)
    ═══════════════════════════════════════════════════════════════
    
    CORE ALGORITHM - Understanding the Triple Nested Loop:
    
    Outer Loop (day): Which day are we currently solving for?
    Middle Loop (last): Which activity do we want to do TODAY?
    Inner Loop (prev): Which activity did we do YESTERDAY?
    
    THE BIG IDEA:
    -------------
    For each day and each activity we might do today, we ask:
    "If I do THIS activity today, what's the BEST I could have done
     till yesterday from a DIFFERENT activity?"
    
    We try ALL possible previous activities (that are different)
    and keep the MAXIMUM.
    */
    for(int day = 1; day < n; day++) {
        
        /*
        For this day, we'll calculate optimal points for ALL 3 activities.
        Each iteration of this loop fills ONE cell in our DP table.
        */
        for(int last = 0; last < 3; last++) {
            
            /*
            CURRENT MISSION:
            Calculate dp[day][last] = max points till 'day' if we do activity 'last' today
            
            STRATEGY:
            We'll try combining today's activity 'last' with all valid yesterday activities.
            Valid means: prev ≠ last (can't repeat activity)
            */
            
            dp[day][last] = 0;  // Initialize: start with 0, will find maximum
            
            /*
            CRITICAL INNER LOOP - The Heart of the Algorithm
            ------------------------------------------------
            
            Question we're answering:
            "I want to do activity 'last' today. What was the best path
             to reach yesterday, considering I must have done something DIFFERENT?"
            
            We check all 3 possible previous activities (prev = 0, 1, 2)
            But we only consider those where prev ≠ last (the constraint!)
            */
            for(int prev = 0; prev < 3; prev++) {
                
                /*
                THE CONSTRAINT CHECK:
                ---------------------
                If prev == last, it means we'd be doing the same activity
                two days in a row, which violates the problem constraint.
                So we SKIP this case.
                
                Example: If last = 0 (Running today), we skip prev = 0
                         (can't have Running yesterday too)
                */
                if(prev != last) {
                    
                    /*
                    RECURRENCE RELATION IN ACTION:
                    ------------------------------
                    
                    point = points[day][last] + dp[day-1][prev]
                            ↑                    ↑
                            |                    |
                    Points for doing         Best points till yesterday
                    activity 'last'          if we did activity 'prev'
                    today                    yesterday
                    
                    INTUITION:
                    - dp[day-1][prev] tells us: "best we could do till yesterday
                      if we ended with activity prev"
                    - points[day][last] tells us: "what we get for activity last today"
                    - Adding them gives us: "total if we follow this path"
                    
                    Example with numbers:
                    If day=1, last=0 (Running), prev=2 (Learning):
                    point = points[1][0] + dp[0][2]
                          = 3 (running today) + 5 (best till yesterday with learning)
                          = 8 total points
                    */
                    int point = points[day][last] + dp[day-1][prev];
                    
                    /*
                    MAXIMIZATION:
                    -------------
                    We're trying multiple 'prev' values (different yesterday activities).
                    We keep the MAXIMUM because we want the optimal solution.
                    
                    After trying all valid 'prev' values, dp[day][last] will contain
                    the best possible points for doing activity 'last' on 'day'.
                    
                    WHY MAX?
                    Because dp[day][last] should represent the BEST way to reach
                    activity 'last' on day 'day', considering all possible paths.
                    */
                    dp[day][last] = max(dp[day][last], point);
                }
            }
            /*
            After inner loop completes:
            dp[day][last] now contains the maximum points achievable
            till 'day' if we do activity 'last' on 'day'
            */
        }
        /*
        After middle loop completes:
        We've filled dp[day][0], dp[day][1], dp[day][2]
        i.e., all three cells for this day
        */
    }
    /*
    After outer loop completes:
    Our entire DP table is filled!
    Every cell dp[i][j] contains the optimal answer for its subproblem.
    */
    
    /*
    ═══════════════════════════════════════════════════════════════
                            FINAL ANSWER
    ═══════════════════════════════════════════════════════════════
    
    The DP table is complete. Now, where's our answer?
    
    On the last day (day n-1), we have 3 possible ending states:
    - dp[n-1][0]: max points if we end with activity 0
    - dp[n-1][1]: max points if we end with activity 1
    - dp[n-1][2]: max points if we end with activity 2
    
    Since we can END with ANY activity (no next day to worry about),
    we take the MAXIMUM of these three values.
    
    INTUITION:
    Imagine three different complete paths through all N days,
    each ending at a different activity. We pick the path with
    the highest total points.
    
    This is our final answer: the maximum merit points Ninja can earn!
    */
    int maxPoints = max(dp[n-1][0], max(dp[n-1][1], dp[n-1][2]));
    return maxPoints;
}

/*
╔════════════════════════════════════════════════════════════════════════════════╗
║                          COMPLEXITY ANALYSIS                                   ║
╚════════════════════════════════════════════════════════════════════════════════╝

TIME COMPLEXITY: O(N × 3 × 3) = O(9N) = O(N)
- Outer loop: N iterations (days)
- Middle loop: 3 iterations (activities for today)
- Inner loop: 3 iterations (activities for yesterday)
- Total: N × 3 × 3 = 9N operations

SPACE COMPLEXITY: O(N × 3) = O(N)
- DP table of size N × 3
- Can be optimized to O(1) by keeping only previous day's values

╔════════════════════════════════════════════════════════════════════════════════╗
║                          EXAMPLE WALKTHROUGH                                   ║
╚════════════════════════════════════════════════════════════════════════════════╝

Input: points = [[1,2,5], [3,1,1], [3,3,3]]

Day 0 (Base case):
dp[0] = [1, 2, 5]  (just copy points[0])

Day 1:
- dp[1][0]: Do activity 0 today
  * From prev=1: 3 + 2 = 5
  * From prev=2: 3 + 5 = 8 ← MAX
  * dp[1][0] = 8

- dp[1][1]: Do activity 1 today
  * From prev=0: 1 + 1 = 2
  * From prev=2: 1 + 5 = 6 ← MAX
  * dp[1][1] = 6

- dp[1][2]: Do activity 2 today
  * From prev=0: 1 + 1 = 2
  * From prev=1: 1 + 2 = 3 ← MAX
  * dp[1][2] = 3

Result: dp[1] = [8, 6, 3]

Day 2:
- dp[2][0]: 3 + max(6,3) = 3 + 6 = 9
- dp[2][1]: 3 + max(8,3) = 3 + 8 = 11 ← FINAL MAX
- dp[2][2]: 3 + max(8,6) = 3 + 8 = 11 ← FINAL MAX

Answer: max(9, 11, 11) = 11

Path: Day 0: Activity 2 (5 points)
      Day 1: Activity 0 (3 points)
      Day 2: Activity 1 (3 points)
      Total: 5 + 3 + 3 = 11 ✓
*/


#include <bits/stdc++.h>
using namespace std;

/*
╔════════════════════════════════════════════════════════════════════════════════╗
║                    NINJA'S TRAINING - TOP-DOWN APPROACH                        ║
║                          (Memoization / Recursion)                             ║
╚════════════════════════════════════════════════════════════════════════════════╝

PROBLEM RECAP:
--------------
Ninja trains for N days. Each day has 3 activities (0: Running, 1: Fighting, 2: Learning).
Each activity gives different merit points on different days.
CONSTRAINT: Cannot do the SAME activity on consecutive days.
GOAL: Maximize total merit points over all N days.

═══════════════════════════════════════════════════════════════════════════════
                    WHY TOP-DOWN (RECURSION + MEMOIZATION)?
═══════════════════════════════════════════════════════════════════════════════

MENTAL MODEL: "Making Choices and Exploring Futures"
-----------------------------------------------------

Imagine you're standing at day 0. You have 3 doors (activities) in front of you.
You think: "If I go through door X, what's the BEST total reward I can get 
from today till the end?"

This is FORWARD THINKING (future rewards), not backward accumulation.

TOP-DOWN MINDSET:
- Start from the beginning (day 0)
- At each step, ASK: "What happens if I choose this activity?"
- RECURSE to find out the future consequences
- REMEMBER the answer (memoization) so we don't recalculate

BOTTOM-UP MINDSET (for comparison):
- Start from the beginning (day 0)
- TELL yourself: "Based on what I've done so far, what's best now?"
- BUILD up answers iteratively

Both solve the same problem, just different thinking styles!

═══════════════════════════════════════════════════════════════════════════════
                         TOP-DOWN DP STATE DEFINITION
═══════════════════════════════════════════════════════════════════════════════

STATE: solve(day, last)

MEANING: 
"What is the MAXIMUM merit points I can earn from day 'day' to day 'n-1',
 given that on day 'day-1' (yesterday), I performed activity 'last'?"

PARAMETERS:
-----------
1. 'day': Current day we're making a decision for (0 to n-1)
2. 'last': Activity performed YESTERDAY (restricts today's choice)
           - Values: 0, 1, 2 (activities) or 3 (special: "no previous activity")
           - Why 3? On day 0, there's no yesterday, so all activities are valid

RETURN VALUE:
-------------
Maximum points achievable from 'day' onwards, respecting the constraint.

WHY THIS STATE WORKS:
---------------------
To make an optimal decision TODAY, we need to know:
1. Which day we're on (determines available points)
2. What we did YESTERDAY (determines what's forbidden today)

This is MINIMAL information needed - nothing more, nothing less.

═══════════════════════════════════════════════════════════════════════════════
                            RECURRENCE RELATION
═══════════════════════════════════════════════════════════════════════════════

solve(day, last) = max over all valid activities of:
                   [points[day][activity] + solve(day+1, activity)]
                    ↑                      ↑
                    |                      |
              Immediate reward       Future reward (recursive call)

INTUITION:
----------
"To find the best path from TODAY, I try each valid door (activity).
 For each door, I calculate: reward from this door + best path from TOMORROW.
 I pick the door that gives me the MAXIMUM total."

CONSTRAINT HANDLING:
--------------------
We only try activities where: activity != last
This ensures we don't repeat yesterday's activity.

BASE CASE:
----------
When day == n (gone past the last day):
- No more days = no more points to earn
- Return 0 (identity element for addition)

This naturally handles the last day (day n-1):
- solve(n-1, last) will try activities and call solve(n, activity)
- solve(n, activity) returns 0
- So we get: points[n-1][activity] + 0 = points[n-1][activity]
- Perfect! The last day just contributes its own points.

═══════════════════════════════════════════════════════════════════════════════
                              MEMOIZATION LOGIC
═══════════════════════════════════════════════════════════════════════════════

WHY MEMOIZATION?
----------------
Without memoization, we'd solve the same subproblems repeatedly.

Example: solve(5, 0) might be called from:
- solve(4, 1) choosing activity 0
- solve(4, 2) choosing activity 0

Instead of calculating solve(5, 0) twice, we:
1. Calculate it ONCE
2. STORE the result in dp[5][0]
3. Next time we need solve(5, 0), just RETURN dp[5][0]

This reduces time complexity from O(3^n) to O(n × 4 × 3) = O(n)

MEMOIZATION TABLE:
------------------
dp[day][last] = Result of solve(day, last)
- Initially: -1 (means "not computed yet")
- After computation: stores the maximum points

═══════════════════════════════════════════════════════════════════════════════
                          COMPARISON WITH BOTTOM-UP
═══════════════════════════════════════════════════════════════════════════════

BOTTOM-UP (Tabulation):
- Direction: Past → Present
- Question: "How did I get here with maximum points?"
- State: dp[day][activity] = max points FROM START TO day, ending with activity
- Natural for: Iterative thinkers

TOP-DOWN (Memoization):
- Direction: Present → Future
- Question: "Where can I go from here to get maximum points?"
- State: solve(day, last) = max points FROM day TO END, given last activity
- Natural for: Recursive thinkers

BOTH GIVE SAME ANSWER! Just different perspectives.
*/

int solve(int day, int last, vector<vector<int>>& points, vector<vector<int>>& dp) {
    
    /*
    ═══════════════════════════════════════════════════════════════════════════
                                BASE CASE
    ═══════════════════════════════════════════════════════════════════════════
    
    CONDITION: day == points.size() (which is n)
    
    MEANING: We've exhausted all days. We're standing "after" the last day.
    
    WHY RETURN 0?
    -------------
    - No more days exist
    - No more points can be earned
    - This is the "termination" of our recursion
    
    ANALOGY:
    --------
    You're reading a book. When you reach "The End", there are no more pages.
    Reading beyond the book = 0 additional content.
    
    HOW THIS HELPS:
    ---------------
    When we're at the ACTUAL last day (day n-1), we'll call solve(n, activity).
    This returns 0, so the last day contributes ONLY its own points.
    
    Example:
    solve(2, 0) where n=3 (last day)
    → Try activity 1: points[2][1] + solve(3, 1)
                      3            +     0        = 3
    → Try activity 2: points[2][2] + solve(3, 2)
                      3            +     0        = 3
    Perfect! Last day just adds its points, nothing extra.
    */
    if(day == points.size()) {
        return 0;
    }
    
    /*
    ═══════════════════════════════════════════════════════════════════════════
                            MEMOIZATION CHECK
    ═══════════════════════════════════════════════════════════════════════════
    
    OPTIMIZATION: Avoid Redundant Calculations
    -------------------------------------------
    
    Before doing expensive recursive computation, check:
    "Have I solved this exact subproblem before?"
    
    HOW WE CHECK:
    -------------
    dp[day][last] stores the result of solve(day, last).
    - If dp[day][last] == -1: Not computed yet, need to calculate
    - If dp[day][last] != -1: Already computed, just return stored value
    
    WHY THIS MATTERS:
    -----------------
    Consider this recursion tree without memoization:
    
                        solve(0, 3)
                    /        |        \
            solve(1,0)  solve(1,1)  solve(1,2)
              /  \         /  \         /  \
           ... ...       ... ...     ... ...
    
    Many branches will call the SAME solve(day, last) with same parameters!
    Without memoization: O(3^n) - exponential time (VERY SLOW)
    With memoization: O(n × 4 × 3) = O(n) - linear time (FAST)
    
    DRAMATIC DIFFERENCE:
    For n=20:
    - Without memoization: ~3,486,784,401 operations
    - With memoization: ~240 operations
    
    That's why this check is CRUCIAL!
    */
    if(dp[day][last] != -1) {
        return dp[day][last];
    }
    
    /*
    ═══════════════════════════════════════════════════════════════════════════
                        RECURSIVE EXPLORATION OF CHOICES
    ═══════════════════════════════════════════════════════════════════════════
    
    CORE ALGORITHM: Try All Valid Activities, Keep Best
    ----------------------------------------------------
    
    STRATEGY:
    1. We're at day 'day', yesterday we did activity 'last'
    2. Today, we can do any activity EXCEPT 'last' (constraint)
    3. For EACH valid activity, calculate: immediate reward + future reward
    4. Keep track of the MAXIMUM across all choices
    
    VISUAL REPRESENTATION:
    ----------------------
    Standing at day 'day' with 3 doors ahead:
    
         ┌──────────────┐
         │   Day 'day'  │
         └──────┬───────┘
                │
         ┌──────┴───────┬───────────┬──────────┐
         │              │           │          │
    Door 0 (Run)   Door 1 (Fight)  Door 2 (Learn)
         │              │           │
    (if last!=0)   (if last!=1)  (if last!=2)
         │              │           │
         ↓              ↓           ↓
    points[day][0]  points[day][1]  points[day][2]
         +              +             +
    solve(day+1,0)  solve(day+1,1)  solve(day+1,2)
    
    We try each valid door, calculate total reward, and pick the best!
    */
    
    int maxPoints = 0;  // Will store the maximum points across all choices
    
    /*
    LOOP THROUGH ALL 3 ACTIVITIES (0: Running, 1: Fighting, 2: Learning)
    ---------------------------------------------------------------------
    
    For each activity, we're asking:
    "What if I do THIS activity today? How much total can I earn from today onwards?"
    */
    for(int activity = 0; activity < 3; activity++) {
        
        /*
        ═══════════════════════════════════════════════════════════════════════
                            CONSTRAINT VALIDATION
        ═══════════════════════════════════════════════════════════════════════
        
        CRITICAL CHECK: activity != last
        ---------------------------------
        
        This enforces the problem constraint:
        "Cannot do the same activity on consecutive days"
        
        CASES:
        ------
        1. If last == 3 (day 0, no previous activity):
           - All activities are valid (3 != 0, 3 != 1, 3 != 2)
           - Ninja can choose freely
        
        2. If last == 0 (yesterday was Running):
           - Can do Fighting (1) or Learning (2)
           - Cannot do Running (0) again
        
        3. If last == 1 (yesterday was Fighting):
           - Can do Running (0) or Learning (2)
           - Cannot do Fighting (1) again
        
        4. If last == 2 (yesterday was Learning):
           - Can do Running (0) or Fighting (1)
           - Cannot do Learning (2) again
        
        WHY SKIP?
        ---------
        If activity == last, this choice is INVALID due to the constraint.
        We don't even calculate it - just skip to next iteration.
        */
        if(activity != last) {
            
            /*
            ═══════════════════════════════════════════════════════════════════
                            RECURSIVE CALCULATION
            ═══════════════════════════════════════════════════════════════════
            
            FORMULA: points[day][activity] + solve(day+1, activity, points, dp)
                     ↑                       ↑
                     |                       |
                Immediate Reward        Future Reward (Recursive Call)
            
            BREAKDOWN:
            ----------
            1. points[day][activity]: 
               - Points we earn by doing 'activity' on 'day'
               - This is our IMMEDIATE payoff for this choice
            
            2. solve(day+1, activity, points, dp):
               - RECURSIVE CALL to find best path from TOMORROW onwards
               - We pass 'activity' as the new 'last' parameter
               - This represents: "I did 'activity' today, now what's best from tomorrow?"
               - This is our FUTURE payoff for this choice
            
            3. Sum = Total reward for choosing this activity today
            
            INTUITION:
            ----------
            Think of it like chess:
            - You make a move (choose an activity) → immediate consequence
            - You evaluate "what's my best play after this?" → future consequence
            - Total value = immediate + future
            
            EXAMPLE:
            --------
            Day 1, last activity was 2 (Learning), trying activity 0 (Running):
            
            points_earned = points[1][0] + solve(2, 0, points, dp)
                          = 3            + (best from day 2 onwards, having done 0)
                          = 3            + solve(2, 0, ...)
            
            The recursive call solve(2, 0, ...) will:
            - Try activities 1 and 2 (not 0)
            - Recursively calculate their futures
            - Return the maximum
            
            Then we add 3 (today's points) to that maximum.
            */
            int points_earned = points[day][activity] + solve(day + 1, activity, points, dp);
            
            /*
            ═══════════════════════════════════════════════════════════════════
                            MAXIMIZATION (GREEDY CHOICE)
            ═══════════════════════════════════════════════════════════════════
            
            OPERATION: maxPoints = max(maxPoints, points_earned)
            
            WHY?
            ----
            We're trying MULTIPLE activities (multiple branches in recursion).
            We want the BEST outcome, so we keep the MAXIMUM.
            
            TRACKING:
            ---------
            - Start with maxPoints = 0 (worst case: no points)
            - Try activity 0 (if valid): update maxPoints if better
            - Try activity 1 (if valid): update maxPoints if better
            - Try activity 2 (if valid): update maxPoints if better
            - After loop: maxPoints contains the BEST choice
            
            ANALOGY:
            --------
            You're trying different routes on a map.
            Each route has a distance. You keep track of the SHORTEST.
            Here, we keep track of the HIGHEST points.
            
            EXAMPLE TRACE:
            --------------
            maxPoints = 0 (initially)
            Try activity 0: points_earned = 6 → maxPoints = max(0, 6) = 6
            Try activity 1: points_earned = 4 → maxPoints = max(6, 4) = 6
            Final: maxPoints = 6 (activity 0 was best)
            */
            maxPoints = max(maxPoints, points_earned);
        }
    }
    
    /*
    ═══════════════════════════════════════════════════════════════════════════
                        MEMOIZATION STORAGE & RETURN
    ═══════════════════════════════════════════════════════════════════════════
    
    STORE RESULT: dp[day][last] = maxPoints
    
    WHY STORE?
    ----------
    We just computed solve(day, last) through expensive recursive exploration.
    If we're ever asked to compute solve(day, last) AGAIN in the future,
    we can instantly return dp[day][last] instead of recalculating.
    
    This is the HEART of memoization: "Remember what you've learned"
    
    CACHE POPULATION:
    -----------------
    As recursion progresses (especially in the RETURN phase), we populate
    the dp table from the bottom up (deeper recursions return first).
    
    Example execution order:
    1. solve(0, 3) calls solve(1, 0), solve(1, 1), solve(1, 2)
    2. solve(1, 0) calls solve(2, 1), solve(2, 2)
    3. solve(2, 1) calls solve(3, 0), solve(3, 2)
    4. solve(3, ...) returns 0 (base case)
    5. solve(2, 1) calculates, stores dp[2][1], returns
    6. solve(2, 2) calculates, stores dp[2][2], returns
    7. solve(1, 0) calculates, stores dp[1][0], returns
    8. ... and so on
    
    By the end, the ENTIRE dp table is filled!
    
    RETURN:
    -------
    We return maxPoints, which is the answer to:
    "Maximum points from day 'day' onwards, given 'last' was done yesterday"
    
    This value propagates up the recursion tree and eventually reaches
    the original call solve(0, 3), which is our final answer!
    */
    dp[day][last] = maxPoints;
    return maxPoints;
}

int ninjaTraining(int n, vector<vector<int>>& points) {
    /*
    ═══════════════════════════════════════════════════════════════════════════
                            MAIN FUNCTION SETUP
    ═══════════════════════════════════════════════════════════════════════════
    
    INITIALIZATION:
    ---------------
    1. Create memoization table: dp[n][4]
       - n rows: one for each day (0 to n-1)
       - 4 columns: one for each possible 'last' value (0, 1, 2, 3)
       - Initialize all to -1: "not computed yet"
    
    2. Make the initial call: solve(0, 3, points, dp)
       - Start at day 0
       - Use last = 3 to represent "no previous activity"
       - This means on day 0, ALL activities are valid
    
    WHY last = 3 for first day?
    ---------------------------
    On day 0, there's NO previous day, so NO constraint.
    Using last = 3 (an invalid activity number) ensures:
    - 0 != 3 ✓ (can do Running)
    - 1 != 3 ✓ (can do Fighting)
    - 2 != 3 ✓ (can do Learning)
    All activities pass the constraint check!
    
    RECURSION FLOW:
    ---------------
    solve(0, 3) will:
    1. Try activity 0: points[0][0] + solve(1, 0, ...)
    2. Try activity 1: points[0][1] + solve(1, 1, ...)
    3. Try activity 2: points[0][2] + solve(1, 2, ...)
    4. Return max of these three
    
    Each recursive call will further branch out, exploring all possible paths.
    Memoization ensures we don't repeat work.
    
    FINAL ANSWER:
    -------------
    The return value is the maximum points Ninja can earn over all N days,
    considering all possible valid paths through the activities!
    */
    vector<vector<int>> dp(n, vector<int>(4, -1));
    return solve(0, 3, points, dp);
}

/*
╔════════════════════════════════════════════════════════════════════════════════╗
║                          DETAILED DRY RUN EXAMPLE                              ║
╚════════════════════════════════════════════════════════════════════════════════╝

INPUT: points = [[1,2,5], [3,1,1], [3,3,3]]
       n = 3

INITIAL SETUP:
--------------
dp = [[-1,-1,-1,-1],
      [-1,-1,-1,-1],
      [-1,-1,-1,-1]]

═══════════════════════════════════════════════════════════════════════════════
                            EXECUTION TRACE
═══════════════════════════════════════════════════════════════════════════════

CALL: solve(0, 3, points, dp)
├─ day=0, last=3 (no previous activity)
├─ NOT base case (0 != 3)
├─ dp[0][3] = -1, so NOT memoized, must compute
├─ maxPoints = 0
│
├─ TRY activity=0 (Running):
│  ├─ 0 != 3 ✓ (valid)
│  ├─ points_earned = points[0][0] + solve(1, 0, points, dp)
│  │                = 1 + solve(1, 0, ...)
│  │
│  ├─ RECURSIVE CALL: solve(1, 0, points, dp)
│  │  ├─ day=1, last=0
│  │  ├─ NOT base case
│  │  ├─ dp[1][0] = -1, must compute
│  │  ├─ maxPoints = 0
│  │  │
│  │  ├─ TRY activity=0: 0 == 0 ✗ SKIP (can't repeat)
│  │  │
│  │  ├─ TRY activity=1 (Fighting):
│  │  │  ├─ 1 != 0 ✓ (valid)
│  │  │  ├─ points_earned = points[1][1] + solve(2, 1, ...)
│  │  │  │                = 1 + solve(2, 1, ...)
│  │  │  │
│  │  │  ├─ RECURSIVE CALL: solve(2, 1, points, dp)
│  │  │  │  ├─ day=2, last=1
│  │  │  │  ├─ NOT base case
│  │  │  │  ├─ dp[2][1] = -1, must compute
│  │  │  │  ├─ maxPoints = 0
│  │  │  │  │
│  │  │  │  ├─ TRY activity=0 (Running):
│  │  │  │  │  ├─ 0 != 1 ✓ (valid)
│  │  │  │  │  ├─ points_earned = points[2][0] + solve(3, 0, ...)
│  │  │  │  │  │                = 3 + solve(3, 0, ...)
│  │  │  │  │  │
│  │  │  │  │  ├─ RECURSIVE CALL: solve(3, 0, points, dp)
│  │  │  │  │  │  └─ day=3, 3 == points.size() → BASE CASE → return 0
│  │  │  │  │  │
│  │  │  │  │  └─ points_earned = 3 + 0 = 3
│  │  │  │  │  └─ maxPoints = max(0, 3) = 3
│  │  │  │  │
│  │  │  │  ├─ TRY activity=1: 1 == 1 ✗ SKIP
│  │  │  │  │
│  │  │  │  ├─ TRY activity=2 (Learning):
│  │  │  │  │  ├─ 2 != 1 ✓ (valid)
│  │  │  │  │  ├─ points_earned = points[2][2] + solve(3, 2, ...)
│  │  │  │  │  │                = 3 + solve(3, 2, ...)
│  │  │  │  │  │
│  │  │  │  │  ├─ RECURSIVE CALL: solve(3, 2, points, dp)
│  │  │  │  │  │  └─ day=3, BASE CASE → return 0
│  │  │  │  │  │
│  │  │  │  │  └─ points_earned = 3 + 0 = 3
│  │  │  │  │  └─ maxPoints = max(3, 3) = 3
│  │  │  │  │
│  │  │  │  └─ dp[2][1] = 3, return 3
│  │  │  │
│  │  │  └─ points_earned = 1 + 3 = 4
│  │  │  └─ maxPoints = max(0, 4) = 4
│  │  │
│  │  ├─ TRY activity=2 (Learning):
│  │  │  ├─ 2 != 0 ✓ (valid)
│  │  │  ├─ points_earned = points[1][2] + solve(2, 2, ...)
│  │  │  │                = 1 + solve(2, 2, ...)
│  │  │  │
│  │  │  ├─ RECURSIVE CALL: solve(2, 2, points, dp)
│  │  │  │  ├─ day=2, last=2
│  │  │  │  ├─ dp[2][2] = -1, must compute
│  │  │  │  ├─ maxPoints = 0
│  │  │  │  │
│  │  │  │  ├─ TRY activity=0: 0 != 2 ✓
│  │  │  │  │  └─ points_earned = 3 + solve(3, 0, ...) = 3 + 0 = 3
│  │  │  │  │  └─ maxPoints = 3
│  │  │  │  │
│  │  │  │  ├─ TRY activity=1: 1 != 2 ✓
│  │  │  │  │  └─ points_earned = 3 + solve(3, 1, ...) = 3 + 0 = 3
│  │  │  │  │  └─ maxPoints = max(3, 3) = 3
│  │  │  │  │
│  │  │  │  ├─ TRY activity=2: 2 == 2 ✗ SKIP
│  │  │  │  │
│  │  │  │  └─ dp[2][2] = 3, return 3
│  │  │  │
│  │  │  └─ points_earned = 1 + 3 = 4
│  │  │  └─ maxPoints = max(4, 4) = 4
│  │  │
│  │  └─ dp[1][0] = 4, return 4
│  │
│  └─ points_earned = 1 + 4 = 5
│  └─ maxPoints = max(0, 5) = 5
│
├─ TRY activity=1 (Fighting):
│  ├─ 1 != 3 ✓ (valid)
│  ├─ points_earned = points[0][1] + solve(1, 1, ...)
│  │                = 2 + solve(1, 1, ...)
│  │
│  ├─ RECURSIVE CALL: solve(1, 1, points, dp)
│  │  ├─ day=1, last=1
│  │  ├─ dp[1][1] = -1, must compute
│  │  ├─ maxPoints = 0
│  │  │
│  │  ├─ TRY activity=0: 0 != 1 ✓
│  │  │  └─ points_earned = 3 + solve(2, 0, ...)
│  │  │     │
│  │  │     ├─ solve(2, 0, ...) computes (similar to above):
│  │  │     │  └─ Try activities 1,2 → max = 3
│  │  │     │  └─ dp[2][0] = 3, return 3
│  │  │     │
│  │  │  └─ points_earned = 3 + 3 = 6
│  │  │  └─ maxPoints = 6
│  │  │
│  │  ├─ TRY activity=1: 1 == 1 ✗ SKIP
│  │  │
│  │  ├─ TRY activity=2: 2 != 1 ✓
│  │  │  └─ points_earned = 1 + solve(2, 2, ...) = 1 + 3 = 4
│  │  │  └─ maxPoints = max(6, 4) = 6
│  │  │
│  │  └─ dp[1][1] = 6, return 6
│  │
│  └─ points_earned = 2 + 6 = 8
│  └─ maxPoints = max(5, 8) = 8
│
├─ TRY activity=2 (Learning):
│  ├─ 2 != 3 ✓ (valid)
│  ├─ points_earned = points[0][2] + solve(1, 2, ...)
│  │                = 5 + solve(1, 2, ...)
│  │
│  ├─ RECURSIVE CALL: solve(1, 2, points, dp)
│  │  ├─ day=1, last=2
│  │  ├─ dp[1][2] = -1, must compute
│  │  ├─ maxPoints = 0
│  │  │
│  │  ├─ TRY activity=0: 0 != 2 ✓
│  │  │  └─ points_earned = 3 + solve(2, 0, ...) = 3 + 3 = 6
│  │  │  └─ maxPoints = 6
│  │  │
│  │  ├─ TRY activity=1: 1 != 2 ✓
│  │  │  └─ points_earned = 1 + solve(2, 1, ...) = 1 + 3 = 4
│  │  │  └─ maxPoints = max(6, 4) = 6
│  │  │
│  │  ├─ TRY activity=2: 2 == 2 ✗ SKIP
│  │  │
│  │  └─ dp[1][2] = 6, return 6
│  │
│  └─ points_earned = 5 + 6 = 11 ← MAXIMUM!
│  └─ maxPoints = max(8, 11) = 11
│
└─ dp[0][3] = 11, return 11

FINAL ANSWER: 11

OPTIMAL PATH:
Day 0: Activity 2 (Learning) → 5 points
Day 1: Activity 0 (Running)  → 3 points
Day 2: Activity 1 (Fighting) → 3 points
Total: 5 + 3 + 3 = 11 ✓

FINAL DP TABLE (after all computations):
dp = [[-1,-1,-1,11],
      [4,6,6,-1],
      [3,3,3,-1]]

Note: Some cells are -1 because those states were never explored
      (not needed for the optimal solution).

═══════════════════════════════════════════════════════════════════════════════
                        COMPLEXITY ANALYSIS
═══════════════════════════════════════════════════════════════════════════════

TIME COMPLEXITY: O(N × 4 × 3) = O(12N) = O(N)
- Total states: N days × 4 possible 'last' values = 4N states
- Per state: Try at most 3 activities
- Total: 4N × 3 = 12N operations
- Linear in N!

SPACE COMPLEXITY: O(N × 4) + O(N) = O(N)
- Memoization table: O(N × 4)
- Recursion stack: O(N) depth
- Total: O(N)

COMPARISON WITH BOTTOM-UP:
- Same time complexity: O(N)
- Bottom-up space can be optimized to O(1), but this needs O(N)
- This approach is more intuitive for recursive thinkers!

╔════════════════════════════════════════════════════════════════════════════════╗
║                          KEY TAKEAWAYS                                         ║
╚════════════════════════════════════════════════════════════════════════════════╝

1. TOP-DOWN = "Make choices, explore futures, remember results"
2. Base case at day==n is CLEANER than day==n-1 (uniform logic)
3. Memoization transforms O(3^N) to O(N) - HUGE speedup
4. State encodes minimal info: where we are + what we can't do
5. Same problem, different perspective than bottom-up, same answer!
*/