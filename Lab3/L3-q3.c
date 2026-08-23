/*
 * Q3: Max and Min using Divide & Conquer  (<= 3n/2 comparisons)
 * ---------------------------------------------------------------
 * Naive scan: 2(n-1) comparisons (compare each element against both
 * running max and running min).
 *
 * D&C idea: split the array into two halves, recursively find (max,min)
 * of each half using only 1 comparison per base pair, then combine the
 * two halves with just 2 more comparisons (max-vs-max, min-vs-min).
 *
 * Base cases:
 *   n == 1: max = min = the single element               -> 0 comparisons
 *   n == 2: 1 comparison decides both max and min for the pair
 *
 * Recurrence (n a power of 2):
 *   T(n) = 2*T(n/2) + 2,   T(2) = 1
 * Solving: T(n) = 3n/2 - 2
 *
 * This is a well known tight lower bound too - no comparison-based
 * algorithm can do better than ceil(3n/2) - 2 comparisons in the worst case.
 */
 #include <stdio.h>
 #include <stdlib.h>
 #include <time.h>
 
 static long comparisons = 0;
 
 static void maxMin(int arr[], int lo, int hi, int *mx, int *mn) {
     if (lo == hi) {                       /* n == 1 */
         *mx = *mn = arr[lo];
         return;
     }
     if (hi - lo == 1) {                   /* n == 2 */
         comparisons++;
         if (arr[lo] > arr[hi]) { *mx = arr[lo]; *mn = arr[hi]; }
         else                   { *mx = arr[hi]; *mn = arr[lo]; }
         return;
     }
     int mid = (lo + hi) / 2;
     int lmax, lmin, rmax, rmin;
     maxMin(arr, lo, mid, &lmax, &lmin);
     maxMin(arr, mid + 1, hi, &rmax, &rmin);
 
     comparisons++;
     *mx = (lmax > rmax) ? lmax : rmax;
     comparisons++;
     *mn = (lmin < rmin) ? lmin : rmin;
 }
 
 int main(void) {
     srand((unsigned)time(NULL));
 
     int sizes[] = {2, 4, 8, 16, 32, 64, 1024, 65536, 1000, 100000, 1000000};
     int ns = sizeof(sizes) / sizeof(sizes[0]);
 
     printf("%10s %14s %14s %14s %16s %10s\n",
            "n", "max", "min", "comparisons", "3n/2 - 2 bound", "naive 2(n-1)");
 
     for (int s = 0; s < ns; s++) {
         int n = sizes[s];
         int *arr = malloc((size_t)n * sizeof(int));
         for (int i = 0; i < n; i++) arr[i] = rand() % 10000000 - 5000000;
 
         int mx, mn;
         comparisons = 0;
         maxMin(arr, 0, n - 1, &mx, &mn);
 
         printf("%10d %14d %14d %14ld %16.0f %10d\n",
                n, mx, mn, comparisons, 1.5 * n - 2, 2 * (n - 1));
 
         /* sanity check against a trivial linear scan */
         int chk_mx = arr[0], chk_mn = arr[0];
         for (int i = 1; i < n; i++) {
             if (arr[i] > chk_mx) chk_mx = arr[i];
             if (arr[i] < chk_mn) chk_mn = arr[i];
         }
         if (chk_mx != mx || chk_mn != mn) {
             printf("  !! MISMATCH for n=%d\n", n);
         }
         free(arr);
     }
 
     return 0;
 }