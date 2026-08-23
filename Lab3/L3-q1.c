/*
 * Q1: Binary vs Ternary Search
 * -----------------------------
 * Binary search:  T(n) = T(n/2) + 2      (>=1 "is it equal" check, 1 "which half" check)
 *                 => about 2*log2(n) element comparisons in the worst case.
 *
 * Ternary search: T(n) = T(n/3) + 4      (up to 2 equality checks + 2 range checks
 *                                          per level to pick 1 of 3 intervals)
 *                 => about 4*log3(n) = (4/log2 3)*log2(n) ~= 2.52*log2(n)
 *                    element comparisons in the worst case.
 *
 * Since 2*log2(n) < 2.52*log2(n) for every n, binary search is asymptotically
 * (and, as shown below, empirically) cheaper than ternary search, even though
 * ternary search visits fewer *levels* of recursion. Doing more comparisons
 * per (cheaper) level loses out to doing fewer comparisons per (more numerous)
 * level in binary search.
 *
 * This program implements both, counts every element comparison actually
 * executed, and averages the count over many random queries (present and
 * absent) to validate the theoretical claim.
 */
 #include <stdio.h>
 #include <stdlib.h>
 #include <math.h>
 
 static long long binary_comparisons = 0;
 static long long ternary_comparisons = 0;
 
 int binarySearch(int arr[], int lo, int hi, int x) {
     while (lo <= hi) {
         int mid = lo + (hi - lo) / 2;
         binary_comparisons++;                 /* arr[mid] == x ? */
         if (arr[mid] == x) return mid;
         binary_comparisons++;                 /* arr[mid] < x ?  */
         if (arr[mid] < x) lo = mid + 1;
         else              hi = mid - 1;
     }
     return -1;
 }
 
 int ternarySearch(int arr[], int lo, int hi, int x) {
     while (lo <= hi) {
         int third = (hi - lo) / 3;
         int mid1 = lo + third;
         int mid2 = hi - third;
 
         ternary_comparisons++;                /* arr[mid1] == x ? */
         if (arr[mid1] == x) return mid1;
         ternary_comparisons++;                /* arr[mid2] == x ? */
         if (arr[mid2] == x) return mid2;
         ternary_comparisons++;                /* x < arr[mid1] ?  */
         if (x < arr[mid1]) {
             hi = mid1 - 1;
         } else {
             ternary_comparisons++;            /* x > arr[mid2] ?  */
             if (x > arr[mid2]) {
                 lo = mid2 + 1;
             } else {
                 lo = mid1 + 1;
                 hi = mid2 - 1;
             }
         }
     }
     return -1;
 }
 
 int main(void) {
     int n = 1000000;
     int *arr = malloc((size_t)n * sizeof(int));
     for (int i = 0; i < n; i++) arr[i] = 2 * i;   /* sorted array, only even numbers present */
 
     int trials = 200000;
     srand(42);
 
     long long total_bin = 0, total_ter = 0;
     long long found_bin = 0, found_ter = 0;
 
     for (int t = 0; t < trials; t++) {
         int target = rand() % (2 * n);   /* ~50% of queries will be absent (odd numbers) */
 
         binary_comparisons = 0;
         int rb = binarySearch(arr, 0, n - 1, target);
         total_bin += binary_comparisons;
         if (rb != -1) found_bin++;
 
         ternary_comparisons = 0;
         int rt = ternarySearch(arr, 0, n - 1, target);
         total_ter += ternary_comparisons;
         if (rt != -1) found_ter++;
     }
 
     double avg_bin = (double)total_bin / trials;
     double avg_ter = (double)total_ter / trials;
 
     printf("Array size n = %d, trials = %d\n", n, trials);
     printf("Sanity check: found_bin = %lld, found_ter = %lld (should match)\n\n", found_bin, found_ter);
 
     printf("Average comparisons  -> Binary  search: %.4f\n", avg_bin);
     printf("Average comparisons  -> Ternary search: %.4f\n\n", avg_ter);
 
     printf("Theoretical worst case:\n");
     printf("  Binary  ~ 2*log2(n)              = %.4f\n", 2.0 * log2(n));
     printf("  Ternary ~ (4/log2 3)*log2(n)      = %.4f\n\n", (4.0 / log2(3.0)) * log2(n));
 
     if (avg_bin < avg_ter)
         printf("Result: Binary search used FEWER comparisons on average -> "
                "binary search is better, confirming the theory.\n");
     else
         printf("Result: Unexpected - ternary search used fewer comparisons.\n");
 
     free(arr);
     return 0;
 }