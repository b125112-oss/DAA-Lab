#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int max(int a, int b) {
    return (a > b) ? a : b;
}

int knapsack(int weights[], int profits[], int n, int W) {
    /* allocate dp table on heap: (n+1) x (W+1) */
    int rows = n + 1;
    int cols = W + 1;
    int *dp = calloc(rows * cols, sizeof(int));
    if (!dp) return 0;

    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            int idx = i * cols + w;
            if (i == 0 || w == 0) {
                dp[idx] = 0;
            } else if (weights[i - 1] <= w) {
                int without = dp[(i - 1) * cols + w];
                int with = profits[i - 1] + dp[(i - 1) * cols + (w - weights[i - 1])];
                dp[idx] = max(without, with);
            } else {
                dp[idx] = dp[(i - 1) * cols + w];
            }
        }
    }

    int result = dp[n * cols + W];
    free(dp);
    return result;
}

int main() {
    srand((unsigned)time(NULL));
    int mode = 0;
    printf("Choose input mode:\n1) Manual input\n2) Random generate\nSelect (1 or 2): ");
    if (scanf("%d", &mode) != 1) return 0;

    int n, W;
    int *weights = NULL, *profits = NULL;

    if (mode == 1) {
        printf("Enter number of items: ");
        if (scanf("%d", &n) != 1) return 0;
        printf("Enter capacity: ");
        if (scanf("%d", &W) != 1) return 0;

        weights = malloc(n * sizeof(int));
        profits = malloc(n * sizeof(int));
        if (!weights || !profits) return 0;

        for (int i = 0; i < n; i++) {
            printf("Enter weight and profit for item %d: ", i + 1);
            if (scanf("%d %d", &weights[i], &profits[i]) != 2) {
                free(weights); free(profits); return 0;
            }
        }
    } else {
        n = rand() % 8 + 3; /* 3..10 items */
        W = rand() % 41 + 10; /* capacity 10..50 */
        printf("Randomly generated n=%d, W=%d\n", n, W);
        weights = malloc(n * sizeof(int));
        profits = malloc(n * sizeof(int));
        if (!weights || !profits) return 0;
        for (int i = 0; i < n; i++) {
            weights[i] = rand() % W + 1;
            profits[i] = rand() % 100 + 1;
            printf("Item %d: weight=%d profit=%d\n", i+1, weights[i], profits[i]);
        }
    }

    printf("Maximum profit: %d\n", knapsack(weights, profits, n, W));
    free(weights); free(profits);
    return 0;
}