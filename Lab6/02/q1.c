#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long fib(int n) {
    if (n <= 1) return n;

    long long *dp = malloc((n + 1) * sizeof(long long));
    if (!dp) return -1; /* allocation failed */

    dp[0] = 0;
    dp[1] = 1;

    for (int i = 2; i <= n; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }

    long long result = dp[n];
    free(dp);
    return result;
}

int main() {
    srand((unsigned)time(NULL));
    int mode = 0;
    printf("Choose input mode:\n1) Manual input\n2) Random n\nSelect (1 or 2): ");
    if (scanf("%d", &mode) != 1) return 0;

    int n;
    if (mode == 1) {
        printf("Enter n: ");
        if (scanf("%d", &n) != 1) return 0;
        if (n < 0) { printf("Invalid n (must be >= 0)\n"); return 0; }
    } else {
        n = rand() % 91; /* keep within safe range for 64-bit Fibonacci */
        printf("Randomly selected n = %d\n", n);
    }

    printf("The %dth Fibonacci number is: %lld\n", n, fib(n));
    return 0;
}