/* ============================================================================
 * kway_merge_empirical.c
 *
 * Problem: merge k sorted arrays, each with n elements, into one sorted
 * array of kn elements.
 *
 *   Method 1 (sequential): merge array 1 with array 2, merge that result
 *     with array 3, then with array 4, ..., then with array k.
 *
 *   Method 2 (pairwise / tournament): pair up the k arrays into k/2 pairs,
 *     merge each pair (k/2 merges of two length-n arrays -> k/2 arrays of
 *     length 2n). Repeat pairing+merging on the resulting arrays until
 *     only one length-kn array remains (a balanced merge tree, exactly
 *     like the merge phase of merge sort applied at the "array" level
 *     instead of the "element" level).
 *
 * Analytical worst case (for reference -- but this program does NOT plot
 * these formulas; it implements both methods for real and measures them):
 *
 *   Method 1: merging a running array of size i*n with the next length-n
 *     array costs Theta((i+1)*n). Summed for i=1..k-1:
 *       sum_{i=2}^{k} Theta(i*n) = Theta(n * k^2)      [because
 *       sum_{i=2}^{k} i = k(k+1)/2 - 1 = Theta(k^2)]
 *     => Method 1 worst case: Theta(k^2 * n)
 *
 *   Method 2: there are ceil(log2 k) merge "rounds". In every round the
 *     total number of elements being merged across all pairs is exactly
 *     kn (every element participates in exactly one merge per round), so
 *     each round costs Theta(kn), and there are Theta(log k) rounds:
 *       => Method 2 worst case: Theta(k * n * log k)
 *
 * This program:
 *   1. Builds k sorted arrays of n random elements.
 *   2. Runs the REAL Method 1 and Method 2 merge implementations on them,
 *      counting actual key comparisons (and wall-clock time).
 *   3. Experiment A: fix n, vary k over powers of two -> see how cost
 *      scales with k.
 *   4. Experiment B: fix k, vary n -> see how cost scales with n.
 *   5. Also reports cmp1/(k^2*n) and cmp2/(k*n*log2(k)): if these ratios
 *      FLATTEN toward a constant as k grows, that is empirical proof of
 *      the Theta(k^2 n) / Theta(kn log k) bounds above.
 *
 * Compile:  gcc -O3 -o kway_merge_empirical kway_merge_empirical.c -lm
 * Run:      ./kway_merge_empirical
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

typedef long long ll;

/* Generic 2-way merge of two sorted arrays a[0..la) and b[0..lb) into out.
 * Counts one comparison per step while both sides still have elements. */
static void merge_two(const int *a, int la, const int *b, int lb,
                       int *out, ll *comparisons) {
    int i = 0, j = 0, t = 0;
    while (i < la && j < lb) {
        (*comparisons)++;
        if (a[i] <= b[j]) out[t++] = a[i++];
        else               out[t++] = b[j++];
    }
    while (i < la) out[t++] = a[i++];
    while (j < lb) out[t++] = b[j++];
}

static int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

/* Build k sorted arrays of n random ints each, stored contiguously:
 * arrays[i] points into a single (k*n)-length buffer. */
static int **build_sorted_arrays(int k, int n, int *storage_out_len, int **storage_out) {
    int *storage = malloc((size_t)k * n * sizeof(int));
    int **arrays = malloc((size_t)k * sizeof(int *));
    for (int i = 0; i < k; i++) {
        int *a = storage + (size_t)i * n;
        for (int j = 0; j < n; j++) a[j] = rand();
        qsort(a, n, sizeof(int), cmp_int);
        arrays[i] = a;
    }
    *storage_out_len = k * n;
    *storage_out = storage;
    return arrays;
}

/* ---------------- Method 1: sequential merging ---------------- */
static ll method1_sequential_merge(int **arrays, int k, int n) {
    ll comparisons = 0;
    int cur_len = n;
    int *cur = malloc((size_t)k * n * sizeof(int));
    memcpy(cur, arrays[0], (size_t)n * sizeof(int));
    int *tmp = malloc((size_t)k * n * sizeof(int));

    for (int i = 1; i < k; i++) {
        int new_len = cur_len + n;
        merge_two(cur, cur_len, arrays[i], n, tmp, &comparisons);
        memcpy(cur, tmp, (size_t)new_len * sizeof(int));
        cur_len = new_len;
    }
    free(cur); free(tmp);
    return comparisons;
}

/* ---------------- Method 2: pairwise / tournament merging ---------------- */
static ll method2_pairwise_merge(int **arrays, int k, int n) {
    ll comparisons = 0;

    /* current[] holds pointers+lengths for the current round's arrays */
    int count = k;
    int **cur_ptr = malloc((size_t)k * sizeof(int *));
    int *cur_len = malloc((size_t)k * sizeof(int));
    int max_total = k * n;

    /* each array owns freshly-allocated memory so we can free as we go */
    for (int i = 0; i < k; i++) {
        cur_ptr[i] = malloc((size_t)n * sizeof(int));
        memcpy(cur_ptr[i], arrays[i], (size_t)n * sizeof(int));
        cur_len[i] = n;
    }

    int **next_ptr = malloc((size_t)k * sizeof(int *));
    int *next_len = malloc((size_t)k * sizeof(int));

    while (count > 1) {
        int next_count = 0;
        int j = 0;
        for (; j + 1 < count; j += 2) {
            int total = cur_len[j] + cur_len[j + 1];
            int *out = malloc((size_t)total * sizeof(int));
            merge_two(cur_ptr[j], cur_len[j], cur_ptr[j + 1], cur_len[j + 1],
                      out, &comparisons);
            free(cur_ptr[j]); free(cur_ptr[j + 1]);
            next_ptr[next_count] = out;
            next_len[next_count] = total;
            next_count++;
        }
        if (j < count) { /* odd one out carries forward unmerged */
            next_ptr[next_count] = cur_ptr[j];
            next_len[next_count] = cur_len[j];
            next_count++;
        }
        memcpy(cur_ptr, next_ptr, (size_t)next_count * sizeof(int *));
        memcpy(cur_len, next_len, (size_t)next_count * sizeof(int));
        count = next_count;
    }

    (void)max_total;
    free(cur_ptr[0]);
    free(cur_ptr); free(cur_len);
    free(next_ptr); free(next_len);
    return comparisons;
}

int main(void) {
    srand(42);

    /* ---------------- Experiment A: fix n, vary k (powers of 2) ---------------- */
    {
        int n_fixed = 200;
        int ks[] = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
        int num_k = (int)(sizeof(ks) / sizeof(ks[0]));
        int trials = 3;

        FILE *fp = fopen("kway_vary_k.dat", "w");
        fprintf(fp, "# k  n=%d(fixed)  cmp_method1  cmp_method2  "
                     "t1_ms  t2_ms  ratio1=cmp1/(k^2*n)  ratio2=cmp2/(k*n*log2k)\n",
                n_fixed);

        for (int ki = 0; ki < num_k; ki++) {
            int k = ks[ki];
            double sum_c1 = 0, sum_c2 = 0, sum_t1 = 0, sum_t2 = 0;
            for (int trial = 0; trial < trials; trial++) {
                int storage_len; int *storage;
                int **arrays = build_sorted_arrays(k, n_fixed, &storage_len, &storage);

                clock_t a0 = clock();
                ll c1 = method1_sequential_merge(arrays, k, n_fixed);
                clock_t a1 = clock();
                ll c2 = method2_pairwise_merge(arrays, k, n_fixed);
                clock_t a2 = clock();

                sum_c1 += (double)c1;
                sum_c2 += (double)c2;
                sum_t1 += 1000.0 * (double)(a1 - a0) / CLOCKS_PER_SEC;
                sum_t2 += 1000.0 * (double)(a2 - a1) / CLOCKS_PER_SEC;

                free(arrays); free(storage);
            }
            double c1 = sum_c1 / trials, c2 = sum_c2 / trials;
            double t1 = sum_t1 / trials, t2 = sum_t2 / trials;
            double ratio1 = c1 / ((double)k * k * n_fixed);
            double ratio2 = c2 / ((double)k * n_fixed * (log((double)k) / log(2.0)));

            fprintf(fp, "%d %d %.2f %.2f %.4f %.4f %.6f %.6f\n",
                    k, n_fixed, c1, c2, t1, t2, ratio1, ratio2);
            printf("[vary k] k=%5d  cmp1=%12.0f  cmp2=%12.0f  "
                   "ratio1=%.4f  ratio2=%.4f\n", k, c1, c2, ratio1, ratio2);
        }
        fclose(fp);
    }

    /* ---------------- Experiment B: fix k, vary n ---------------- */
    {
        int k_fixed = 16;
        int ns[] = {1000, 2000, 4000, 8000, 16000, 32000, 64000,
                    125000, 250000, 500000};
        int num_n = (int)(sizeof(ns) / sizeof(ns[0]));
        int trials = 3;

        FILE *fp = fopen("kway_vary_n.dat", "w");
        fprintf(fp, "# n  k=%d(fixed)  cmp_method1  cmp_method2  "
                     "t1_ms  t2_ms  cmp1_over_n  cmp2_over_n\n", k_fixed);

        for (int ni = 0; ni < num_n; ni++) {
            int n = ns[ni];
            double sum_c1 = 0, sum_c2 = 0, sum_t1 = 0, sum_t2 = 0;
            for (int trial = 0; trial < trials; trial++) {
                int storage_len; int *storage;
                int **arrays = build_sorted_arrays(k_fixed, n, &storage_len, &storage);

                clock_t a0 = clock();
                ll c1 = method1_sequential_merge(arrays, k_fixed, n);
                clock_t a1 = clock();
                ll c2 = method2_pairwise_merge(arrays, k_fixed, n);
                clock_t a2 = clock();

                sum_c1 += (double)c1;
                sum_c2 += (double)c2;
                sum_t1 += 1000.0 * (double)(a1 - a0) / CLOCKS_PER_SEC;
                sum_t2 += 1000.0 * (double)(a2 - a1) / CLOCKS_PER_SEC;

                free(arrays); free(storage);
            }
            double c1 = sum_c1 / trials, c2 = sum_c2 / trials;
            double t1 = sum_t1 / trials, t2 = sum_t2 / trials;

            fprintf(fp, "%d %d %.2f %.2f %.4f %.4f %.6f %.6f\n",
                    n, k_fixed, c1, c2, t1, t2, c1 / n, c2 / n);
            printf("[vary n] n=%7d  cmp1=%12.0f  cmp2=%12.0f\n", n, c1, c2);
        }
        fclose(fp);
    }

    printf("\nWrote kway_vary_k.dat and kway_vary_n.dat\n");
    printf("Method 1 (sequential merge):  worst case Theta(k^2 * n)\n");
    printf("Method 2 (pairwise/tournament merge): worst case Theta(k * n * log2 k)\n");
    return 0;
}
