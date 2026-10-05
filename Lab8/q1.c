/* Q1: Minimum Coin Change (unbounded, bottom-up DP)
 * INPUT : n, then n coin values, then target V
 * OUTPUT: minimum number of coins (or -1) and one optimal set of coins
 *
 * Recurrence: dp[0]=0 ; dp[v] = 1 + min{ dp[v-c] : c in C, c<=v, dp[v-c] finite }
 * Time  : O(n*V)  (V subproblems, each tries n coins)
 * Space : O(V)    (dp[] and last[] for reconstruction)
 * Note  : pseudo-polynomial (polynomial in the VALUE of V, not in its bit length).
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
    printf("Enter %d coin values: ", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &c[i]) != 1 || c[i] <= 0) { fprintf(stderr, "Invalid coin\n"); return 1; }
    }
    printf("Enter target amount V: ");
    if (scanf("%d", &V) != 1 || V < 0) { fprintf(stderr, "Invalid V\n"); return 1; }

    int *dp = malloc((size_t)(V + 1) * sizeof(int));
    int *last = malloc((size_t)(V + 1) * sizeof(int));
    if (!dp || !last) { fprintf(stderr, "Out of memory\n"); return 1; }

    dp[0] = 0; last[0] = -1;
    for (int v = 1; v <= V; v++) {
        dp[v] = INT_MAX; last[v] = -1;
        for (int k = 0; k < n; k++) {
            if (c[k] <= v && dp[v - c[k]] != INT_MAX && dp[v - c[k]] + 1 < dp[v]) {
                dp[v] = dp[v - c[k]] + 1;
                last[v] = c[k];
            }
        }
    }

    if (dp[V] == INT_MAX) {
        printf("Result: -1 (amount %d cannot be formed)\n", V);
    } else {
        printf("Minimum coins: %d\nCoins used: ", dp[V]);
        for (int v = V; v > 0; v -= last[v]) printf("%d ", last[v]);
        printf("\n");
    }
    free(c); free(dp); free(last);
    return 0;
}