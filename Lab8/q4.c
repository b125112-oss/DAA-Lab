/* Q4: Longest (strictly) Increasing Subsequence
 * INPUT : n, then n integers
 * OUTPUT: LIS length and one LIS
 *
 * Method A (DP, with reconstruction): dp[i] = 1 + max{dp[j] : j<i, a[j]<a[i]}
 *     Time O(n^2), Space O(n)
 * Method B (patience / binary search, length only): tails[k] = smallest tail of an
 *     increasing subsequence of length k+1. For x, find first tails[idx] >= x (strict).
 *     Time O(n log n), Space O(n)
 * Both are run and cross-checked.
 */
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;
    printf("Enter n: ");
    if (scanf("%d", &n) != 1 || n < 0) { fprintf(stderr, "Invalid n\n"); return 1; }
    if (n == 0) { printf("LIS length: 0\n"); return 0; }
    int *a = malloc(n * sizeof(int)), *dp = malloc(n * sizeof(int)),
        *prev = malloc(n * sizeof(int)), *tails = malloc(n * sizeof(int));
    if (!a || !dp || !prev || !tails) return 1;
    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++)
        if (scanf("%d", &a[i]) != 1) { fprintf(stderr, "Bad input\n"); return 1; }

    int best = 0;
    dp[0] = 1; prev[0] = -1;
    for (int i = 0; i < n; i++) {
        dp[i] = 1; prev[i] = -1;
        for (int j = 0; j < i; j++)
            if (a[j] < a[i] && dp[j] + 1 > dp[i]) { dp[i] = dp[j] + 1; prev[i] = j; }
        if (dp[i] > dp[best]) best = i;
    }
    int len = dp[best];

    int sz = 0;
    for (int i = 0; i < n; i++) {
        int lo = 0, hi = sz;                 /* first index with tails[idx] >= a[i] */
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (tails[mid] < a[i]) lo = mid + 1; else hi = mid;
        }
        tails[lo] = a[i];
        if (lo == sz) sz++;
    }

    int *seq = malloc(len * sizeof(int)), k = len;
    for (int i = best; i != -1; i = prev[i]) seq[--k] = a[i];

    printf("LIS length (O(n^2) DP)      : %d\n", len);
    printf("LIS length (O(n log n))     : %d %s\n", sz, sz == len ? "[match]" : "[MISMATCH]");
    printf("One LIS: ");
    for (int i = 0; i < len; i++) printf("%d ", seq[i]);
    printf("\n");
    free(a); free(dp); free(prev); free(tails); free(seq);
    return 0;
}