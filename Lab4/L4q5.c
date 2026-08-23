

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x, y; /* start, end */
} Interval;

static int cmpInterval(const void *a, const void *b) {
    return ((const Interval *)a)->x - ((const Interval *)b)->x;
}

/* O(n log n): sort by start, then sweep merging overlapping intervals.
 * Returns the number of merged intervals written into `out`. */
static int mergeIntervals(Interval *in, int n, Interval *out) {
    qsort(in, (size_t)n, sizeof(Interval), cmpInterval);

    int m = 0;
    Interval cur = in[0];
    for (int i = 1; i < n; i++) {
        if (in[i].x <= cur.y) {
            if (in[i].y > cur.y) cur.y = in[i].y;
        } else {
            out[m++] = cur;
            cur = in[i];
        }
    }
    out[m++] = cur;
    return m;
}

int main(void) {
    int n;
    printf("Enter number of intervals (n): ");
    if (scanf("%d", &n) != 1 || n <= 0) { fprintf(stderr, "Invalid n\n"); return 1; }

    Interval *in  = malloc((size_t)n * sizeof(Interval));
    Interval *out = malloc((size_t)n * sizeof(Interval));
    if (!in || !out) { fprintf(stderr, "Out of memory\n"); return 1; }

    printf("Enter %d intervals as: <x> <y>\n", n);
    for (int i = 0; i < n; i++) if (scanf("%d %d", &in[i].x, &in[i].y) != 2) { fprintf(stderr, "Bad input\n"); return 1; }

    int m = mergeIntervals(in, n, out);

    printf("Merged intervals:\n");
    for (int i = 0; i < m; i++) printf("(%d, %d)\n", out[i].x, out[i].y);

    free(in);
    free(out);
    return 0;
}