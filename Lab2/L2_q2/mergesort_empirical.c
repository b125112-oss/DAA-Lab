/* ============================================================================
 * mergesort_empirical.c
 *
 * Purpose:
 *   EMPIRICALLY validate the worst-case running time of:
 *     (a) standard merge sort   : divide into HALVES, 2-way merge
 *     (b) modified merge sort   : divide into THIRDS, 3-way merge
 *
 *   Unlike the previous version of this program, this one does NOT just
 *   evaluate a closed-form formula. It actually:
 *     1. Builds a randomly-ordered (worst-case-representative) array of
 *        size n for a range of n values.
 *     2. Runs the real recursive merge-sort implementation on it.
 *     3. Counts the actual number of key comparisons performed during
 *        the merge steps (the dominant operation), AND measures actual
 *        wall-clock time with clock().
 *     4. Repeats several trials per n and averages, to reduce noise.
 *     5. Writes n, measured-comparisons, measured-time (ms) for both
 *        algorithms to a .dat file for gnuplot.
 *
 *   It also writes out cost/(n log_k n) for each algorithm (k=2 for
 *   standard, k=3 for modified). If the implementation really is
 *   Theta(n log_k n), this ratio should flatten out to a constant as n
 *   grows -- that flattening (not a fitted formula) is the empirical
 *   proof of the O(n log n) order of growth.
 *
 * Compile:  gcc -O3 -o mergesort_empirical mergesort_empirical.c -lm
 * Run:      ./mergesort_empirical                 (uses built-in size list)
 *           ./mergesort_empirical <max_n> <trials> (custom, see main())
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

typedef long long ll;

/* ---------------------------------------------------------------------
 * Standard merge sort: divide into HALVES, 2-way merge.
 * comparisons is accumulated during the merge step only (the dominant
 * cost); array partitioning itself does no comparisons.
 * --------------------------------------------------------------------- */
static void merge2(int *arr, int l, int mid, int r, int *tmp, ll *comparisons) {
    int i = l, j = mid, k = l;
    while (i < mid && j < r) {
        (*comparisons)++;
        if (arr[i] <= arr[j]) tmp[k++] = arr[i++];
        else                  tmp[k++] = arr[j++];
    }
    while (i < mid) tmp[k++] = arr[i++];
    while (j < r)   tmp[k++] = arr[j++];
    memcpy(arr + l, tmp + l, (size_t)(r - l) * sizeof(int));
}

static void mergesort2(int *arr, int l, int r, int *tmp, ll *comparisons) {
    if (r - l <= 1) return;
    int mid = l + (r - l) / 2;
    mergesort2(arr, l, mid, tmp, comparisons);
    mergesort2(arr, mid, r, tmp, comparisons);
    merge2(arr, l, mid, r, tmp, comparisons);
}

/* ---------------------------------------------------------------------
 * Modified merge sort: divide into THIRDS, 3-way merge.
 * At every output position we pick the smallest of (up to) 3 fronts,
 * which costs up to 2 comparisons -- exactly as in the analytical
 * derivation from the previous part.
 * --------------------------------------------------------------------- */
static void merge3(int *arr, int l, int m1, int m2, int r, int *tmp, ll *comparisons) {
    int i = l, j = m1, k = m2, t = l;
    while (i < m1 || j < m2 || k < r) {
        int has_i = (i < m1), has_j = (j < m2), has_k = (k < r);
        int best = -1; /* 0=i,1=j,2=k */
        if (has_i) best = 0;
        if (has_j) {
            if (best == -1) best = 1;
            else { (*comparisons)++; if (arr[j] < arr[i]) best = 1; }
        }
        if (has_k) {
            if (best == -1) best = 2;
            else {
                int cur = (best == 0) ? arr[i] : arr[j];
                (*comparisons)++;
                if (arr[k] < cur) best = 2;
            }
        }
        if (best == 0) tmp[t++] = arr[i++];
        else if (best == 1) tmp[t++] = arr[j++];
        else tmp[t++] = arr[k++];
    }
    memcpy(arr + l, tmp + l, (size_t)(r - l) * sizeof(int));
}

static void mergesort3(int *arr, int l, int r, int *tmp, ll *comparisons) {
    if (r - l <= 1) return;
    int len = r - l;
    int m1 = l + len / 3;
    int m2 = l + (2 * len) / 3;
    if (m1 == l) m1++;          /* guard against degenerate thirds for tiny n */
    if (m2 <= m1) m2 = m1 + 1;
    if (m2 > r) m2 = r;
    mergesort3(arr, l, m1, tmp, comparisons);
    mergesort3(arr, m1, m2, tmp, comparisons);
    mergesort3(arr, m2, r, tmp, comparisons);
    merge3(arr, l, m1, m2, r, tmp, comparisons);
}

/* ---------------------------------------------------------------------
 * Helpers
 * --------------------------------------------------------------------- */
static void fill_random(int *arr, int n) {
    for (int i = 0; i < n; i++) arr[i] = rand();
}

int main(int argc, char **argv) {
    /* Logarithmically-spaced sample sizes so the plot has good coverage
     * from small to large n without needing millions of tiny points. */
    int default_sizes[] = {
        500, 1000, 2000, 4000, 8000, 16000, 32000, 64000, 125000,
        250000, 500000, 1000000, 2000000
    };
    int num_sizes = (int)(sizeof(default_sizes) / sizeof(default_sizes[0]));
    int *sizes = default_sizes;

    int trials = 5;
    if (argc > 2) trials = atoi(argv[2]);
    if (trials < 1) trials = 1;

    srand(42); /* reproducible */

    FILE *fp = fopen("mergesort_empirical.dat", "w");
    if (!fp) { perror("mergesort_empirical.dat"); exit(1); }
    fprintf(fp, "# n  cmp_2way  cmp_3way  time_2way_ms  time_3way_ms  "
                "cmp2/(n*log2n)  cmp3/(n*log3n)\n");

    int *original = NULL, *work = NULL, *tmp = NULL;
    int max_n = 0;
    for (int s = 0; s < num_sizes; s++) if (sizes[s] > max_n) max_n = sizes[s];
    original = malloc((size_t)max_n * sizeof(int));
    work      = malloc((size_t)max_n * sizeof(int));
    tmp       = malloc((size_t)max_n * sizeof(int));
    if (!original || !work || !tmp) { fprintf(stderr, "OOM\n"); exit(1); }

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        double sum_cmp2 = 0, sum_cmp3 = 0, sum_t2 = 0, sum_t3 = 0;

        for (int trial = 0; trial < trials; trial++) {
            fill_random(original, n);

            /* --- standard 2-way merge sort --- */
            memcpy(work, original, (size_t)n * sizeof(int));
            ll cmp2 = 0;
            clock_t t0 = clock();
            mergesort2(work, 0, n, tmp, &cmp2);
            clock_t t1 = clock();
            sum_cmp2 += (double)cmp2;
            sum_t2   += 1000.0 * (double)(t1 - t0) / CLOCKS_PER_SEC;

            /* --- modified 3-way merge sort (same input, fresh copy) --- */
            memcpy(work, original, (size_t)n * sizeof(int));
            ll cmp3 = 0;
            clock_t t2 = clock();
            mergesort3(work, 0, n, tmp, &cmp3);
            clock_t t3 = clock();
            sum_cmp3 += (double)cmp3;
            sum_t3   += 1000.0 * (double)(t3 - t2) / CLOCKS_PER_SEC;
        }

        double avg_cmp2 = sum_cmp2 / trials;
        double avg_cmp3 = sum_cmp3 / trials;
        double avg_t2   = sum_t2   / trials;
        double avg_t3   = sum_t3   / trials;

        double nlog2 = (double)n * (log((double)n) / log(2.0));
        double nlog3 = (double)n * (log((double)n) / log(3.0));
        double ratio2 = (nlog2 > 0) ? avg_cmp2 / nlog2 : 0.0;
        double ratio3 = (nlog3 > 0) ? avg_cmp3 / nlog3 : 0.0;

        fprintf(fp, "%d %.2f %.2f %.4f %.4f %.6f %.6f\n",
                n, avg_cmp2, avg_cmp3, avg_t2, avg_t3, ratio2, ratio3);
        fflush(fp);

        printf("n=%8d  cmp2way=%12.0f  cmp3way=%12.0f  "
               "t2way=%8.3fms  t3way=%8.3fms\n",
               n, avg_cmp2, avg_cmp3, avg_t2, avg_t3);
    }

    fclose(fp);
    free(original); free(work); free(tmp);

    printf("\nWrote mergesort_empirical.dat (%d size points, %d trials each).\n",
           num_sizes, trials);
    printf("Columns: n cmp_2way cmp_3way time_2way_ms time_3way_ms "
           "cmp2/(n*log2n) cmp3/(n*log3n)\n");
    printf("If the last two columns flatten toward a constant as n grows, "
           "that is the empirical\nconfirmation that both algorithms run in "
           "Theta(n log n).\n");
    return 0;
}
