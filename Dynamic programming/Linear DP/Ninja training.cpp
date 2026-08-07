#include <bits/stdc++.h>
using namespace std;

/*
    Problem: Ninja's Training
    Link: https://www.naukri.com/code360/problems/ninja-s-training_3621003

    Ninja trains for n days. On every day, he must perform exactly
    one of three activities.

    points[day][activity] gives the merit points earned by performing
    that activity on that day.

    The same activity cannot be performed on two consecutive days.

    Find the maximum total merit points Ninja can earn.
*/

class Solution
{
public:
    // ============================================================
    // MEMOIZATION
    // ============================================================

    /*
        solve(day, last) returns the maximum points obtainable
        from 'day' to the final day.

        last represents the activity performed on the previous day.

        last = 0, 1 or 2:
            That activity cannot be performed on the current day.

        last = 3:
            No activity was performed previously, so all three
            activities are available. This is used for day 0.
    */
    int solve(
        int day,
        int last,
        vector<vector<int>> &points,
        vector<vector<int>> &dp)
    {
        int n = points.size();

        // All training days have been completed.
        if (day == n)
        {
            return 0;
        }

        if (dp[day][last] != -1)
        {
            return dp[day][last];
        }

        int maxPoints = 0;

        // Try every activity except the one performed on the previous day.
        for (int activity = 0; activity < 3; activity++)
        {
            if (activity != last)
            {
                /*
                    Perform 'activity' today.

                    Earn its points and move to the next day.
                    The activity chosen today becomes 'last' for tomorrow.
                */
                int pointsEarned =
                    points[day][activity] +
                    solve(day + 1, activity, points, dp);

                maxPoints = max(maxPoints, pointsEarned);
            }
        }

        return dp[day][last] = maxPoints;
    }

    int ninjaTrainingMemo(int n, vector<vector<int>> &points)
    {
        /*
            Four possible values of 'last':

            0, 1, 2 = one of the three activities
            3       = no previous activity
        */
        vector<vector<int>> dp(n, vector<int>(4, -1));

        // On day 0, there is no previously performed activity.
        return solve(0, 3, points, dp);
    }

    // ============================================================
    // TABULATION
    // ============================================================

    int ninjaTrainingTabulation(int n, vector<vector<int>> &points)
    {
        /*
            dp[day][activity] = maximum total points earned up to
            'day' when 'activity' is performed on that day.
        */
        vector<vector<int>> dp(n, vector<int>(3, 0));

        /*
            On the first day, any activity can be performed because
            there is no previous-day restriction.
        */
        dp[0][0] = points[0][0];
        dp[0][1] = points[0][1];
        dp[0][2] = points[0][2];

        for (int day = 1; day < n; day++)
        {
            // Choose the activity performed on the current day.
            for (int activity = 0; activity < 3; activity++)
            {
                dp[day][activity] = 0;

                // Check which activity was performed on the previous day.
                for (int previousActivity = 0;
                     previousActivity < 3;
                     previousActivity++)
                {
                    /*
                        The current and previous activities must differ
                        because the same activity cannot be repeated
                        on consecutive days.
                    */
                    if (previousActivity != activity)
                    {
                        int pointsEarned =
                            points[day][activity] +
                            dp[day - 1][previousActivity];

                        dp[day][activity] = max(
                            dp[day][activity],
                            pointsEarned);
                    }
                }
            }
        }

        /*
            On the final day, Ninja may finish with any of the
            three activities. Take the best result.
        */
        int maxPoints = max(
            dp[n - 1][0],
            max(dp[n - 1][1], dp[n - 1][2]));

        return maxPoints;
    }

    int ninjaTraining(int n, vector<vector<int>> &points)
    {
        return ninjaTrainingTabulation(n, points);
    }
};

/*
    Memoization:
    Time:  O(n * 4 * 3) = O(n)
    Space: O(n * 4) DP + O(n) recursion stack

    Tabulation:
    Time:  O(n * 3 * 3) = O(n)
    Space: O(n * 3) = O(n)
*/