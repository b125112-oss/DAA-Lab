/* =====================================================================
   DAA Lab-07, Q6: The best time to be alive
   =====================================================================

   PROBLEM
   -------
   Given the birth and death years of n prominent (now deceased)
   scientists (sorted alphabetically in the book's index -- their
   order does NOT matter for this computation), find the year during
   which the largest number of them were alive simultaneously. Tie
   rule: if person A died the same year person B was born, A's death
   is considered to have happened BEFORE B's birth that year (so they
   are never counted as both alive together purely because of a
   shared boundary year).

   INPUT REPRESENTATION
   ---------------------
   n pairs (birth[i], death[i]), birth[i] < death[i]. This is turned
   into 2n "events": a +1 event at each birth year and a -1 event at
   each death year (a person counts as alive in the CLOSED interval
   [birth, death]; the tie rule above is what fixes the exact instant
   of the -1 vs +1 at a shared year).

   ALGORITHM (SWEEP LINE)
   --------------------------
   1. Build 2n events, each event = (year, type) with type = -1
      (death, "leaves the population") or +1 (birth, "joins").
   2. Sort all events by year; break ties by putting DEATH events
      before BIRTH events in the same year (this directly encodes the
      rule that a death is processed before a same-year birth).
   3. Sweep through the sorted events left to right, keeping a running
      counter `alive`. Process a death event BEFORE checking the max
      -- no, more precisely: apply the event's delta to `alive`, and
      whenever we cross into a NEW year for the first time after
      applying all of that year's events, or more simply: apply all
      events of a given year in the tie-broken order, then compare
      `alive` at the year boundary against the running maximum. The
      code below achieves this cleanly by only updating the "best"
      record right after finishing all events that belong to a given
      year and specifically after birth-events are applied (since a
      birth can only raise, never lower, the count for that year).
   4. Report the year(s) achieving the maximum.

   Because it only cares about the RELATIVE order of years (not gaps
   between them), sorting handles arbitrarily spaced birth/death years
   without needing a huge array indexed by calendar year.

   COMPLEXITY
   -----------
     Building events        : O(n)          time,  O(n) space
     Sorting events         : O(n log n)    time
     Sweep                  : O(n)          time
     TOTAL                  : O(n log n) time, O(n) space
   ===================================================================== */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int year;
    int type;     /* -1 = death, +1 = birth */
} Event;

int cmp_event(const void *a, const void *b) {
    const Event *ea = a, *eb = b;
    if (ea->year != eb->year) return ea->year - eb->year;
    return ea->type - eb->type;     /* -1 (death) sorts before +1 (birth) */
}

int main(void) {
    printf("=== Q6: The best time to be alive ===\n\n");

    int n;
    printf("Enter number of scientists n: ");
    if (scanf("%d", &n) != 1 || n < 1) { printf("Invalid input.\n"); return 1; }

    int *birth = malloc(n * sizeof(int));
    int *death = malloc(n * sizeof(int));
    printf("Enter %d lines of \"birth_year death_year\":\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%d %d", &birth[i], &death[i]) != 2 || birth[i] >= death[i]) {
            printf("Invalid data for entry %d.\n", i + 1);
            free(birth); free(death); return 1;
        }
    }

    Event *ev = malloc(2 * n * sizeof(Event));
    for (int i = 0; i < n; i++) {
        ev[2 * i].year = birth[i]; ev[2 * i].type = +1;
        ev[2 * i + 1].year = death[i]; ev[2 * i + 1].type = -1;
    }
    qsort(ev, 2 * n, sizeof(Event), cmp_event);

    int alive = 0, best = -1;
    /* store all years achieving the max (there can be a tie) */
    int *bestYears = malloc(2 * n * sizeof(int));
    int nBest = 0;

    int i = 0;
    while (i < 2 * n) {
        int y = ev[i].year;
        /* apply ALL events of this year (deaths already sorted first) */
        while (i < 2 * n && ev[i].year == y) { alive += ev[i].type; i++; }
        /* the count that matters for year y is `alive` AFTER applying
           this year's deaths and births (represents "alive during y") */
        if (alive > best) { best = alive; nBest = 0; bestYears[nBest++] = y; }
        else if (alive == best) { bestYears[nBest++] = y; }
    }

    printf("\nMaximum number of scientists alive simultaneously = %d\n", best);
    printf("Achieved starting in year(s): ");
    for (int k = 0; k < nBest; k++) printf("%d ", bestYears[k]);
    printf("\n");

    /* -------- self-test with a known hand-checkable example -------- */
    printf("\n--- Built-in self-test ---\n");
    {
        /* 5 scientists:
           A: 1600-1650   B: 1630-1700  C: 1690-1750
           D: 1645-1660   E: 1655-1720
           Expected: around 1655-1660, A(no,ended1650) ... let's compute
           by brute force over the integer years to cross-check.       */
        int tb[5] = {1600, 1630, 1690, 1645, 1655};
        int td[5] = {1650, 1700, 1750, 1660, 1720};
        int lo = 1600, hi = 1750, bruteBest = 0, bruteYear = lo;
        for (int y = lo; y <= hi; y++) {
            int c = 0;
            for (int k = 0; k < 5; k++) if (tb[k] <= y && y <= td[k]) c++;
            if (c > bruteBest) { bruteBest = c; bruteYear = y; }
        }
        Event te[10];
        for (int k = 0; k < 5; k++) {
            te[2*k].year = tb[k]; te[2*k].type = +1;
            te[2*k+1].year = td[k]; te[2*k+1].type = -1;
        }
        qsort(te, 10, sizeof(Event), cmp_event);
        int a2 = 0, b2 = -1, j = 0;
        while (j < 10) {
            int y = te[j].year;
            while (j < 10 && te[j].year == y) { a2 += te[j].type; j++; }
            if (a2 > b2) b2 = a2;
        }
        printf("Brute-force max-alive (year scan) = %d (at year %d)\n", bruteBest, bruteYear);
        printf("Sweep-line max-alive              = %d\n", b2);
        printf("Match: %s\n", (bruteBest == b2) ? "YES" : "NO -- BUG!");
    }

    free(birth); free(death); free(ev); free(bestYears);
    return 0;
}