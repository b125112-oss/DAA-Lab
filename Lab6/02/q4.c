#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

int matrixChainMultiplication(int arr[], int n) {
    /* n is the number of dimensions (m+1 for m matrices) */
    int *dp = malloc(n * n * sizeof(int));
    if (!dp) return 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) dp[i * n + j] = 0;
    }

    for (int i = 1; i < n; i++) {
        dp[i * n + i] = 0;
    }

    for (int len = 2; len < n; len++) {
        for (int i = 1; i < n - len + 1; i++) {
            int j = i + len - 1;
            dp[i * n + j] = INT_MAX;
            for (int k = i; k < j; k++) {
                int cost = dp[i * n + k] + dp[(k + 1) * n + j] + arr[i - 1] * arr[k] * arr[j];
                if (cost < dp[i * n + j]) {
                    dp[i * n + j] = cost;
                }
            }
        }
    }

    int result = dp[1 * n + (n - 1)];
    free(dp);
    return result;
}

int main() {
    srand((unsigned)time(NULL));
    int mode = 0;
    printf("Choose input mode:\n1) Manual input\n2) Random generate\nSelect (1 or 2): ");
    if (scanf("%d", &mode) != 1) return 0;

    int m;
    int *arr = NULL;

    if (mode == 1) {
        printf("Enter number of matrices: ");
        if (scanf("%d", &m) != 1) return 0;
        int dims = m + 1;
        arr = malloc(dims * sizeof(int));
        if (!arr) return 0;
        printf("Enter %d dimensions (p0 p1 ... p%d): ", dims, dims - 1);
        for (int i = 0; i < dims; i++) {
            if (scanf("%d", &arr[i]) != 1) { free(arr); return 0; }
        }
        printf("Minimum scalar multiplications: %d\n", matrixChainMultiplication(arr, dims));
        free(arr);
    } else {
        m = rand() % 5 + 2; /* 2..6 matrices */
        int dims = m + 1;
        arr = malloc(dims * sizeof(int));
        if (!arr) return 0;
        for (int i = 0; i < dims; i++) arr[i] = rand() % 100 + 1;
        printf("Randomly generated m=%d dimensions:", m);
        for (int i = 0; i < dims; i++) printf(" %d", arr[i]);
        printf("\nMinimum scalar multiplications: %d\n", matrixChainMultiplication(arr, dims));
        free(arr);
    }
    return 0;
}