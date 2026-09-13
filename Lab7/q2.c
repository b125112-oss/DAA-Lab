/* =====================================================================
   DAA Lab-07, Q2: Super egg testing experiment
   =====================================================================

   PROBLEM
   -------
   Given E identical eggs and an F-storey building, find the minimum
   number of droppings that is GUARANTEED (worst case) to determine the
   critical floor (highest floor from which an egg does not break;
   floor 0 means even dropping from the ground floor breaks it, and it
   is guaranteed that from floor F+... no floor breaks it is not
   possible - the standard formulation: floors 1..F, an egg dropped
   from floor <= critical floor survives, from > critical floor
   breaks). For the concrete instance in the problem, E = 2, F = 100.

   INPUT REPRESENTATION
   ---------------------
   Two integers E (eggs) and F (floors). The state space of the DP is
   the 2-D table dp[e][f], 0 <= e <= E, 0 <= f <= F.

   ALGORITHM (DYNAMIC PROGRAMMING)
   ---------------------------------
   Let dp[e][f] = minimum number of droppings that guarantees finding
   the critical floor, using at most e eggs and f floors to test.

   Base cases:
       dp[e][0] = 0            (no floors -> no drops needed)
       dp[1][f] = f             (one egg -> must test floors 1,2,...
                                  one at a time, linear search)
       dp[e][1] = 1            (one floor -> exactly one drop settles it)

   Recurrence: if we drop an egg from floor k (1<=k<=f):
       - it breaks   -> problem reduces to (e-1) eggs, (k-1) floors below
       - it survives -> problem reduces to (e) eggs,   (f-k) floors above
   Worst case over the two outcomes, plus the drop just used:
       dp[e][f] = 1 + min_{1<=k<=f} max( dp[e-1][k-1], dp[e][f-k] )

   This is the textbook DP formulation (Levitin / CLRS-style egg
   drop). We fill the table for all e=1..E and f=1..F, choosing k by
   a direct scan of all f candidate floors (see complexity below);
   because dp[e-1][k-1] is non-decreasing in k and dp[e][f-k] is
   non-increasing in k, the optimal k can also be found by binary
   search on k in O(log f) per cell -- both variants are implemented
   below and cross-checked against each other.

   COMPLEXITY
   -----------
     Straightforward DP  (scan all k)   : O(E * F^2) time, O(E*F) space
     Binary-search-on-k optimisation    : O(E * F log F) time, O(E*F) space
     (A further O(E*F) "inverse" DP -- maximum floors distinguishable
      with t trials and e eggs, f(t,e)=f(t-1,e-1)+f(t-1,e)+1 -- answers
      the SAME question in only O(E log F) time; both are also
      implemented for cross-validation.)

   For E = 2, F = 100 the well-known answer is 14 droppings
   (since 14*15/2 = 105 >= 100, the smallest triangular number
   exceeding 100), which the program below confirms.
   ===================================================================== */

#include <stdio.h>
#include <stdlib.h>

#define MAXV 1000000000

/* ---------- Variant 1: plain O(E*F^2) DP (scan every k) -------------- */
int egg_drop_On2(int E, int F, int **dp) {
    for (int f = 0; f <= F; f++) dp[1][f] = f;               /* 1 egg  */
    for (int e = 1; e <= E; e++) dp[e][0] = 0;                /* 0 floors */
    for (int e = 2; e <= E; e++) {
        for (int f = 1; f <= F; f++) {
            int best = MAXV;
            for (int k = 1; k <= f; k++) {
                int broke = dp[e - 1][k - 1];
                int lived = dp[e][f - k];
                int worst = (broke > lived) ? broke : lived;
                if (worst < best) best = worst;
            }
            dp[e][f] = best + 1;
        }
    }
    return dp[E][F];
}

/* ---------- Variant 2: O(E*F log F) DP using binary search on k -------
   For fixed e,f, g(k) = dp[e-1][k-1] is non-decreasing in k and
   h(k) = dp[e][f-k] is non-increasing in k, so the max(g,h) is
   "unimodal-ish" (first h dominates, then g dominates); binary search
   finds the crossover point in O(log f). */
int egg_drop_OnLogn(int E, int F, int **dp2) {
    for (int f = 0; f <= F; f++) dp2[1][f] = f;
    for (int e = 1; e <= E; e++) dp2[e][0] = 0;
    for (int e = 2; e <= E; e++) {
        for (int f = 1; f <= F; f++) {
            int lo = 1, hi = f, best = MAXV;
            while (lo <= hi) {
                int mid = (lo + hi) / 2;
                int broke = dp2[e - 1][mid - 1];
                int lived = dp2[e][f - mid];
                int worst = (broke > lived) ? broke : lived;
                if (worst < best) best = worst;
                if (broke > lived) hi = mid - 1;   /* too many floors below, move k down */
                else lo = mid + 1;                  /* too few floors below, move k up   */
            }
            dp2[e][f] = best + 1;
        }
    }
    return dp2[E][F];
}

/* ---------- Variant 3: O(E log F) "max floors coverable" DP -----------
   f(t,e) = max number of floors distinguishable with t trials, e eggs.
   f(t,e) = f(t-1,e-1) + f(t-1,e) + 1   (drop once: 1 floor "used up"
   directly, f(t-1,e-1) floors below covered if it breaks,
   f(t-1,e) floors above covered if it survives)
   Answer = minimum t such that f(t,E) >= F.                         */
int egg_drop_inverse(int E, int F) {
    if (F <= 0) return 0;                     /* no floors -> no drops needed */
    int t = 0;
    int *prev = calloc(E + 1, sizeof(int));   /* f(t-1, e) for e=0..E */
    int *cur  = calloc(E + 1, sizeof(int));
    while (1) {
        t++;
        cur[0] = 0;
        for (int e = 1; e <= E; e++)
            cur[e] = prev[e - 1] + prev[e] + 1;
        if (cur[E] >= F) break;                    /* check the row we just built */
        int *tmp = prev; prev = cur; cur = tmp;     /* prev := this t's row, reuse old prev as next cur */
    }
    free(prev); free(cur);
    return t;
}

int main(void) {
    printf("=== Q2: Super egg testing experiment (generalized E eggs, F floors) ===\n\n");

    int E, F;
    printf("Enter number of eggs E and number of floors F: ");
    if (scanf("%d %d", &E, &F) != 2 || E < 1 || F < 0) { printf("Invalid input.\n"); return 1; }

    int **dp  = malloc((E + 1) * sizeof(int *));
    int **dp2 = malloc((E + 1) * sizeof(int *));
    for (int e = 0; e <= E; e++) {
        dp[e]  = malloc((F + 1) * sizeof(int));
        dp2[e] = malloc((F + 1) * sizeof(int));
    }

    int ans1 = egg_drop_On2(E, F, dp);
    int ans2 = egg_drop_OnLogn(E, F, dp2);
    int ans3 = egg_drop_inverse(E, F);

    printf("\nO(E*F^2)  DP result       : %d droppings\n", ans1);
    printf("O(E*F logF) DP result     : %d droppings\n", ans2);
    printf("O(E logF) inverse-DP result: %d droppings\n", ans3);
    printf("All three methods agree: %s\n", (ans1 == ans2 && ans2 == ans3) ? "YES" : "NO -- BUG!");

    printf("\nSanity check against the classic instance (E=2, F=100 -> 14):\n");
    {
        int e = 2, f = 100;
        int **t1 = malloc((e + 1) * sizeof(int *));
        for (int i = 0; i <= e; i++) t1[i] = malloc((f + 1) * sizeof(int));
        int r1 = egg_drop_On2(e, f, t1);
        int r3 = egg_drop_inverse(e, f);
        printf("  E=2, F=100 -> %d (expected 14), inverse-DP -> %d\n", r1, r3);
        for (int i = 0; i <= e; i++) free(t1[i]);
        free(t1);
    }

    for (int e = 0; e <= E; e++) { free(dp[e]); free(dp2[e]); }
    free(dp); free(dp2);
    return 0;
}