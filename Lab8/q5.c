/* Q5: Maximum Sum Increasing Subsequence (strictly increasing, positive integers)
 * INPUT : n, then n positive integers
 * OUTPUT: maximum sum and the subsequence achieving it
 *
 * msis[i] = a[i] + max{ msis[j] : j<i, a[j]<a[i] }  (or just a[i] if none)
 * answer  = max_i msis[i]
 * Time  : O(n^2)   Space : O(n)
 * (O(n log n) is possible with a Fenwick tree / segment tree over compressed values.)
 */
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;
    printf("Enter n: ");
    if (scanf("%d", &n) != 1 || n <= 0) { fprintf(stderr, "Invalid n\n"); return 1; }
    long long *a = malloc(n * sizeof(long long)), *s = malloc(n * sizeof(long long));
    int *prev = malloc(n * sizeof(int));
    if (!a || !s || !prev) return 1;
    printf("Enter %d positive integers: ", n);
    for (int i = 0; i < n; i++)
        if (scanf("%lld", &a[i]) != 1 || a[i] <= 0) { fprintf(stderr, "Need positive integers\n"); return 1; }

    int best = 0;
    for (int i = 0; i < n; i++) {
        s[i] = a[i]; prev[i] = -1;
        for (int j = 0; j < i; j++)
            if (a[j] < a[i] && s[j] + a[i] > s[i]) { s[i] = s[j] + a[i]; prev[i] = j; }
        if (s[i] > s[best]) best = i;
    }
    printf("Maximum sum: %lld\nSubsequence: ", s[best]);
    int *idx = malloc(n * sizeof(int)), cnt = 0;
    for (int i = best; i != -1; i = prev[i]) idx[cnt++] = i;
    for (int i = cnt - 1; i >= 0; i--) printf("%lld ", a[idx[i]]);
    printf("\n");
    free(a); free(s); free(prev); free(idx);
    return 0;
}