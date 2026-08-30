/*
 * DAA Lab-04, Q3: Application of sorting-III
 * -------------------------------------------
 * Input : a set S of n integers, an integer k, and a target T.
 * Output: whether some k elements of S sum to T (and if so, which ones).
 *
 * Algorithm (O(n^(k-1) * log n)):
 *   1. Sort S.                                              -- O(n log n)
 *   2. Fix the first k-2 elements using k-2 nested loops (implemented
 *      here as recursion over increasing indices, so no index is reused):
 *      this generates O(n^(k-2)) combinations.
 *   3. For each such combination, we must find 2 more elements (from the
 *      remaining part of the sorted array) that sum to the leftover
 *      target. For every candidate element arr[i] in the remaining
 *      range, binary-search for (leftover - arr[i]) among the elements
 *      after it. Scanning all i with a binary search each time costs
 *      O(n log n).
 *   Total: O(n^(k-2)) combinations * O(n log n) each
 *          = O(n^(k-1) log n).
 *
 *   (Special case k = 2 is just step 3 by itself: O(n log n), which is
 *    consistent with the formula O(n^(k-1) log n) = O(n log n).)
 */

#include <stdio.h>
#include <stdlib.h>

static int cmpAsc(const void *a, const void *b) { return (*(const int *)a - *(const int *)b); }

/* Binary search for `val` in arr[lo..hi] (inclusive). Returns an index or -1. */
static int binarySearchRange(const int *arr, int lo, int hi, int val) {
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == val) return mid;
        else if (arr[mid] < val) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

/* O(n log n): find two elements in arr[start..n-1] that sum to `target`. */
static int twoSum(const int *arr, int start, int n, int target, int *i1, int *i2) {
    for (int i = start; i < n; i++) {
        int need = target - arr[i];
        int j = binarySearchRange(arr, i + 1, n - 1, need);
        if (j != -1) { *i1 = i; *i2 = j; return 1; }
    }
    return 0;
}

/*
 * Recursively fix indices for the first (k-2) chosen elements
 * (chosen[0..depth-1]), then hand off the last two to twoSum().
 * `target` is what the remaining (k - depth) elements must sum to.
 */
static int findKSum(const int *arr, int n, int k, int target, int start, int *chosen, int depth) {
    if (k - depth == 2) {
        int i1, i2;
        if (twoSum(arr, start, n, target, &i1, &i2)) {
            chosen[depth] = i1;
            chosen[depth + 1] = i2;
            return 1;
        }
        return 0;
    }
    /* leave enough elements after position i for the rest of the picks */
    for (int i = start; i <= n - (k - depth); i++) {
        chosen[depth] = i;
        if (findKSum(arr, n, k, target - arr[i], i + 1, chosen, depth + 1))
            return 1;
    }
    return 0;
}

int main(void) {
    int n;
    printf("Enter size of set S (n): ");
    if (scanf("%d", &n) != 1 || n <= 0) { fprintf(stderr, "Invalid n\n"); return 1; }

    int *S = malloc((size_t)n * sizeof(int));
    if (!S) { fprintf(stderr, "Out of memory\n"); return 1; }

    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++) if (scanf("%d", &S[i]) != 1) { fprintf(stderr, "Bad input\n"); return 1; }

    int k;
    printf("Enter k (how many integers should sum to T): ");
    if (scanf("%d", &k) != 1) { fprintf(stderr, "Bad input\n"); return 1; }

    int T;
    printf("Enter target sum T: ");
    if (scanf("%d", &T) != 1) { fprintf(stderr, "Bad input\n"); return 1; }

    if (k < 2 || k > n) {
        fprintf(stderr, "k must satisfy 2 <= k <= n\n");
        free(S);
        return 1;
    }

    qsort(S, (size_t)n, sizeof(int), cmpAsc);

    int *chosen = malloc((size_t)k * sizeof(int));
    if (findKSum(S, n, k, T, 0, chosen, 0)) {
        printf("Found %d integers summing to %d:", k, T);
        for (int i = 0; i < k; i++) printf(" %d", S[chosen[i]]);
        printf("\n");
    } else {
        printf("No %d integers in S sum to %d\n", k, T);
    }

    free(S);
    free(chosen);
    return 0;
}