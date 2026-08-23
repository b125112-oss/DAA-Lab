#include <stdio.h>
#include <stdlib.h>

static int cmpAsc(const void *a, const void *b)  { return (*(const int *)a - *(const int *)b); }
static int cmpDesc(const void *a, const void *b) { return (*(const int *)b - *(const int *)a); }

/* O(n log n): sort S1 ascending, S2 descending, then two-pointer scan. */
static int findPairWithSum(int *S1, int n1, int *S2, int n2, int x, int *a, int *b) {
    qsort(S1, (size_t)n1, sizeof(int), cmpAsc);
    qsort(S2, (size_t)n2, sizeof(int), cmpDesc);

    int i = 0, j = 0;
    while (i < n1 && j < n2) {
        int sum = S1[i] + S2[j];
        if (sum == x) { *a = S1[i]; *b = S2[j]; return 1; }
        else if (sum < x) i++;
        else j++;
    }
    return 0;
}

int main(void) {
    int n;
    printf("Enter size n of each set: ");
    if (scanf("%d", &n) != 1 || n <= 0) { fprintf(stderr, "Invalid n\n"); return 1; }

    int *S1 = malloc((size_t)n * sizeof(int));
    int *S2 = malloc((size_t)n * sizeof(int));
    if (!S1 || !S2) { fprintf(stderr, "Out of memory\n"); return 1; }

    printf("Enter %d elements of S1: ", n);
    for (int i = 0; i < n; i++) if (scanf("%d", &S1[i]) != 1) { fprintf(stderr, "Bad input\n"); return 1; }

    printf("Enter %d elements of S2: ", n);
    for (int i = 0; i < n; i++) if (scanf("%d", &S2[i]) != 1) { fprintf(stderr, "Bad input\n"); return 1; }

    int x;
    printf("Enter target sum x: ");
    if (scanf("%d", &x) != 1) { fprintf(stderr, "Bad input\n"); return 1; }

    int a, b;
    if (findPairWithSum(S1, n, S2, n, x, &a, &b))
        printf("Found pair: %d (from S1) + %d (from S2) = %d\n", a, b, x);
    else
        printf("No pair (one from S1, one from S2) sums to %d\n", x);

    free(S1);
    free(S2);
    return 0;
}