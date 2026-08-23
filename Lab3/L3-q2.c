/*
 * Q2: Search the Defective Coin
 * ------------------------------
 * We are given n coins. At most ONE of them may be lighter than the
 * (common, correct) required weight; the rest are guaranteed perfect.
 * We must find that lighter coin using a balance scale, or correctly
 * report that none is lighter, in floor(log2 n) + c weighings.
 *
 * KEY IDEA
 * --------
 * A balance scale can hold several coins on each pan, so we can compare
 * the TOTAL weight of one group of coins against another group of the
 * SAME size.
 *
 * Split the current candidate range of size n into two equal halves
 * (if n is odd, split n-1 coins into two equal halves and set 1 coin
 * aside as "leftover") and weigh half A against half B:
 *
 *   - If they balance: every coin weighed is genuine.
 *       * If n was even, that accounts for ALL coins in range -> no
 *         defective coin exists anywhere in this range. Done (return NONE).
 *       * If n was odd, the only coin not yet weighed is the leftover
 *         coin, so it is the only remaining suspect.
 *
 *   - If they don't balance: the defective coin must be in the LIGHTER
 *     half (never the heavier half, since coins can only be light, not
 *     heavy). We now KNOW a defective coin is present in that half
 *     ("guaranteed" mode) and recurse purely by halving from here on -
 *     no more balanced-case ambiguity is possible in guaranteed mode,
 *     because if the defective is truly present in a range split into
 *     two equal halves, one half MUST be lighter.
 *
 * This means at most ONE extra weighing (checking the leftover coin
 * against a known-genuine coin) is ever spent before we lock into
 * "guaranteed" mode, after which each weighing exactly halves the
 * search range. Hence total weighings <= floor(log2 n) + 1, i.e.
 * log2(n) + c with c = 1.
 *
 * COMPLEXITY: Theta(log n) weighings, Theta(n) time to sum pans.
 */
 #include <stdio.h>
 #include <stdlib.h>
 #include <time.h>
 
 #define MAXN 100000
 static int weight[MAXN];      /* ground-truth weight table (simulated coins)   */
 static long weighings = 0;    /* counts number of times the scale is used      */
 
 /* Weigh group g1 (n1 coins) against group g2 (n2 coins), n1 == n2 required.
  * Returns -1 if g1 lighter, 0 if balanced, +1 if g1 heavier.               */
 static int weigh(const int *g1, int n1, const int *g2, int n2) {
     weighings++;
     long s1 = 0, s2 = 0;
     for (int i = 0; i < n1; i++) s1 += weight[g1[i]];
     for (int i = 0; i < n2; i++) s2 += weight[g2[i]];
     if (s1 < s2) return -1;
     if (s1 > s2) return 1;
     return 0;
 }
 
 /* idx[lo..hi] are coin indices under consideration.
  * 'guaranteed' = 1 means a defective coin is known to exist in this range.
  * Returns the coin index of the defective (lighter) coin, or -1 if none. */
 static int findDefective(int *idx, int lo, int hi, int guaranteed) {
     int n = hi - lo + 1;
     if (n <= 0) return -1;
 
     if (n == 1) {
         /* A single coin cannot be judged in isolation without a reference
          * except when we already KNOW (guaranteed) it must be the one. */
         return guaranteed ? idx[lo] : -1;
     }
 
     if (n % 2 == 0) {
         int half = n / 2;
         int *left  = &idx[lo];
         int *right = &idx[lo + half];
         int r = weigh(left, half, right, half);
         if (r == 0) {
             /* every coin weighed genuine, and that's all n of them */
             return -1;
         } else if (r < 0) {
             return findDefective(idx, lo, lo + half - 1, 1);
         } else {
             return findDefective(idx, lo + half, hi, 1);
         }
     } else {
         int half = (n - 1) / 2;
         int *left  = &idx[lo];
         int *right = &idx[lo + half];
         int leftover = idx[hi];               /* the odd one out */
         int r = weigh(left, half, right, half);
         if (r == 0) {
             if (guaranteed) {
                 /* both weighed halves are genuine, so the leftover MUST
                  * be the defective one - no extra weighing needed        */
                 return leftover;
             }
             if (half == 0) {
                 /* n == 1 total in this call, nothing genuine to compare
                  * against; cannot determine -> treat as none (edge case) */
                 return -1;
             }
             /* not guaranteed: leftover might be defective or might not
              * exist at all -> one more weighing against a proven-genuine
              * coin settles it.                                          */
             int ref[1]  = { left[0] };
             int susp[1] = { leftover };
             int r2 = weigh(susp, 1, ref, 1);
             return (r2 < 0) ? leftover : -1;
         } else if (r < 0) {
             return findDefective(idx, lo, lo + half - 1, 1);
         } else {
             return findDefective(idx, lo + half, hi, 1);
         }
     }
 }
 
 #include <math.h>
 
 int main(void) {
     printf("Format: planted defective index (-1 = none) vs what the algorithm found\n\n");
 
     int cases_n[]       = {2, 5, 8, 10, 15, 16, 33, 64, 100, 1000};
     int ncases = sizeof(cases_n) / sizeof(cases_n[0]);
 
     for (int c = 0; c < ncases; c++) {
         int n = cases_n[c];
 
         /* Case A: no defective coin at all */
         {
             int *idx = malloc((size_t)n * sizeof(int));
             for (int i = 0; i < n; i++) { idx[i] = i; weight[i] = 100; }
             weighings = 0;
             int found = findDefective(idx, 0, n - 1, 0);
             double bound = log2((double)n) + 1;
             printf("n=%5d  planted=NONE   found=%-6s  weighings=%-3ld  (log2(n)+1=%.2f)  %s\n",
                    n, (found == -1) ? "NONE" : "coin", weighings, bound,
                    (found == -1) ? "OK" : "MISMATCH");
             free(idx);
         }
 
         /* Case B: defective coin planted at a few different positions */
         int positions[3] = { 0, n / 2, n - 1 };
         for (int p = 0; p < 3; p++) {
             int defIdx = positions[p];
             int *idx = malloc((size_t)n * sizeof(int));
             for (int i = 0; i < n; i++) { idx[i] = i; weight[i] = 100; }
             weight[defIdx] = 99;
             weighings = 0;
             int found = findDefective(idx, 0, n - 1, 0);
             double bound = log2((double)n) + 1;
             printf("n=%5d  planted=%-6d found=%-6d weighings=%-3ld  (log2(n)+1=%.2f)  %s\n",
                    n, defIdx, found, weighings, bound,
                    (found == defIdx) ? "OK" : "MISMATCH");
             free(idx);
         }
         printf("\n");
     }
     return 0;
 }