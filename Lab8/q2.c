/* Q2: Coin Change - number of distinct combinations (order ignored)
 * INPUT : n, then n DISTINCT positive coin values, then target V
 * OUTPUT: number of combinations summing to V
 *
 * Trick: iterate coins in the OUTER loop, amounts in the INNER loop. Each coin is
 * then "introduced" once, so 1+2 and 2+1 are counted once (combinations, not permutations).
 * dp[0]=1 ; for each coin c: for v=c..V: dp[v] += dp[v-c]
 * Time  : O(n*V)
 * Space : O(V)
 * Counts can grow very fast; unsigned long long is used and overflow is detected.
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(void) {
    int n, V;
    printf("Enter number of denominations n: ");
    if (scanf("%d", &n) != 1 || n <= 0) { fprintf(stderr, "Invalid n\n"); return 1; }
    int *c = malloc(n * sizeof(int));
    if (!c) return 1;
    printf("Enter %d distinct positive coin values: ", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &c[i]) != 1 || c[i] <= 0) { fprintf(stderr, "Invalid coin\n"); return 1; }
        for (int j = 0; j < i; j++)
            if (c[j] == c[i]) { fprintf(stderr, "Coins must be distinct\n"); return 1; }
    }
    printf("Enter target amount V: ");
    if (scanf("%d", &V) != 1 || V < 0) { fprintf(stderr, "Invalid V\n"); return 1; }

    unsigned long long *dp = calloc((size_t)V + 1, sizeof(unsigned long long));
    if (!dp) { fprintf(stderr, "Out of memory\n"); return 1; }
    int overflow = 0;
    dp[0] = 1;                      /* one way to make 0: take nothing */
    for (int k = 0; k < n; k++)
        for (int v = c[k]; v <= V; v++) {
            if (dp[v] > ULLONG_MAX - dp[v - c[k]]) overflow = 1;
            dp[v] += dp[v - c[k]];
        }

    if (overflow) printf("Warning: count exceeds 64 bits; result below is invalid.\n");
    printf("Number of combinations: %llu\n", dp[V]);
    free(c); free(dp);
    return 0;
}