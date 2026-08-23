/*
 * Q6: Use of Loop Invariants in Sorting  (Selection Sort)
 * -----------------------------------------------------------
 * PSEUDOCODE  (1-indexed, as in the question; array A[1..n])
 * -----------------------------------------------------------
 *   SELECTION-SORT(A, n)
 *     for i = 1 to n-1
 *         min = i
 *         for j = i+1 to n
 *             if A[j] < A[min]
 *                 min = j
 *         exchange A[i] with A[min]
 *
 * LOOP INVARIANT (outer loop, on variable i)
 * -----------------------------------------------------------
 *   At the start of every iteration of the outer for-loop, the subarray
 *   A[1 .. i-1] contains the (i-1) smallest elements of the whole array,
 *   in sorted (non-decreasing) order.
 *
 *   Initialization: Before the first iteration, i = 1, so A[1..0] is the
 *     empty subarray - trivially "the 0 smallest elements, sorted."
 *
 *   Maintenance: Assume A[1..i-1] holds the (i-1) smallest elements in
 *     sorted order before an iteration. The inner loop scans A[i..n] and
 *     finds the index `min` of the smallest element there. Since every
 *     element in A[1..i-1] is already <= every element in A[i..n] (by the
 *     invariant, A[1..i-1] holds the i-1 globally smallest values), the
 *     element found is the i-th smallest overall. Swapping it into A[i]
 *     makes A[1..i] the i smallest elements, still sorted (because it's
 *     exactly A[1..i-1] followed by a value >= all of them). This is
 *     precisely the invariant with i incremented by one - true for the
 *     next iteration.
 *
 *   Termination: The loop stops when i = n. Substituting i = n into the
 *     invariant: A[1..n-1] contains the (n-1) smallest elements in sorted
 *     order. Since A[n] is the one remaining, largest, element, the whole
 *     array A[1..n] is now sorted - which is exactly what we needed to
 *     show for correctness.
 *
 * WHY ONLY THE FIRST (n-1) ELEMENTS, NOT ALL n?
 * -----------------------------------------------------------
 *   After placing the (n-1) smallest elements correctly into A[1..n-1],
 *   only ONE element remains unplaced: A[n]. Since it is the only element
 *   left, it must be the maximum, and it is automatically in its correct
 *   (final, sorted) position - no comparison or swap is needed to "select"
 *   it. Running the outer loop for i = n as well would search A[n..n], a
 *   single-element range, and accomplish nothing.
 *
 * RUNNING TIME
 * -----------------------------------------------------------
 *   The inner loop's comparison count does NOT depend on the input order
 *   at all - for every i, it always scans all (n-i) remaining elements to
 *   find the minimum, regardless of whether the array happens to already
 *   be sorted, reverse sorted, or random.
 *
 *     comparisons = sum_{i=1}^{n-1} (n - i) = (n-1) + (n-2) + ... + 1
 *                 = n(n-1)/2
 *
 *   Worst-case running time: Theta(n^2).
 *   Best-case running time:  Also Theta(n^2) - selection sort has NO
 *     better best case, because the inner "find the minimum" scan always
 *     runs to completion; it can't be short-circuited even if the array
 *     is already sorted (unlike, e.g., insertion sort, whose best case on
 *     an already-sorted array is Theta(n)). Only the number of SWAPS can
 *     vary with input (0 swaps if already sorted) - comparisons cannot.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

static long long comparisons = 0;
static long long swaps = 0;

static void selectionSort(int A[], int n) {
    /* Loop invariant: at the start of each outer iteration, A[0..i-1]
     * holds the i smallest elements of A, in sorted order (0-indexed
     * version of the 1-indexed pseudocode above).                     */
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            comparisons++;
            if (A[j] < A[minIdx]) minIdx = j;
        }
        if (minIdx != i) {
            int t = A[i]; A[i] = A[minIdx]; A[minIdx] = t;
            swaps++;
        }
    }
}

static int isSorted(int A[], int n) {
    for (int i = 1; i < n; i++) if (A[i - 1] > A[i]) return 0;
    return 1;
}

static void runCase(const char *label, int A[], int n) {
    comparisons = 0; swaps = 0;
    selectionSort(A, n);
    printf("  %-16s n=%-6d comparisons=%-9lld swaps=%-6lld  n(n-1)/2=%-9lld  sorted=%s\n",
           label, n, comparisons, swaps, (long long)n * (n - 1) / 2,
           isSorted(A, n) ? "YES" : "NO");
}

int main(void) {
    srand(11);
    int sizes[] = {10, 100, 1000, 5000};

    for (int s = 0; s < 4; s++) {
        int n = sizes[s];
        int *random_arr  = malloc((size_t)n * sizeof(int));
        int *sorted_arr  = malloc((size_t)n * sizeof(int));
        int *reverse_arr = malloc((size_t)n * sizeof(int));

        for (int i = 0; i < n; i++) random_arr[i] = rand() % 1000000;
        for (int i = 0; i < n; i++) sorted_arr[i] = i;
        for (int i = 0; i < n; i++) reverse_arr[i] = n - i;

        printf("n = %d\n", n);
        runCase("random",        random_arr,  n);
        runCase("already sorted", sorted_arr,  n);
        runCase("reverse sorted", reverse_arr, n);
        printf("\n");

        free(random_arr); free(sorted_arr); free(reverse_arr);
    }

    printf("Observation: 'comparisons' is IDENTICAL (= n(n-1)/2) across random,\n"
           "sorted, and reverse-sorted input for every n - confirming selection\n"
           "sort's best case is Theta(n^2), same as its worst case. Only the\n"
           "'swaps' column changes with input order (0 for already-sorted input).\n");
    return 0;
}