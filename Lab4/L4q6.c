/*
 * DAA Lab-04, Q6: Application of sorting-VI
 * -------------------------------------------
 * Input : a set S of n intervals (l_i, r_i) on a line; endpoints count
 *         as being inside the interval.
 * Output: a point p that lies in the largest number of intervals, and
 *         that count.
 *   Example: {(10,40),(20,60),(50,90),(15,70)} -> 3 intervals share a
 *            point (e.g. p = 50, contained in (20,60),(50,90),(15,70)).
 *
 * Algorithm (O(n log n)):
 *   1. Turn every interval into two events: (l_i, START) and (r_i, END).
 *   2. Sort the 2n events by position; when a START and an END fall on
 *      the same coordinate, the START is processed first. This tie-break
 *      is what makes shared endpoints count correctly -- if interval A
 *      ends at 40 and interval B starts at 40, the point p = 40 lies in
 *      BOTH (endpoints are inclusive), so B's start must be counted
 *      before A's end is removed.                          -- O(n log n)
 *   3. Sweep left to right with a running "coverage count": +1 on a
 *      START, -1 on an END. Track the maximum count and the position of
 *      a START event where that maximum is (first) reached (only a
 *      START can create a new maximum).                    -- O(n)
 *   Total: O(n log n), dominated by the sort.
 *
 *   Note: several points may achieve the same maximum coverage; this
 *   program reports one of them.
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int pos;
    int type; /* 0 = start (processed first at equal pos), 1 = end */
} Event;

static int cmpEvent(const void *a, const void *b) {
    const Event *e1 = (const Event *)a, *e2 = (const Event *)b;
    if (e1->pos != e2->pos) return e1->pos - e2->pos;
    return e1->type - e2->type; /* starts (0) before ends (1) */
}

/* O(n log n): sweep with starts processed before ends at equal
 * coordinates, so shared endpoints are counted correctly. */
static int maxCoveragePoint(const int *l, const int *r, int n, int *bestCount) {
    Event *events = malloc((size_t)(2 * n) * sizeof(Event));
    for (int i = 0; i < n; i++) {
        events[2 * i]     = (Event){l[i], 0};
        events[2 * i + 1] = (Event){r[i], 1};
    }
    qsort(events, (size_t)(2 * n), sizeof(Event), cmpEvent);

    int count = 0, maxCount = 0, bestPoint = 0;
    for (int i = 0; i < 2 * n; i++) {
        if (events[i].type == 0) {
            count++;
            if (count > maxCount) { maxCount = count; bestPoint = events[i].pos; }
        } else {
            count--;
        }
    }
    free(events);
    *bestCount = maxCount;
    return bestPoint;
}

int main(void) {
    int n;
    printf("Enter number of intervals (n): ");
    if (scanf("%d", &n) != 1 || n <= 0) { fprintf(stderr, "Invalid n\n"); return 1; }

    int *l = malloc((size_t)n * sizeof(int));
    int *r = malloc((size_t)n * sizeof(int));
    if (!l || !r) { fprintf(stderr, "Out of memory\n"); return 1; }

    printf("Enter %d intervals as: <l> <r>\n", n);
    for (int i = 0; i < n; i++) if (scanf("%d %d", &l[i], &r[i]) != 2) { fprintf(stderr, "Bad input\n"); return 1; }

    int bestCount;
    int p = maxCoveragePoint(l, r, n, &bestCount);

    printf("Point p = %d lies in the maximum number of intervals: %d\n", p, bestCount);

    free(l);
    free(r);
    return 0;
}