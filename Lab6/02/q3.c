#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int max(int a, int b) {
    return (a > b) ? a : b;
}

void lcs(char *X, char *Y) {
    int m = strlen(X);
    int n = strlen(Y);
    int rows = m + 1;
    int cols = n + 1;
    int *dp = calloc(rows * cols, sizeof(int));
    if (!dp) return;

    for (int i = 0; i <= m; i++) dp[i * cols + 0] = 0;
    for (int j = 0; j <= n; j++) dp[0 * cols + j] = 0;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (X[i - 1] == Y[j - 1]) {
                dp[i * cols + j] = dp[(i - 1) * cols + (j - 1)] + 1;
            } else {
                dp[i * cols + j] = max(dp[(i - 1) * cols + j], dp[i * cols + (j - 1)]);
            }
        }
    }

    int index = dp[m * cols + n];
    char *lcs_str = malloc(index + 1);
    if (!lcs_str) { free(dp); return; }
    lcs_str[index] = '\0';

    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (X[i - 1] == Y[j - 1]) {
            lcs_str[index - 1] = X[i - 1];
            i--; j--; index--;
        } else if (dp[(i - 1) * cols + j] > dp[i * cols + (j - 1)]) {
            i--;
        } else {
            j--;
        }
    }

    printf("LCS length: %d\n", dp[m * cols + n]);
    printf("LCS: %s\n", lcs_str);
    free(lcs_str);
    free(dp);
}

int main() {
    srand((unsigned)time(NULL));
    int mode = 0;
    printf("Choose input mode:\n1) Manual input\n2) Random strings\nSelect (1 or 2): ");
    if (scanf("%d", &mode) != 1) return 0;

    char X[100], Y[100];

    if (mode == 1) {
        printf("Enter first string: ");
        if (scanf("%99s", X) != 1) return 0;
        printf("Enter second string: ");
        if (scanf("%99s", Y) != 1) return 0;
    } else {
        int m = rand() % 20 + 1;
        int n = rand() % 20 + 1;
        for (int i = 0; i < m && i < 99; i++) X[i] = 'A' + (rand() % 26);
        X[m < 99 ? m : 99] = '\0';
        for (int j = 0; j < n && j < 99; j++) Y[j] = 'A' + (rand() % 26);
        Y[n < 99 ? n : 99] = '\0';
        printf("Random X: %s\n", X);
        printf("Random Y: %s\n", Y);
    }

    lcs(X, Y);
    return 0;
}